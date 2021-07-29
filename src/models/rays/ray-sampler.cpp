#include "utils/mat.hpp"
#include "utils/utils.hpp"
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/lens-camera.hpp"
#include "models/rays/ray.hpp"

#include "ray-sampler.hpp"

void RaySampler::get_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays) {
    if (method == UNIFORM) get_uniform_sample_rays(x, y, rays);
    else if (camera->type == CameraType::CAMERA_LENS) get_stratified_lens_rays(x, y, rays);
    else get_stratified_sample_rays(x, y, rays);
}

void RaySampler::get_uniform_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays) {
    vec3f worldDir;

    for (size_t j = 0; j < samples; j++) {
        // Calculate a uniform sample offset.
        float sx = pixelWidth * RND;
        float sy = pixelHeight * RND;

        float u = (x + sx) / (frameWidth - 1.f);
        float v = (y + sy) / (frameHeight - 1.f);

        // Convert the pixel coordinates to world coordinates.
        worldDir = blCorner + horiz * u + vert * v - camera->position;
        worldDir.normalize();

        rays.push_back(RayInfo(camera->position, worldDir));
    }
}

void RaySampler::get_stratified_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays) {
    vec3f worldDir;
    float e = powf(samples, 0.5f);

    for (size_t j = 0; j < samples; j++) {
        float sx = RND2;
        float sy = RND2;

        // Calculate a stratified sample offset.
        sx = (pixelWidth / e) * (fmod(j, e) + sx);
        sy = (pixelHeight / e) * (j / e + sy);

        float u = (x + sx) / (frameWidth - 1.f);
        float v = (y + sy) / (frameHeight - 1.f);

        // Convert the pixel coordinates to world coordinates.
        worldDir = blCorner + horiz * u + vert * v - camera->position;
        worldDir.normalize();

        rays.push_back(RayInfo(camera->position, worldDir));
    }
}

void RaySampler::get_stratified_lens_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays) {
    std::shared_ptr<LensCamera> lensCamera = std::dynamic_pointer_cast<LensCamera>(camera);
    vec3f worldDir;

    for (size_t j = 0; j < samples; j++) {
        // Calculate a an offset along the aperture.

        float theta = 2 * M_PI * RND2;
        float r = (lensCamera->alpha / 2.f * sqrtf(RND2)) / 2.f;
        vec3f offset = U * (r * cosf(theta)) + V * (r * sinf(theta));
        vec3f newOrigin = lensCamera->position + offset;

        float u = (x) / (frameWidth - 1.f);
        float v = (y) / (frameHeight - 1.f);

        // Convert the pixel coordinates to world coordinates.
        worldDir = blCorner + horiz * u + vert * v - newOrigin;
        worldDir.normalize();

        rays.push_back(RayInfo(newOrigin, worldDir));
    }
}
