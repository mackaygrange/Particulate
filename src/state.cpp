#include "state.hpp"
#include <SDL3/SDL_version.h>
#include <filesystem>
#include <memory>
#include <random>
#include "particle_system.hpp"
#include "simple_particle.hpp"

State::State()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // Provide extra diagnostics because SDL_GetError() may be empty in some failure modes
        SDL_Log("Fatal error: SDL initialization failed: %s", SDL_GetError());
        SDL_Log("Platform: %s", SDL_GetPlatform());
        try {
            auto cwd = std::filesystem::current_path().string();
            SDL_Log("CWD: %s", cwd.c_str());
        } catch (...) {
            SDL_Log("CWD: <unavailable>");
        }
        throw std::runtime_error(std::string("SDL initialization failed: ") + SDL_GetError());
    }

    const Config& config = Config::get_instance();

    window = SDL_CreateWindow("Ferl", config.get_window_width(), config.get_window_height(), static_cast<SDL_WindowFlags>(config.get_window_flags()));
    if (!window) { throw std::runtime_error(std::string("Window initialization failed: ") + SDL_GetError()); }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(config.get_display_index()), SDL_WINDOWPOS_CENTERED_DISPLAY(config.get_display_index()));

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) { throw std::runtime_error(std::string("Renderer initialization failed: ") + SDL_GetError( )); }
    uint32_t rflags = config.get_renderer_flags();
    #ifdef SDL_RENDERER_PRESENTVSYNC
    if (rflags & SDL_RENDERER_PRESENTVSYNC) { SDL_SetRenderVSync(renderer, 1); }
    #endif

    delta_time = Config::get_instance().get_target_frame_delta();
    last_frame_time = SDL_GetTicksNS();

    // Camera is in world coordinates (centered on origin by default).
    camera.x = 0.0f; camera.y = 0.0f; camera.z = 0.0f;

    setup_scene();
}

State::~State()
{
    // No managed textures in particle sandbox
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

State& State::get_instance()
{
    static State instance;
    return instance;
}

void State::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    particle_system.render(renderer, camera);

    SDL_RenderPresent(renderer);
}

void State::process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            quit = true;
            break;

        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE)
            {
                quit = true;
                break;
            }
            // Simple camera pan in world units (move by 1.0 world unit per keypress)
            const float panStepWorld = 1.0f;
            if (event.key.key == SDLK_A) camera.x -= panStepWorld;
            if (event.key.key == SDLK_D) camera.x += panStepWorld;
            if (event.key.key == SDLK_W) camera.y -= panStepWorld;
            if (event.key.key == SDLK_S) camera.y += panStepWorld;
            break;
        }
    }
}

void State::update()
{
    delay();
    // Update particle simulation using the elapsed delta_time (seconds)
    particle_system.update(delta_time);
    last_frame_time = SDL_GetTicksNS();
}

void State::delay()
{
    // Convert target frame delta (ms) to nanoseconds and compute end time
    Uint64 frame_end_time = last_frame_time + static_cast<Uint64>(static_cast<double>(Config::get_instance().get_target_frame_delta()) * 1e6);
    Uint64 current_time;

    while ((current_time = SDL_GetTicksNS()) < frame_end_time)
    {
        if (frame_end_time - current_time > 1000000ULL) { SDL_Delay(1); }
    }

    // delta_time in seconds (nanoseconds -> seconds)
    delta_time = static_cast<float>(static_cast<double>(current_time - last_frame_time) / 1e9);
    last_frame_time = current_time;
}

bool State::should_quit() const { return quit; }

void State::setup_scene()
{
    const Config& cfg = Config::get_instance();

    // Center camera on world center by default.
    this->camera.x = 0.0f;
    this->camera.y = 0.0f;
}