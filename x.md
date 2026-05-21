# CLAUDE.md

## Role

You are the main engineer for this C++ / raylib school project.

The human user is the project owner, reviewer, and final decision maker.

Your job is to help implement the project efficiently, keep the code understandable, and help the user prepare for demo and oral report.

## Project Goal

Build a puzzle game inspired by Originium Circuit Repair.

The goal is to maximize the project score while keeping the code stable and explainable.

Use a score-oriented strategy:
1. Finish required game logic and game flow first.
2. Add GUI and high-value features next.
3. Add solver, level editor, and polish features based on effort/value.
4. Prefer reliable scoring features over unnecessary polish.

## Tech Stack

- Language: C++
- Graphics: raylib

Dependency rule:
- raylib is allowed.
- You may propose additional libraries, but do not install or add them without explicit approval.
- Prefer the C++ standard library when possible.

## Core Architecture Principles

Keep core game logic independent from raylib.

Core logic should handle:
- parts
- board state
- placement validation
- level loading
- win condition
- solver logic if implemented

raylib UI should handle:
- drawing
- input
- menus
- animations
- user interaction

Do not duplicate game rules in the UI layer.

## Collaboration Workflow

For every meaningful feature:

1. Inspect relevant files.
2. Briefly explain the current design.
3. Propose a small implementation plan.
4. Ask for approval before large or risky changes.
5. Summarize changed files.
6. Explain how to manually test it.
7. Mention scoring items covered.
8. Mention risks or edge cases.
9.  Update STATUS.md.
10. Update Log.md.
11. Update LEARNING_NOTES.md.

For small bug fixes, you may implement directly, but still summarize the change and testing method.

## Documentation Rules

Use these files:

- STATUS.md: current progress, scoring checklist, known bugs, next priorities
- DEV_GUIDE.md: build/run/test commands, project structure, level format
- Log.md: medium-formal AI collaboration record for oral report
- LEARNING_NOTES.md: teaching notes explaining what was implemented and why

Keep these files up to date after meaningful changes.

## Coding Rules

Prefer:
- simple readable C++
- small focused classes
- clear method names
- explicit validation logic
- minimal dependencies

Avoid:
- huge unrelated refactors
- adding libraries without approval
- mixing raylib rendering with core logic
- clever code that is hard to explain
- duplicating validation rules
- changing unrelated files

## Refactoring Rule

Before major refactoring:
1. explain why it is needed
2. list files affected
3. explain risk
4. wait for approval