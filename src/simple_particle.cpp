#include "simple_particle.hpp"
#include "config.hpp"
#include <SDL3/SDL.h>
#include <cmath>

SimpleParticle::SimpleParticle(float x, float y, float vx, float vy, float radius_world, SDL_Color color)
    : m_x(x), m_y(y), m_vx(vx), m_vy(vy), m_radius(radius_world), m_color(color)
{}

void SimpleParticle::update(float dt)
{
    const Config& cfg = Config::get_instance();
    // Apply gravity (world units) and simple damping
    m_vx += cfg.get_gravity_x() * dt;
    m_vy += cfg.get_gravity_y() * dt;

    // Apply global damping as a simple linear factor (clamped)
    float damping = cfg.get_global_damping();
    if (damping > 0.0f)
    {
        float factor = 1.0f - damping * dt;
        if (factor < 0.0f) factor = 0.0f;
        m_vx *= factor;
        m_vy *= factor;
    }

    // Simple Euler integration; no wrapping (infinite plane)
    m_x += m_vx * dt;
    m_y += m_vy * dt;
}

void SimpleParticle::render(SDL_Renderer* renderer, const SimpleCamera& cam) const
{
    const Config& cfg = Config::get_instance();
    const float width = static_cast<float>(cfg.get_window_width());
    const float height = static_cast<float>(cfg.get_window_height());

    // Map world -> screen. Camera (cam.x,cam.y) is centered on screen.
    float sx = (m_x - cam.x) * cam.scale + width * 0.5f;
    float sy = (m_y - cam.y) * cam.scale + height * 0.5f;

    float rpx = m_radius * cam.scale; // radius in pixels based on camera scale
    SDL_FRect frect{ sx - rpx, sy - rpx, rpx * 2.0f, rpx * 2.0f };

    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &frect);
}
