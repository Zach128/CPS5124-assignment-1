#include <iostream>
#include <nlohmann/json.hpp>
#include "loaders/scene-loader.hpp"
#include "processors/box-blur.hpp"
#include "processors/sigmoidal-tonemapper.hpp"
#include "processors/linear-tonemapper.hpp"
#include "models/scene.hpp"

int main() {
    SceneLoader loader = SceneLoader();
    Scene s;

    loader.LoadSceneFile("assignment_02.json", s);

    s.prepare();
    s.render();

    BoxBlur blurer = BoxBlur((size_t) s.renderer->width, (size_t) s.renderer->height);
    SigmoidalToneMapper sigTone = SigmoidalToneMapper((size_t) s.renderer->width, (size_t) s.renderer->height);
    LinearToneMapper linTone = LinearToneMapper((size_t) s.renderer->width, (size_t) s.renderer->height);

    // sigTone.postProcess(s);
    linTone.postProcess(s);
    // blurer.postProcess(s);

    s.save();
}