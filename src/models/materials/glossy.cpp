#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

#include "glossy.hpp"

vec3f GlossyMaterial::get_diffuse(Renderer &) {
    return rho;
}

vec3f GlossyMaterial::get_specular(Renderer &) {
    return rho;
}

float GlossyMaterial::get_roughness() {
    return roughness;
}

void from_json(const json &j, GlossyMaterial &d) {
    nlohmann::from_json(j, static_cast<Material &>(d));

    j.at("roughness").get_to(d.roughness);
    d.rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<GlossyMaterial> &d) {
    d = std::make_shared<GlossyMaterial>();

    j.at("id").get_to(d->id);
    j.at("type").get_to(d->type);
    j.at("roughness").get_to(d->roughness);

    d->rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}