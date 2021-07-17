#pragma once

#include "utils/mat.hpp"
#include "utils/utils.hpp"
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/rays/ray.hpp"

enum PrimaryRaySampling { UNIFORM, STRATIFIED };

class RaySampler {
    const Camera &camera;
    const size_t frameWidth;
    const size_t frameHeight;
    const size_t samples;
    const PrimaryRaySampling method;
    size_t size;

    Matrix44f cameraToWorld;
    float scale;
    float pixelWidth;
    float pixelHeight;

public:
    RaySampler(const Camera &camera, const size_t frameWidth, const size_t frameHeight, const size_t samples, const PrimaryRaySampling method = STRATIFIED)
        : camera(camera), frameWidth(frameWidth), frameHeight(frameHeight), samples(samples), method(method), size(frameWidth * frameHeight) {
        // Get a camera-to-world matrix for the camera's location and orientation.
        cameraToWorld = lookAt(camera.position, camera.target);
        // Calculate the 'spread factor' for the generated rays.
        scale = camera.aspect * tan(DEG_RAD(camera.fov * 0.5));

        pixelWidth = 1.f / frameWidth;
        pixelHeight = 1.f / frameHeight;
    }

    void get_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
    void get_uniform_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
    void get_stratified_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays);
};
