#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "models/scene.hpp"
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/cameras/pinhole.hpp"
#include "models/shapes/sphere.hpp"
#include "models/primitive.hpp"

#include "whitted-renderer.hpp"

const static int fov = M_PI/2.;
static std::vector<std::shared_ptr<Primitive>> primitives;
static size_t curr_x = 0, curr_y = 0;
static vec3f curr_orig, curr_dir;

void WhittedRenderer::prepare(const Scene &scene) {
    std::cout << "Preparing Whitted Renderer" << std::endl;

    primitives = scene.primitives;
    framebuffer = std::vector<vec3f>(width * height);
    depthbuffer = std::vector<float>(width * height);
}

void WhittedRenderer::render(const std::shared_ptr<Camera> &camera) {

    std::cout << "Rendering..." << std::endl;

    // Save the current origin of the camera.
    curr_orig = camera->position;

    for (curr_y = 0; curr_y < height; curr_y++) {
        for(curr_x = 0; curr_x < width; curr_x++) {
            
            framebuffer[curr_x + curr_y * width] = camera->renderer_cast_ray(*this);
        }
    }

    depth_to_frame();
}

vec3f WhittedRenderer::cast_ray(PinholeCamera &camera) {
    // Generate direction coordinates for the given ray.
    float dir_x = (curr_x + 0.5) - width / 2.;
    float dir_y = -(curr_y + 0.5) + height / 2.;
    float dir_z = -height / (2. * tan(fov / 2.));

    // Compute the camera origin and facing direction.
    curr_dir = vec3f(dir_x, dir_y, dir_z).normalize();

    // Get a pointer to the current pixel's depth buffer.
    float &sphere_dist = depthbuffer[curr_x + curr_y * width];

    // Iterate over the entire list of primitives, checking which one is hit by the ray.
    for(const std::shared_ptr<Primitive> &primitive : primitives) {
        if(primitive->shape->renderer_ray_intersect(*this, sphere_dist)) {
            // Save the depth of the sphere.
            depthbuffer[curr_x + curr_y * width] = sphere_dist;

            // If we hit one, return its colour.
            return primitive->material->renderer_get_colour(*this);
        }
    }

    depthbuffer[curr_x + curr_y * width] = sphere_dist;

    // Else, return the environment colour.
    return vec3f(0.2, 0.7, 0.8);
}

void WhittedRenderer::depth_to_frame() {
    for (curr_y = 0; curr_y < height; curr_y++) {
        for(curr_x = 0; curr_x < width; curr_x++) {
            // Get the current pixel.
            float &curr_pixel = depthbuffer[curr_x + curr_y * width];

            // Post-process the pixel using an inserse exponential function.
            curr_pixel = 1 - curr_pixel / (1 + curr_pixel);
            
            framebuffer[curr_x + curr_y * width] = vec3f(curr_pixel, curr_pixel, curr_pixel);
        }
    }
}

bool WhittedRenderer::ray_intersect(const Sphere &sphere, float &t0) const {
    vec3f L = sphere.position - curr_orig;
    float sq_radius = sphere.radius * sphere.radius;
    float tca = L * curr_dir;
    float d2 = L * L - tca * tca;

    if (d2 > sq_radius) return false;

    float thc = sqrtf(sq_radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;

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

    for (size_t i = 0; i < width * height; i++) {
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].x)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].y)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].z)));
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
