# Protocol Zero - Game Design

## Overview

Protocol Zero is a single-player, top-down sci-fi arena survival shooter for Windows PC, built in Unreal Engine 5. The player controls a lone security operative trapped in a compromised research facility after its experimental AI defense system turns hostile.

A run lasts approximately 8-12 minutes. The player survives five escalating waves of hostile drones, takes post-wave upgrades, and clears Wave 5 to initiate Protocol Zero and shut down the rogue defense system.

## Player Experience

- Camera: fixed 45-degree isometric perspective.
- Movement: WASD free movement.
- Aim: the character continuously faces the mouse cursor.
- Fire: hold the left mouse button for automatic fire at the weapon cooldown.
- Pause: Esc opens a pause menu.
- Restart: defeat results include a restart option.
- Base survivability: health only; there are no baseline shields, dodge, ammo, reload, or weapon heat systems.
- Damage feedback: HUD health updates, a screen flash, and a directional hit indicator.
- Damage grace: a short post-hit invulnerability window prevents stacked attacks from causing instant deaths.

## Weapon

The starting weapon is a fast plasma rifle with moderate damage and rapid, automatic visible energy bolts. Bolts have finite travel time.

- Player bolts collide with enemies and arena cover.
- Player projectiles do not have friendly-fire interactions.
- The rifle can fire continuously as long as its cooldown permits.

## Enemy Roster

### Hunter Drone

A fast melee drone that uses Unreal navigation to pursue the player around obstacles. At close range, it performs a short-range melee attack with a cooldown rather than dealing continuous contact damage.

### Sentry Drone

A slower ranged drone that navigates to its preferred range, remains stationary, and repeatedly fires projectiles at the player.

- Sentry projectiles collide with the player and arena cover.
- Enemy projectiles do not use friendly-fire or projectile-versus-projectile interactions.
- On death, drones play a brief destruction animation and then despawn.

## Arena

The game uses one stylized low-poly sci-fi research-facility arena. Clear emissive colors support combat readability.

- Permanent obstacles provide cover.
- Obstacles block both movement and all projectiles.
- Drones use navigation paths to move around obstacles.
- Enemies arrive from configured arena-edge spawn points.
- Enemies spawn in small timed groups, rather than all at the beginning of a wave.
- Spawn points must be outside a minimum distance from the player.

## Waves and Difficulty

Each run consists of five waves. Clearing every enemy unlocks the upgrade selection, then a visible countdown begins the next wave.

- Difficulty increases through enemy count, health, and damage.
- Early waves are Hunter-heavy.
- Later waves introduce a larger Sentry presence.
- Wave 5 is the largest and most demanding standard-drone wave.
- There is no boss, miniboss, dynamic arena modifier, or additional enemy type in the initial scope.

## Upgrades

After each cleared wave, the game pauses behind a full-screen selection overlay. The player chooses one of three random upgrade options.

Available categories:

- Damage
- Fire rate
- Projectile count
- Maximum health

Upgrade behavior:

- Upgrades use percentage increases.
- Upgrade categories can be selected repeatedly during a run.
- Projectile-count upgrades fire a narrow forward spread centered on the cursor.
- Maximum-health upgrades also immediately restore health by the amount gained.
- No progression carries between runs; every run starts from the same baseline.

## HUD and Screens

The combat HUD always displays:

- Current health
- Current wave
- Enemies remaining
- Current upgrades

Victory and defeat screens display:

- Outcome
- Wave reached
- Enemies defeated
- Time survived
- Damage taken
- Restart option

On player death, play a short death effect before displaying the results screen. On victory, complete the shutdown sequence and then show results.

## Narrative and Presentation

The narrative is delivered through introductory text and a concise victory shutdown message. Combat does not include voiced AI dialogue or collectible logs in the initial scope.

Audio includes:

- Shooting, impact, enemy death, and UI sound effects
- One looping music track

Player settings include master volume and mouse sensitivity.

## Explicitly Deferred

The following decisions remain intentionally open for implementation planning:

- UE5 C++ and Blueprint architecture
- Exact player, enemy, projectile, and wave tuning values
- Concrete arena layout and spawn-point placement
- UI visual treatment and final asset production pipeline
- Save-game behavior and nonessential graphics or accessibility settings

## Initial Scope Boundaries

The initial game does not include multiplayer, persistent progression, a minimap, boss encounters, advanced movement mechanics, adaptive music, voice acting, key rebinding, graphics presets, or an expanded accessibility settings suite.
