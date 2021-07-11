#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "models/scene.hpp"
#include "utils/vec.hpp"
#include "utils/utils.hpp"
#include "utils/mat.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/pinhole.hpp"
#include "models/shapes/sphere.hpp"
#include "models/lights/light.hpp"
#include "models/lights/point-light.hpp"
#include "models/primitive.hpp"

#include "whitted-renderer.hpp"

static Matrix44f cameraToWorld;
static float scale;
static std::vector<std::shared_ptr<Light>> lights;
static std::vector<std::shared_ptr<Primitive>> primitives;

void WhittedRenderer::prepare(const Scene &scene) {
    std::cout << "Preparing Whitted Renderer" << std::endl;

    lights = scene.lights;
    primitives = scene.primitives;
    framebuffer = std::vector<vec3f>(width * height);
    depthbuffer = std::vector<float>(width * height);
}

void WhittedRenderer::render(const std::shared_ptr<Camera> &camera) {
    std::cout << "Rendering..." << std::endl;

    // Get a camera-to-world matrix for the camera's location and orientation.
    cameraToWorld = lookAt(camera->position, camera->target);
    // Calculate the 'spread factor' for the generated rays.
    scale = tan(DEG_RAD(camera->fov * 0.5));

    for (int curr_y = 0; curr_y < height; curr_y++) {
        for(int curr_x = 0; curr_x < width; curr_x++) {
            framebuffer[curr_x + curr_y * width] = camera->renderer_cast_ray(*this, vec2i(curr_x, curr_y));
        }
    }
}

void compute_diffuse_intensity(const vec3f &hit, const vec3f &N, vec3f &out) {
    vec3f intensity(0, 0, 0);

    // Calculate diffuse lighting
    for (size_t i = 0; i < lights.size(); i++) {
        vec3f light_dir = (lights[i]->position - hit).normalize();
        intensity = intensity + lights[i]->intensity * std::max(0.f, light_dir * N);
    }

    // Apply the intensity to the output vector.
    out.x *= intensity.x;
    out.y *= intensity.y;
    out.z *= intensity.z;
}

bool WhittedRenderer::scene_intersect(const vec3f &orig, const vec3f &dir, vec3f &hit, vec3f &N, float &dist, std::shared_ptr<Material> &material) {
    dist = std::numeric_limits<float>::max();

    // Iterate over each sphere.
    for (size_t i = 0; i < primitives.size(); i++) {
        float dist_i;

        // If the sphere is hit, record it.
        if (primitives[i]->shape->renderer_ray_intersect(*this, orig, dir, dist_i) && dist_i <= dist) {
            dist = dist_i;
            hit = orig + dir * dist_i;
            N = (hit - primitives[i]->shape->position).normalize();
            material = primitives[i]->material;
        }
    }

    // If the sphere is closer than the max draw distance, return true.
    return dist < 1000;
}

vec3f WhittedRenderer::cast_ray(PinholeCamera &camera, const vec2i &frame_coords) {
    const int &x = frame_coords.x, &y = frame_coords.y;

    vec3f hit, N, dir;
    std::shared_ptr<Material> material;
    vec3f total_color, diffuse_intensity;

    // Generate direction coordinates the new ray.
    float dir_x = -(2. * (x + .5) / width - 1) * camera.aspect * scale;
    float dir_y = (1 - 2 * (y + .5) / height) * scale;
    float dir_z = -1;

    // Compute the camera origin and facing direction.
    cameraToWorld.multDirMatrix(vec3f(dir_x, dir_y, dir_z), dir);
    dir = dir.normalize();

    // Get a pointer to the current pixel's depth buffer.
    float &sphere_dist = depthbuffer[x + y * width];

    // Perform the hit-test, saving the hit coordinates, angle, and material that was hit.
    if(!scene_intersect(camera.position, dir, hit, N, sphere_dist, material)) {
        // If nothing was hit, set the distance to be the clipping distance.
        depthbuffer[x + y * width] = sphere_dist;

        // If we didn't hit anything, return only the background colour.
        return vec3f(0.2, 0.7, 0.8);
    }

    // Record the depth value as an inverse reciprocal of the actual distance.
    depthbuffer[x + y * width] = 1 - sphere_dist / (1 + sphere_dist);

    total_color = material->get_diffuse(*this);

    compute_diffuse_intensity(hit, N, total_color);

    return total_color;
}

bool WhittedRenderer::ray_intersect(const Sphere &sphere, const vec3f &orig, const vec3f &dir, float &t0) const {
    vec3f L = sphere.position - orig;
    float sq_radius = sphere.radius * sphere.radius;
    float tca = L * dir;
    float d2 = L * L - tca * tca;

    if (d2 > sq_radius) return false;

    float thc = sqrtf(sq_radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 1e-3) t0 = t1;
    if (t0 < 1e-3) return false;

    return true;
}

vec3f WhittedRenderer::get_diffuse(const DiffuseMaterial &mat) {
    return mat.rho;
}

void WhittedRenderer::save() {
    std::cout << "Saving final result..." << std::endl;

    std::ofstream ofs;

    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height; i++) {
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].x)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].y)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].z)));
    }

    ofs.close();

    std::cout << "Done" << std::endl;

    save_depth();
}

void WhittedRenderer::save_depth() {
    std::cout << "Saving depth buffer..." << std::endl;

    std::ofstream ofs;

    ofs.open("./out-depth.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height; i++) {
        ofs << (char)(255.f * std::max(0.f, std::min(1.f, depthbuffer[i])));
        ofs << (char)(255.f * std::max(0.f, std::min(1.f, depthbuffer[i])));
        ofs << (char)(255.f * std::max(0.f, std::min(1.f, depthbuffer[i])));
    }

    ofs.close();

    std::cout << "Done" << std::endl;
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
