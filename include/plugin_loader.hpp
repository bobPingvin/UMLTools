#pragma once
#include <vector>
#include <string>
#include <memory>
#include "code_rule.hpp"

class PluginLoader {
public:
    ~PluginLoader();
    void loadPlugins(const std::string& dirPath);
    const std::vector<std::shared_ptr<CodeRule>>& getRules() const { return rules_; }

private:
    std::vector<void*> handles_;
    std::vector<std::shared_ptr<CodeRule>> rules_;
};
