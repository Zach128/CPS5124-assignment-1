#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <random>

#include "models/shapes/shape.hpp"
#include "models/rays/ray.hpp"
#include "models/scene.hpp"
#include "utils/vec.hpp"
#include "utils/utils.hpp"
#include "utils/mat.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/pinhole.hpp"
#include "models/shapes/sphere.hpp"
#include "models/lights/light.hpp"
#include "models/lights/area-light.hpp"
#include "models/lights/point-light.hpp"
#include "models/primitives/primitive.hpp"
#include "models/rays/ray-sampler.hpp"
#include "models/rays/ray.hpp"

#include "path-renderer.hpp"

#define MAX_RAY_DEPTH 20
#define MAX_LIGHT_SAMPLES 4
#define RR_PROB 0.1

static size_t max_depth = MAX_RAY_DEPTH;
static size_t russian_depth;
static std::vector<std::shared_ptr<Light>> lights;
static std::vector<std::shared_ptr<Primitive>> primitives;

void PathRenderer::prepare(const Scene &scene) {
    std::cout << "Preparing Path Renderer" << std::endl;

    lights = scene.lights;
    primitives = scene.primitives;
    max_depth = std::min(max_depth, depth);
    russian_depth = std::max(5, static_cast<int>(max_depth / 2));

    // Initialise frame buffers.
    Renderer::prepare(scene);
}

void PathRenderer::render(const std::shared_ptr<Camera> &camera) {
    RaySampler sampler = RaySampler(camera, width, height, samples);

    #pragma omp parallel for schedule(dynamic) private(lights)
    for (int x = 0; x < width; ++x) {
        fprintf(stdout, "\rRendering at %dspp: %8.3f%%", samples, (float) x / width * 100);

        for(int y = 0; y < height; ++y) {
            int i = x + y * width;

            vec3f sample = vec3f(0, 0, 0);
            std::vector<RayInfo> rays;

            sampler.get_sample_rays(x, y, rays);

            for (RayInfo &ray : rays) {
                sample = sample + cast_ray(*camera, ray, depthbuffer[i], 0);
            }

            framebuffer[i] = sample / samples;
        }
    }
}

void PathRenderer::compute_area_diffuse_intensity(const std::shared_ptr<AreaLight> &light, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out) {
    if (light->shape->type == ShapeType::SHAPE_SPHERE) {
        std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(light->shape);

        vec3f lightIntensity = light->intensity;
        vec3f lightPosition = sphere->position;
        float radius = sphere->radius;

        vec3f dirToCenter = (lightPosition - hit).normalOf();
        vec3f rotX, rotY;

        // Create a 3d matrix for converting from object space to world space.
        createCoordinateSystem(dirToCenter, rotX, rotY);
        vec3f rowX = vec3f(rotX.x, rotY.x, dirToCenter.x);
        vec3f rowY = vec3f(rotX.y, rotY.y, dirToCenter.y);
        vec3f rowZ = vec3f(rotX.z, rotY.z, dirToCenter.z);

        vec3f total_intensity;

        for(int scan = 0; scan < MAX_LIGHT_SAMPLES; ++scan) {
            // Pick a random point on a hemisphere.
            vec3f local = uniformSampleHemisphere(RND2, RND2);
            // The normal of the picked point.
            vec3f nwp;

            // Transform the point so that it faces the incident hit point.
            nwp.x = dot(rowX, local);
            nwp.y = dot(rowY, local);
            nwp.z = dot(rowZ, local);

            // The actual point of the light in world-space.
            vec3f pointInWorld = lightPosition - nwp * radius;
            vec3f dirToPoint = (pointInWorld - hit).normalize();
            float distToPoint = (pointInWorld - hit).norm();

            compute_diffuse_intensity(dirToPoint, lightIntensity, distToPoint, ray, hit, N, total_intensity);
        }

        out = out + total_intensity / MAX_LIGHT_SAMPLES;
    } else {
        compute_diffuse_intensity(light, ray, hit, N, out);
    }
}

vec3f PathRenderer::cast_ray(const Camera &camera, const RayInfo &ray, float &dist, size_t depth) {
    vec3f hit, N;
    std::shared_ptr<Primitive> primitive;

    vec3f total_color;

    // Russian roulette: starting at depth 5, each recursive step will stop with a probability of 0.1
	double rrFactor = 1.0;
	if (depth >= russian_depth) {
		if (RND2 <= RR_PROB) {
			return vec3f(0, 0, 0);
		}
		rrFactor = 1.0 / (1.0 - RR_PROB);
	}

    // Perform the hit-test, saving the hit coordinates, angle, and material that was hit.
    if(depth > max_depth || !scene_intersect(ray, hit, N, dist, primitive)) {
        // If we didn't hit anything, return only the background colour.
        return vec3f();
    }

    // Record the depth value as an inverse reciprocal of the actual distance.
    dist = 1.f - dist / (1.f + dist);

    // Add the primitive's emittance in case it's an emissive primitive.
    total_color = total_color + primitive->get_emittance() * primitive->material->get_diffuse() * rrFactor;

    if (primitive->material->type == MaterialType::MATERIAL_DIFFUSE) {
        vec3f diffuse_intensity;
        float tmp_dist;

        // Compute the direct diffusion lighting.
        for(std::shared_ptr<Light> light : lights) {

            if (light->type == LightType::LIGHT_AREA) {
                compute_area_diffuse_intensity(std::dynamic_pointer_cast<AreaLight>(light), ray, hit, N, diffuse_intensity);
            } else {
                compute_diffuse_intensity(light, ray, hit, N, diffuse_intensity);
            }
        }

        // Apply the intensity to the output vector.
        total_color = total_color + primitive->material->get_diffuse() * diffuse_intensity * rrFactor;

        // Compute the indirect diffusion lighting.
        vec3f rotX, rotY;
        createCoordinateSystem(N, rotX, rotY);

        vec3f sampledDir = uniformSampleHemisphere(RND2, RND2);
        vec3f rotatedDir;

        // Calculate the rotated coordinates.
        rotatedDir.x = dot(vec3f(rotX.x, rotY.x, N.x), sampledDir);
        rotatedDir.y = dot(vec3f(rotX.y, rotY.y, N.y), sampledDir);
        rotatedDir.z = dot(vec3f(rotX.z, rotY.z, N.z), sampledDir);
        RayInfo sampleRay(hit, rotatedDir); // Normally the direction would be normalised, but it's already been normalised.

        double cost = dot(sampleRay.dir, N);
        vec3f tmp = cast_ray(camera, sampleRay, tmp_dist, depth + 1);
        total_color = total_color + (tmp * primitive->material->get_diffuse()) * cost * rrFactor;
        // total_color = total_color + (tmp * primitive->material->get_diffuse()) * cost * 0.1 * rrFactor;
    }

    if (primitive->material->type == MaterialType::MATERIAL_GLOSSY || primitive->material->type == MaterialType::MATERIAL_SPECULAR) {
        vec3f glossy_intensity;

        compute_glossy(camera, ray, hit, N, primitive->material, depth, glossy_intensity);

        total_color = total_color + glossy_intensity * rrFactor;
    }

    if (primitive->material->type == MaterialType::MATERIAL_FRESNEL) {
        vec3f fresnel_intensity;

        compute_fresnel(camera, ray, hit, N, primitive->material, depth, fresnel_intensity);

        total_color = total_color + fresnel_intensity * rrFactor;
    }

    return total_color;
}

void from_json(const json &j, PathRenderer &r) {
    j.at("type").get_to(r.type);
    j.at("samples").get_to(r.samples);
    j.at("depth").get_to(r.depth);
    j.at("output").get_to(r.output);
    j.at("dimensions")[0].get_to(r.width);
    j.at("dimensions")[1].get_to(r.height);
}

void from_json(const json &j, std::shared_ptr<PathRenderer> &r) {
    r = std::make_shared<PathRenderer>();

    j.at("type").get_to(r->type);
    j.at("samples").get_to(r->samples);
    j.at("depth").get_to(r->depth);
    j.at("output").get_to(r->output);
    j.at("dimensions")[0].get_to(r->width);
    j.at("dimensions")[1].get_to(r->height);
}
