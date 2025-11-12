#include "grid_plane.hpp"
#include "config.hpp"

GridPlane::GridPlane(float depth, float parallax, bool overlay, int cellSizePx)
    : m_depth(depth), m_parallax(parallax), m_overlay(overlay), m_cellSizePx(cellSizePx)
{
    // initially empty; caller will populate cells via setCell
}

void GridPlane::setCell(int gx, int gy, SDL_Color color, SDL_Texture* texture, std::optional<UVRect> uv)
{
    // Convert grid coords (gx,gy) to top-left pixel coordinate of the cell
    const Config& cfg = Config::get_instance();
    float px = static_cast<float>(gx * m_cellSizePx);
    float py = static_cast<float>(gy * m_cellSizePx);
    appendCellQuad(px, py, static_cast<float>(m_cellSizePx), static_cast<float>(m_cellSizePx), color, texture, uv);
}

void GridPlane::appendCellQuad(float px, float py, float wpx, float hpx, SDL_Color color, SDL_Texture* texture, std::optional<UVRect> uv)
{
    const Config& cfg = Config::get_instance();
    float width = static_cast<float>(cfg.get_window_width());
    float height = static_cast<float>(cfg.get_window_height());

    // Convert pixel coordinates to normalized [0,1] top-left origin coordinates
    float nx = px / width;
    float ny = py / height;
    float nw = wpx / width;
    float nh = hpx / height;

    Vertex a{ nx,     ny,     m_depth };
    Vertex b{ nx+nw,  ny,     m_depth };
    Vertex c{ nx+nw,  ny+nh,  m_depth };
    Vertex d{ nx,     ny+nh,  m_depth };

    m_colors.push_back(color);
    m_tris.emplace_back(a,b,c);
    m_colors.push_back(color);
    m_tris.emplace_back(a,c,d);

    m_textures.push_back(texture); m_textures.push_back(texture);

    float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
    if (uv.has_value()) { u0 = uv->u0; v0 = uv->v0; u1 = uv->u1; v1 = uv->v1; }
    m_uvs.push_back({ SDL_FPoint{u0,v0}, SDL_FPoint{u1,v0}, SDL_FPoint{u1,v1} });
    m_uvs.push_back({ SDL_FPoint{u0,v0}, SDL_FPoint{u1,v1}, SDL_FPoint{u0,v1} });
}
