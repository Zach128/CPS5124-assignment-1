#include <iostream>
#include <fstream>
#include <cmath>

#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/primitive.hpp"

#include "whitted-renderer.hpp"

void WhittedRenderer::prepare() {
    std::cout << "Preparing Whitted Renderer" << std::endl;

    framebuffer = std::vector<vec3f>(width * height);
}

void WhittedRenderer::render(const std::shared_ptr<Camera> &camera, const std::vector<std::shared_ptr<Primitive>> &primitives) {
    const int fov = M_PI/2.;

    std::cout << "Rendering..." << std::endl;

    for (size_t j = 0; j < height; j++) {
        for(size_t i = 0; i < width; i++) {
            float dir_x = (i + 0.5) - width / 2.;
            float dir_y = -(j + 0.5) + height / 2.;
            float dir_z = -height / (2. * tan(fov / 2.));

            framebuffer[i + j * width] = camera->cast_ray(vec3f(0, 0, 0), vec3f(dir_x, dir_y, dir_z).normalize(), primitives);
        }
    }

    // for (size_t j = 0; j < height; j++) {
    //     for(size_t i = 0; i < width; i++) {
    //         framebuffer[i + j * width] = vec3f(j / float(height), i / float(width), 0);
    //     }
    // }
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
