#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>
#include <optional>
#include <memory>
#include <SDL3/SDL.h>
#include "triangle.hpp"

// Forward-declare UVRect used by both PlaneLayer and GridPlane
struct UVRect { float u0, v0, u1, v1; };

// Abstract interface for a renderable plane/layer. Allows different plane types
// (simple quad-based layers, grid-based layers, overlays) to be stored and
// rendered polymorphically by LayeredScene.
class Plane
{
public:
    virtual ~Plane() = default;
    virtual const std::vector<Triangle>& triangles() const = 0;
    virtual const std::vector<SDL_Color>& colors() const = 0;
    virtual const std::vector<SDL_Texture*>& textures() const = 0;
    virtual const std::vector<std::array<SDL_FPoint,3>>& uvs() const = 0;
    virtual float depth() const = 0;
    virtual float parallax() const = 0;
    virtual bool is_overlay() const = 0;
};

// Represents a logical depth layer composed of 2D quads (as two triangles) projected with simple parallax.
class PlaneLayer : public Plane
{
public:
    explicit PlaneLayer(float depth = 0.0f, float parallax = 1.0f, bool overlay = false)
        : m_depth(depth), m_parallax(parallax), m_overlay(overlay) {}

    // Adds a quad defined by top-left (x,y) and size (w,h); z is this layer's depth.
    // Coordinates are normalized in [0,1] with origin at the top-left of the render target
    // (so x=0,y=0 => top-left; x=1,y=1 => bottom-right). Width/height are also normalized
    // (w = fraction of render width, h = fraction of render height).
    // Optionally pass a UV rectangle (u0,v0,u1,v1) where values are normalized [0,1].
    void addQuad(float x,float y,float w,float h, SDL_Color color, std::optional<SDL_Texture*> texture = std::nullopt, std::optional<UVRect> uvRect = std::nullopt)
    {
        // Two triangles forming a rectangle.
        Vertex a{x,     y,     m_depth};
        Vertex b{x+w,   y,     m_depth};
        Vertex c{x+w,   y+h,   m_depth};
        Vertex d{x,     y+h,   m_depth};
        m_colors.push_back(color); // for first triangle
        m_tris.emplace_back(a,b,c);
        m_colors.push_back(color); // second triangle color (same)
        m_tris.emplace_back(a,c,d);
        if (texture.has_value()) { m_textures.push_back(texture.value()); m_textures.push_back(texture.value()); }
        else { m_textures.push_back(nullptr); m_textures.push_back(nullptr); }

        // Compute and store per-triangle UVs. If uvRect provided, map quad area to that rect; otherwise default full-rect.
        float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
        if (uvRect.has_value()) { u0 = uvRect->u0; v0 = uvRect->v0; u1 = uvRect->u1; v1 = uvRect->v1; }
        // Triangle 1 (a,b,c)
        m_uvs.push_back({ SDL_FPoint{u0,v0}, SDL_FPoint{u1,v0}, SDL_FPoint{u1,v1} });
        // Triangle 2 (a,c,d)
        m_uvs.push_back({ SDL_FPoint{u0,v0}, SDL_FPoint{u1,v1}, SDL_FPoint{u0,v1} });
    }

    const std::vector<Triangle>& triangles() const override { return m_tris; }
    const std::vector<SDL_Color>& colors() const override { return m_colors; }
    const std::vector<SDL_Texture*>& textures() const override { return m_textures; }
    const std::vector<std::array<SDL_FPoint,3>>& uvs() const override { return m_uvs; }

    float depth() const override { return m_depth; }
    float parallax() const override { return m_parallax; }
    bool is_overlay() const override { return m_overlay; }

private:
    float m_depth;
    float m_parallax; // 1.0 = foreground, smaller = slower movement
    std::vector<Triangle> m_tris;
    std::vector<SDL_Color> m_colors;
    std::vector<SDL_Texture*> m_textures;
    bool m_overlay = false;
    std::vector<std::array<SDL_FPoint,3>> m_uvs; // per-triangle UVs
};

// Explicit overlay plane: screen-space UI plane whose coordinates are interpreted
// as normalized top-left [0,1] coordinates and which is always treated as overlay.
class OverlayPlane : public Plane
{
public:
    explicit OverlayPlane(float depth = 0.0f)
        : m_depth(depth)
    {}

    // Add quad in normalized top-left coordinates
    void addQuad(float x,float y,float w,float h, SDL_Color color, std::optional<SDL_Texture*> texture = std::nullopt, std::optional<UVRect> uvRect = std::nullopt)
    {
        Vertex a{x,     y,     m_depth};
        Vertex b{x+w,   y,     m_depth};
        Vertex c{x+w,   y+h,   m_depth};
        Vertex d{x,     y+h,   m_depth};
        m_colors.push_back(color);
        m_tris.emplace_back(a,b,c);
        m_colors.push_back(color);
        m_tris.emplace_back(a,c,d);
        if (texture.has_value()) { m_textures.push_back(texture.value()); m_textures.push_back(texture.value()); }
        else { m_textures.push_back(nullptr); m_textures.push_back(nullptr); }

        float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
        if (uvRect.has_value()) { u0 = uvRect->u0; v0 = uvRect->v0; u1 = uvRect->u1; v1 = uvRect->v1; }
        m_uvs.push_back({ SDL_FPoint{u0,v0}, SDL_FPoint{u1,v0}, SDL_FPoint{u1,v1} });
        m_uvs.push_back({ SDL_FPoint{u0,v0}, SDL_FPoint{u1,v1}, SDL_FPoint{u0,v1} });
    }

    const std::vector<Triangle>& triangles() const override { return m_tris; }
    const std::vector<SDL_Color>& colors() const override { return m_colors; }
    const std::vector<SDL_Texture*>& textures() const override { return m_textures; }
    const std::vector<std::array<SDL_FPoint,3>>& uvs() const override { return m_uvs; }
    float depth() const override { return m_depth; }
    float parallax() const override { return 1.0f; }
    bool is_overlay() const override { return true; }

private:
    float m_depth{0.0f};
    std::vector<Triangle> m_tris;
    std::vector<SDL_Color> m_colors;
    std::vector<SDL_Texture*> m_textures;
    std::vector<std::array<SDL_FPoint,3>> m_uvs;
};

#endif