#include "utils/vec.hpp"
#include "utils/mat.hpp"

#include <cmath>

#define DEG_RAD(deg) (deg * M_PI / 180.)

void createCoordinateSystem(const vec3f &N, vec3f &Nt, vec3f &Nb) {
    if (std::fabs(N.x) > std::fabs(N.y))
        Nt = vec3f(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
    else
        Nt = vec3f(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
    Nb = cross(N, Nt);
}

vec3f uniformSampleHemisphere(const float &r1, const float &r2) {
    // cos(theta) = u1 = y
    // cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
    float sinTheta = sqrtf(1 - r1 * r1);
    float phi = 2 * M_PI * r2;
    float x = sinTheta * cosf(phi);
    float z = sinTheta * sinf(phi);
    return vec3f(x, r1, z);
}

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) {
        x0 = x1 = - 0.5 * b / a;
    }
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    return true;
}

// Taken from https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
Matrix44f lookAt(const vec3f& from, const vec3f& to, const vec3f& tmp = vec3f(0, 1, 0)) {
    // Compute the direction vectors relative to the camera.
    vec3f forward = (from - to).normalize();
    vec3f right = cross(tmp.normalOf(), forward);
    vec3f up = cross(forward, right);

    Matrix44f camToWorld;

    // Record the directional vectors.
    camToWorld[0][0] = right.x;
    camToWorld[0][1] = right.y;
    camToWorld[0][2] = right.z;
    camToWorld[1][0] = up.x;
    camToWorld[1][1] = up.y;
    camToWorld[1][2] = up.z;
    camToWorld[2][0] = forward.x;
    camToWorld[2][1] = forward.y;
    camToWorld[2][2] = forward.z;

    // Record the target/orientation vector.
    camToWorld[3][0] = from.x;
    camToWorld[3][1] = from.y;
    camToWorld[3][2] = from.z;

    return camToWorld;
}

vec3f reflect(const vec3f &I, const vec3f &N) {
    return I - N * 2.f * (dot(I, N));
}

vec3f refract(const vec3f &I, const vec3f &N, const float &refractive_index) {
    //Snell's law

    float cosi = -std::max(-1.f, std::min(1.f, dot(I, N)));
    float etai = 1, etat = refractive_index;
    vec3f n = N;

    // Ensure the ray is outside the object, swapping the indices and inverting the normal if it is inside the object.
    if (cosi < 0) {
        cosi = -cosi;
        std::swap(etai, etat);
        n = -N;
    }

    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);

    return k < 0 ? vec3f(1, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
}

float fresnel(const vec3f &I, const vec3f &N, const float &refractive_index, float &kr)
{
    float cosi = std::max(-1.f, std::min(1.f, dot(I, N)));
    float etai = 1, etat = refractive_index;

    if (cosi > 0) { std::swap(etai, etat); }

    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));

    // If we have total internal reflection, No need to calculate refraction.
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }

    return 1 - kr;
}
