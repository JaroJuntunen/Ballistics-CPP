# Ballistic Trajectory Simulator

A real-time ballistic trajectory simulator written in C++ with SDL3 visualization.
Simulates projectile flight using real physics — gravity, aerodynamic drag, wind, and optionally the Coriolis effect at long ranges.
Built as a personal project to practice modern C++ and explore the physics behind real-world ballistic systems.

## Features

- Real-time trajectory simulation and visualization
- Aerodynamic drag simulation (7.62mm bullet defaults)
- Side-by-side drag vs. no-drag trajectory comparison
- Procedurally generated terrain using Perlin noise
- Mouse-aimed projectile firing, multiple projectiles simultaneously
- Trajectory path tracing — trail persists after landing
- Smooth camera with lock/follow system
- Movable launcher

## Controls

| Input | Action |
|---|---|
| `Space` | Fire with drag (yellow dot, white trail) |
| `Shift+Space` | Fire without drag (red dot, yellow trail) |
| `Ctrl+Space` | Fire both simultaneously for comparison |
| `C` | Clear oldest projectile |
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

**Aerodynamic drag:**
```
Fd = 0.5 * Cd * rho * A * v²
```
Where:
- `Cd` = drag coefficient (0.3 for a bullet)
- `rho` = air density (1.225 kg/m³ at sea level)
- `A` = cross-sectional area of the projectile (π * r²)
- `v` = current speed

Drag acts opposite to the velocity vector, decomposed into x and y components:
```
ax = -(Fd / m) * (vx / v)
ay = -g - (Fd / m) * (vy / v)
```

**Wind:**

Wind is simulated as a base velocity plus dynamic gusts using Perlin noise:
```
windVelocity(t) = baseWind + perlinGust(t)
```
Drag is computed against the relative airspeed, not the projectile's ground velocity:
```
vRel = v_projectile - v_wind
```
This means wind directly affects drag magnitude and direction.

**Altitude-varying air density:**
```
rho(h) = rho_0 * exp(-h / 8500)
```
Where `rho_0 = 1.225 kg/m³` at sea level and `8500 m` is the atmospheric scale height.
Air density decreases with altitude, reducing drag at higher elevations.

**Coriolis effect** *(optional, long range)*: relevant at ranges above ~1 km.

## Project Structure

```
Ballistics-CPP
├── CMakeLists.txt
├── README.md
└── src
    ├── main.cpp
    ├── simulation
    │   ├── Environment.hpp/.cpp
    │   ├── Projectile.hpp/.cpp
    │   └── Simulator.hpp/.cpp
    ├── renderer
    │   └── Renderer.hpp/.cpp
    ├── input
    │   └── InputHandler.hpp/.cpp
    └── util/
        └── math/
            └── Vec2.hpp
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
- [x] Aerodynamic drag
- [x] Decompose drag into x/y components correctly
- [x] Visual comparison of trajectories with and without drag
- [x] Wind simulation with Perlin noise gusts
- [x] Drag computed against relative airspeed (wind-aware)
- [x] Altitude-varying air density (barometric formula)

**Stage 3 — User control**
- [ ] Dear ImGui UI for runtime parameter adjustment
- [ ] Adjust muzzle velocity
- [ ] Adjust wind speed and direction
- [ ] Reset and re-fire

**Stage 4 — Extras (if motivated)**
- [ ] Multiple projectile types (different mass and drag coefficients)
- [ ] Coriolis effect toggle
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
