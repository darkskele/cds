# docs

How cds fits together. One directory per module, each with a `README.md` and a `DECISIONS.md`.

## Diagram

```
producer thread(s) ──▶ queue ──▶ consumer thread(s)

one thread
   write in place ──▶ buffer ──▶ lent out ──▶ handed back in any order
```

## Modules

- **[queue](queue/README.md)**. Bounded lock-free queues, one per producer and consumer shape.
- **[buffer](buffer/README.md)**. Fixed memory written in place, then lent out without copying.

## Milestones

- [ ] Every assurance claim in the top `README.md` checked on every build
    - [ ] Warnings as errors, the full flag set, GCC and Clang
    - [ ] Clang build with `-Wfunction-effects`, every push, pop, lend and release marked `[[clang::nonblocking]]`
    - [ ] Clang build with `-Wunsafe-buffer-usage`
    - [ ] Hardened standard library in the debug and tsan presets
    - [ ] Compile-time tests over small capacities, every wrap point
    - [ ] Random operation sequences checked against a plain container
    - [ ] `static_assert` on alignment and padding of every shared cursor
    - [ ] Memory ordering checked by a memory-model checker (GenMC or Relacy)
- [ ] CI on GCC 16 and a recent Clang, C++26 features in use
