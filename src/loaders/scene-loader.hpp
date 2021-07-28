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
#include "models/primitives/primitive.hpp"
#include "models/primitives/emitter-primitive.hpp"
#include "loaders/camera-loader.hpp"
#include "loaders/material-loader.hpp"
#include "loaders/shapes-loader.hpp"
#include "loaders/lights-loader.hpp"
#include "renderers/renderer.hpp"
#include "renderers/whitted-renderer/whitted-renderer.hpp"
#include "renderers/path-renderer/path-renderer.hpp"

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
    std::shared_ptr<Renderer> renderer;

    SceneLoader() {}
    SceneLoader(std::string base_path) : base_path(base_path) {}

    void LoadSceneFile(const std::string &file_name, Scene &scene) {
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

            const std::vector<std::string> activeLightIds = j.at("scene").at("lights").get<std::vector<std::string>>();
            const std::string selectedCamera = j.at("scene").at("camera").get<std::string>();

            // Load all the properties of the scene.
            LoadCameras(j, cameras);
            LoadMaterials(j, materials);
            LoadShapes(j, shapes);
            LoadLights(j, lights, activeLightIds);
            LoadPrimitives(j, primitives);
            LoadRenderer(j);

            AssignShapesToAreaLights();

            scene = Scene(cameras, materials, shapes, lights, renderer, primitives, selectedCamera);
        } else {
            std::cerr << "Failed to load " << file_name << std::endl;
        }
    }

private:
    void AssignShapesToAreaLights() {

        for(std::shared_ptr<Light> light : lights) {
            if (light->type == "area") {
                std::shared_ptr<AreaLight> areaLight = std::dynamic_pointer_cast<AreaLight>(light);
                std::string shape_id = areaLight->shape_id;

                const auto shape_filter = [&shape_id](const std::shared_ptr<Shape> &shape) { return shape->id == shape_id; };
                auto found_shape = std::find_if(shapes.begin(), shapes.end(), shape_filter);

                if (found_shape == shapes.end()) {
                    std::out_of_range("Area light " + areaLight->id + " references shape " + shape_id + ", but it was not found.\n");
                }

                areaLight->shape = found_shape[0];
            }
        }
    }

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

                    std::string primitive_id = primitive.at("id").get<std::string>();
                    std::string primitive_type = primitive.at("type").get<std::string>();

                    // Search for the material and shape.
                    auto found_shape = std::find_if(shapes.begin(), shapes.end(), shape_filter);
                    auto found_mat = std::find_if(materials.begin(), materials.end(), mat_filter);

                    // Check we found the shape.
                    if (found_shape == shapes.end()) {
                        std::out_of_range("Primitive " + primitive_id + " references shape " + shape_id + ", but it was not found.\n");
                    }

                    // Check we found the material.
                    if (found_mat == materials.end()) {
                        std::out_of_range("Primitive " + primitive_id + " references material " + mat_id + ", but it was not found.\n");
                    }

                    if (primitive_type == "emissive") {
                        std::string light_id = primitive.at("light").get<std::string>();

                        auto light_filter = [&light_id](const std::shared_ptr<Light> &light) { return light->id == light_id; };
                        auto found_light = std::find_if(lights.begin(), lights.end(), light_filter);

                        if (found_light == lights.end()) {
                            std::out_of_range("Emitter primitive " + primitive_id + " references area light " + light_id + ", but it was not found.\n");
                        }

                        // Save the primitive record.
                        primitives.push_back(std::make_shared<EmitterPrimitive>(EmitterPrimitive(
                            primitive_id,
                            primitive_type,
                            found_shape[0],
                            found_mat[0],
                            std::dynamic_pointer_cast<AreaLight>(found_light[0])
                        )));
                    } else {
                        // Save the primitive record.
                        primitives.push_back(std::make_shared<Primitive>(Primitive(
                            primitive_id,
                            primitive_type,
                            found_shape[0],
                            found_mat[0]
                        )));
                    }
                }
            }
        }
    }

    void LoadRenderer(const json &j) {
        const json &r = j.at("scene").at("renderer");

        if (!r.empty()) {
            if (r.at("type").get<std::string>() == "WRT") {
                renderer = r.get<std::shared_ptr<WhittedRenderer>>();
            } else if (r.at("type").get<std::string>() == "PT") {
                renderer = r.get<std::shared_ptr<PathRenderer>>();
            } else {
                renderer = r.get<std::shared_ptr<Renderer>>();
            }
        }
    }
};
