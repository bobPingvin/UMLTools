#pragma once
#include "diagram_models.hpp"
#include <string>

class MarkdownExporter {
public:
    static bool exportUseCase(const UseCaseDiagram& diagram, const std::string& outPath);
    static bool exportClassDiagram(const ClassDiagram& diagram, const std::string& outPath);
};
