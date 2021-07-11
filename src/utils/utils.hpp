#pragma once

#include "utils/vec.hpp"
#include "utils/mat.hpp"

#include <cmath>

#define DEG_RAD(deg) (deg * M_PI / 180.)

// Taken from https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
Matrix44f lookAt(const vec3f& from, const vec3f& to, const vec3f& tmp = vec3f(0, 1, 0))
{
    vec3f forward = (from - to).normalize();
    vec3f right = cross(tmp * (1 / tmp.norm()), forward);
    vec3f up = cross(forward, right);

    Matrix44f camToWorld;

    camToWorld[0][0] = right.x;
    camToWorld[0][1] = right.y;
    camToWorld[0][2] = right.z;
    camToWorld[1][0] = up.x;
    camToWorld[1][1] = up.y;
    camToWorld[1][2] = up.z;
    camToWorld[2][0] = forward.x;
    camToWorld[2][1] = forward.y;
    camToWorld[2][2] = forward.z;

    camToWorld[3][0] = from.x;
    camToWorld[3][1] = from.y;
    camToWorld[3][2] = from.z;

    return camToWorld;
}
