#ifndef SIMPLE_PARTICLE_HPP
#define SIMPLE_PARTICLE_HPP

#include "particle.hpp"
#include <SDL3/SDL.h>

// Simple concrete particle: moves with velocity and renders as a filled rectangle
// (square) approximating a circle. Position and radius are in world coordinates
// (floats, unbounded). The camera maps world->screen using pixels-per-unit.
class SimpleParticle : public Particle
{
public:
    SimpleParticle(float x, float y, float vx, float vy, float radius_world, SDL_Color color);
    virtual ~SimpleParticle() = default;

    void update(float dt) override;
    void render(SDL_Renderer* renderer, const SimpleCamera& cam) const override;

private:
    float m_x, m_y;
    float m_vx, m_vy;
    float m_radius; // normalized radius (fraction of width)
    SDL_Color m_color;
};

#endif
