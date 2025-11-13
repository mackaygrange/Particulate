#ifndef CAMERA_HPP
#define CAMERA_HPP

struct SimpleCamera
{
    // World-space camera position (world units)
    float x{0}, y{0}, z{0} ;

    // Scale: pixels per world-unit (how many pixels represent 1.0 in world space)
    float scale{10.0f};
};

#endif
