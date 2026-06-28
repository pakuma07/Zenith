# Chapter 31: Engineering Practice at Scale

Everything so far made *the code* good. This chapter is about the *engineering
system* around it — how a staff/principal engineer ships safely, leads technically
without managing, and keeps a large C++ codebase and team healthy over years. These
are the skills that multiply an org: **design docs, code review, testing strategy,
CI/CD, release engineering, incident response, and technical leadership.** Less
syntax, more judgment — but no less concrete, and with the C++-specific edges that
make them especially load-bearing.

> **Why this matters more in C++:** with no GC and no runtime safety net, the
> *process* around the code (review, sanitizers in CI, careful migrations) is what
> keeps a large native codebase from accumulating latent memory bugs and ABI
> breakage. The discipline *is* the safety.

---

## 31.1 Design Docs and RFCs

Before significant work, write it down. A **design doc** forces clarity, surfaces
disagreement *cheaply* (before code exists), and records *why*.

```text
1. Context & problem    what we're solving, for whom, why now
2. Goals / non-goals    explicitly scope OUT  ◀ prevents scope creep
3. Proposed design      the approach, key decisions visible
4. Alternatives         what else, and why rejected  ◀ the staff-level content
5. Trade-offs & risks   cost, failure modes, how you'll know
6. Rollout & migration  incremental ship, how to back out
7. Open questions       what you want reviewed
```

The senior signal is **Alternatives** and **Non-goals**: junior docs describe *a*
solution; senior docs show the solution space and a defended choice.

> **Reversible vs irreversible** (one-way/two-way doors): spend effort in proportion
> to reversibility. In C++, **public APIs and ABIs, wire formats, and a chosen
> dependency's threading/ownership model** are one-way doors — they deserve the full
> treatment because you'll live with them for years (Chapters 21, 30).

---

## 31.2 Code Review

Review catches defects, spreads knowledge, and raises the bar — the cultural effect
matters more than any single bug. **Author:** small, single-purpose PRs (a 200-line
PR gets a real review; a 2,000-line one gets a rubber stamp); separate refactors from
behavior changes. **Reviewer:**

- **Order:** correctness → design/interfaces → **memory safety & ownership** → tests
  → readability → style. (The C++-specific addition: who owns this pointer? can this
  reference dangle? is this exception-safe?)
- **Blocking vs nit:** prefix `nit:` for non-blocking suggestions.
- **Ask, don't command:** "what frees this?" teaches better than "this leaks."
- **Be timely:** a stalled review blocks a human.

> The most valuable reviews question the **approach**, not the lines — which requires
> reviewing the design doc (31.1) before the code exists.

---

## 31.3 Automated Quality Gates

Humans review judgment; tools handle the mechanical — and in C++ the tools also catch
*correctness* bugs humans miss (Chapter 22):

| Tool | Catches |
|---|---|
| clang-format | formatting — auto-applied, never debated |
| clang-tidy (`bugprone-*`, `cppcoreguidelines-*`, `modernize-*`) | bug patterns, modernization |
| **ASan/UBSan/TSan** in CI | memory/UB/race bugs at test time |
| fuzzers | crashing inputs on parsers |
| Dependency scan + SBOM | known CVEs (Chapter 27) |
| `-Wall -Wextra -Werror` | free static analysis at compile time |

> **Make the right thing automatic.** In a language this unforgiving, a standard a
> human must remember erodes; a standard CI enforces holds. Sanitizers-in-CI is the
> single highest-leverage gate.

---

## 31.4 Testing Strategy

Chapter 22 covered *how* (GoogleTest/Catch2); at scale the question is *which* tests.
The **pyramid**: many fast **unit** tests, fewer **integration** tests (real DB/queue
via containers), few **end-to-end**.

- Unit tests run on every save — keep them fast and deterministic, or people stop
  running them.
- **Run the whole suite under ASan+UBSan** (and TSan for concurrent code) — in C++ a
  test that passes plain but fails under a sanitizer caught a real bug.
- **Flaky tests are worse than none** — they train the team to ignore red; quarantine
  and fix.
- **Property-based** (RapidCheck) and **fuzz** tests find edge cases examples miss
  (Chapter 26 parsers).
- **Test behavior, not implementation** — tests coupled to internals break on every
  refactor.

---

## 31.5 Version Control and CI/CD

- **Trunk-based development** with short-lived branches and **feature flags** keeps
  integration continuous; atomic, well-described commits make `git bisect` and
  reverts work.
- **CI on every push:** configure + build (Debug *and* Release, multiple compilers —
  Chapter 22) → format/lint → tests under sanitizers → fuzz smoke → dependency scan →
  build artifact.
- **CD with progressive delivery:** canary or blue-green with **automated rollback**
  on SLO regression (Chapters 25, 28) — never a big-bang deploy.

```text
push ─▶ CI: build(GCC+Clang, Debug+Release) + clang-tidy + tests(ASan/UBSan/TSan) + scan
        │ green
        ▼
   staging ─▶ smoke ─▶ canary (1%→10%→100%, watch SLOs) ─▶ full   [auto-rollback on regression]
```

- **Fast pipelines matter** — C++ builds are slow; use **ccache/sccache**,
  precompiled headers or modules (Chapter 20), and distributed builds to keep CI in
  minutes, or people route around it.

---

## 31.6 Incident Response

Incidents are inevitable; the craft is responding and learning (Chapter 25
observability):

- **Mitigate before diagnosing:** roll back, flip a flag, shed load *first*; the
  latest deploy is the first suspect.
- **Roles:** an Incident Commander coordinates; others investigate/communicate.
- **Blameless postmortems:** timeline, impact, root cause, and **systemic** fixes
  (the process/tooling gap), not "be more careful." For a C++ crash, the systemic fix
  is usually "this class of bug should have been caught by a sanitizer/fuzzer in CI"
  (Chapter 22) — close that gap, not just the one bug.
- **Track and close action items**, or you guarantee the repeat.

> **You build it, you run it.** Teams on-call for their own services build more
> operable ones; make services operable (logs, metrics, runbooks, core-dump capture —
> Chapter 25) as a feature, not an afterthought.

---

## 31.7 Technical Debt and Large-Scale Change

- **Debt is a tool, not a sin** — taken deliberately, tracked, repaid. The dangerous
  kind is unacknowledged and compounding (the codebase stuck on C++11, an
  un-upgradable dependency, a flaky core). Make it visible.
- **Boy Scout rule:** leave each file a little better; incremental beats the doomed
  big rewrite.
- **Large-scale migrations** (C++11 → C++23 idioms, a toolchain or dependency
  upgrade, an ABI change across a fleet) are a discipline: automate with **clang-tidy
  `modernize-*` / clang-based codemods**, run old and new in parallel, migrate behind
  flags, and *delete the old path*.
- **Rewrites are usually a trap** (Spolsky) — you discard encoded bug fixes and bet on
  a flag day. Prefer **strangler-fig**: route functionality to the new component
  incrementally until the old one is dead. (When a component's *risk* — not just its
  age — justifies it, rewriting a security-critical C++ module in a memory-safe
  language is a legitimate strategic call, Chapter 27.)

---

## 31.8 Technical Leadership Without Authority

The staff/principal role is influence, not management:

- **Multiply, don't hoard:** your output is increasingly others' output — the design
  review that prevents a bad path, the doc that aligns teams, the abstraction that
  makes everyone faster. Optimize team throughput, not personal commits.
- **Choose the right battles:** strong opinions held loosely; disagree and commit once
  decided. Reserve hard lines for irreversible decisions (31.1).
- **Make the implicit explicit:** write the design principles, the ownership/threading
  conventions, the "why we don't do X" doc — tribal knowledge doesn't scale.
- **Mentor and sponsor:** review (31.2) and design review are teaching surfaces; level
  up the people around you rather than being the single point of failure.

---

## 31.9 Summary

- **Write design docs** (alternatives + non-goals; effort ∝ reversibility — and in
  C++, APIs/ABIs/wire formats are one-way doors).
- **Code review** for correctness, design, and **memory/ownership** first; push style
  to **automated gates**, with **sanitizers in CI** as the highest-leverage one.
- Allocate tests by the **pyramid**, run them under **sanitizers**, kill flakiness,
  test behavior not implementation.
- **Trunk-based dev + feature flags** and **CI/CD with progressive delivery + auto
  rollback**; keep slow C++ builds fast with caching/modules.
- Treat **incidents** as learning (mitigate first, blameless postmortems, close the
  *class* of bug); manage **debt** deliberately and prefer **strangler-fig** to
  rewrites.
- Lead by **multiplying others** — written conventions, good interfaces, mentorship.

**This concludes the modern-C++ and engineering-at-scale arc (Chapters 19–31).** From
C++23 language features through build/ABI, tooling, performance, security,
distributed systems, and the human system that ships them, these are the concepts a
staff/principal C++ engineer reasons about long after the syntax has become reflex.

## Next Steps

- Write a one-page design doc for your next change, with explicit non-goals and two
  rejected alternatives.
- Add an integration test with a real containerized dependency alongside your unit
  tests, all under ASan in CI.
- Add ccache + a multi-compiler matrix to CI and measure the build-time improvement.
- Run a blameless postmortem template on a past crash and identify the CI gate
  (sanitizer/fuzzer) that would have caught it.
- Revisit **[Chapter 22: Tooling, Testing & Sanitizers](../22_tooling_testing/README.md)**
  and **[Chapter 25: Production & Operational Concerns](../25_production_operational/README.md)**.
