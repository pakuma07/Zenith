# Chapter 27: Security & Supply Chain

Security in C++ starts somewhere no managed language has to worry: **memory
safety**. Roughly **70% of serious CVEs in C/C++ codebases are memory-safety bugs**
— buffer overflows, use-after-free, uninitialized reads — each a potential remote
code execution. On top of that sit the usual application and supply-chain concerns.
This chapter covers writing secure C++: eliminating memory-safety bugs, hardening
the binary, handling input and secrets safely, and securing the dependency chain.

> **The defining C++ security fact:** the language will happily compile code with
> undefined behavior that an attacker can weaponize. Your first job is to make
> memory-safety bugs *impossible or detectable* — via modern idioms (Chapters 12,
> 19–20) and the sanitizers/fuzzing of Chapter 22.

---

## 27.1 Memory Safety: The Dominant Risk

The bug classes that account for most C++ exploits, and how modern C++ defuses each:

| Vulnerability | Cause | Mitigation |
|---|---|---|
| **Buffer overflow** | writing past an array/buffer | `std::vector`/`std::array` + `.at()`, `std::span` bounds (Ch 20) |
| **Use-after-free** | using freed memory | RAII + smart pointers (Ch 12); no raw `delete` |
| **Dangling pointer/view** | reference/`string_view`/`span` outliving its data | lifetime discipline (Ch 4, 19) |
| **Uninitialized read** | reading before assignment | always initialize; `-Wuninitialized`, MSan |
| **Integer overflow** | overflow → undersized allocation/index | checked arithmetic, validate sizes |
| **Double free** | freeing twice | unique ownership (`unique_ptr`); never manual `delete` |

```cpp
// The classic exploitable bug — and the modern fix.
char buf[64];
// strcpy(buf, user_input);                  // ⚠ overflow if input > 63 chars -> RCE
std::string safe = user_input;               // owns + grows; no fixed buffer to overflow
// Bounds-checked access at the boundary:
std::span<const std::byte> packet = recv();
if (offset + len > packet.size()) return Error::Truncated;   // VALIDATE before reading
```

> **Prefer safe constructs by default:** owning containers over raw buffers, smart
> pointers over `new`/`delete` (Chapter 12), `.at()`/`span` bounds where input-driven,
> and `std::string`/`string_view` over `char*` + `strcpy`/`strcat`. Most C++ CVEs
> simply don't occur in code written this way.

---

## 27.2 Detecting and Hardening

You cannot eyeball memory safety; you tool for it (Chapter 22) and harden the
binary:

- **Sanitizers in CI:** run the test suite under **ASan + UBSan** (and TSan for
  threads); they catch use-after-free, overflows, and UB before shipping.
- **Fuzzing:** coverage-guided fuzzing (libFuzzer/AFL++) + ASan on every parser of
  untrusted input (Chapter 26) — the highest-ROI way to find exploitable bugs.
- **Compiler hardening flags** for release builds:

```bash
g++ -O2 -D_FORTIFY_SOURCE=3 -fstack-protector-strong \
    -fstack-clash-protection -fcf-protection \
    -Wformat -Wformat-security -Werror=format-security \
    -fPIE -pie -Wl,-z,relro,-z,now app.cpp -o app
# Stack canaries, fortified libc calls, ASLR-friendly PIE, RELRO, CFI.
```

- **Static analysis:** clang-tidy `cppcoreguidelines-*`/`bugprone-*`, Clang Static
  Analyzer, Coverity/PVS-Studio for path-sensitive detection.

> The industry direction is **memory-safe by construction**: hardened C++ profiles,
> and migrating the most security-critical components to Rust. As a staff C++
> engineer, know both — write safe modern C++, and recognize when a component's risk
> justifies a memory-safe language.

---

## 27.3 Injection and Input Handling

The same injection rules as any language (separate data from code), plus
C++-specific ones:

```cpp
// SQL: use parameterized queries, never string concatenation (Chapter 29).
// stmt.prepare("SELECT * FROM users WHERE name = ?"); stmt.bind(1, name);

// Command execution: avoid system() with untrusted input; use exec-family with an
// argument array (no shell to interpret metacharacters).
// system(("convert " + userFile).c_str());   // ⚠ shell injection
// -> posix_spawn / execv with an argv array, userFile as one literal argument.

// Format strings: NEVER pass user input as the format string.
// printf(user_input);                          // ⚠ format-string vuln (%n, info leak)
printf("%s", user_input.c_str());               // user data is an ARGUMENT
std::print("{}", user_input);                   // type-safe (Chapter 20) — immune
```

Always **validate and bound** untrusted input (Chapter 26): check every
length/offset/size field from the wire *before* using it for allocation or
indexing — an unchecked length is the root of most overflow exploits.

---

## 27.4 Cryptography and Secrets

- **Don't roll your own crypto.** Use **libsodium** (high-level, hard to misuse) or
  OpenSSL/BoringSSL; use **authenticated encryption** (AES-GCM, ChaCha20-Poly1305)
  and never reuse a nonce.
- **Hash passwords** with a slow KDF — Argon2id (libsodium `crypto_pwhash`), bcrypt,
  or scrypt — never a fast hash.
- **Secrets** come from the environment / a secret manager, never the binary or repo
  (strings are trivially extracted from a binary with `strings`). Scan with gitleaks.
- **Constant-time comparison** for secrets/MACs (`sodium_memcmp`), and **zero
  sensitive memory** after use (`sodium_memzero` — a plain `memset` can be optimized
  away; `std::memset` to a buffer you then free is not guaranteed to run).

```cpp
sodium_memzero(key.data(), key.size());   // guaranteed wipe (memset may be elided)
```

---

## 27.5 The Software Supply Chain

C++ has historically had the *weakest* supply-chain story (vendored sources, no
universal manifest), which makes discipline essential:

```text
   upstream source ──▶ vcpkg/Conan ──▶ your build ──▶ binary (static-linked deps)
        ▲                  ▲                ▲              ▲
   compromised        unpinned          build-time     a CVE in a vendored
   maintainer         version           code exec      transitive lib (no auto-update)
```

- **Pin exact dependency versions** in vcpkg/Conan manifests (Chapter 22) and verify
  checksums — reproducible builds are the foundation.
- **The vendoring trap:** C++ projects often copy dependency sources in-tree, so a
  CVE in a vendored library is invisible to scanners and never auto-updates. Track
  what you vendor.
- **Scan dependencies:** OWASP Dependency-Check, **Grype/Trivy** on the produced
  image/SBOM, and watch CVE feeds for your libraries (Chapter 25's openssl/zlib are
  perennial sources).

---

## 27.6 SBOMs, Signing, and Provenance

- **SBOM:** generate a CycloneDX/SPDX bill of materials (Conan/vcpkg can emit one)
  so consumers can scan your binary's components for vulnerabilities.
- **Signing & provenance:** sign release artifacts with **Sigstore/cosign**
  (keyless, recorded in the Rekor transparency log); attest build provenance with
  **SLSA** so pipeline tampering is detectable.
- **CVE triage at scale** (Chapter 32 of the other editions): prioritize by
  reachability — is the vulnerable code path actually called on untrusted input? —
  not raw CVSS, and SLA the actually-reachable criticals.

---

## 27.7 Summary

| Risk | Defense |
|---|---|
| **Memory safety (70% of CVEs)** | RAII + smart pointers + owning containers + bounds (`span`/`.at()`) |
| Detection | **ASan/UBSan/TSan + fuzzing** in CI (Chapter 22) |
| Binary hardening | `_FORTIFY_SOURCE`, stack protector, PIE/RELRO/CFI |
| Injection | parameterized queries, argv exec, never user-controlled format strings |
| Crypto/secrets | libsodium/OpenSSL, Argon2, env-injected secrets, constant-time + memzero |
| Supply chain | pin versions, track vendored deps, scan, SBOM, sign (Sigstore/SLSA) |

- C++ security is **first and foremost memory safety** — write modern safe-by-default
  C++ (Chapters 12, 19–20), and *detect* the rest with sanitizers and fuzzing.
- Harden release binaries, validate/bound all untrusted input, never roll your own
  crypto, and keep secrets out of the binary.
- Secure the supply chain with pinned deps, vendoring awareness, scanning, SBOMs,
  and signing — and know when a component's risk justifies a memory-safe language.

## Next Steps

- Run your parsers under libFuzzer + ASan and fix every crash (Chapter 22).
- Add the hardening flags to your Release build and verify with `checksec`.
- Replace any `char[]` + `strcpy`/`sprintf` with `std::string`/`std::format`.
- Generate an SBOM from your Conan/vcpkg manifest and scan it.
- Continue to **[Chapter 28: System Design & Distributed Systems](../28_system_design_distributed/README.md)**.
