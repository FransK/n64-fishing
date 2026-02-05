# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
make              # Build the ROM (n64base.z64)
make clean        # Clean build artifacts
make FINAL=1      # Build with compression optimized for final release
make DEBUG=0      # Build without debug symbols and printf debugging
```

Requires Libdragon SDK (N64_INST environment variable) and Tiny3D graphics library. Assets must be downloaded from https://github.com/FransK/n64-assets and placed in the `assets/` directory.

## Architecture Overview

This is an N64 fishing minigame built with Libdragon SDK and Tiny3D. The game uses a fixed-timestep loop (60 FPS for logic, variable for rendering).

### Core Structure

```
main.cpp → World → Scene (central hub)
                     ├── InputComponents (Strategy pattern: Player vs AI)
                     ├── CollisionScene (GJK/EPA physics)
                     ├── AnimationComponents (Observer pattern)
                     ├── PlayerState machines
                     └── PlayerAi (AI decision system)
```

### Key Patterns

- **Strategy Pattern**: `InputComponent<T>` uses `PlayerInputStrategy` or `AIInputStrategy` to abstract input sources
- **Observer Pattern**: `AnimationComponent` observes `PlayerState` changes to trigger animations
- **Variant Pattern**: `InputComponentVariant` (std::variant) handles player vs AI without virtual inheritance
- **Singleton**: `GlobalSettings` accessed via `getGlobalSettingsInterface()`
- **State Machine**: Both Scene (INTRO→GAME→GAME_OVER) and PlayerState (IDLE→WALKING→ATTACKING/FISHING→STUNNED)

### Directory Layout

- `include/core/collision/` - GJK/EPA collision detection and physics
- `include/core/input/` - PlayerState machine and InputState data
- `include/core/animation/` - Animation system with T3D integration
- `include/core/common/` - Reusable patterns (Component, Observer, Observable)
- `include/scene/` - Game entities (Scene, Player, Camera, PlayerColliders)
- `include/input/` - Input strategy implementations
- `code/` - Implementation files mirroring header structure

### Game Loop

The game uses two update paths:
- `fixedloop()` - Fixed delta time for physics, input, state machines, collision
- `loop()` - Variable delta time for animations, camera, rendering

### Collision System

Uses custom GJK/EPA implementation with Minkowski sums. Two pruning passes: single-dimension edge checking and AABB overlap checking. Players have separate body colliders and damage triggers for hit detection.

## Naming Conventions

- **File names**: PascalCase (e.g., `PlayerAi.h`, `GameSettings.cpp`)
- **Type names**: PascalCase (e.g., `PlayerState`, `AnimationComponent`, `CollisionScene`)
- **Function names**: camelCase (e.g., `getGlobalSettingsInterface()`, `updateFixed()`, `drawBillboard()`)
- **Variable names**: camelCase
- **Parameter names**: camelCase

## Code Style

- **Prefer `auto`**: Use `auto` for variable declarations where the type is clear from context or initialization