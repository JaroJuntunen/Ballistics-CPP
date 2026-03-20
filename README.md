Ballistic Trajectory Simulator
A real-time ballistic trajectory simulator written in C++ with SDL3 visualization.
Simulates projectile flight using real physics — gravity, aerodynamic drag, wind, and optionally the Coriolis effect at long ranges.
Built as a personal project to practice modern C++ and explore the physics behind real-world ballistic systems.

Features

Real-time trajectory simulation and visualization
Adjustable parameters: muzzle velocity, launch angle, projectile mass, drag coefficient, wind
Trajectory path tracing with landing point indication
Side-by-side comparison of trajectories with and without drag


Physics
Equations of motion
The simulator uses numerical integration (Euler or Runge-Kutta) to update position and velocity each frame.
Gravity:
ay = -g        (g = 9.81 m/s²)
Aerodynamic drag:
Fd = 0.5 * Cd * rho * A * v²
Where:

Cd = drag coefficient (depends on projectile shape)
rho = air density (1.225 kg/m³ at sea level)
A = cross-sectional area of the projectile
v = current speed

Drag acts opposite to the velocity vector, so it must be decomposed into x and y components:
ax = -(Fd / m) * (vx / v)
ay = -g - (Fd / m) * (vy / v)
Wind:
Wind adds a constant horizontal acceleration offset based on wind speed and direction.
Coriolis effect (optional, long range):
ac = 2 * v × Ω
Where Ω is Earth's angular velocity vector. Relevant at ranges above ~1km.

Project Structure
ballistic-sim/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp               # Entry point, main loop
│   ├── simulation/
│   │   ├── Projectile.hpp     # Projectile state and properties
│   │   ├── Projectile.cpp
│   │   ├── Environment.hpp    # Wind, air density, gravity
│   │   ├── Environment.cpp
│   │   └── Simulator.hpp      # Runs the physics update loop
│   │   └── Simulator.cpp
│   ├── renderer/
│   │   ├── Renderer.hpp       # SDL2 rendering, draws trajectory and UI
│   │   └── Renderer.cpp
│   └── input/
│       ├── InputHandler.hpp   # Keyboard input, parameter adjustment
│       └── InputHandler.cpp

Roadmap
Stage 1 — Get something on screen

- [x] Create SDL3 window
- [x] Show ground line
- [ ] Draw a point moving under gravity only (no drag)
- [ ] Trace the path as the projectile moves
- [ ] Show landing point

Stage 2 — Real physics

- [ ] Add aerodynamic drag
- [ ] Decompose drag into x/y components correctly
- [ ] Add wind as a parameter
- [ ] Compare trajectory with and without drag visually

Stage 3 — User control

- [ ] Adjust launch angle with keyboard
- [ ] Adjust muzzle velocity
- [ ] Adjust wind speed and direction
- [ ] Reset and re-fire

Stage 4 — C++ structure

- [ ] Refactor into proper classes (Projectile, Environment, Simulator, Renderer)
- [ ] Use smart pointers where appropriate
- [ ] Separate simulation logic fully from rendering

Stage 5 — Extras (if motivated)

- [ ] Multiple projectile types with different mass and drag coefficients
- [ ] Coriolis effect toggle
- [ ] Air density variation with altitude
- [ ] Export trajectory data to CSV


Dependencies

C++17 or later
SDL3 (sudo apt install libsdl2-dev on Ubuntu)
CMake 3.15+


Building
bashmkdir build && cd build
cmake ..
make
./ballistic-sim

Notes
This is a learning project. The focus is on correct physics implementation and clean C++ structure, not visual polish.
