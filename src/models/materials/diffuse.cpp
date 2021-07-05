#include "utils/vec.hpp"
#include "renderers/renderer.hpp"

#include "diffuse.hpp"

vec3f DiffuseMaterial::renderer_get_colour(Renderer &renderer) {
    return renderer.get_diffuse(*this);
}

void from_json(const json &j, DiffuseMaterial &d) {
    nlohmann::from_json(j, static_cast<Material &>(d));

    d.rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}

void from_json(const json &j, std::shared_ptr<DiffuseMaterial> &d) {
    d = std::make_shared<DiffuseMaterial>();
    
    j.at("id").get_to(d->id);
    j.at("type").get_to(d->type);

    d->rho = vec3f(j.at("rho").get<std::vector<float>>().data());
}