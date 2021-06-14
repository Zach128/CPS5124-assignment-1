#include <iostream>
#include <fstream>

#include "utils/vec.hpp"
#include "whitted-renderer.hpp"

void WhittedRenderer::prepare() {
    std::cout << "Preparing Whitted Renderer" << std::endl;

    framebuffer = std::vector<vec3f>(dimensions.x * dimensions.y);
}

void WhittedRenderer::render() {
    const int width = dimensions.x;
    const int height = dimensions.y;

    std::cout << "Rendering..." << std::endl;
    
    for (size_t j = 0; j < height; j++) {
        for(size_t i = 0; i < width; i++) {
            framebuffer[i + j * width] = vec3f(j / float(height), i / float(width), 0);
        }
    }
}

void WhittedRenderer::save() {
    const int width = dimensions.x;
    const int height = dimensions.y;

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
    r.dimensions = vec2i(j.at("dimensions").get<std::vector<int>>().data());
}

void from_json(const json &j, std::shared_ptr<WhittedRenderer> &r) {
    r = std::make_shared<WhittedRenderer>();

    j.at("type").get_to(r->type);
    j.at("samples").get_to(r->samples);
    j.at("depth").get_to(r->depth);
    j.at("output").get_to(r->output);
    r->dimensions = vec2i(j.at("dimensions").get<std::vector<int>>().data());
}
