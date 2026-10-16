#pragma once
#include "diagram_models.hpp"
#include <string>

class UseCaseParser {
public:
    static UseCaseDiagram parse_file(const std::string& path);
    static std::string to_text(const UseCaseDiagram& diagram);
};

class ClassDiagramParser {
public:
    static ClassDiagram parse_file(const std::string& path);
    static std::string to_text(const ClassDiagram& diagram);
    static Attribute parse_attribute_string(const std::string& raw);
    static Method parse_method_string(const std::string& raw);
};
