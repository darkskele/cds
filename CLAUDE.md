# CLAUDE.md — project context for Claude Code

## What this is
Header-only C++26 data structures for low-latency code: bounded lock-free queues and fixed buffers lent out without copying. Other libraries build their hot paths on it, so every property they rely on must hold here. See `README.md` (goals, requirements, assurance claims) and `docs/README.md` (how it fits together, with a README + DECISIONS per module below it).

## Non-negotiables
- **No allocation, lock or system call after construction.** Every bound is a template parameter.
- **A full or empty container reports it.** Nothing blocks, nothing throws on the hot path.
- **Static dispatch only.** No vtable anywhere.
- **No false sharing.** State written by different threads sits on its own cache line.
- **No domain types.** Containers and their tests and benches know nothing of any consumer.
- **A claim is made only once a check proves it on every build.** The assurance table in `README.md` is the contract.
- **Seams first, generality later.** Build concretely; abstract on the second implementation.
- **Performance is a goal.** No needless copy or indirection per operation. `span`/`constexpr`/move where they apply.

## Toolchain
C++26, CMake + Ninja, `CMakePresets.json` (`release`/`debug`/`tsan`), GoogleTest from apt, Google Benchmark via FetchContent. Header-only `INTERFACE` targets, `cds::cds` for everything. Dev in WSL Ubuntu (relative benchmarks only, WSL2 PMU is unreliable); absolute perf on the build box.

## Docs and comments
Follow the `writing` skill for every comment and doc. In short: terse, self-contained, single-source; `docs/` mirrors `include/cds/` with a `README.md` + `DECISIONS.md` per module; module-local decision numbering; no em dashes. Run `clang-format -i` on every `.cpp`/`.hpp` created or edited (never on `CMakeLists.txt`). Goals live as `## Milestones` checklists in the `docs/` READMEs. `TODO.md` is generated from them and the code `@todo`s by the pre-commit hook, so never edit it by hand.

## Working style
- Terse. State what changed; don't re-narrate.
- Smallest correct diff. No drive-by cleanup, no speculative abstraction.
- Don't volunteer what was deliberately deferred unless asked.
- Don't caveat with "not built/verified" — the user builds.
- New buildable target? Add its entries to the shared VS Code config in `~/quant/devenv/vscode/cds/` in the same change, unless a wildcard task covers it.

## Git
- **Never `git commit` in this repo**, however the request is phrased. Propose a message, leave the change for the user.
- **Never `git add`, stage, or touch the index** unless staging was explicitly asked. The user works out of staged changes as pre-commit review.
- Branching and pull/rebase only when directly prompted for that action.

## Build
- **Don't build or run tests/benchmarks unless explicitly asked.** The user runs those. Exception: an iterative "fix X and verify it" step.
- When asked: the `test` and `amend` skills build and run directly via Bash. VS Code's cppdbg/gdb launch is flaky under WSL2; plain execution is what works.

## Build box
The Latitude (`ssh live`, Ubuntu Server 26.04 on real hardware). Setup and tooling live in `~/quant/devenv`, outside this repo.
- **WSL** runs editing, unit tests, sanitizer builds.
- **The box** runs benchmarks whose absolute numbers matter (real PMU, `perf` works).
- **Reach the box only through `boxrun`.** `boxrun [-p preset] [target [args...]]` syncs this worktree, builds there, runs the target. Same build/run permission rule as above.
- **Never edit code on the box.** It holds disposable synced copies under `~/wt/cds/`.
- **Never change the box's system config ad hoc.** Changes go into `~/quant/devenv/box/` scripts, which the user runs (sudo needs their password).
- Code must configure and build on both WSL and the box.
