#include "Particle.h"
#include "PhysicsConstants.h"

#include <cmath>

Particle::Particle(
    sf::Vector2f initialPosition,
    sf::Vector2f initialVelocity,
    sf::Vector2f initialAcceleration,
    float mass,
    float charge,
    float radius,
    sf::Color colour
)
    : position(initialPosition / PhysicsConstants::PixelsPerMetre),
      velocity(initialVelocity),
      acceleration(initialAcceleration),
      mass(mass),
      charge(charge),
      radius(radius),
      shape(radius)
{
    shape.setOrigin({radius, radius});
    shape.setFillColor(colour);
    shape.setPosition(PhysicsConstants::PixelsPerMetre * position);
}

void Particle::update(float dt, sf::Vector2u windowSizePixels)
{
    velocity += acceleration * dt;
    position += velocity * dt;

    acceleration = {0.0f, 0.0f};

    const float width =
        static_cast<float>(windowSizePixels.x) /
        PhysicsConstants::PixelsPerMetre;

    const float height =
        static_cast<float>(windowSizePixels.y) /
        PhysicsConstants::PixelsPerMetre;

    // Detect numerical instability before wrapping.
    if (!std::isfinite(position.x) || !std::isfinite(position.y))
        return;

    position.x = std::fmod(position.x, width);
    position.y = std::fmod(position.y, height);

    if (position.x < 0.0f)
        position.x += width;

    if (position.y < 0.0f)
        position.y += height;

    shape.setPosition(
        PhysicsConstants::PixelsPerMetre * position
    );
}

void Particle::draw(sf::RenderWindow& window)
{
    shape.setPosition(
        PhysicsConstants::PixelsPerMetre * position
    );

    window.draw(shape);
}

float Particle::getMass() const
{
    return mass;
}

float Particle::getCharge() const
{
    return charge;
}

float Particle::getRadius() const
{
    return radius;
}

void Particle::applyForce(sf::Vector2f force)
{
    acceleration += force / mass;
}

Vector2f Particle::minimumImageDisplacement(const Particle& other, Vector2u windowSizePixels) const
{
    float dx = other.position.x - position.x;
    float dy = other.position.y - position.y;

    const float width = static_cast<float>(windowSizePixels.x) / PhysicsConstants::PixelsPerMetre;
    const float height = static_cast<float>(windowSizePixels.y) / PhysicsConstants::PixelsPerMetre;

    if (dx > width / 2.0f)
        dx -= width;
    else if (dx < -width / 2.0f)
        dx += width;

    if (dy > height / 2.0f)
        dy -= height;
    else if (dy < -height / 2.0f)
        dy += height;

    return {dx, dy};
}

void Particle::applyElectrostaticForce(const Particle& other, sf::Vector2u windowSizePixels)
{
    // Neutral particles experience no electrostatic force.
    if (charge == 0.0f || other.charge == 0.0f)
        return;

    const sf::Vector2f displacement =
        minimumImageDisplacement(other, windowSizePixels);

    const float distanceSquared =
        displacement.x * displacement.x +
        displacement.y * displacement.y;

    if (distanceSquared == 0.0f)
        return;

    const float distance = std::sqrt(distanceSquared);

    const float forceMagnitudeFactor =
        -PhysicsConstants::CoulombConstant * charge * other.charge /
        (distanceSquared * distance);

    const sf::Vector2f force{
        forceMagnitudeFactor * displacement.x,
        forceMagnitudeFactor * displacement.y
    };

    applyForce(force);
}

bool Particle::resolveCollision(Particle& other, sf::Vector2u windowSizePixels)
{
    // Find displacement from this particle to the other.
    const Vector2f displacement = minimumImageDisplacement(other, windowSizePixels);

    const float dx = displacement.x;
    const float dy = displacement.y;

    // Apply the minimum-image convention.
    const float width = static_cast<float>(windowSizePixels.x) /
        PhysicsConstants::PixelsPerMetre;
    const float height = static_cast<float>(windowSizePixels.y) /
        PhysicsConstants::PixelsPerMetre;

    const float distanceSquared = dx * dx + dy * dy;
    const float combinedRadius =
        (radius + other.radius) / PhysicsConstants::PixelsPerMetre;

    // No collision if the particles do not overlap.
    if (distanceSquared >= combinedRadius * combinedRadius)
        return false;

    const float distance = std::sqrt(distanceSquared);

    // Unit vector pointing from this particle to the other.
    sf::Vector2f normal;

    if (distance > 0.0001f / PhysicsConstants::PixelsPerMetre)
        normal = {dx / distance, dy / distance};
    else
        normal = {1.0f, 0.0f};

    // Inverse masses.
    const float invMass1 = 1.0f / mass;
    const float invMass2 = 1.0f / other.mass;
    const float invMassSum = invMass1 + invMass2;

    // Separate overlapping particles in proportion to inverse mass.
    const float overlap = combinedRadius - distance;

    position.x -= normal.x * overlap * invMass1 / invMassSum;
    position.y -= normal.y * overlap * invMass1 / invMassSum;

    other.position.x += normal.x * overlap * invMass2 / invMassSum;
    other.position.y += normal.y * overlap * invMass2 / invMassSum;

    // Relative velocity.
    const float relativeVelocityX = other.velocity.x - velocity.x;
    const float relativeVelocityY = other.velocity.y - velocity.y;

    const float velocityAlongNormal =
        relativeVelocityX * normal.x +
        relativeVelocityY * normal.y;

    // Apply an elastic collision impulse only if approaching.
    if (velocityAlongNormal < 0.0f)
    {
        const float impulseMagnitude =
            -2.0f * velocityAlongNormal / invMassSum;

        const float impulseX = impulseMagnitude * normal.x;
        const float impulseY = impulseMagnitude * normal.y;

        velocity.x -= impulseX * invMass1;
        velocity.y -= impulseY * invMass1;

        other.velocity.x += impulseX * invMass2;
        other.velocity.y += impulseY * invMass2;
    }

    // Wrap corrected positions back into the simulation domain.
    auto wrap = [](float value, float size)
    {
        value = std::fmod(value, size);
        if (value < 0.0f)
            value += size;
        return value;
    };

    position.x = wrap(position.x, width);
    position.y = wrap(position.y, height);

    other.position.x = wrap(other.position.x, width);
    other.position.y = wrap(other.position.y, height);

    // Synchronise SFML shapes with their corrected positions.
    shape.setPosition(PhysicsConstants::PixelsPerMetre * position);
    other.shape.setPosition(PhysicsConstants::PixelsPerMetre * other.position);

    return true;
}