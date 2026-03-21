# Ballistic Trajectory Simulator

A real-time ballistic trajectory simulator written in C++ with SDL3 visualization.
Simulates projectile flight using real physics — gravity, aerodynamic drag, wind, and optionally the Coriolis effect at long ranges.
Built as a personal project to practice modern C++ and explore the physics behind real-world ballistic systems.

## Features

- Real-time trajectory simulation and visualization
- Procedurally generated terrain using Perlin noise
- Mouse-aimed projectile firing
- Trajectory path tracing with landing point indication
- Smooth camera with lock/follow system
- Movable launcher

## Controls

| Input | Action |
|---|---|
| `Space` | Fire projectile toward mouse cursor |
| `A` / `D` | Move launcher left / right |
| `Left mouse drag` | Pan camera |
| `Right mouse click` | Return camera to launcher |
| `Scroll wheel` | Zoom in / out |

## Physics

The simulator uses Euler integration to update position and velocity each frame.

**Gravity:**
```
ay = -g        (g = 9.81 m/s²)
```

**Aerodynamic drag** *(planned)*:
```
Fd = 0.5 * Cd * rho * A * v²
```
Where:
- `Cd` = drag coefficient (depends on projectile shape)
- `rho` = air density (1.225 kg/m³ at sea level)
- `A` = cross-sectional area of the projectile
- `v` = current speed

Drag acts opposite to the velocity vector, decomposed into x and y components:
```
ax = -(Fd / m) * (vx / v)
ay = -g - (Fd / m) * (vy / v)
```

**Wind** *(planned)*: constant horizontal acceleration offset.

**Coriolis effect** *(optional, long range)*: relevant at ranges above ~1 km.

## Project Structure

```
Ballistics-CPP/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp
    ├── simulation/
    │   ├── Environment.hpp/.cpp    # Terrain height (Perlin noise)
    │   ├── Projectile.hpp/.cpp     # Projectile state, physics step, path recording
    │   └── Simulator.hpp/.cpp      # (planned) physics update loop
    ├── renderer/
    │   └── Renderer.hpp/.cpp       # SDL3 rendering, camera, world↔screen conversion
    ├── input/
    │   └── InputHandler.hpp/.cpp   # Keyboard and mouse input
    └── util/
        └── math/
            └── Vec2.hpp            # 2D vector with operators and normalize
```

## Roadmap

**Stage 1 — Get something on screen**
- [x] SDL3 window with terrain
- [x] Procedural terrain with Perlin noise
- [x] Movable launcher
- [x] Camera with lock/follow/pan/zoom
- [x] Fire projectile under gravity
- [x] Trace flight path
- [x] Show landing point (trail persists after landing)

**Stage 2 — Real physics**
- [ ] Aerodynamic drag
- [ ] Decompose drag into x/y components correctly
- [ ] Wind as a parameter
- [ ] Visual comparison of trajectories with and without drag

**Stage 3 — User control**
- [ ] Adjust muzzle velocity
- [ ] Adjust wind speed and direction
- [ ] Reset and re-fire

**Stage 4 — Extras (if motivated)**
- [ ] Multiple projectile types (different mass and drag coefficients)
- [ ] Coriolis effect toggle
- [ ] Air density variation with altitude
- [ ] Export trajectory data to CSV

## Dependencies

- C++17 or later
- SDL3 (fetched automatically via CMake FetchContent)
- CMake 3.15+

## Building

```bash
cmake -S . -B build
cmake --build build
```

On Windows, copy `build/_deps/sdl3-build/Debug/SDL3.dll` next to the executable before running.

## Notes

This is a learning project focused on correct physics and clean modern C++ structure.
