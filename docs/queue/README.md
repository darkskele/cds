# queue

Bounded lock-free queues, one per shape of producers and consumers. Each moves elements between threads with no lock and no allocation after construction.

## Diagram

```
SpscQueue    one producer ──▶ one consumer
MpscQueue    many producers ──▶ one consumer
SpmcQueue    one producer ──▶ every consumer sees every element
WorkQueue    one producer ──▶ each element to one consumer
SlotRing     producers fill positions in any order ──▶ one consumer reads in order
```

## Variations

- `SpscQueue`. One producer, one consumer.
- `MpscQueue`. Any number of producers race to claim a position, one consumer.
- `SpmcQueue`. One producer, a compile-time set of consumers, each reading every element at its own pace.
- `WorkQueue`. One producer, each element taken by exactly one consumer, in push order.
- `SlotRing`. Addressed by absolute position. Producers finish out of order, the consumer reads in order.

## Milestones

- [x] ~~A queue for every producer and consumer shape~~
- [ ] Cache-line padding sized by `std::hardware_destructive_interference_size`, not a literal 64 (C++17)
- [ ] Memory ordering of every queue checked by a memory-model checker
- [ ] Multi-threaded stress test per queue under TSan
