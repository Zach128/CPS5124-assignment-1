#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

#include "fresnel.hpp"

vec3f FresnelMaterial::get_diffuse(Renderer &) {
    return rhoR;
}

vec3f FresnelMaterial::get_specular(Renderer &) {
    return rhoR;
}

vec3f FresnelMaterial::get_transmission() {
    return rhoT;
}

float FresnelMaterial::get_eta() {
    return eta;
}

void from_json(const json &j, FresnelMaterial &d) {
    nlohmann::from_json(j, static_cast<Material &>(d));

    j.at("eta").get_to(d.eta);
    d.rhoR = vec3f(j.at("rhoR").get<std::vector<float>>().data());
    d.rhoT = vec3f(j.at("rhoT").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<FresnelMaterial> &d) {
    d = std::make_shared<FresnelMaterial>();

    j.at("id").get_to(d->id);
    j.at("type").get_to(d->type);

    j.at("eta").get_to(d->eta);
    d->rhoR = vec3f(j.at("rhoR").get<std::vector<float>>().data());
    d->rhoT = vec3f(j.at("rhoT").get<std::vector<float>>().data());
}