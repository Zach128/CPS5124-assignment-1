#pragma once
#include "utils/vec.hpp"
#include "models/rays/ray.hpp"
#include "models/shapes/shape.hpp"
#include "models/primitives/primitive.hpp"

using json = nlohmann::json;

// Forward declare visited classes.
class Renderer;

enum CameraType { CAMERA_NONE = -1, CAMERA_PINHOLE, CAMERA_LENS };

NLOHMANN_JSON_SERIALIZE_ENUM(CameraType, {
    { CAMERA_NONE, nullptr},
    { CAMERA_PINHOLE, "pinhole" },
    { CAMERA_LENS, "lens-based" }
})
struct Camera {
    std::string id;
    CameraType type;

    float fov;      // Field of View
    float aspect;   // Aspect ratio
    float distance; // Distance from position to target plane.

    vec3f position; // Position of camera.
    vec3f target;   // Looking direction.

    Camera(const std::string &id, const CameraType type, const float fov, const float aspect, const float distance, const vec3f &position, const vec3f &target)
        :   id(id),
            type(type),
            fov(fov),
            aspect(aspect),
            distance(distance),
            position(position),
            target(target) {}

    Camera() {}

    virtual vec3f renderer_cast_ray(Renderer &, const RayInfo &, float &, size_t = 0) { return vec3f(0, 0, 0); };
};

void from_json(const json &j, Camera &c);
void from_json(const json &j, std::shared_ptr<Camera> &c);
