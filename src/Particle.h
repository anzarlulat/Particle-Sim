#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Particle
{
    protected:
        Vector2f position;
        Vector2f velocity;
        Vector2f acceleration;

        float mass;
        float charge;

        float radius;
        CircleShape shape;
        Color color;

    public:
        // Position and radius use pixels; velocity and acceleration use SI units.
        Particle(
            sf::Vector2f initialPosition,
            sf::Vector2f initialVelocity,
            sf::Vector2f initialAcceleration,
            float mass,
            float charge,
            float radius,
            sf::Color colour
        );

        virtual ~Particle() = default;

        virtual void update(float dt, Vector2u windowSizePixels);
        virtual void draw(RenderWindow& window);

        bool resolveCollision(Particle& other, Vector2u windowSizePixels);
        void applyElectrostaticForce(const Particle& other, Vector2u windowSizePixels);

        float getMass() const;
        float getCharge() const;
        float getRadius() const;

    private:
        void applyForce(Vector2f force);
        Vector2f minimumImageDisplacement(const Particle& other, Vector2u windowSizePixels) const;
};
