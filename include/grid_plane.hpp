#ifndef GRID_PLANE_HPP
#define GRID_PLANE_HPP

#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>
#include "layer.hpp"

// Represents a grid of fixed-size cells (64x64 pixels) addressable by integer Cartesian coordinates.
// Internally the grid stores quads as triangles in normalized coordinates so the renderer can draw them
// via the existing Plane interface.
class GridPlane : public Plane
{
public:
    explicit GridPlane(float depth = 0.0f, float parallax = 1.0f, bool overlay = false, int cellSizePx = 64);

    // Place or update a cell at integer grid coordinates (x,y). The provided color and optional texture
    // will be used for the cell quad. The cell occupies a cellSizePx x cellSizePx area in pixel space.
    void setCell(int gx, int gy, SDL_Color color, SDL_Texture* texture = nullptr, std::optional<UVRect> uv = std::nullopt);

    // Plane interface
    const std::vector<Triangle>& triangles() const override { return m_tris; }
    const std::vector<SDL_Color>& colors() const override { return m_colors; }
    const std::vector<SDL_Texture*>& textures() const override { return m_textures; }
    const std::vector<std::array<SDL_FPoint,3>>& uvs() const override { return m_uvs; }
    float depth() const override { return m_depth; }
    float parallax() const override { return m_parallax; }
    bool is_overlay() const override { return m_overlay; }

private:
    // Helper to convert a pixel rect to normalized [0,1] coordinates using current config at generation time.
    void appendCellQuad(float px, float py, float wpx, float hpx, SDL_Color color, SDL_Texture* texture, std::optional<UVRect> uv);

    float m_depth;
    float m_parallax;
    bool m_overlay;
    int m_cellSizePx;

    std::vector<Triangle> m_tris;
    std::vector<SDL_Color> m_colors;
    std::vector<SDL_Texture*> m_textures;
    std::vector<std::array<SDL_FPoint,3>> m_uvs;
};

#endif
