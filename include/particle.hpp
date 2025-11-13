#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SDL3/SDL.h>
#include "camera.hpp"

// Abstract particle base class. Concrete particles implement update() and render().
class Particle
{
public:
    virtual ~Particle() = default;

    // Update simulation state for this particle by dt seconds.
    virtual void update(float dt) = 0;

    // Render particle to the provided renderer using the camera for world->screen mapping.
    virtual void render(SDL_Renderer* renderer, const SimpleCamera& cam) const = 0;
};

#endif
