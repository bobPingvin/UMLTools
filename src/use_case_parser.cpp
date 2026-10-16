#include "diagram_parsers.hpp"
#include "json_parser.hpp"
#include <sstream>

UseCaseDiagram UseCaseParser::parse_file(const std::string& path) {
    auto root = simple_json::Parser::parse_file(path);
    UseCaseDiagram diag;

    if (root.has("system")) {
        diag.system_name = root["system"].as_string();
    } else {
        diag.system_name = "Accounting System";
    }

    if (root.has("actors")) {
        for (const auto& a : root["actors"].as_array()) {
            Actor actor;
            actor.name = a["name"].as_string();
            if (a.has("use_cases")) {
                for (const auto& uc : a["use_cases"].as_array()) {
                    actor.use_cases.push_back(uc.as_string());
                }
            }
            diag.actors.push_back(actor);
        }
    }
    return diag;
}

std::string UseCaseParser::to_text(const UseCaseDiagram& diagram) {
    std::ostringstream ss;
    ss << "Use Case Diagram:\n";
    for (const auto& actor : diagram.actors) {
        ss << "  Actor: " << actor.name << "\n";
        for (const auto& uc : actor.use_cases) {
            ss << "    - " << uc << "\n";
        }
    }
    ss << "  System: " << diagram.system_name << "\n";
    return ss.str();
}
