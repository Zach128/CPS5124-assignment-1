#include <iostream>
#include <nlohmann/json.hpp>
#include "loaders/scene-loader.hpp"

int main() {
    SceneLoader loader = SceneLoader();

    loader.LoadSceneFile("assignment_01.json");
}