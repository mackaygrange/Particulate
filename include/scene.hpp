#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <algorithm>
#include <SDL3/SDL.h>
#include "layer.hpp"
#include "config.hpp"

struct SimpleCamera
{
    float x{0}, y{0}, z{0};
};

class LayeredScene
{
public:
    // Create a plain PlaneLayer (normalized coordinate based quad layer)
    PlaneLayer& createLayer(float depth, float parallax = 1.0f, bool overlay = false)
    {
        auto p = std::make_unique<PlaneLayer>(depth, parallax, overlay);
        PlaneLayer* raw = p.get();
        m_layers.push_back(std::move(p));
        return *raw;
    }

    // Create an arbitrary Plane (useful for GridPlane factory in the future)
    void addPlane(std::unique_ptr<Plane> plane)
    {
        m_layers.push_back(std::move(plane));
    }

    void sortByDepth()
    {
        std::sort(m_layers.begin(), m_layers.end(), [](const std::unique_ptr<Plane>& a, const std::unique_ptr<Plane>& b) { return a->depth() < b->depth(); });
    }

    void render(SDL_Renderer* renderer, const SimpleCamera& cam) const
    {
        const Config& cfg = Config::get_instance();
        const float width = static_cast<float>(cfg.get_window_width());
        const float height = static_cast<float>(cfg.get_window_height());
        const float depthScale = cfg.get_depth_scale();

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // First pass: draw world-space layers (non-overlay) from farthest to nearest
        for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
        {
            const Plane& layer = *(*it);
            if (layer.is_overlay()) continue; // skip overlays in world pass
            float d = layer.depth();
            float scale = 1.0f / (1.0f + d * depthScale); // perspective-like shrink
            if (scale <= 0.001f) continue;

            const auto& tris = layer.triangles();
            const auto& cols = layer.colors();
            const auto& texs = layer.textures();
            const auto& uvs = layer.uvs();
            for (size_t i = 0; i < tris.size(); ++i)
            {
                const Triangle& t = tris[i];
                SDL_Color col = cols[i];
                // SDL3 uses floating-point colors for vertex APIs
                SDL_FColor fcol = SDL_FColor{ col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, col.a / 255.0f };
                SDL_FPoint pts[3];
                for (int v = 0; v < 3; ++v)
                {
                    // t.v are normalized coords [0,1]. Apply camera (normalized) and parallax, then scale about center
                    float nx = (t.v[v].x - cam.x) * layer.parallax();
                    float ny = (t.v[v].y - cam.y) * layer.parallax();
                    // apply layer scale (perspective-like) around normalized center 0.5
                    nx = nx * scale + 0.5f;
                    ny = ny * scale + 0.5f;
                    float sx = nx * width;
                    float sy = ny * height;
                    pts[v] = SDL_FPoint{ sx, sy };
                }
                // Use stored per-triangle UVs if available
                SDL_Vertex sdlVerts[3];
                for (int vi = 0; vi < 3; ++vi) {
                    sdlVerts[vi].position = pts[vi];
                    sdlVerts[vi].color = fcol;
                    if (uvs.size() > i) sdlVerts[vi].tex_coord = uvs[i][vi];
                    else sdlVerts[vi].tex_coord = SDL_FPoint{0.0f, 0.0f};
                }
                SDL_Texture* tex = texs.size() > i ? texs[i] : nullptr;
                SDL_RenderGeometry(renderer, tex, sdlVerts, 3, nullptr, 0);
            }
        }

        // Second pass: draw overlay layers in screen-space (no camera/parallax/scale). Draw in insertion order.
        for (const auto& layerPtr : m_layers)
        {
            const Plane& layer = *layerPtr;
            if (!layer.is_overlay()) continue;
            // overlay quads: interpret triangle coordinates directly as screen-space positions (no cam/scale)
            const auto& tris = layer.triangles();
            const auto& cols = layer.colors();
            const auto& texs = layer.textures();
            const auto& uvs = layer.uvs();
            for (size_t i = 0; i < tris.size(); ++i)
            {
                const Triangle& t = tris[i];
                SDL_Color col = cols[i];
                SDL_FColor fcol = SDL_FColor{ col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, col.a / 255.0f };
                SDL_FPoint pts[3];
                for (int v = 0; v < 3; ++v)
                {
                    // Overlay coordinates are provided in normalized top-left space [0,1]
                    pts[v] = SDL_FPoint{ t.v[v].x * width, t.v[v].y * height };
                }
                // Use stored UVs if available, otherwise default to zero
                SDL_Vertex sdlVerts[3];
                for (int vi = 0; vi < 3; ++vi) {
                    sdlVerts[vi].position = pts[vi];
                    sdlVerts[vi].color = fcol;
                    if (uvs.size() > i) sdlVerts[vi].tex_coord = uvs[i][vi];
                    else sdlVerts[vi].tex_coord = SDL_FPoint{0.0f, 0.0f};
                }
                SDL_Texture* tex = texs.size() > i ? texs[i] : nullptr;
                SDL_RenderGeometry(renderer, tex, sdlVerts, 3, nullptr, 0);
            }
        }

        if (cfg.is_debug_overlay())
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            float cx = width * 0.5f;
            float cy = height * 0.5f;
            SDL_RenderLine(renderer, cx - 5.0f, cy, cx + 5.0f, cy);
            SDL_RenderLine(renderer, cx, cy - 5.0f, cx, cy + 5.0f);
        }

        // Detailed debug: draw layer bounds as rectangles and log extents
        if (cfg.is_debug_overlay())
        {
            int layerIndex = 0;
            for (const auto& layerPtr : m_layers)
            {
                const Plane& layer = *layerPtr;
                float minX = 1e9f, minY = 1e9f, maxX = -1e9f, maxY = -1e9f;
                const auto& tris = layer.triangles();
                for (const auto& t : tris)
                {
                    for (int v = 0; v < 3; ++v)
                    {
                        float sx, sy;
                        if (!layer.is_overlay()) {
                            // world mapping (same math as above)
                            float nx = (t.v[v].x - cam.x) * layer.parallax();
                            float ny = (t.v[v].y - cam.y) * layer.parallax();
                            nx = nx * (1.0f / (1.0f + layer.depth() * depthScale)) + 0.5f;
                            ny = ny * (1.0f / (1.0f + layer.depth() * depthScale)) + 0.5f;
                            sx = nx * width; sy = ny * height;
                        } else {
                            sx = t.v[v].x * width; sy = t.v[v].y * height;
                        }
                        minX = std::min(minX, sx); minY = std::min(minY, sy);
                        maxX = std::max(maxX, sx); maxY = std::max(maxY, sy);
                    }
                }
                SDL_Rect r{ static_cast<int>(minX), static_cast<int>(minY), static_cast<int>(std::max(1.0f, maxX - minX)), static_cast<int>(std::max(1.0f, maxY - minY)) };
                // choose a color per layer index
                Uint8 cr = (Uint8)((layerIndex * 47) & 0xFF);
                Uint8 cg = (Uint8)((layerIndex * 97) & 0xFF);
                Uint8 cb = (Uint8)((layerIndex * 157) & 0xFF);
                SDL_SetRenderDrawColor(renderer, cr, cg, cb, 200);
                // Draw rectangle outline with lines (SDL3 may not expose the older DrawRect helper)
                SDL_RenderLine(renderer, static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.x + r.w), static_cast<float>(r.y));
                SDL_RenderLine(renderer, static_cast<float>(r.x + r.w), static_cast<float>(r.y), static_cast<float>(r.x + r.w), static_cast<float>(r.y + r.h));
                SDL_RenderLine(renderer, static_cast<float>(r.x + r.w), static_cast<float>(r.y + r.h), static_cast<float>(r.x), static_cast<float>(r.y + r.h));
                SDL_RenderLine(renderer, static_cast<float>(r.x), static_cast<float>(r.y + r.h), static_cast<float>(r.x), static_cast<float>(r.y));
                SDL_Log("Layer %d bounds normalized [%f,%f]-[%f,%f] pixels [%d,%d]-[%d,%d]", layerIndex, minX/width, minY/height, maxX/width, maxY/height, r.x, r.y, r.x + r.w, r.y + r.h);
                layerIndex++;
            }
        }
    }

private:
    std::vector<std::unique_ptr<Plane>> m_layers;
};

#endif