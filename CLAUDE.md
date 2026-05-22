# CLAUDE.md

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
   This is the single highest-leverage non-obvious scoring item. After **every meaningful change**, append an entry to [docs/LOG.md](docs/LOG.md):
   - **Large changes** (Phase milestone / new feature / decision / refactor): use the 10-field template defined at the top of LOG.md.
   - **Small changes** (bug fix / UI tweak / helper): use the one-liner format defined in LOG.md's "大事 vs 小事" section.

   Do not let this accumulate until the final week; it cannot be reconstructed from memory.

---

## Source of Truth

[plan.md](docs/plan.md) is the canonical plan for the entire project. Before starting any task:

1. Open `plan.md` and find the current Phase.
2. Read that Phase's section in full.
3. Work from the checklist there.

If a request seems to conflict with `plan.md`, surface the conflict before acting — don't silently deviate.

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

Two tracks depending on change size.

### Large or risky changes — full workflow

Trigger any of the following → treat as "large":
- Touches `CMakeLists.txt`
- Changes the public interface of `Part` or `Board`
- Introduces a new third-party library or dependency
- Deletes more than ~50 lines
- Modifies any file under `docs/`
- Modifies [docs/plan.md](docs/plan.md) or [x.md](x.md) (this file)
- Crosses the core/UI boundary in a non-obvious way

Steps:
1. Inspect relevant files.
2. Briefly explain the current design.
3. Propose a small implementation plan.
4. **Wait for the user's approval** before writing code.
5. After implementing: summarize changed files.
6. Explain how to manually test it.
7. Mention scoring items covered (cite line numbers in [docs/scoring.md](docs/scoring.md)).
8. Mention risks or edge cases.
9. Update [plan.md](docs/plan.md) timeline section, and tick checklist items in [docs/STATUS.md](docs/STATUS.md).
10. Append a 10-field entry to [docs/LOG.md](docs/LOG.md).
11. Add a topic-organized note to [docs/learning-notes.md](docs/learning-notes.md) if the feature introduces a non-trivial concept the user must explain in the oral exam.

### Small changes — light workflow

Bug fixes, helper additions, tweaking UI constants, adding a test case, fixing a typo:

1. Implement directly.
2. One-sentence summary of what changed and how to verify.
3. Append a one-liner entry to [docs/LOG.md](docs/LOG.md) (see LOG.md's "大事 vs 小事" section).

`plan.md` and `STATUS.md` don't need updating for small fixes unless they complete a checklist item.

---

## Documentation Files

These files are the canonical set. **Do not create new `.md` files** without approval — that fragments the documentation.

Each file has a single purpose; if information could go in two places, prefer the one whose purpose it matches more tightly.

- **[plan.md](docs/plan.md)** — phases, scoring strategy, progress timeline. The single source of truth for *what to do next*.
- **[docs/STATUS.md](docs/STATUS.md)** — current progress + scoring checklist + known bugs. Tick checklist items as features land. Do not store plans or decisions here.
- **[docs/DEV_GUIDE.md](docs/DEV_GUIDE.md)** — onboarding Quick Start: build / run / test commands, project layout, how to load test data. Keep concise — a teammate joining mid-project should be able to start work from this file alone.
- **[docs/LOG.md](docs/LOG.md)** — AI collaboration log. Append after every meaningful change (large = 10-field template, small = one-liner). Source for the oral-report "如何用 AI 實作程式" section (30% of oral grade — see Hard Constraint #5).
- **[docs/learning-notes.md](docs/learning-notes.md)** — topic-organized teaching notes per non-trivial feature/module: how it works, why this design, what the user must be able to explain. Source for the oral-report "對程式的熟悉度" section (15%).
- **[my-note.md](docs/my-note.md)** — user's personal scratchpad. Do not write here unless the user asks.

If a piece of information doesn't clearly belong in one of the above, ask before creating a new file.

---

## Coding Rules

- Don't duplicate validation logic between core and UI — call `Board::canPlace` etc., don't re-implement inline.
- Don't rename `Part` / `Board` or their basic fields (Hard Constraint #4).
- Avoid clever code — the user must defend it in the oral exam.
- Use named constants for UI magic numbers (cell size, margins, colors).
- **Default: don't refactor.** A 2-week project has no budget for cleanup that doesn't earn points. If working code is ugly but banks points, leave it. Refactors that touch `Part` / `Board` public interface, `CMakeLists.txt`, or cross the core/UI boundary go through the "large changes" workflow above.
