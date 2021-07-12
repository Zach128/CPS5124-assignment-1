#include <iostream>
#include <nlohmann/json.hpp>
#include "loaders/scene-loader.hpp"
#include "models/scene.hpp"

int main() {
    SceneLoader loader = SceneLoader();
    Scene s;

    loader.LoadSceneFile("assignment_01.json", s);

    s.prepare();
    s.render();
    s.save();
}