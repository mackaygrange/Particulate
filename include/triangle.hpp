#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <array>
#include <SDL3/SDL.h>
#include "vertex.hpp"

struct Triangle
{
    std::array<Vertex, 3> v;
    Triangle() = default;
    Triangle(const Vertex& a, const Vertex& b, const Vertex& c) { v[0] = a; v[1] = b; v[2] = c; }
    void translate(float dx, float dy, float dz) { for (auto& p : v) p.translate(dx, dy, dz); }
};

inline Vertex face_normal(const Triangle& t)
{
    Vertex l1{t.v[1].x - t.v[0].x, t.v[1].y - t.v[0].y, t.v[1].z - t.v[0].z};
    Vertex l2{t.v[2].x - t.v[0].x, t.v[2].y - t.v[0].y, t.v[2].z - t.v[0].z};
    return normalized(cross(l1, l2));
}

#endif