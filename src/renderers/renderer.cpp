#include <iostream>

#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

void Renderer::prepare() {
    std::cout << "Preparing..." << std::endl;
}

void Renderer::render() {
    std::cout << "Rendering..." << std::endl;
}

void Renderer::save() {
    std::cout << "Saving..." << std::endl;
}

void from_json(const json &j, Renderer &r) {
    j.at("type").get_to(r.type);
    j.at("samples").get_to(r.samples);
    j.at("depth").get_to(r.depth);
    j.at("output").get_to(r.output);
    r.dimensions = vec2i(j.at("dimensions").get<std::vector<int>>().data());
}

void from_json(const json &j, std::shared_ptr<Renderer> &r) {
    r = std::make_shared<Renderer>();

    j.at("type").get_to(r->type);
    j.at("samples").get_to(r->samples);
    j.at("depth").get_to(r->depth);
    j.at("output").get_to(r->output);
    r->dimensions = vec2i(j.at("dimensions").get<std::vector<int>>().data());
}
