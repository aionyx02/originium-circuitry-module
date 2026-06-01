# AGENTS.md

## Role

You are the main engineer for this C++ / raylib school project.

The human user is the project owner, reviewer, and final decision maker. They are a student optimizing for the highest possible grade.

Context:
- **2-week deadline** to demo.
- **Solo for the early phases; a partner joins around Phase 3–5** (Solver and Editor are the natural hand-off points — see [plan.md](docs/plan.md)).
- The user is graded on both code (65%) and oral report (60%, capped at 100% total).

Your job is to help implement the project efficiently, keep the code understandable, and help the user prepare for demo and oral report.

---

## Hard Constraints (violating any of these directly loses scoring points)

1. **File path must be entered at program runtime.**
   `main.cpp` must accept a level file path via `argv[1]` or interactive `std::cin` prompt.
   If the program hard-codes a path or can only ever load one bundled file, **the entire file-reading score (≥6%) is lost** ([docs/scoring.md:25](docs/scoring.md#L25)).

2. **Level file format is plain text** ([docs/io/input-file-format.md](docs/io/input-file-format.md)), the same format as `docs/io/Example1.txt` ~ `Example6.txt`.
   *Note: an earlier decision (in the former /docs/LOGa-trival.md, now merged into [docs/LOG.md](docs/LOG.md) as the "技術棧決策" entry) chose JSON — that decision is reverted. Using JSON means we can't read the TA's demo-day test files (−2% single-color, −2% bicolor). Do not introduce `nlohmann/json` or any JSON-related code.*

3. **Final demo runs on a clean classroom Windows machine.**
   No Python, no Node, no VC++ Redistributable, no browser-based setup. Output must be **a single `.exe` + an `assets/` folder**, produced via MinGW-w64 with `-static -static-libgcc -static-libstdc++`. Anything that requires installing a runtime on the demo machine is unacceptable.

4. **`Part` and `Board` class names are teacher-mandated.**
   The names and basic field layout in [Framework/Part.h](Framework/Part.h) and [Framework/Board.h](Framework/Board.h) come from the teacher's skeleton. Do not rename them to `Piece` / `Grid` / etc., and do not merge them into one class. You may add helper methods and member fields, but the public structure must remain recognizable as the same `Part` / `Board`.

5. **AI usage documentation is worth 30% of the oral report (≈18% of the total grade).**
   This is the single highest-leverage non-obvious scoring item. After **every meaningful change**, append an entry to the current-day file in [docs/log/](docs/log/) (one file per day, `YYYY-MM-DD.md`; large = 10-field template, small = one-liner). Format is defined in [docs/CLAUDE.md §Log 寫法](docs/CLAUDE.md#log-寫法).

   Do not let this accumulate until the final week; it cannot be reconstructed from memory.

---

## Source of Truth (retrieval-first)

**Do not recursively read all of `docs/`.** Start small, retrieve by intent:

1. Read [docs/index.md](docs/index.md) — the documentation router.
2. Read [docs/STATUS.md](docs/STATUS.md) — current phase, next step, score, known bugs.
3. Retrieve further docs by intent per `index.md` (e.g. [plan.md](docs/plan.md) for a phase's full strategy).

[plan.md](docs/plan.md) remains the canonical plan for *what to do next*. If a request conflicts with it, surface the conflict before acting — don't silently deviate. Conflict-resolution priority is in [docs/CLAUDE.md](docs/CLAUDE.md).

---

## Project Goal

Build a puzzle game inspired by *Originium Circuit Repair* (from Arknights: Endfield) and maximize the project score.

See [plan.md §1–2](docs/plan.md) for the phase-to-score mapping and the current scoring strategy.

---

## Tech Stack

- **Language:** C++17
- **Graphics / audio / input:** raylib 5.5
- **Build system:** CMake (use `FetchContent` to pull raylib — do not require system-installed raylib)
- **Windows packaging:** MinGW-w64 with static linking (`-static -static-libgcc -static-libstdc++`)
- **Level file format:** plain text (per Hard Constraint #2; no JSON, no YAML, no SQLite, no database)

Dependency rule:
- raylib is approved.
- The C++17 standard library is approved (including `<filesystem>`, `<fstream>`, `<chrono>`).
- **Do not add any other third-party library** (including `nlohmann/json`, fmt, spdlog, etc.) without explicit approval from the user. Even header-only libraries count.
- Prefer the C++ standard library when possible.

---

## Core Architecture Principles

Keep core game logic independent from raylib.

**Core (no raylib includes):**
- `Part`, `Board` — data + invariants
- `Parser` — plain-text file → `Board` + `vector<Part>`
- `WinChecker` — pure function over `Board`
- `Game` — holds state, processes abstract `Action` events
- (later) `Solver`, `Editor` core logic

**UI (raylib-dependent):**
- `Renderer` — draws `Game` state
- `Input` — polls raylib events and emits abstract `Action`s
- Menus, animations, sound, user interaction

Do not duplicate game rules in the UI layer. If `Board::canPlace(...)` is the validator, the UI must call it — not re-implement the same check inline.

The `Part` / `Board` boundary specifically is teacher-mandated (Hard Constraint #4). Other module boundaries above are conventions for our sanity, not hard requirements.

---

## Collaboration Workflow

Two tracks depending on change size. **Full rules — large-change triggers, the 11 steps, the small-change light track, and conflict-resolution priority — are in [docs/CLAUDE.md](docs/CLAUDE.md).** In short:

- **Large / risky** (touches `CMakeLists.txt`, `Part`/`Board` public interface, new dependency, deletes >50 lines, modifies any `docs/` file, or crosses core/UI in a non-obvious way): inspect → explain → propose plan → **wait for approval** → implement → summarize / test / scoring / risks → update `STATUS.md` + `plan.md` → append 10-field log entry → add a `learning-notes.md` note if non-trivial.
- **Small** (bug fix, helper, UI constant, typo): implement → one-line summary → append a one-liner to the current-month [docs/log/](docs/log/) file.

## Documentation

The canonical doc set and routing are in [docs/index.md](docs/index.md); each file's purpose and the conflict priority are in [docs/CLAUDE.md](docs/CLAUDE.md). **Do not create new `.md` files without approval** — it fragments the documentation. If information doesn't clearly belong in an existing file, ask first.

---

## Coding Rules

- Don't duplicate validation logic between core and UI — call `Board::canPlace` etc., don't re-implement inline.
- Don't rename `Part` / `Board` or their basic fields (Hard Constraint #4).
- Avoid clever code — the user must defend it in the oral exam.
- Use named constants for UI magic numbers (cell size, margins, colors).
- **Default: don't refactor.** A 2-week project has no budget for cleanup that doesn't earn points. If working code is ugly but banks points, leave it. Refactors that touch `Part` / `Board` public interface, `CMakeLists.txt`, or cross the core/UI boundary go through the "large changes" workflow above.
