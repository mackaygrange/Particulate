#include "state.hpp"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_version.h>
#include <filesystem>
#include "grid_plane.hpp"
#include <memory>

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
    if (!renderer) { throw std::runtime_error(std::string("Renderer initialization failed: ") + SDL_GetError()); }
    uint32_t rflags = config.get_renderer_flags();
    #ifdef SDL_RENDERER_PRESENTVSYNC
    if (rflags & SDL_RENDERER_PRESENTVSYNC) { SDL_SetRenderVSync(renderer, 1); }
    #endif

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, config.get_window_width(), config.get_window_height());
    if (!texture) { throw std::runtime_error(std::string("Texture initialization failed: ") + SDL_GetError()); }

    delta_time = Config::get_instance().get_target_frame_delta();
    last_frame_time = SDL_GetTicksNS();
    // Camera is in normalized coordinates where (0.5,0.5) centers the view.
    camera.x = 0.5f; camera.y = 0.5f; camera.z = 0.0f;

    setup_scene();
}

State::~State()
{
    for (auto* tex : loaded_textures) if (tex) SDL_DestroyTexture(tex);
    loaded_textures.clear();
    SDL_DestroyTexture(texture);
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

    scene.render(renderer, camera);

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
            // Simple camera pan in normalized units (1/100th of screen)
            const float panStep = 0.01f;
            if (event.key.key == SDLK_A) camera.x -= panStep;
            if (event.key.key == SDLK_D) camera.x += panStep;
            if (event.key.key == SDLK_W) camera.y -= panStep;
            if (event.key.key == SDLK_S) camera.y += panStep;
            break;
        }
    }
}

void State::update()
{
    delay();
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
    // Create a few demo layers with parallax
    // We'll convert legacy center-relative pixel coordinates into normalized coords so the demo layout
    // matches previous behavior across different window sizes.
    float centerX = cfg.get_window_width() * 0.5f;
    float centerY = cfg.get_window_height() * 0.5f;
        // Demo no longer uses pixel->normalized helper lambdas; geometry is created in normalized coords

    // Foreground (explicit stronger parallax)
    auto& fg = scene.createLayer(0.1f, cfg.get_default_parallax() * 1.2f);
    fg.addQuad((centerX + -100.0f) / static_cast<float>(cfg.get_window_width()), (centerY + -50.0f) / static_cast<float>(cfg.get_window_height()), 200.0f / static_cast<float>(cfg.get_window_width()), 100.0f / static_cast<float>(cfg.get_window_height()), SDL_Color{255, 128, 64, 255});

    // Load entity texture (if present)
    auto loadTexture = [&](const char* path)->SDL_Texture* {
        SDL_Texture* t = IMG_LoadTexture(renderer, path);
        if (!t) {
            SDL_Log("Failed to load texture %s: %s", path, SDL_GetError());
            return nullptr; // Non-fatal: continue without textured entity
        }
        SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
        loaded_textures.push_back(t);
        return t;
    };
    // Expect assets/husk.png (64x64)
    if (SDL_Texture* husk = loadTexture("assets/husk.png")) {
        // Legacy husk placement was center-relative pixels (-32, -120, 64, 64)
        // Provide an explicit UV rect mapping the full texture so the new UV API is exercised.
        fg.addQuad((centerX + -32.0f) / static_cast<float>(cfg.get_window_width()), (centerY + -120.0f) / static_cast<float>(cfg.get_window_height()), 64.0f / static_cast<float>(cfg.get_window_width()), 64.0f / static_cast<float>(cfg.get_window_height()), SDL_Color{255,255,255,255}, husk, UVRect{0.0f, 0.0f, 1.0f, 1.0f});
    }

    // Midground
    auto& mid = scene.createLayer(0.5f, cfg.get_default_parallax());
    mid.addQuad((centerX + -150.0f) / static_cast<float>(cfg.get_window_width()), (centerY + -80.0f) / static_cast<float>(cfg.get_window_height()), 300.0f / static_cast<float>(cfg.get_window_width()), 160.0f / static_cast<float>(cfg.get_window_height()), SDL_Color{64, 160, 255, 255});

    // Background
    auto& bg = scene.createLayer(1.0f, cfg.get_default_parallax() * 0.6f);
    bg.addQuad((centerX + -400.0f) / static_cast<float>(cfg.get_window_width()), (centerY + -300.0f) / static_cast<float>(cfg.get_window_height()), 800.0f / static_cast<float>(cfg.get_window_width()), 600.0f / static_cast<float>(cfg.get_window_height()), SDL_Color{40, 40, 80, 255});
    scene.sortByDepth();

    // Demo grid outline: draw a simple grid of 64px cells as thin lines when the
    // debug overlay is enabled in config. This uses a PlaneLayer (screen-space
    // normalized quads) so it integrates with existing rendering with parallax/depth.
    if (cfg.is_debug_overlay())
    {
        const int cellPx = 64;
        const int w = cfg.get_window_width();
        const int h = cfg.get_window_height();
        auto& gridLayer = scene.createLayer(0.2f, cfg.get_default_parallax());

        // line thickness in pixels (we draw as thin quads). Ensure at least 1px.
        const int linePx = 1;

        // Vertical lines
        for (int x = 0; x <= w; x += cellPx)
        {
            float nx = static_cast<float>(x) / static_cast<float>(w);
            float nw = static_cast<float>(linePx) / static_cast<float>(w);
            gridLayer.addQuad(nx, 0.0f, nw, 1.0f, SDL_Color{200,200,200,200});
        }

        // Horizontal lines
        for (int y = 0; y <= h; y += cellPx)
        {
            float ny = static_cast<float>(y) / static_cast<float>(h);
            float nh = static_cast<float>(linePx) / static_cast<float>(h);
            gridLayer.addQuad(0.0f, ny, 1.0f, nh, SDL_Color{200,200,200,200});
        }

        // Highlight origin cell boundaries (grid origin at top-left pixel (0,0))
        // Draw a slightly brighter cross at the origin
        gridLayer.addQuad(0.0f, 0.5f - (static_cast<float>(linePx) / (2.0f * h)), 1.0f, static_cast<float>(linePx) / static_cast<float>(h), SDL_Color{255,255,0,200});
        gridLayer.addQuad(0.5f - (static_cast<float>(linePx) / (2.0f * w)), 0.0f, static_cast<float>(linePx) / static_cast<float>(w), 1.0f, SDL_Color{255,255,0,200});
    }

    // Demo overlay: create a screen-space UI panel in the top-left corner (normalized coords)
        // overlay demo quad in top-left using explicit OverlayPlane
        {
            auto overlay = std::make_unique<OverlayPlane>(0.0f);
            const int panelW = 320, panelH = 166;
            const float nx = 30.0f / static_cast<float>(cfg.get_window_width());
            const float ny = 30.0f / static_cast<float>(cfg.get_window_height());
            const float nw = static_cast<float>(panelW) / static_cast<float>(cfg.get_window_width());
            const float nh = static_cast<float>(panelH) / static_cast<float>(cfg.get_window_height());
            overlay->addQuad(nx, ny, nw, nh, SDL_Color{30,30,30,220});
            scene.addPlane(std::move(overlay));
        }

    // Set the camera to look at grid coordinate (0,0). We convert the grid origin pixel (0,0)
    // to normalized coords so the camera.x/y match the scene's normalized coordinate system.
        // set camera to center on origin (0,0) in normalized coords
        // The grid plane maps cell (0,0) to normalized coords; center camera on that cell
        this->camera.x = 0.0f;
        this->camera.y = 0.0f;
}