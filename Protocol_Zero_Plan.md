# Protocol Zero: Assessment and Implementation Plan

2026-10-24 · Read-only assessment. Nothing in the Unreal project has been changed.
Source of truth: `Protocol_Zero_Design.md`. Rigor: minimal (no GDDs, no ADRs, the design doc is the design record).

---

## 1. Decisions needed before anything is written

1. **Approve this plan** (architecture, milestones, Milestone 1 stories).
2. **Allow config writes**: `/setup-engine` to pin UE 5.8 + C++ in `project.yaml`, and `/start`'s `modes.rigor: minimal`. These are the only `/adopt` actions needed.
3. **Initialise git** in the project folder (it is not a repository yet). Recommended before the first code change.
4. **UI framework**: plain UMG (recommended, fewest moving parts) or CommonUI (settings exist in `DefaultGame.ini`, but the plugin is not enabled in the `.uproject`).

---

## 2. Project stage assessment (`/project-stage-detect` equivalent)

**Stage: Pre-production. The concept and design are complete; no gameplay is implemented.**

### Working
- UE 5.8 C++ project with one runtime module, `Protocol_Zero`, which has compiled (editor DLL exists in `Binaries/Win64`).
- `EnhancedInput` is a module dependency and the default player input and input component classes are Enhanced Input (`Config/DefaultInput.ini`).
- Windows DX12 / SM6 target. Lumen, virtual shadow maps, ray tracing and Substrate are on (`Config/DefaultEngine.ini`).
- Studio framework installed (`.claude/`, `design/`, `docs/`, `production/`), with UE 5.8 engine reference docs.

### Incomplete or misconfigured
- `GameDefaultMap` points at the engine's `OpenWorld` template, not a project map. No `GlobalDefaultGameMode` is set.
- `Content/` holds only `Test.umap` (6.6 KB, effectively empty) and a `Developers/AngusChou` folder.
- Module source is the UE template stub only (`Protocol_Zero.cpp/.h`, `Build.cs`).
- `project.yaml` has no `engine:` or `modes:` block, so `/start` and `/setup-engine` have never run. `docs/engine-reference/unreal/VERSION.md` says the installed version is "NOT DETERMINED".
- `DefaultGame.ini` carries CommonUI settings while the CommonUI plugin is not enabled.
- Not a git repository.
- Observation, no action proposed now: ray tracing and the full Lumen stack are expensive for a low-poly top-down arena. Worth revisiting during the performance pass.

### Missing (every system in the design)
Player movement, cursor aim and camera · health, invulnerability window and death · plasma rifle and projectiles · Hunter Drone · Sentry Drone · arena, cover and navmesh · spawn points and timed groups · five-wave progression · upgrade selection (4 categories) · HUD and damage feedback · pause, victory, defeat and restart · intro text and shutdown message · SFX and music · master volume and mouse sensitivity settings.

### `/adopt` verdict
There is no existing gameplay code or content to migrate. Adoption is limited to the config steps in Decision 2.

---

## 3. Recommended architecture

**Rule of thumb:** C++ owns rules, state and math (testable). Blueprint subclasses own meshes, VFX, sounds, widget layout and tuning data. All tuning values live in Data Assets, per the coding standards.

Folder layout under `Source/Protocol_Zero/` (Public/Private split):

| Area | C++ classes | Blueprint / data |
|---|---|---|
| **Core / run flow** | `APZGameMode`: run state machine (Intro, WaveActive, UpgradeSelect, Countdown, Victory, Defeat). `APZGameState`: current wave, enemies remaining, run stats (enemies defeated, time survived, damage taken). | `BP_PZGameMode` |
| **Player** | `APZPlayerCharacter` (ACharacter, spring-arm camera fixed at 45°). `APZPlayerController`: Enhanced Input, cursor ground trace, pause. | `BP_PlayerCharacter`, `IA_Move`, `IA_Fire`, `IA_Pause`, `IMC_Default` |
| **Shared combat** | `UPZHealthComponent`: health, max health, post-hit invulnerability window, `OnDamaged(amount, direction)`, `OnDeath`. Used by player and drones. | `DA_PlayerTuning` |
| **Weapon** | `UPZWeaponComponent`: cooldown, damage, projectile count, narrow spread centred on the cursor. `APZProjectile`: `UProjectileMovementComponent` (finite speed). | `BP_PlasmaBolt`, `DA_PlasmaRifle` |
| **Collision** | Custom channels / profiles: `PlayerProjectile` hits Enemy + WorldStatic, `EnemyProjectile` hits Player + WorldStatic, projectiles ignore each other and their own side. | Set in Project Settings |
| **Enemies** | `APZDroneBase` (ACharacter, nav-driven) + `APZDroneAIController`. `APZHunterDrone`: pursue via navmesh, melee on cooldown in range. `APZSentryDrone`: move to preferred range, hold, fire on cooldown. Death: play destruction effect, then despawn. Plain C++ state logic in the controller; two simple behaviours do not justify Behavior Trees. | `BP_HunterDrone`, `BP_SentryDrone`, `DA_Hunter`, `DA_Sentry` |
| **Waves** | `UPZWaveDefinition` (Data Asset): spawn groups (drone class, count, delay) plus health and damage multipliers. `UPZWaveManagerComponent` on the GameMode. `APZSpawnPoint` actors at arena edges with a minimum-distance-from-player check. | `DA_Wave_01..05` |
| **Upgrades** | `UPZUpgradeDefinition` (category: Damage, FireRate, ProjectileCount, MaxHealth; percent). `UPZUpgradeComponent` on the player: stacks, rolls 3 random offers, applies to weapon and health (max health also heals by the amount gained). | `DA_Upgrade_*` |
| **UI** | Thin C++ base widgets exposing data: HUD, upgrade overlay, pause, results, intro, settings. | `WBP_*` layouts in UMG |
| **Settings** | `UPZGameUserSettings` (subclass of `UGameUserSettings`): master volume, mouse sensitivity. | `SC_Master` sound class + mix |

Tests: Unreal automation tests in `Source/Protocol_Zero/Private/Tests/` for health and invulnerability, weapon cooldown and spread, upgrade math, wave definition parsing and the spawn distance rule.

---

## 4. Epics and milestones (`/create-epics` equivalent)

Each epic maps to one architecture area above. Every item traces to the design doc; nothing is added.

**Milestone 1: Core combat slice**
- E1 Project foundation (config, map, GameMode, collision channels, test scaffold)
- E2 Player (movement, cursor aim, camera, health and invulnerability)
- E3 Weapon and projectiles
- E4 Hunter Drone
- E5 Greybox arena (cover, navmesh, edge spawn points)

**Milestone 2: Full wave loop**
- E6 Wave and spawn system (timed groups, five waves, countdown)
- E7 Sentry Drone
- E8 Run flow (pause, victory, defeat with death effect, results screen, restart)

**Milestone 3: Upgrades and HUD**
- E9 Upgrades (three random picks, four categories, repeatable, percentage-based)
- E10 HUD and damage feedback (health, wave, enemies remaining, current upgrades, screen flash, directional hit indicator)

**Milestone 4: Presentation and shipping**
- E11 Narrative (intro text, Protocol Zero shutdown sequence and message)
- E12 Audio (shooting, impact, enemy death, UI SFX, one looping music track)
- E13 Settings (master volume, mouse sensitivity)
- E14 Art pass, tuning to the 8-12 minute run target, Windows packaging

---

## 5. Milestone 1 stories (`/create-stories` equivalent)

| ID | Story | Done when | Evidence |
|---|---|---|---|
| M1-S01 | Configure project: create `L_Arena_Greybox`, set it as the default map, create and assign `BP_PZGameMode`, add projectile collision channels | PIE opens the arena with the project GameMode | Screenshot |
| M1-S02 | `UPZHealthComponent`: damage, death event, invulnerability window | Automation tests pass: damage reduces health, hits inside the window are ignored, death fires once at 0 | Unit test |
| M1-S03 | Player character and controller: WASD with Enhanced Input, fixed 45° camera | Player moves freely in 8 directions, camera angle stays fixed | Screenshot |
| M1-S04 | Cursor aim: character continuously faces the mouse cursor | Character rotates to the cursor's ground point every frame | Screenshot |
| M1-S05 | `UPZWeaponComponent`: hold LMB for automatic fire at the cooldown, tuning from `DA_PlasmaRifle` | Test: shots per second matches the cooldown; holding fires continuously | Unit test + screenshot |
| M1-S06 | `APZProjectile`: visible bolt with finite travel time, hits enemies and cover, no friendly fire | Bolts stop on cover, damage drones, pass no damage to the player | Integration test |
| M1-S07 | Greybox arena: floor, permanent cover blocking movement and projectiles, NavMeshBoundsVolume, edge `APZSpawnPoint` actors | Navmesh covers the floor and routes around cover | Screenshot (nav view) |
| M1-S08 | Hunter Drone: navmesh pursuit around cover, short-range melee on cooldown (no contact damage) | Hunter reaches the player around cover and hits at the configured interval | Integration test + screenshot |
| M1-S09 | Drone death: brief destruction effect, then despawn | Killed drones play the effect and are removed | Screenshot |
| M1-S10 | Player death and restart: short death effect, then a minimal defeat prompt that restarts the run | Dying shows the prompt; restart reloads the arena at baseline | Screenshot |
| M1-S11 | Minimal health readout on the HUD | Health text updates when the player is hit | Screenshot |

Order: S01, S02, then S03-S06 in parallel with S07, then S08-S11.

---

## 6. Recommended first vertical slice

**"Survive the Hunters": the end state of Milestone 1.**
The player spawns in the greybox arena, moves with WASD, aims at the cursor and holds LMB to fire plasma bolts. A small fixed set of Hunter Drones (placed or spawned from edge points, no wave system yet) pathfind around cover and melee on cooldown. The player can kill them, can die, and can restart.

Why this first: it proves the riskiest pieces of the design together (cursor aim feel, projectile travel and collision against cover, navmesh pursuit, melee timing with the invulnerability window) using only systems every later milestone builds on. Waves, Sentries and upgrades then slot in without reworking it.
