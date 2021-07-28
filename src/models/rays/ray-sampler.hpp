#pragma once

#include "utils/mat.hpp"
#include "utils/utils.hpp"
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/rays/ray.hpp"

enum PrimaryRaySampling { UNIFORM, STRATIFIED };

class RaySampler {
    const std::shared_ptr<Camera> camera;
    const size_t frameWidth;
    const size_t frameHeight;
    const size_t samples;
    const PrimaryRaySampling method;
    size_t size;

    float viewportWidth;
    float viewportHeight;
    float focusDist;
    vec3f blCorner;
    vec3f horiz;
    vec3f vert;
    vec3f W, U, V;

    Matrix44f cameraToWorld;
    float scale;
    float pixelWidth;
    float pixelHeight;

public:
    RaySampler(const std::shared_ptr<Camera> &camera, const size_t frameWidth, const size_t frameHeight, const size_t samples, const PrimaryRaySampling method = STRATIFIED)
        : camera(camera), frameWidth(frameWidth), frameHeight(frameHeight), samples(samples), method(method), size(frameWidth * frameHeight) {
        // Get a camera-to-world matrix for the camera's location and orientation.
        cameraToWorld = lookAt(camera->position, camera->target);

        // Calculate the 'spread factor' for the generated rays.
        scale = camera->aspect * tan(DEG_RAD(camera->fov * 0.5));

        pixelWidth = 1.f / frameWidth;
        pixelHeight = 1.f / frameHeight;

        float theta = DEG_RAD(camera->fov);
        float h = tan(theta / 2);
        float focalDistance = 15.f;

        // Calculate a set of translation vectors for our camera's orientation and position.

        W = (camera->position - camera->target).normalOf();
        U = -cross(vec3f(0, 1, 0).normalOf(), W).normalOf();
        V = cross(W, U);

        viewportHeight = h * 2.f;
        viewportWidth = camera->aspect * viewportHeight;

        horiz = U * viewportWidth * focalDistance;
        vert = V * viewportHeight * focalDistance;
        blCorner = camera->position - horiz / 2.f - vert / 2.f - W * focalDistance;
    }

    void get_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
    void get_uniform_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
    void get_stratified_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
    void get_stratified_lens_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
};
