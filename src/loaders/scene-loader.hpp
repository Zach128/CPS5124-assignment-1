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
#include "loaders/material-loader.hpp"
#include "loaders/shapes-loader.hpp"
#include "loaders/lights-loader.hpp"
#include "models/renderers/renderer.hpp"

namespace fs = std::filesystem;

using json = nlohmann::json;

class SceneLoader {
public:
    std::string base_path = "res/scenes";
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Primitive>> primitives;
    Renderer renderer;
    
    SceneLoader() {}
    SceneLoader(std::string base_path) : base_path(base_path) {}

    void LoadSceneFile(const std::string &file_name) {
        // Build the file path to the scene.
        fs::path dir(base_path);
        fs::path file(file_name);
        fs::path scene_path = dir / file;

        std::cout << "Loading path: " << scene_path << std::endl;

        json j;
        std::ifstream input(scene_path);

        if (input.is_open()) {

            // Load and parse the json contents.
            input >> j;
            input.close();

            // Load all the properties of the scene.
            LoadCameras(j, cameras);
            LoadMaterials(j, materials);
            LoadShapes(j, shapes);
            LoadLights(j, lights);
            LoadPrimitives(j, primitives);

            // Load the renderer properties.
            j.at("scene").at("renderer").get_to(renderer);

            std::cout << cameras[0]->position.z << std::endl;
            std::cout << std::static_pointer_cast<Sphere>(shapes[0])->position.x << std::endl;
            std::cout << std::static_pointer_cast<DiffuseMaterial>(materials[0])->rho.x << std::endl;
            std::cout << lights[0]->intensity.x << std::endl;
            std::cout << primitives[0]->shape.position.x << std::endl;
        } else {
            std::cerr << "Failed to load " << file_name << std::endl;
        }
    }

private:
    void LoadPrimitives(const json &j, std::vector<std::shared_ptr<Primitive>> &primitives ) {
        std::string mat_id, shape_id;
        
        primitives = std::vector<std::shared_ptr<Primitive>>();
        json scene = j.at("scene");


        // Declare filters for finding materials and shapes by id.
        const auto shape_filter = [&shape_id](const std::shared_ptr<Shape> &shape) { return shape->id == shape_id; };
        const auto mat_filter = [&mat_id](const std::shared_ptr<Material> &mat) { return mat->id == mat_id; };

        // Check there are primitives to load and that there are shapes and scenes loaded in the config list.
        if (!scene.at("primitives").empty()) {
            if (!shapes.empty() && !materials.empty()) {
                for (json primitive : scene.at("primitives")) {
                    // Get the ids of the shape and material to search for.
                    primitive.at("shape").get_to<std::string>(shape_id);
                    primitive.at("material").get_to<std::string>(mat_id);

                    // Search for the material and shape.
                    auto found_shape = std::find_if(shapes.begin(), shapes.end(), shape_filter);
                    auto found_mat = std::find_if(materials.begin(), materials.end(), mat_filter);

                    // Check we found the shape.
                    if (found_shape == shapes.end()) {
                        std::out_of_range("Shape " + shape_id + " was not found.\n");
                    }

                    // Check we found the material.
                    if (found_mat == materials.end()) {
                        std::out_of_range("Material " + mat_id + " was not found.\n");
                    }
                    
                    // Save the primitive record.
                    primitives.push_back(std::make_shared<Primitive>(Primitive(
                        primitive.at("id").get<std::string>(),
                        primitive.at("type").get<std::string>(),
                        *(found_shape->get()),
                        *(found_mat->get())
                    )));
                }
            }
        }
    }
};
