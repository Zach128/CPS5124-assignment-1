#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    json test = {
        {"Testing", 1234}
    };

    std::cout << test.dump(2) << std::endl;
}