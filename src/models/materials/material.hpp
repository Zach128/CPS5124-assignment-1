#pragma once
#include <iostream>
#include "models/object.hpp"

struct Material : TypedElement {

    Material(const std::string &id, const std::string &type) : TypedElement(id, type) {}
};