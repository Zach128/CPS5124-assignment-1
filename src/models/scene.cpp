#include "models/cameras/camera.hpp"
#include "renderers/renderer.hpp"

#include "scene.hpp"

void Scene::prepare() {
    renderer->prepare(*this);
}

void Scene::render() {
    const std::string &id = selectedCamera;
    const auto camera_filter = [&id](const std::shared_ptr<Camera> &camera) { return camera->id == id; };
    auto found_camera = std::find_if(cameras.begin(), cameras.end(), camera_filter);

    if (found_camera == cameras.end()) {
        std::out_of_range("Camera " + id + " was not found.\n");
    } else {
        renderer->render(found_camera[0]);
    }
}

void Scene::save() {
    renderer->save();
}