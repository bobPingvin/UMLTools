#pragma once
#include <string>
#include "diagram_models.hpp"

class CodeRule {
public:
    virtual ~CodeRule() = default;
    virtual std::string name() const = 0;
    virtual void modifyHeader(const UmlClass& umlClass, std::string& headerCode) = 0;
    virtual void modifySource(const UmlClass& umlClass, std::string& sourceCode) = 0;
};

extern "C" CodeRule* create_rule();
