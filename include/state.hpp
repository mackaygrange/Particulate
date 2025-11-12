#ifndef STATE_HPP
#define STATE_HPP

#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "config.hpp"
#include "scene.hpp"

class State
{
private:
    State();
    ~State();
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    Config& config = Config::get_instance();

    SDL_Window* window = nullptr;
    SDL_Texture* texture = nullptr;
    std::vector<SDL_Texture*> loaded_textures; // managed textures
    SDL_Renderer* renderer = nullptr;

    Uint64 last_frame_time = 0; // SDL_GetTicksNS timestamps (nanoseconds)
    float delta_time = 0.0f;    // seconds

    bool quit = false;

    LayeredScene scene;        // New layered scene
    SimpleCamera camera;       // Simple camera for parallax

    void setup_scene();        // Internal helper to populate layers

public:
    static State& get_instance();

    void render();
    void process_input();
    void update();
    void delay();
    bool should_quit() const;
    SimpleCamera& get_camera() { return camera; }
    LayeredScene& get_scene() { return scene; }
};

#endif