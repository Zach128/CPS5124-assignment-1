#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/vec.hpp"

using json = nlohmann::json;

struct Renderer {
    std::string type;
    std::string output;
    vec2i dimensions;
    int samples;
    int depth;

    Renderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth) : type(type), output(output), dimensions(dimensions), samples(samples), depth(depth) {}
    Renderer() {}
};

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
