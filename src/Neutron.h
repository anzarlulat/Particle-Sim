#pragma once

#include "Particle.h"

class Neutron : public Particle
{
    public:
        Neutron(
            Vector2f position,
            Vector2f velocity
        );
};