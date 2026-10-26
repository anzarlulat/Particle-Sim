#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include "Proton.h"
#include "Neutron.h"
#include "Electron.h"

int main()
{
    const sf::Vector2u windowSize(800u, 600u);

    sf::RenderWindow window(
        sf::VideoMode(windowSize),
        "ParticleSim"
    );

    window.setFramerateLimit(60);

    
    std::vector<std::unique_ptr<Particle>> particles;

    // Outer ring: six protons.
    constexpr float outerRadius = 220.0f;
    constexpr float centreX = 400.0f;
    constexpr float centreY = 300.0f;

    for (int i = 0; i < 6; ++i)
    {
        const float angle = 2.0f * 3.14159265f * i / 6.0f;

        const sf::Vector2f position{
            centreX + outerRadius * std::cos(angle),
            centreY + outerRadius * std::sin(angle)
        };

        particles.push_back(std::make_unique<Proton>(
            position,
            sf::Vector2f{0.0f, 0.0f}
        ));
    }

    // Inner ring: six electrons, offset by half an angular step.
    constexpr float innerRadius = 100.0f;

    for (int i = 0; i < 6; ++i)
    {
        const float angle =
            2.0f * 3.14159265f * (i + 0.5f) / 6.0f;

        const sf::Vector2f position{
            centreX + innerRadius * std::cos(angle),
            centreY + innerRadius * std::sin(angle)
        };

        particles.push_back(std::make_unique<Electron>(
            position,
            sf::Vector2f{0.0f, 0.0f}
        ));
    }

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        
        // Simulated time advanced per rendered frame.
        constexpr int substeps = 100;
        constexpr double simulatedFrameTime = 1.0e-13;

        const float dt = static_cast<float>(
            simulatedFrameTime / substeps
        );

        for (int step = 0; step < substeps; ++step)
        {
            // Calculate forces for every unique pair.
            // Accumulate all forces before updating any particle.
            for (std::size_t i = 0; i < particles.size(); ++i)
            {
                for (std::size_t j = i + 1;
                     j < particles.size(); ++j)
                {
                    particles[i]->applyElectrostaticForce(
                        *particles[j], windowSize
                    );

                    particles[j]->applyElectrostaticForce(
                        *particles[i], windowSize
                    );
                }
            }

            // Integrate every particle using the accumulated forces.
            for (auto& particle : particles)
                particle->update(dt, windowSize);

            // Resolve collisions after integration.
            for (std::size_t i = 0; i < particles.size(); ++i)
            {
                for (std::size_t j = i + 1;
                     j < particles.size(); ++j)
                {
                    particles[i]->resolveCollision(
                        *particles[j], windowSize
                    );
                }
            }
        }

        window.clear(sf::Color(25, 25, 35));

        for (const auto& particle : particles)
            particle->draw(window);

        window.display();
    }

    return 0;
}
