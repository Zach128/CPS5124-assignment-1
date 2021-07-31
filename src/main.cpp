#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include "loaders/scene-loader.hpp"
#include "processors/box-blur.hpp"
#include "processors/sigmoidal-tonemapper.hpp"
#include "processors/linear-tonemapper.hpp"
#include "models/scene.hpp"

int main() {
    std::vector<std::string> files = {
        "assignment_01.json",
        "assignment_02.json",
        "assignment_03.json",
        "assignment_04.json",
        "assignment_05.json",
        "assignment_06.json",
        "assignment_07.json",
        "assignment_08.json",
    };

    for(std::string file : files) {
        SceneLoader loader = SceneLoader();
        Scene s;

        loader.LoadSceneFile(file, s);

        s.prepare();

        auto t1 = std::chrono::high_resolution_clock::now();

        std::cout << "Rendering..." << std::endl;
        s.render();
        std::cout << "\nDone" << std::endl;

        auto t2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> timeTaken = t2 - t1;

        std::cout << "Rendering completed in " << timeTaken.count() / 1000.0 << " seconds." << std::endl;

        // BoxBlur blurer = BoxBlur((size_t) s.renderer->width, (size_t) s.renderer->height);
        SigmoidalToneMapper sigTone = SigmoidalToneMapper((size_t) s.renderer->width, (size_t) s.renderer->height);
        // LinearToneMapper linTone = LinearToneMapper((size_t) s.renderer->width, (size_t) s.renderer->height);

        sigTone.postProcess(s);
        // linTone.postProcess(s);
        // blurer.postProcess(s);

        s.save();
    }

}