#include <iostream>
#include <nlohmann/json.hpp>
#include "loaders/scene-loader.hpp"
#include "renderers/renderer.hpp"

int main() {
    SceneLoader loader = SceneLoader();
    Scene s;

    loader.LoadSceneFile("assignment_01.json", s);
    
    s.renderer->prepare();
    s.renderer->render();
    s.renderer->save();
}