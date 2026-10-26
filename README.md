# ParticleSim

A 2D particle simulation written in C++ using SFML 3.1.0. The project explores object-oriented programming, numerical integration, electrostatic interactions and real-time graphics.

## Overview

ParticleSim simulates charged particles moving within a two-dimensional, periodically bounded domain. Particles interact through electrostatic forces calculated using Coulomb's law, while a collision system handles overlapping particles through positional correction and elastic collision impulses.

The project uses SI units for physical quantities and converts physical positions into screen coordinates for rendering.

## Features

- **Object-oriented particle model:** A common `Particle` base class with specialised `Proton`, `Neutron` and `Electron` subclasses.
- **Polymorphic particle storage:** Particles are stored using `std::unique_ptr<Particle>` in a single collection.
- **Electrostatic interactions:** Charged particles exert forces on one another according to Coulomb's law.
- **Numerical integration:** Particle velocities and positions are updated using semi-implicit Euler integration.
- **Periodic boundary conditions:** Particles leaving one edge of the simulation domain reappear at the opposite edge.
- **Collision resolution:** Overlapping particles are separated according to their masses, with an elastic impulse applied when they are approaching.
- **Real-time rendering:** SFML renders particles using distinct colours and display radii.

## Physics

### Electrostatic forces

The simulation calculates electrostatic interactions using Coulomb's law:

\[
F = k\frac{|q_1q_2|}{r^2}
\]

where \(k\) is Coulomb's constant, \(q_1\) and \(q_2\) are the particle charges, and \(r\) is their separation.

The force direction depends on the signs of the charges: like charges repel, while opposite charges attract. Neutral particles experience no electrostatic force.

Newton's second law determines the resulting acceleration:

\[
\mathbf{a} = \frac{\mathbf{F}}{m}
\]

### Numerical integration

Particle motion is updated using semi-implicit Euler integration:

\[
\mathbf{v}_{n+1}
=
\mathbf{v}_n+\mathbf{a}_n\Delta t
\]

\[
\mathbf{x}_{n+1}
=
\mathbf{x}_n+\mathbf{v}_{n+1}\Delta t
\]

The simulation uses physics substeps to divide the simulated time interval into smaller integration steps. This is particularly important for electrostatic interactions, which can produce very large accelerations for small particle separations.

The timestep and number of substeps affect numerical stability and accuracy. Smooth visual behaviour alone does not establish that a trajectory is physically accurate.

### Boundary conditions

The simulation uses periodic boundary conditions. When a particle crosses a boundary, its position wraps to the opposite side of the domain.

For pairwise interactions, the minimum-image convention selects the shortest displacement between particles across the periodic boundaries.

### Collision model

Collisions are resolved using mass-weighted positional correction and an elastic impulse when particles are moving towards one another.

The collision radii are based on the particles' display radii, converted from pixels to metres. This is a simplified modelling choice: the visual sizes are not intended to represent the actual physical radii of protons, neutrons or electrons.

## Technology

- **Language:** C++
- **Graphics:** SFML 3.1.0
- **Compiler:** Microsoft Visual C++ (MSVC)
- **C++ standard:** C++17
- **Build system:** Windows batch script using `cl`

## Project structure

```text
ParticleSim/
├── src/
│   ├── main.cpp
│   ├── Particle.h
│   ├── Particle.cpp
│   ├── Proton.h
│   ├── Proton.cpp
│   ├── Neutron.h
│   ├── Neutron.cpp
│   ├── Electron.h
│   ├── Electron.cpp
│   └── PhysicsConstants.h
├── build.bat
├── .gitignore
└── README.md
```

## Building and running

### Requirements

- Windows
- Visual Studio 2022 with the C++ desktop development workload
- An SFML 3.1.0 build compatible with your MSVC compiler and architecture

### Build

1. Install Visual Studio 2022 and SFML 3.1.0.
2. Configure the include, library and runtime paths in `build.bat` to match your local SFML installation.
3. Open a Visual Studio Developer Command Prompt so that the MSVC compiler is available.
4. Navigate to the project directory and run:

```bat
build.bat
```

5. Run the generated executable.

The SFML runtime DLLs must be available beside the executable or through the system's DLL search path.

The exact build commands and linker libraries depend on the SFML installation and configuration used by `build.bat`.

## Limitations

ParticleSim is an educational simulation rather than a high-precision molecular or particle-physics simulator.

- Numerical accuracy depends on the timestep, integration method and particle separation.
- Close encounters can produce large accelerations and numerical instability.
- The current implementation uses single-precision floating-point values for physical calculations.
- Collision radii are simplified and based on display sizes.
- The two-dimensional domain and periodic boundaries are modelling choices rather than a complete representation of physical space.
- The simulation does not model strong nuclear interactions, quantum mechanics, radiation or relativistic effects.

Consequently, the particles and their interactions should be interpreted as a simplified classical model, not a realistic simulation of atomic structure.

## Learning objectives

This project was developed to explore:

- C++ classes, inheritance and polymorphism.
- Dynamic memory management with smart pointers.
- Vector arithmetic and Newtonian mechanics.
- Coulomb's law and electrostatic force calculations.
- Numerical integration and timestep selection.
- Collision detection and impulse-based resolution.
- Real-time rendering with SFML.
- The separation of physical coordinates from screen coordinates.

## Future development

Potential improvements include:

- Separating the physics engine from the rendering layer.
- Using double precision for physical calculations.
- Improving integration accuracy and handling close encounters.
- Separating physical collision radii from visual radii.
- Adding controls for simulation speed and particle configuration.
- Improving visualisation and simulation diagnostics.
