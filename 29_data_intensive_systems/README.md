# Chapter 29: Data-Intensive Systems

Most large systems are bottlenecked on **data**, not CPU — so data architecture *is*
system architecture. This chapter covers what sits underneath and around a database:
how storage engines store bytes, how indexes and transactions actually work, how to
choose SQL vs NoSQL, and how databases, caches, and streams compose into a data
platform. C++ engineers especially need this because C++ frequently *implements* the
data layer — storage engines, in-memory stores, and stream processors are written in
C++ precisely for the control this chapter explains.

> **C++ is often on both sides of the database boundary:** you call data stores from
> services *and* you build them (RocksDB, ScyllaDB, ClickHouse, kdb+ are C++). The
> storage-engine and memory-layout reasoning here connects directly to Chapter 24.

---

## 29.1 Storage Engines: B-tree vs LSM

Two families dominate; the choice shapes read/write latency:

- **B-tree (read-optimized):** PostgreSQL, MySQL/InnoDB. A balanced tree of pages;
  `O(log n)` lookups and range scans with few seeks; in-place writes (+ write-ahead
  log). Predictable reads, write amplification.
- **LSM-tree (write-optimized):** RocksDB, Cassandra, ScyllaDB. Writes hit an
  in-memory **memtable** + append log, flush to immutable **SSTables**, periodically
  **compacted**; reads may check several SSTables (mitigated by **Bloom filters**).
  Excellent write throughput; read/space amplification.

```text
B-tree:   read O(log n), in-place writes      ── read-heavy, range scans
LSM-tree: sequential writes, compaction       ── write-heavy ingest (logs, metrics)
```

RocksDB (a C++ LSM library) is the embedded engine behind countless systems —
understanding LSM internals is core C++ data-systems knowledge.

---

## 29.2 Indexes: The Top Performance Lever

An index turns an `O(n)` scan into an `O(log n)` lookup at the cost of slower writes
and storage:

- **Composite indexes are order-sensitive** (leftmost-prefix rule): `INDEX(a,b)`
  serves `WHERE a=?` and `a=? AND b=?`, not `b=?` alone.
- **Covering index:** contains all columns a query needs → answer without touching
  the table.
- **Selectivity:** a low-cardinality index (3-value `status`) may be ignored.
- **Write cost:** each index slows writes; drop unused ones.

**Always read the query plan** (`EXPLAIN ANALYZE`): seq scan on a big table is the
smell; watch estimated-vs-actual rows (stale stats → bad plans) and nested-loop
joins over large sets.

---

## 29.3 Transactions and Isolation

Transactions are **ACID**; the subtle property is **isolation**, which trades
correctness for concurrency:

| Level | Prevents | Allows |
|---|---|---|
| Read Committed (PG default) | dirty reads | non-repeatable reads, phantoms |
| Repeatable Read | non-repeatable reads | phantoms (mostly) |
| Serializable | everything | nothing (as if serial) |

Anomalies: dirty read, non-repeatable read, phantom, and **lost update / write
skew**. Most engines use **MVCC** (readers see a snapshot without blocking writers;
writers create new versions) — which is why long transactions bloat the DB with old
versions. Resolve contention with **pessimistic locks** (`SELECT ... FOR UPDATE`) or
**optimistic version checks** (a `version` column + CAS — the database analogue of
Chapter 18's compare-and-swap).

---

## 29.4 Normalization, SQL vs NoSQL

- **Normalize until joins hurt, then denormalize deliberately** for specific read
  paths (and own the consistency burden, Chapter 28).
- **Choose by access pattern, not hype:**

| Family | Examples | Sweet spot |
|---|---|---|
| Relational | PostgreSQL, MySQL | default; complex queries, strong consistency |
| Key-value | Redis, RocksDB, DynamoDB | caches, sessions, lookups at scale |
| Document | MongoDB | flexible schema, read-as-a-unit |
| Wide-column | Cassandra, Bigtable | massive write throughput, time series |
| Graph | Neo4j | relationship-heavy traversals |

**Start relational** (Postgres covers an enormous range); migrate off only when a
measured need forces it. Polyglot persistence (relational + Redis + search + OLAP) is
normal.

---

## 29.5 OLTP vs OLAP

- **OLTP:** many small low-latency reads/writes of current state; row-oriented,
  indexed, normalized — your application DB.
- **OLAP:** few huge aggregating scans; **column-oriented** (read only needed
  columns), compressed, denormalized star schemas — warehouses (ClickHouse,
  Snowflake, BigQuery).

Never run analytics on the OLTP store — a full-table aggregation locks up the store
users depend on. Move data via **ETL/CDC** (stream the replication log, e.g.
Debezium; or a transactional-outbox) into a columnar warehouse. (Column stores like
ClickHouse are C++ — their speed is exactly the cache/SIMD story of Chapter 24
applied to analytics.)

---

## 29.6 Messaging and Streaming

- **Message queues (RabbitMQ, SQS):** a message goes to one consumer, removed after
  ack — work distribution.
- **Event logs (Kafka, Redpanda):** append-only, partitioned, *retained*; consumers
  track their own **offset** and can replay; many groups read the same stream. The
  log is the source of truth.

```text
Queue: producer ─▶ [ m m m ] ─▶ one consumer per msg, then gone
Log:   producer ─▶ [ 0 1 2 3 4 ... ] ─▶ group A @5, group B @2  (retained, replayable)
```

Properties: **ordering** only within a partition (choose a partition key so related
events stay ordered); **at-least-once** delivery by default (consumers must be
**idempotent**, Chapter 28); **consumer lag** is the key health metric (alert on it);
route repeatedly-failing messages to a **dead-letter queue**. Commit offsets *after*
processing (at-least-once + idempotent), never before (at-most-once, lossy).

---

## 29.7 Data Modeling for Access Patterns

The relational instinct ("model entities, query later") breaks at scale and in
NoSQL. Invert it — **start from the queries**:

1. List read/write patterns with frequencies and latency targets.
2. Choose partition keys so each common query hits **one** partition (Chapter 28.5).
3. Denormalize so hot reads need no joins or scatter-gather.
4. Manage the resulting write fan-out and eventual consistency.

This is why single-table design is idiomatic in DynamoDB and time-series stores
partition by `(series, time-bucket)` — the model is shaped by the read path.

---

## 29.8 The Operational Edges

- **Connection pooling:** databases handle a bounded connection count; a fleet ×
  big per-instance pool can exhaust the DB — size pools deliberately, front Postgres
  with PgBouncer at high instance counts.
- **Hot partitions / hot rows:** a celebrity key or a monotonic write key
  (timestamp, autoincrement) concentrates load on one shard — design keys to spread.
- **Online migrations** (Chapter 31): adding an index or `NOT NULL` column can lock a
  huge table; use concurrent/online migrations and the expand/contract pattern so a
  schema change isn't an outage.

---

## 29.9 Summary

- Know your **storage engine** (B-tree vs LSM) — it predicts latency and fit; C++
  often *is* the engine (RocksDB, ClickHouse).
- **Indexes** are the top lever (leftmost-prefix, covering, selectivity); always
  **read the query plan**.
- **Transactions** trade isolation for concurrency (MVCC); resolve contention with
  pessimistic locks or optimistic versioning.
- **Normalize then denormalize deliberately**; pick SQL vs NoSQL by **access
  pattern**; separate **OLTP from OLAP** and move data with **ETL/CDC**.
- Use **queues** for work and **event logs** (Kafka) for replayable ordered streams;
  assume **at-least-once** with **idempotent** consumers; watch **consumer lag**.
- Model **from the queries**; respect pools, hot partitions, and online migrations.

## Next Steps

- Run `EXPLAIN ANALYZE` on a slow query, add a composite index, and compare.
- Sketch a Kafka topic's partition key and consumer-group layout for a feature, and
  mark where idempotency is required.
- Read the RocksDB wiki on LSM compaction and connect it to Chapter 24's cache story.
- Continue to **[Chapter 30: API & Interface Design](../30_api_design/README.md)**.
