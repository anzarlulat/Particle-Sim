#pragma once

#include "Particle.h"

class Electron : public Particle
{
    public:
        Electron(
            Vector2f position,
            Vector2f velocity
        );
};