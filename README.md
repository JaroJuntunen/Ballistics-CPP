# Ballistic Trajectory Simulator

A real-time ballistic trajectory simulator written in C++17 with SDL3 visualization.
Simulates projectile flight using real-world physics — gravity, aerodynamic drag, altitude-varying air density, and dynamic wind with Perlin noise gusts.

Built as a personal project to strengthen C++ skills and explore the physics behind real ballistic systems.

---

## Technical highlights

- **Modern C++17** — RAII resource management, `std::unique_ptr`, `constexpr`, structured code separation
- **Physics simulation** — correct drag model with relative airspeed, barometric density, Euler integration
- **SDL3 rendering** — custom camera system with smooth follow, pan, zoom, and world-space coordinate mapping
- **Dear ImGui** — runtime parameter adjustment with collapsible UI panels
- **Procedural terrain** — Perlin noise via `stb_perlin`, sampled per-pixel each frame
- **Clean architecture** — simulation, rendering, and input handling in separate classes with no cross-dependencies

---

## Features

- Real-time 2D trajectory simulation at 60 fps
- Side-by-side drag vs. no-drag trajectory comparison
- Adjustable projectile parameters — muzzle velocity, mass, drag coefficient, diameter
- Adjustable environment — base wind X/Y, gust severity, gust frequency
- Lockable firing direction with live angle display
- Trajectory path tracing — trail persists after landing
- Procedurally generated terrain
- Smooth camera with lock/follow/pan/zoom
- Movable launcher

---

## Physics model

Euler integration is used to step position and velocity each frame (`dt = 1/60 s`).

**Gravity**
```
ay = -g        (g = 9.81 m/s²)
```

**Aerodynamic drag**
```
Fd = 0.5 * Cd * rho * A * v²
```
| Symbol | Meaning |
|---|---|
| `Cd` | Drag coefficient (default 0.3 for a 7.62mm bullet) |
| `rho` | Air density — varies with altitude (see below) |
| `A` | Cross-sectional area (π * r²) |
| `v` | Speed relative to wind |

Drag is decomposed into x/y components acting opposite to the velocity vector:
```
ax = -(Fd / m) * (vx / v)
ay = -g - (Fd / m) * (vy / v)
```

**Wind**

Wind is a constant base velocity plus dynamic gusts driven by Perlin noise:
```
windVelocity(t) = baseWind + perlinGust(t)
```
Drag is computed against the **relative airspeed**, not ground speed:
```
vRel = v_projectile - v_wind
```
This means a tailwind reduces drag, a headwind increases it.

**Altitude-varying air density (barometric formula)**
```
rho(h) = 1.225 * exp(-h / 8500)
```
Air density decreases with altitude, reducing drag at higher elevations.

---

## Controls

| Input | Action |
|---|---|
| `Space` | Fire with drag (yellow trail) |
| `Shift+Space` | Fire without drag (red dot, yellow trail) |
| `Ctrl+Space` | Fire both simultaneously for direct comparison |
| `C` | Remove oldest projectile |
| `L` | Toggle firing direction lock |
| `A` / `D` | Move launcher left / right |
| `Right mouse drag` | Pan camera |
| `Middle mouse` / `Numpad Enter` | Snap camera back to launcher |
| `Scroll wheel` / `Numpad +/-` | Zoom in / out |

## Parameter panel

The **Simulator** panel (top-left) has two collapsible sections:

| Section | Parameters |
|---|---|
| **Environment** | Base wind X/Y (m/s), gust severity, gust frequency |
| **Projectile** | Muzzle velocity (m/s), mass (g), drag coefficient, diameter (mm) |

Drag a field to change its value, or double-click to type directly.
The **Firing direction** sub-section shows the live mouse angle. Lock it with `L` or the checkbox to fix the angle and shoot repeatedly at the same elevation.

---

## Project structure

```
Ballistics-CPP/
├── CMakeLists.txt
└── src/
    ├── main.cpp                        # Game loop, input dispatch, frame timing
    ├── simulation/
    │   ├── Environment.hpp/.cpp        # Wind, air density, terrain height
    │   └── Projectile.hpp/.cpp         # Projectile state and physics step
    ├── renderer/
    │   └── Renderer.hpp/.cpp           # SDL3 rendering, camera, ImGui
    ├── input/
    │   └── InputHandler.hpp/.cpp       # SDL event handling, input state
    └── util/math/
        └── Vec2.hpp                    # 2D vector math
```

---

## Building

Dependencies are fetched automatically via CMake `FetchContent` (SDL3, Dear ImGui).

```bash
cmake -S . -B build
cmake --build build
```


---

## Roadmap

**Stage 1 — Rendering and basic simulation**
- [x] SDL3 window, procedural Perlin noise terrain
- [x] Movable launcher, camera with lock/follow/pan/zoom
- [x] Projectile fired under gravity with path tracing

**Stage 2 — Real physics**
- [x] Aerodynamic drag (correct x/y decomposition)
- [x] Drag vs. no-drag visual comparison
- [x] Wind with Perlin noise gusts, relative airspeed drag
- [x] Altitude-varying air density (barometric formula)

**Stage 3 — User control**
- [x] Dear ImGui runtime parameter panel
- [x] Muzzle velocity, mass, drag coefficient, diameter
- [x] Wind speed, direction, gust severity and frequency
- [x] Lockable firing direction with live angle display

**Stage 4 — Full 3D rebuild**

The 2D model has a natural ceiling — effects like Coriolis deflection and spin drift are lateral forces that simply cannot be observed in a side-view plane. The logical next step is a full rebuild in 3D with an OpenGL renderer, where the complete ballistic model becomes demonstrable. The 3D version will be private for now, but when it haves basic functionality it will be linked and made public.

## AI usage

- **Model:** Claude Sonnet (Anthropic)
- **Used for:**
  - Debugging and explaining error messages
  - Explaining C++ concepts and physics/math (essentially a faster search engine)
  - Refactoring and code organisation (splitting functions, code alignment)
  - ImGui wiring and SDL3 API lookups
  - Writing and formatting this README
- All physics, architecture, and design decisions are my own
