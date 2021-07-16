#pragma once

#include "utils/vec.hpp"
#include "utils/mat.hpp"

#include <cmath>

#define DEG_RAD(deg) (deg * M_PI / 180.)

void createCoordinateSystem(const vec3f &N, vec3f &Nt, vec3f &Nb);
vec3f uniformSampleHemisphere(const float &r1, const float &r2);
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
Matrix44f lookAt(const vec3f& from, const vec3f& to, const vec3f& tmp = vec3f(0, 1, 0));
vec3f reflect(const vec3f &I, const vec3f &N);
vec3f refract(const vec3f &I, const vec3f &N, const float &refractive_index);
float fresnel(const vec3f &I, const vec3f &N, const float &refractive_index, float &kr);
