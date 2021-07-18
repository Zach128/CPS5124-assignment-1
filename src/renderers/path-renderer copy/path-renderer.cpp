#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <random>

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

#include "path-renderer.hpp"

#define MAX_RAY_DEPTH 20
#define DIFFUSE_ALBEDO 1.0
#define SPEC_SHINE 16
#define SPEC_ALBEDO 0.3
#define GLOSS_ALBEDO 0.8
#define FRES_ALBEDO 0.8

static const float infinity = std::numeric_limits<float>::max();
static const float epsilon = 1e-8;
static const float bias = 1e-3;

static size_t max_depth = MAX_RAY_DEPTH;
static Matrix44f cameraToWorld;
static float scale;
static std::vector<std::shared_ptr<Light>> lights;
static std::vector<std::shared_ptr<Primitive>> primitives;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

void PathRenderer::prepare(const Scene &scene) {
    std::cout << "Preparing Path Renderer" << std::endl;

    lights = scene.lights;
    primitives = scene.primitives;
    max_depth = std::min(max_depth, depth);

    // Initialise frame buffers.
    framebuffer = std::vector<vec3f>(width * height);
    depthbuffer = std::vector<float>(width * height);
}

void PathRenderer::render(const std::shared_ptr<Camera> &camera) {
    std::cout << "Rendering..." << std::endl;
    vec3f dir;
    float dir_x, dir_y, dir_z;

    // Get a camera-to-world matrix for the camera's location and orientation.
    cameraToWorld = lookAt(camera->position, camera->target);
    // Calculate the 'spread factor' for the generated rays.
    scale = camera->aspect * tan(DEG_RAD(camera->fov * 0.5));

    for (int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            // Calculate the ray direction of the current pixel in the framebuffer.
            dir_x = -(2. * (x + .5) / width - 1) * scale;
            dir_y = (1 - 2 * (y + .5) / height) * scale;
            dir_z = -1;

            // Convert the ray direction to world space to obtain the true coordinates.
            cameraToWorld.multDirMatrix(vec3f(dir_x, dir_y, dir_z), dir);
            dir = dir.normalize();

            framebuffer[x + y * width] = camera->renderer_cast_ray(*this, camera->position, dir, depthbuffer[x + y * width]);
        }
    }

    std::cout << "Done" << std::endl;
}

bool PathRenderer::trace(const vec3f &orig, const vec3f &dir, IsectInfo &isect, const RayType) {
    isect.hitObject = nullptr;
    for (std::shared_ptr<Primitive> primitive : primitives) {
        float tNear = infinity;

        if (primitive->shape->renderer_ray_intersect((*this), orig, dir, tNear) && tNear < isect.tNear) {
            isect.hitObject = primitive;
            isect.tNear = tNear;
        }
    }

    return (isect.hitObject != nullptr);
}

vec3f PathRenderer::cast_ray(const PinholeCamera &camera, const vec3f &orig, const vec3f &dir, float &dist, size_t depth = 0) {
    if (depth > max_depth) return 0;//options.backgroundColor;

    vec3f hitColor;
    IsectInfo isect;
    if (trace(orig, dir, isect)) {
        vec3f hitPoint = orig + dir * isect.tNear;
        vec3f hitNormal;

        dist = 1 - isect.tNear / (1 + isect.tNear);

        isect.hitObject->shape->getSurfaceProperties(hitPoint, hitNormal);
        if (isect.hitObject->material->type == "diffuse") {
            vec3f directLighting;

            for (std::shared_ptr<Light> light : lights) {
                vec3f lightDir, lightIntensity;
                IsectInfo isectShad;

                light->illuminate(hitPoint, lightDir, lightIntensity, isectShad.tNear);

                bool vis = !trace(hitPoint + hitNormal * bias, -lightDir, isectShad, shadowRay);

                if(vis) directLighting = isect.hitObject->material->get_diffuse() * lightIntensity * std::max(0.f, dot(hitNormal, -lightDir));
            }

            vec3f indirectLigthing;
// #ifdef GI
            uint32_t N = 128;// / (depth + 1);
            vec3f Nt, Nb;
            createCoordinateSystem(hitNormal, Nt, Nb);
            float pdf = 1 / (2 * M_PI);
            float indirectDistance;
            for (uint32_t n = 0; n < N; ++n) {
                float r1 = distribution(generator);
                float r2 = distribution(generator);
                vec3f sample = uniformSampleHemisphere(r1, r2);
                vec3f sampleWorld(
                    sample.x * Nb.x + sample.y * hitNormal.x + sample.z * Nt.x,
                    sample.x * Nb.y + sample.y * hitNormal.y + sample.z * Nt.y,
                    sample.x * Nb.z + sample.y * hitNormal.z + sample.z * Nt.z);
                // don't forget to divide by PDF and multiply by cos(theta)
                indirectLigthing = indirectLigthing + (cast_ray(camera, hitPoint + sampleWorld * bias, sampleWorld, indirectDistance, depth + 1) / pdf) * r1;
            }
            // divide by N
            indirectLigthing = indirectLigthing / (float)N;
// #endif

            hitColor = (directLighting / M_PI + indirectLigthing * 2.f) * DIFFUSE_ALBEDO;
        }
    }
    else {
        hitColor = vec3f(1, 1, 1);
    }

    return hitColor;
}

bool PathRenderer::ray_intersect(const Sphere &sphere, const vec3f &orig, const vec3f &dir, float &t0) const {
    vec3f L = sphere.position - orig;
    float sq_radius = sphere.radius * sphere.radius;
    float tca = dot(L, dir);
    float d2 = dot(L, L) - tca * tca;

    if (d2 > sq_radius) return false;

    float thc = sqrtf(sq_radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 1e-3) t0 = t1;
    if (t0 < 1e-3) return false;

    return true;
}

void PathRenderer::save() {
    std::cout << "Saving final result..." << std::endl;

    float gamma = 1;
    std::ofstream ofs;

    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height; i++) {
        ofs << (char)(255 * std::clamp(powf(framebuffer[i].x, 1/gamma), 0.f, 1.f));
        ofs << (char)(255 * std::clamp(powf(framebuffer[i].y, 1/gamma), 0.f, 1.f));
        ofs << (char)(255 * std::clamp(powf(framebuffer[i].z, 1/gamma), 0.f, 1.f));
    }

    ofs.close();

    std::cout << "Done" << std::endl;

    save_depth();
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
