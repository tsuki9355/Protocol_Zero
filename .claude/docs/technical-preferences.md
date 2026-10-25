# Technical Preferences

<!-- project.yaml at the repo root is the machine-readable source of truth for
     engine, specialists, naming, performance, platform, and testing.framework.
     This file is the human-readable LEGACY FALLBACK: agents and skills resolve
     each key from project.yaml first and fall back here only when the
     project.yaml key is absent. /setup-engine dual-writes both.
     Forbidden patterns and allowed libraries are NOT migrated — they live only
     in this file. Populated by /setup-engine; updated as decisions are made. -->

## Engine & Language

- **Engine**: Unreal Engine 5.8
- **Language**: C++ (primary), Blueprint (gameplay prototyping)
- **Rendering**: Lumen + Nanite (deferred)
- **Physics**: Chaos

## Input & Platform

<!-- Written by /setup-engine. Read by /ux-design, /ux-review, /test-setup, /team-ui, and /dev-story -->
<!-- to scope interaction specs, test helpers, and implementation to the correct input methods. -->

- **Target Platforms**: PC (Windows)
- **Input Methods**: Keyboard/Mouse
- **Primary Input**: Keyboard/Mouse (WASD move, cursor aim, hold LMB fire, Esc pause)
- **Gamepad Support**: None (not in Protocol_Zero_Design.md scope)
- **Touch Support**: None
- **Platform Notes**: No key rebinding in initial scope. Mouse sensitivity is a player setting.

## Naming Conventions

- **Classes**: Prefixed PascalCase (`A` Actor, `U` UObject, `F` struct, `E` enum, `I` interface); project prefix `PZ` (e.g. `APZPlayerCharacter`)
- **Variables**: PascalCase (e.g. `MoveSpeed`); booleans take a `b` prefix (`bIsAlive`)
- **Signals/Events**: Delegates, PascalCase `On<Event>` (e.g. `OnDeath`)
- **Files**: Match class without prefix (e.g. `PZPlayerCharacter.h`)
- **Scenes/Prefabs**: Levels `L_<Name>`, Blueprints `BP_<Name>`, widgets `WBP_<Name>`, data assets `DA_<Name>`
- **Constants**: SCREAMING_SNAKE (unverified against engine reference)

## Performance Budgets

- **Target Framerate**: 60 fps
- **Frame Budget**: 16.6 ms
- **Draw Calls**: < 2000 (UE default guidance)
- **Memory Ceiling**: [TO BE CONFIGURED]

## Testing

- **Framework**: Unreal Automation Spec / Automation Tests in `Source/Protocol_Zero/Private/Tests/`
- **Minimum Coverage**: [TO BE CONFIGURED]
- **Required Tests**: Balance formulas, gameplay systems, networking (if applicable)

## Forbidden Patterns

<!-- Add patterns that should never appear in this project's codebase -->
- [None configured yet — add as architectural decisions are made]

## Allowed Libraries / Addons

<!-- Add approved third-party dependencies here -->
- [None configured yet — add as dependencies are approved]

## Architecture Decisions Log

<!-- Quick reference linking to full ADRs in docs/architecture/ -->
- [No ADRs yet — use /architecture-decision to create one]

## Engine Specialists

<!-- Written by /setup-engine when engine is configured. -->
<!-- Read by /code-review, /architecture-decision, /architecture-review, and team skills -->
<!-- to know which specialist to spawn for engine-specific validation. -->

- **Primary**: unreal-specialist
- **Language/Code Specialist**: ue-blueprint-specialist (Blueprint graphs) or unreal-specialist (C++)
- **Shader Specialist**: unreal-specialist (no dedicated shader specialist — primary covers materials)
- **UI Specialist**: ue-umg-specialist (UMG widgets, input routing, widget styling)
- **Additional Specialists**: ue-gas-specialist (Gameplay Ability System), ue-replication-specialist (netcode — not used, single-player)
- **Routing Notes**: Invoke primary for C++ architecture and broad engine decisions. Invoke Blueprint specialist for Blueprint graph architecture and BP/C++ boundary design. Invoke UMG specialist for all UI implementation. UI uses plain UMG (no CommonUI).

### File Extension Routing

<!-- Skills use this table to select the right specialist per file type. -->
<!-- If a row says [TO BE CONFIGURED], fall back to Primary for that file type. -->

| File Extension / Type | Specialist to Spawn |
|-----------------------|---------------------|
| Game code (.cpp, .h files) | unreal-specialist |
| Shader / material files (.usf, .ush, Material assets) | unreal-specialist |
| UI / screen files (UMG Widget Blueprints) | ue-umg-specialist |
| Scene / prefab / level files (.umap, .uasset) | unreal-specialist |
| Native extension / plugin files (.uplugin, modules) | unreal-specialist |
| Blueprint graphs (.uasset BP classes) | ue-blueprint-specialist |
| General architecture review | Primary |
