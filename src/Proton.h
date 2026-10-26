#pragma once

#include "Particle.h"

class Proton : public Particle
{
    public:
        Proton(
            Vector2f position,
            Vector2f velocity
        );
};