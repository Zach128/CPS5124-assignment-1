#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "models/scene.hpp"
#include "models/cameras/camera.hpp"
#include "models/materials/material.hpp"
#include "models/shapes/shape.hpp"
#include "models/lights/light.hpp"
#include "models/primitive.hpp"
#include "loaders/camera-loader.hpp"

namespace fs = std::filesystem;

using json = nlohmann::json;

class SceneLoader {
public:
    std::string base_path = "res/scenes";
    
    SceneLoader() {}

    void LoadSceneFile(const std::string &filename) {
        // Build the file path to the scene.
        fs::path dir(base_path);
        fs::path file(filename);
        fs::path scene_path = dir / file;

        std::cout << "Loading path: " << scene_path << std::endl;

        json j;
        std::ifstream input(scene_path);

        if (input.is_open()) {
            std::vector<Camera> cameras;

            // Load and parse the json contents.
            input >> j;
            input.close();

            LoadCameras(j, cameras);

            std::cout << cameras[0].position.z << std::endl;
        } else {
            std::cerr << "Failed to load " << filename << std::endl;
        }
    }
};
