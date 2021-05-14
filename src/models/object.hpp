#pragma once
#include <string>

struct TypedElement {
    std::string id;
    std::string type;

    TypedElement(const std::string &id, const std::string &type) : id(id), type(type) {}
};
