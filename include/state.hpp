#ifndef STATE_HPP
#define STATE_HPP

#include <SDL3/SDL.h>

#include "config.hpp"
#include "camera.hpp"
#include "particle_system.hpp"

class State
{
private:
    State();
    ~State();
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    Config& config = Config::get_instance();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    Uint64 last_frame_time = 0; // SDL_GetTicksNS timestamps (nanoseconds)
    float delta_time = 0.0f;    // seconds

    bool quit = false;

    ParticleSystem particle_system; // Particle-based simulation
    SimpleCamera camera;            // Simple camera for panning over the 2D world

    void setup_scene();        // Internal helper to populate layers

public:
    static State& get_instance();

    void render();
    void process_input();
    void update();
    void delay();
    bool should_quit() const;
    SimpleCamera& get_camera() { return camera; }
    ParticleSystem& get_particle_system() { return particle_system; }
};

#endif