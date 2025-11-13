#include "particle_system.hpp"
#include <SDL3/SDL.h>

#include "config.hpp"

void ParticleSystem::addParticle(std::unique_ptr<Particle> p)
{
    const Config& cfg = Config::get_instance();
    if (!p) return;
    if (static_cast<int>(m_particles.size()) >= cfg.get_max_particles()) return; // respect max_particles
    m_particles.push_back(std::move(p));
}

void ParticleSystem::update(float dt)
{
    for (auto& p : m_particles) if (p) p->update(dt);
}

void ParticleSystem::render(SDL_Renderer* renderer, const SimpleCamera& cam) const
{
    for (const auto& p : m_particles) if (p) p->render(renderer, cam);
}
