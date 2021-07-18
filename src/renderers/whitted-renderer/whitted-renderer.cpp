#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#include "models/rays/ray.hpp"
#include "models/scene.hpp"
#include "utils/vec.hpp"
#include "utils/utils.hpp"
#include "utils/mat.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/pinhole.hpp"
#include "models/shapes/sphere.hpp"
#include "models/lights/light.hpp"
#include "models/lights/point-light.hpp"
#include "models/primitives/primitive.hpp"
#include "models/rays/ray-sampler.hpp"

#include "whitted-renderer.hpp"

#define MAX_RAY_DEPTH 20
#define SPEC_ALBEDO 0.3
#define GLOSS_ALBEDO 0.8
#define FRES_ALBEDO 0.8

static size_t max_depth = MAX_RAY_DEPTH;
static std::vector<std::shared_ptr<Light>> lights;
static std::vector<std::shared_ptr<Primitive>> primitives;

void WhittedRenderer::prepare(const Scene &scene) {
    std::cout << "Preparing Whitted Renderer" << std::endl;

    lights = scene.lights;
    primitives = scene.primitives;
    max_depth = std::min(max_depth, depth);

    // Initialise frame buffers.
    Renderer::prepare(scene);
}

void WhittedRenderer::render(const std::shared_ptr<Camera> &camera) {
    std::cout << "Rendering..." << std::endl;
    RaySampler sampler = RaySampler(*camera, width, height, samples);
    size_t size = width * height;

    #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
        size_t x = i % width;
        size_t y = i / height;

        vec3f sample = vec3f(0, 0, 0);
        std::vector<RayInfo> rays;
        sampler.get_sample_rays(x, y, rays);

        for (RayInfo &ray : rays) {
            sample = sample + camera->renderer_cast_ray(*this, ray, depthbuffer[i]);
        }

        framebuffer[i] = sample / samples;
    }

    std::cout << "Done" << std::endl;
}

vec3f WhittedRenderer::cast_ray(const PinholeCamera &camera, const RayInfo &ray, float &dist, size_t depth = 0) {
    vec3f hit, N;
    std::shared_ptr<Primitive> primitive;
    vec3f total_color, diffuse_intensity, specular_intensity;

    // Perform the hit-test, saving the hit coordinates, angle, and material that was hit.
    if(depth > max_depth || !scene_intersect(ray, hit, N, dist, primitive)) {
        // If we didn't hit anything, return only the background colour.
        return vec3f(0.2, 0.7, 0.8);
    }

    // Record the depth value as an inverse reciprocal of the actual distance.
    dist = 1.f - dist / (1.f + dist);

    for (std::shared_ptr<Light> light : lights) {
        compute_diffuse_intensity(light, ray, hit, N, diffuse_intensity);
        // If the material is specular, calculate it's specular highlights.
        if (primitive->material->type == "specular reflection" || primitive->material->type == "glossy reflection")
            compute_specular_intensity(light, ray, hit, N, specular_intensity);
    }

    // Apply the intensity to the output vector.
    total_color = total_color + primitive->material->get_diffuse() * diffuse_intensity;
    // Add the new colour to the output.
    total_color = total_color + (primitive->material->get_specular() + specular_intensity * SPEC_ALBEDO);

    if (primitive->material->type == "glossy reflection") compute_glossy(camera, ray, hit, N, primitive->material, depth, total_color);
    else if (primitive->material->type == "fresnel dielectric") compute_fresnel(camera, ray, hit, N, primitive->material, depth, total_color);

    return total_color;
}

void from_json(const json &j, WhittedRenderer &r) {
    j.at("type").get_to(r.type);
    j.at("samples").get_to(r.samples);
    j.at("depth").get_to(r.depth);
    j.at("output").get_to(r.output);
    j.at("dimensions")[0].get_to(r.width);
    j.at("dimensions")[1].get_to(r.height);
}

void from_json(const json &j, std::shared_ptr<WhittedRenderer> &r) {
    r = std::make_shared<WhittedRenderer>();

    j.at("type").get_to(r->type);
    j.at("samples").get_to(r->samples);
    j.at("depth").get_to(r->depth);
    j.at("output").get_to(r->output);
    j.at("dimensions")[0].get_to(r->width);
    j.at("dimensions")[1].get_to(r->height);
}
