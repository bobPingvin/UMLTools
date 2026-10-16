#pragma once
#include "diagram_models.hpp"
#include "code_rule.hpp"
#include <string>
#include <vector>
#include <memory>

class CodeGenerator {
public:
    static std::vector<std::string> generate(
        const ClassDiagram& diagram,
        const std::string& outDir,
        const std::vector<std::shared_ptr<CodeRule>>& rules
    );
};
