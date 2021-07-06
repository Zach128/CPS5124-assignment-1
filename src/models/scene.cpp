#include "models/cameras/camera.hpp"
#include "renderers/renderer.hpp"

#include "scene.hpp"

void Scene::prepare() {
    renderer->prepare(*this);
}

void Scene::render() {
    renderer->render(cameras[0]);
}

void Scene::save() {
    renderer->save();
}