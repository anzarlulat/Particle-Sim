#include "Proton.h"

Proton::Proton(
    Vector2f position,
    Vector2f velocity
)
    : Particle(
        position,
        velocity,
        {0.0f, 0.0f},
        1.673e-27f,       // Mass (kg)
        1.602e-19f,       // Charge (C)
        12.0f,            // Display radius (pixels)
        Color::Red
    )
{}