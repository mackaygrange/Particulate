#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <cmath>
#include <cstdint>

struct Vertex
{
    float x{0}, y{0}, z{0}, w{1};
    Vertex() = default;
    Vertex(float _x,float _y,float _z,float _w=1.0f):x(_x), y(_y), z(_z), w(_w) {}
    Vertex& translate(float dx,float dy,float dz) { x+=dx; y+=dy; z+=dz; return *this; }
};

inline float dot(const Vertex& a, const Vertex& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float length(const Vertex& v) { return std::sqrt(dot(v, v)); }
inline Vertex normalized(const Vertex& v) { float len = length(v); return (len > 0) ? Vertex(v.x / len, v.y / len, v.z / len, 1.0f) : v; }
inline Vertex cross(const Vertex& a, const Vertex& b)
{
    return Vertex(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x,
    1.0f);
}

#endif