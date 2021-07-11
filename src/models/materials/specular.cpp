#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

#include "specular.hpp"

vec3f SpecularMaterial::get_diffuse(Renderer &renderer) {
    return rho;
}

vec3f SpecularMaterial::get_specular(Renderer &renderer) {
    return rho;
}

void from_json(const json &j, SpecularMaterial &d) {
    nlohmann::from_json(j, static_cast<Material &>(d));

    d.rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<SpecularMaterial> &d) {
    d = std::make_shared<SpecularMaterial>();

    j.at("id").get_to(d->id);
    j.at("type").get_to(d->type);

    d->rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}