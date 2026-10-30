# Protocol Zero: Development Plan

Design source of truth: [Protocol_Zero_Design.md](Protocol_Zero_Design.md). This plan turns that design into milestones and small, testable stories. It adds no mechanics the design doesn't name.

**Engine:** Unreal Engine 5.8 · **Language:** C++ first, with Data Assets for tuning · **UI:** plain UMG built in C++ · **Platform:** Windows PC, keyboard and mouse

---

## Status

| Milestone | Goal | Status |
|---|---|---|
| 1. Core combat slice | Move, aim, shoot and survive Hunter drones in a greybox arena | **Done** (11 of 11 stories) |
| 2. Full wave loop | A complete run: five waves, both drone types, pause, and victory or defeat results | **Next** (0 of 11 stories) |
| 3. Upgrades and HUD | Post-wave upgrade picks and the full combat HUD | Planned |
| 4. Presentation | Narrative, audio, settings, art pass, tuning and packaging | Planned |

---

## Architecture

C++ owns rules, state and math. Data Assets own every tuning number. Blueprints and art replace greybox visuals later without code changes. Rules that can be pure (health, fire cadence, melee, aim, movement, wave schedule, spawn rules, run state) live in engine-free C++ structs and functions, so they're unit-tested deterministically, with time passed in.

| Area | Built (Milestone 1) | Planned |
|---|---|---|
| Run flow | `APZGameMode` | Run state machine (waves, countdown, victory, defeat), `APZGameState` run stats |
| Player | `APZPlayerCharacter`, `APZPlayerController`, `PZMovementMath`, `PZAimMath` | Pause input |
| Combat | `UPZHealthComponent` / `FPZHealthState`, `UPZWeaponComponent` / `FPZFireCadence`, `APZProjectile`, `FPZMeleeAttack`, `UPZWeaponData` | Projectile count and spread (upgrades) |
| Enemies | `APZDrone`, `APZHunterAIController`, `UPZDroneData` (`DA_Hunter`), `PZDestructionMath` | Sentry drone and its AI controller (`DA_Sentry`) |
| Waves | `APZSpawnPoint` ×8 in `L_Arena_Greybox` | `UPZWaveData` (`DA_Wave_01..05`), wave schedule, spawn-point rules, wave manager |
| Upgrades | | Upgrade data, upgrade component, selection overlay |
| UI | `UPZHudWidget` (health), `UPZDefeatWidget` | Countdown, results screen, pause menu, later wave/enemies/upgrades HUD |
| Collision | `PlayerProjectile`, `EnemyProjectile`, `Enemy` channels and profiles | |

Tests live in `Source/Protocol_Zero/Private/Tests/`, named `Protocol_Zero.[System].[Scenario]`. 36 pass at the end of Milestone 1.

---

## Milestones

**Milestone 1: Core combat slice** (done)
- E1 Project foundation · E2 Player · E3 Weapon and projectiles · E4 Hunter drone · E5 Greybox arena

**Milestone 2: Full wave loop**
- E6 Wave and spawn system: timed spawn groups, five escalating waves, countdown between waves
- E7 Sentry drone
- E8 Run flow: pause, victory, defeat, results screen, restart

**Milestone 3: Upgrades and HUD**
- E9 Upgrades: three random picks after each cleared wave from four repeatable, percentage-based categories
- E10 HUD and damage feedback: wave, enemies remaining, current upgrades, screen flash, directional hit indicator

**Milestone 4: Presentation and shipping**
- E11 Narrative: intro text, Protocol Zero shutdown sequence and message
- E12 Audio: shooting, impact, enemy death and UI sound effects, one looping music track
- E13 Settings: master volume, mouse sensitivity
- E14 Art pass, tuning to the 8 to 12 minute run, Windows packaging

---

## Milestone 1 stories (done)

| ID | Story | Result |
|---|---|---|
| M1-S01 | Arena map as the default map, project GameMode, projectile and enemy collision channels | Done |
| M1-S02 | Health component with a post-hit invulnerability window and single death event | Done, 6 tests |
| M1-S03 | Player character: WASD movement relative to a fixed 45° isometric camera | Done, 4 tests |
| M1-S04 | Character continuously faces the mouse cursor | Done, 5 tests |
| M1-S05 | Plasma rifle: hold the left mouse button for automatic fire at the weapon cooldown | Done, 6 tests |
| M1-S06 | Plasma bolt with finite travel time; hits enemies and cover, never the player | Done, 3 world tests |
| M1-S07 | Greybox arena: walls, permanent cover, navmesh, 8 edge spawn points | Done (navmesh fixed in S08) |
| M1-S08 | Hunter drone: navmesh pursuit around cover, melee on a cooldown, no contact damage | Done, 5 tests |
| M1-S09 | Drone destruction effect, then despawn | Done, 4 tests |
| M1-S10 | Player death effect, defeat prompt, restart | Done |
| M1-S11 | Health readout on the HUD | Done, 3 tests |

---

## Milestone 2 stories

| ID | Story | Done when | Evidence |
|---|---|---|---|
| M2-S01 | **Wave data and schedule.** `UPZWaveData` Data Asset holding ordered spawn groups (drone type, count, delay after the previous group) plus health and damage multipliers. A pure `FPZWaveSchedule` reports which groups are due at a given time. | Tests: groups release in order at their delays; a wave reports when every group has released | Unit tests |
| M2-S02 | **Spawn-point rules.** Pure selection that picks edge spawn points outside a minimum distance from the player, with the distance as tuning. | Tests: points inside the minimum distance are never chosen; a fallback picks the farthest point if none qualify | Unit tests |
| M2-S03 | **Wave manager.** A component on the GameMode spawns each group at chosen spawn points, applies the wave's health and damage multipliers, tracks enemies remaining and fires a wave-cleared event when all are destroyed. The three placed test Hunters are removed. Waves are Hunter-only until S05. | Starting a wave spawns timed Hunter groups from the edges; killing them all fires wave cleared | Integration test + in-game log |
| M2-S04 | **Sentry drone.** A slower ranged drone that paths to its preferred range, stops, and fires enemy projectiles at the player on a cooldown. Its shots hit the player and cover only. Tuning in `DA_Sentry`. | Sentry holds range around cover and its shots damage the player and stop at cover | Unit tests (range logic) + in-game log |
| M2-S05 | **Five-wave roster.** `DA_Wave_01..05` with placeholder numbers: early waves Hunter-heavy, later waves a larger Sentry presence, enemy count, health and damage rising each wave, and Wave 5 the largest. | All five assets load, and composition and multipliers rise wave over wave | Data test |
| M2-S06 | **Run state machine.** Pure `FPZRunState`: WaveActive → WaveCleared → Countdown → next wave; after Wave 5 → Victory; on player death → Defeat. The GameMode drives the wave manager from it. Upgrade selection slots in between WaveCleared and Countdown in Milestone 3. | Tests cover every transition, including no second outcome after Victory or Defeat | Unit tests |
| M2-S07 | **Visible countdown.** A countdown overlay between waves, from the countdown length in tuning. | The countdown appears after each cleared wave and the next wave starts at zero | Screenshot |
| M2-S08 | **Run stats.** `APZGameState` records wave reached, enemies defeated, time survived and damage taken. | Tests for stat accumulation; values match a played run | Unit tests + log |
| M2-S09 | **Results screen.** Replaces the defeat prompt. Shows outcome, wave reached, enemies defeated, time survived, damage taken and Restart, for both victory and defeat. Victory goes straight to results until the shutdown sequence lands in Milestone 4. | Both outcomes show correct stats; Restart starts a fresh run | Screenshot of each outcome |
| M2-S10 | **Pause menu.** Esc pauses the game and opens a menu with Resume and Restart. | Esc freezes drones, projectiles and timers; Resume continues; Restart resets | Screenshot |
| M2-S11 | **End-to-end run check.** Play a full run from Wave 1 to victory, and one to defeat, then fix anything that breaks. | One logged victory run and one defeat run with no errors | Playtest log |

**Order:** S01 and S02 in either order, then S03. S04 can start any time after S03. Then S05, S06 and S07. S08 can go alongside S06. Then S09, S10 and S11.

**Milestone 2 slice: "Survive Protocol Zero."** Start a run, fight five escalating waves of Hunters and Sentries with a countdown between them, pause at any time, and finish on a victory or defeat results screen that restarts the run. Numbers are placeholders until the Milestone 4 tuning pass.

---

## Explicitly out of scope until later milestones

Upgrade selection and upgrade effects (M3) · wave, enemies remaining and upgrades on the HUD, screen flash and hit indicator (M3) · intro text and shutdown sequence (M4) · audio and settings (M4) · final art and tuning (M4). Nothing beyond the design doc is planned: no bosses, extra enemy types, arena modifiers, persistent progression or extra settings.
