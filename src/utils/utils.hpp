#pragma once

#include "utils/vec.hpp"
#include "utils/mat.hpp"

#include <random>
#include <cmath>

// Helpers for random number generation
static std::mt19937 mersenneTwister;
static std::uniform_real_distribution<double> uniform;

#define RND (2.0*uniform(mersenneTwister)-1.0)
#define RND2 (uniform(mersenneTwister))

#define DEG_RAD(deg) (deg * M_PI / 180.)

void createCoordinateSystem(const vec3f &N, vec3f &Nt, vec3f &Nb);
vec3f uniformSampleHemisphere(const double r1, const double r2);
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
Matrix44f lookAt(const vec3f& from, const vec3f& to, const vec3f& tmp = vec3f(0, 1, 0));
vec3f reflect(const vec3f &dir, const vec3f &N);
vec3f refract(const vec3f &dir, const vec3f &N, const float &refractive_index);
float fresnel(const vec3f &dir, const vec3f &N, const float &refractive_index, float &kr);
