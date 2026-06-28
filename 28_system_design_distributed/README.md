# Chapter 28: System Design & Distributed Systems

The previous chapters made a *single C++ process* fast, correct, and operable. This
chapter is about what happens when one process is not enough — when you split work
across machines that fail independently, communicate over an unreliable network, and
must still present a coherent system. This is the material a staff/principal engineer
reasons about daily: **latency vs consistency, partitioning, replication, consensus,
caching, and the failure modes that emerge only at scale.** The principles are
language-agnostic; the examples are C++.

> **Why C++ is often the node, not the orchestrator:** C++ frequently *is* the
> high-performance backend (databases, storage engines, exchanges, caches) that
> other services depend on. Its job is to be the fast, predictable node in a
> distributed system — which makes tail latency (Chapter 24) and these distributed
> concerns equally its problem.

---

## 28.1 The Fallacies and the Latency Numbers

Every distributed-systems bug traces to forgetting one of the **eight fallacies**:
the network is *not* reliable, *not* zero-latency, *not* infinite-bandwidth, *not*
secure, *not* free, topology *changes*, there is *not* one administrator, and it is
*not* homogeneous. Design as if every remote call can be slow, fail, or duplicate.

```text
L1 cache ~1 ns · main memory ~100 ns · SSD read ~16 µs
same-DC round trip ~500 µs · disk seek ~2 ms · CA<->EU round trip ~150 ms
```

**Memory ~100× SSD ~100× a network hop; cross-continent ~300× same-DC.** This is why
we cache, batch, and co-locate — and why "just add a network call" is never free.
(In C++ you've already removed GC jitter from your node, Chapter 24 — don't add it
back as needless network chatter.)

---

## 28.2 Scaling and Statelessness

- **Vertical scaling** (bigger machine) is simplest — often right first; hits a
  ceiling.
- **Horizontal scaling** (more machines) is unbounded but requires **stateless**
  services: any instance serves any request because no request-affecting state lives
  in process memory. State moves to shared backing services (DB, cache, blob store).

```text
            ┌── instance 1 ──┐
client ─▶ LB ┼── instance 2 ──┼─▶ shared state (DB / cache / queue / blob)
            └── instance N ──┘
```

> **Sticky sessions are a smell** — they reintroduce state and break elasticity and
> failover. Keep instances fungible.

---

## 28.3 CAP and PACELC

**CAP**: during a network **P**artition you choose **C**onsistency or
**A**vailability — not both. **PACELC** sharpens it: **if P**artitioned choose **A**
or **C**; **E**lse (normal) choose **L**atency or **C**onsistency. Every store sits
on this map (CP RDBMS, AP/tunable Dynamo/Cassandra, CP etcd/ZooKeeper).

The takeaway: **consistency is a per-operation choice.** "Add to cart" can be
eventually consistent; "charge the card" cannot. Choose per data flow.

---

## 28.4 Consistency Models

From strongest/most expensive to weakest/cheapest:

- **Linearizable:** every op appears atomic at one instant; once a write returns, all
  reads see it. The single-machine model — costly across nodes.
- **Causal:** causally related ops are ordered for everyone; concurrent ones may
  differ. Often enough, much cheaper.
- **Read-your-writes / monotonic reads:** a client sees its own writes, never goes
  backward — usually the minimum users notice.
- **Eventual:** replicas converge if writes stop — says nothing about *when*.

The classic footgun: write to the primary, immediately read a stale replica, "lose"
your own update. Fix with read-your-writes (route a just-written client to the
primary briefly). **Eventual consistency is a contract, not a bug** — name the
guarantee each path needs.

---

## 28.5 Partitioning (Sharding)

- **Range** — contiguous key ranges; great for scans, prone to hot spots (e.g.
  partitioning by timestamp).
- **Hash** — `hash(key) % N`; even load, no scans; naive `% N` remaps everything when
  `N` changes.
- **Consistent hashing** — keys and nodes on a ring; a key goes to the next node
  clockwise, so adding/removing a node remaps only `~1/N` keys. **Virtual nodes**
  smooth imbalance.

```cpp
#include <map>
#include <functional>
class HashRing {                          // consistent hashing with virtual nodes
    std::map<size_t, std::string> ring_;  // hash -> node (ordered for clockwise lookup)
    int vnodes_;
public:
    HashRing(const std::vector<std::string>& nodes, int vnodes) : vnodes_(vnodes) {
        for (auto& n : nodes) add(n);
    }
    void add(const std::string& node) {
        for (int v = 0; v < vnodes_; ++v)
            ring_[std::hash<std::string>{}(node + "#" + std::to_string(v))] = node;
    }
    const std::string& node_for(const std::string& key) const {
        size_t h = std::hash<std::string>{}(key);
        auto it = ring_.lower_bound(h);                 // next node clockwise
        if (it == ring_.end()) it = ring_.begin();      // wrap around
        return it->second;
    }
};
```

The hard parts are **rebalancing** (moving data when shards change) and **cross-shard
operations** (joins/transactions spanning shards — expensive, usually avoided by
denormalizing, Chapter 29).

---

## 28.6 Replication and Quorums

Replicate for **availability** and **read scaling**:

- **Single-leader:** writes to the leader, streamed to followers (stale by the
  replication lag); failover on leader loss is the dangerous moment.
- **Multi-leader / leaderless:** multi-region writes or Dynamo-style quorums.

**Quorum math:** with `N` replicas, `W` write acks + `R` read acks give strong
consistency when **W + R > N** (the sets overlap). `N=3, W=2, R=2` is a common
balanced default. **Replication lag** is something you design around (read-your-
writes), not ignore.

---

## 28.7 Consensus, Coordination, and Locks

Some decisions need all nodes to agree exactly once (leader, lock, commit order):
**consensus**, solved by **Raft/Paxos** and exposed by **etcd/ZooKeeper/Consul**.
A majority quorum must be alive (5 nodes tolerate 2 failures). Don't build a lock
from a DB row under load — use the coordination service.

> **Distributed locks need fencing tokens.** A node can acquire a lock, stall (a
> page fault, scheduler preemption, or — in mixed systems — a GC pause elsewhere),
> have its TTL expire and the lock reassigned, then wake and act → two holders. The
> lock service must issue a monotonically increasing **fencing token** that the
> protected resource checks. A lock without fencing reduces contention but is not
> safe for correctness.

---

## 28.8 Time, Ordering, and Idempotency

There is **no global clock** — wall-clock time drifts and jumps across machines
(never order distributed events by `system_clock`; Chapter 23). Use **logical
clocks** (Lamport) for happens-before and **vector clocks** to detect concurrency.

Because the network duplicates and retries, **idempotency** is what makes a system
survivable — doing an operation twice equals once:

```cpp
// Idempotency key: the client sends a unique key; the server dedupes.
Charge charge(const std::string& key, Money amount, const Card& card) {
    if (auto prior = store.find(key)) return *prior;     // already done — same result
    Charge result = gateway.charge(amount, card);
    store.put(key, result, 24h);                          // Chapter 23 chrono literal
    return result;                                        // a retry can't double-charge
}
```

Design writes to be **idempotent or idempotency-keyed** from day one — the most
leveraged habit for surviving retries and at-least-once delivery.

---

## 28.9 Communication and Caching

- **Sync (REST/gRPC, Chapter 30):** simple, but couples caller and callee in time and
  latency. **Async (queue/log, Chapter 29):** decouples, absorbs spikes, enables
  retries — at the cost of eventual consistency.
- **Delivery:** at-most-once (lossy), **at-least-once** (the common default — pair
  with idempotent consumers), effectively-once (idempotency + dedup).
- **Caching** (Chapter 24 covered in-process, e.g. an LRU) is the top distributed
  perf lever: CDN → reverse proxy → in-process → distributed (Redis) → DB. Beware
  the two classic failures — **stampede** (hot key expires, herd hits the DB; fix
  with single-flight / jittered TTL) and **invalidation** (prefer short TTLs and
  event-driven invalidation).

---

## 28.10 Designing for Failure

At scale *something is always broken*; keep partial failure partial:

- **Redundancy / no SPOF:** N+1 replicas, multi-AZ, multi-region for top tiers.
- **Blast-radius reduction:** **bulkheads** (isolate pools), **cell architecture**
  (independent cells), **graceful degradation** (serve stale/partial results).
- **Backpressure & load shedding** (Chapters 24, 25): bound queues, shed early.
- **Chaos testing:** you don't know you survive failure until you cause it.

> **Metastable failure:** under retry amplification or cache stampede a system can
> stay stuck failed after the trigger is gone, because the load it generates sustains
> the failure. Retry budgets, circuit breakers, and load shedding break the loop.

---

## 28.11 Summary

- Distributed systems are **partial failure over an unreliable network**;
  internalize the fallacies and latency numbers.
- Scale **horizontally with stateless services**; make **consistency a per-operation
  choice** (CAP/PACELC) and design around **replication lag**.
- **Partition** with consistent hashing, **replicate** with quorums (`W+R>N`), use
  **consensus systems** for the few decisions needing agreement — with **fencing
  tokens** for locks.
- No global clock: order with logical/vector clocks, make writes **idempotent**, and
  assume **at-least-once** delivery.
- **Cache** aggressively (mind stampede/invalidation) and **design for failure**
  (redundancy, bulkheads, backpressure, chaos) — beware **metastable** loops.

## Next Steps

- Implement the `HashRing` and confirm only ~1/N keys move when a node joins.
- Add idempotency keys to a write path and prove a duplicate request is a no-op.
- Pick a feature and name the consistency model each of its data paths needs.
- Continue to **[Chapter 29: Data-Intensive Systems](../29_data_intensive_systems/README.md)**.
