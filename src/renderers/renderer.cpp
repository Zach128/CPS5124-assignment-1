#include <iostream>
#include <fstream>

#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

void Renderer::save() {
    std::cout << "Saving final result..." << std::endl;

    std::ofstream ofs;

    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height; i++) {
        ofs << (char)(std::max(0.f, std::min(255.f, framebuffer[i].x)));
        ofs << (char)(std::max(0.f, std::min(255.f, framebuffer[i].y)));
        ofs << (char)(std::max(0.f, std::min(255.f, framebuffer[i].z)));
    }

    ofs.close();

    std::cout << "Done" << std::endl;

    save_depth();
}

void Renderer::save_depth() {
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

void from_json(const json &j, Renderer &r) {
    j.at("type").get_to(r.type);
    j.at("samples").get_to(r.samples);
    j.at("depth").get_to(r.depth);
    j.at("output").get_to(r.output);

    j.at("dimensions")[0].get_to(r.width);
    j.at("dimensions")[1].get_to(r.height);
}

void from_json(const json &j, std::shared_ptr<Renderer> &r) {
    r = std::make_shared<Renderer>();

    j.at("type").get_to(r->type);
    j.at("samples").get_to(r->samples);
    j.at("depth").get_to(r->depth);
    j.at("output").get_to(r->output);
    j.at("dimensions")[0].get_to(r->width);
    j.at("dimensions")[1].get_to(r->height);
}
