# Arcade Survivors: Project Roadmap & Implementation Plan

Hey! This document outlines our 10-Act roadmap for building a scalable, high-performance web/action arcade game in C using Raylib.

The architecture is a Hybrid OOS & ECS setup (kind of like how Unreal Engine mixes standard Actors with MassEntity/Niagara). Our "Hero" object (the Player) will use a traditional Object-Oriented Component system, while the "Swarm" (Enemies, Projectiles) will leverage a data-oriented ECS approach so we can push thousands of entities on-screen at once without dropping frames.

## Act 1: Core Foundation & Separated Game Loop [DONE]
- Setup the core folder structures and build scripts.
- Implement the separated `ProcessInput()`, `UpdateLogic(deltaTime)`, and `RenderGraphics()` loop.

## Act 2: The OOS Framework (For the Player) [DONE]
- Build a UE-style Actor/Component framework natively in C.
- Create `Actor` base structures and polymorphically-linked `Component` arrays.

## Act 3: Player Architecture [DONE]
- Implement the `PlayerState` (stats, health), `PlayerController` (input), and `PlayerCharacter` (physical actor).
- Refactor logic into hyper-specific components natively within our `src/game/core` footprint.

## Act 4: The ECS Framework (For the Swarm)
- Build the data-oriented backend natively inside `Framework_ECS`.
- Setup raw Arrays (Structure of Arrays) for pooling max allowed enemies (`PositionData`, `VelocityData`, etc).
- Create a `SwarmRendererSystem` to batch render every active ECS entity instantly.

## Act 5: Enemy Swarm Logic
- Implement an `EnemySystem` spawner that pulls from our ECS data pool.
- Build the `EnemyAISystem` to iteratively direct packed array velocities towards the Player.

## Act 6: Combat & Projectiles 
- Add a fast `ProjectileSystem` data pool.
- Enable auto-firing logic on the Player that rapidly spawns entities into the ECS system.
- Add O(N) friendly continuous distance checking in the fast array loops to resolve bullet hits.

## Act 7: Bridging OOS and ECS Collisions
- Build the crossover: allow the ECS Enemy loop to check distances against the single OOS Player bounding box.
- Trigger `PlayerState` damage states on collisions.

## Act 8: Power-ups System & Advanced Enemies
- Add XP gems and powerups as ECS points that spawn on enemy death.
- Make the ECS `EnemySystem` route distinct behaviors using an `EnemyType` flag (e.g., chasing vs strafing).

## Act 9: Audio & Polish
- Wire up a proper `AudioManager` singleton.
- Bind SFX triggers to the player shooting, taking damage, and picking up items.

## Act 10: Multiplayer Scalability Pass
- Audit the separation of state to guarantee deterministic updates.
- Verify our controller input vs visual state separation so that predictive networking / rollbacks would be clean.
