#include "Electron.h"

Electron::Electron(
    sf::Vector2f position,
    sf::Vector2f velocity
)
    : Particle(
        position,
        velocity,
        {0.0f, 0.0f},
        9.109e-31f,       // Mass (kg)
        -1.602e-19f,      // Charge (C)
        6.0f,             // Display radius (pixels)
        sf::Color::Cyan
    )
{}
