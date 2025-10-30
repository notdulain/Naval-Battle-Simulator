# Naval Battle Simulator

A first-year programming assignment taken to fleet-command scale. This C-based simulator lets you stage battles between a lone battleship and waves of escort ships, experiment with firing strategies, and capture the run in detailed logs for later analysis.

## Overview

The project is split across a set of progressively richer simulations (Part 1 A–C and Part 2 A). Each part builds on the same naval battlefield abstraction while layering in new mechanics such as movement trajectories, weapon reliability, impact power tracking, and reload-aware target prioritisation. Source code lives in platform-neutral C; the repository also keeps sample outputs produced during development.

## Feature Highlights

- **Configurable battlefield** — interactively set grid dimensions, choose a battleship class, and pick the escort mix before every run.
- **Probabilistic combat logic** — random generators seed ship placement, velocity bands, and firing arcs to create varied encounters.
- **Sequential evolution of mechanics**
  - *Part 1A*: Core combat loop, hit detection, battle canvas, and run bookkeeping.
  - *Part 1B*: Battleship pathing plus gun malfunction scenarios to stress decision-making.
  - *Part 1C*: Escort impact power tracking so cumulative damage determines survival.
  - *Part 2A*: Reload-aware firing order to maximise enemy losses while minimising incoming damage.
- **Rich telemetry** — every module records the initial setup, per-run statistics, and final battlefield snapshot to plain-text reports.

## Repository Tour

- `Part-1-A/Part-1-A.c` — Baseline simulation and logging pipeline shown in the supplied `.txt` artefacts.
- `Part-1-B/Simulation_1/Part-1-B-Sim1.c` — Adds predefined movement paths and escorts that exploit battleship gun failures.
- `Part-1-B/Simulation_2/Part-1-B-Sim2.c` — Alternative experiment for Part 1B with tweaked movement logic.
- `Part-1-C/Simulating A/Part-1-C_A.c` & `Part-1-C/Simulating B/Part-1-C_B.c` — Re-run Part 1 scenarios while tracking impact power and accumulated hull damage.
- `Part-2-A/Simulating_Part-1-A/Part-2-A_A.c` & `Part-2-A/Simulating_Part-1-B/Part-2-A_B.c` — Replay earlier stages with reload times and a prioritised attack queue.
- `*.txt` inside each folder — Captured initial conditions, final states, per-run statistics, and rolling run counts; handy references for expected output.
- `IT23750760-Report.pdf` — The full write-up that narrates the development journey and design rationale.

## Building & Running

1. Choose the module you want to explore (for example Part 1A).
2. Compile with `gcc` (linking the math library where required), e.g.:

   ```sh
   gcc Part-1-A/Part-1-A.c -std=c99 -lm -o part1a
   gcc Part-1-B/Simulation_1/Part-1-B-Sim1.c -std=c99 -lm -o part1b_sim1
   gcc Part-1-C/Simulating\ A/Part-1-C_A.c -std=c99 -lm -o part1c_a
   gcc Part-2-A/Simulating_Part-1-A/Part-2-A_A.c -std=c99 -lm -o part2a_a
   ```

3. Run the resulting executable. Each program will prompt for battlefield parameters and generate output artefacts in its folder.
4. Inspect `Initial_Conditions.txt`, `Simulation_Statistics.txt`, `Final_Conditions.txt`, and `Run_Count.txt` to review what happened.

> 💡 The repository ships with `.exe` binaries compiled on Windows; recompile locally for your platform to avoid compatibility issues.

## Simulation Outputs

- `Initial_Conditions.txt` — Snapshot of the battlefield grid, ship roster, and derived ranges before the first shot.
- `Simulation_Statistics.txt` — Appended log of each run highlighting who fired, who scored hits, and how the encounter ended.
- `Final_Conditions.txt` — Battlefield canvas after the simulation, including destroyed ships and remaining positions.
- `Run_Count.txt` — Simple counter that tracks how many simulations have been executed for that module.

## Implementation Notes

- **Data modelling** — Battleships and escort ships are C structs with shared attributes (position, velocity, firing angles) plus bespoke data (impact power, IDs, malfunction flags). Paths in Part 1B are modelled as waypoint arrays.
- **Geometry utilities** — Helper functions convert between radians and degrees, calculate projectile range, and measure Euclidean distance to decide if a target is hittable.
- **Firing logic** — Combat is evaluated in two passes (battleship → escorts, escorts → battleship). Reload-aware modules add queues that prioritise targets by proximity and risk before consuming remaining reload budget.
- **Stateful logging** — Each run reuses the same text files, appending statistics and rotating the run counter so you can compare multiple attempts without losing history.

## Future Ideas

The accompanying report proposes extending the sandbox with weather modifiers, richer path-finding, graphical front-ends, and weapon degradation for both factions. Those enhancements would push the simulator closer to tactical training software while deepening the programming challenge.

---

Curious about the full development story? Dive into `IT23750760-Report.pdf` for methodology, challenges, and reflections straight from the author.
