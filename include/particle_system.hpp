#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <vector>
#include <memory>
#include "particle.hpp"

class ParticleSystem
{
public:
    ParticleSystem() = default;
    ~ParticleSystem() = default;

    void addParticle(std::unique_ptr<Particle> p);
    void update(float dt);
    void render(SDL_Renderer* renderer, const SimpleCamera& cam) const;
    size_t count() const { return m_particles.size(); }

private:
    std::vector<std::unique_ptr<Particle>> m_particles;
};

#endif
