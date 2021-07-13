#include <iostream>
#include <nlohmann/json.hpp>
#include "loaders/scene-loader.hpp"
#include "processors/box-blur.hpp"
#include "models/scene.hpp"

int main() {
    SceneLoader loader = SceneLoader();
    Scene s;

    loader.LoadSceneFile("assignment_01.json", s);

    s.prepare();
    s.render();

    BoxBlur blurer = BoxBlur((size_t) s.renderer->width, (size_t) s.renderer->height);
    blurer.postProcess(s);

    s.save();
}