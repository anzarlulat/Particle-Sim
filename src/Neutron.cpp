#include "Neutron.h"

Neutron::Neutron(
    Vector2f position,
    Vector2f velocity
)
    : Particle(
        position,
        velocity,
        {0.0f, 0.0f},
        1.675e-27f,       // Mass (kg)
        0.0f,             // Charge (C)
        12.0f,            // Display radius (pixels)
        sf::Color(160, 160, 160)
    )
{}