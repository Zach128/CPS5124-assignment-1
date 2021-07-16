#include "models/cameras/camera.hpp"
#include "renderers/renderer.hpp"

#include "scene.hpp"

void Scene::prepare() {
    renderer->prepare(*this);
}

void Scene::render(const int &cam_id) {
    renderer->render(cameras[cam_id]);
}

void Scene::save() {
    renderer->save();
}