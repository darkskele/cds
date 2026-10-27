# cds

Custom data structures for low-latency C++. Bounded lock-free queues and fixed buffers lent out without copying. Header-only, C++26, Linux.

## Goals

- The fastest correct container for each job, benched in isolation and tuned there.
- Safe to build other libraries on. Every property a consumer relies on is proven by a check that runs on every build.

## Requirements

Low latency
- No allocation, lock or system call after construction.
- No copy where a view or a move does the job.
- Shared state padded to its own cache line, no false sharing.
- Capacities fixed at compile time. Every bound known up front.
- Static dispatch only. No virtual calls.

Ease of use
- A container is one type with its sizes as template parameters.
- Full containers report it and the caller decides. Nothing blocks.
- Misuse a type can catch fails to compile.

Assurance

| Claim | Checked by |
|---|---|
| No undefined behavior | ASan and UBSan preset, hardened standard library in test builds, compile-time tests |
| No data races | TSan preset with multi-threaded stress tests per queue |
| No allocation or blocking after construction | `[[clang::nonblocking]]` on every push, pop, lend and release, `-Wfunction-effects` in a Clang build |
| Index and capacity logic correct | Compile-time tests over small capacities, every wrap point |
| Behaves like a plain container | Random operation sequences checked against `std::deque` |
| Layout as intended | `static_assert` on cache-line alignment and padding |
| Memory ordering correct beyond x86 | A memory-model checker. x86 hides ordering bugs that weaker CPUs expose. |

A claim is made only once its check runs automatically. Progress is in the `docs/` milestones.

## Build

Toolchain. GCC 16 or a recent Clang for C++26, CMake 3.25 or newer, Ninja, GoogleTest from apt. Google Benchmark comes through FetchContent.

```bash
cmake --preset release
```

```bash
cmake --build --preset release
```

```bash
./build/release/cds_tests
```

- Presets. `release`, `debug` (ASan and UBSan), `tsan`.
- `cds_bench` runs every bench, writes `bench_results.json` and regenerates `BENCHMARKS.md` with a diff against the committed results.
- On the build box, `boxrun cds_tests` syncs, builds and runs there. Benches whose absolute numbers matter run there with `boxrun -x`.

## Use from another CMake project

```cmake
add_subdirectory(extern/cds)
target_link_libraries(my_target PRIVATE cds::cds)
```

Headers are included as `"cds/queue/spsc_queue.hpp"`. Tests and benches build only when cds is the top-level project.
