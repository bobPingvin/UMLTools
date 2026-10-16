#include "diagram_parsers.hpp"
#include "json_parser.hpp"
#include <sstream>
#include <algorithm>

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

Attribute ClassDiagramParser::parse_attribute_string(const std::string& raw) {
    Attribute attr;
    size_t colon = raw.find(':');
    if (colon == std::string::npos) {
        attr.name = trim(raw);
        attr.raw_type = "void";
        attr.cpp_type = "void";
        return attr;
    }

    attr.name = trim(raw.substr(0, colon));
    std::string type = trim(raw.substr(colon + 1));
    attr.raw_type = type;

    if (type == "int") {
        attr.cpp_type = "int";
    } else if (type == "string") {
        attr.cpp_type = "std::string";
        attr.extra_include = "#include <string>\n";
    } else if (type.rfind("List<", 0) == 0 && type.back() == '>') {
        std::string inner = trim(type.substr(5, type.size() - 6));
        attr.cpp_type = "std::vector<" + inner + ">";
        attr.extra_include = "#include <vector>\n#include \"" + inner + ".h\"\n";
    } else if (type == "double" || type == "float") {
        attr.cpp_type = "double";
    } else if (type == "bool") {
        attr.cpp_type = "bool";
    } else {
        attr.cpp_type = type;
        attr.extra_include = "#include \"" + type + ".h\"\n";
    }
    return attr;
}

Method ClassDiagramParser::parse_method_string(const std::string& raw) {
    Method m;
    std::string s = trim(raw);
    size_t lparen = s.find('(');
    size_t rparen = s.find(')');

    if (lparen == std::string::npos) {
        m.name = s;
        return m;
    }

    m.name = trim(s.substr(0, lparen));
    m.return_type = "void";

    // Проверка возвращаемого типа после двоеточия (если указан: Method(): type)
    if (rparen != std::string::npos && rparen + 1 < s.size()) {
        size_t colon = s.find(':', rparen);
        if (colon != std::string::npos) {
            m.return_type = trim(s.substr(colon + 1));
        }
    }
    return m;
}

ClassDiagram ClassDiagramParser::parse_file(const std::string& path) {
    auto root = simple_json::Parser::parse_file(path);
    ClassDiagram diag;

    if (!root.has("classes")) {
        throw std::runtime_error("JSON не содержит обязательного узла 'classes'");
    }

    for (const auto& item : root["classes"].as_array()) {
        UmlClass c;
        if (!item.has("name")) {
            throw std::runtime_error("Класс не содержит обязательного поля 'name'");
        }
        c.name = item["name"].as_string();

        if (item.has("attributes")) {
            for (const auto& attr : item["attributes"].as_array()) {
                c.attributes.push_back(parse_attribute_string(attr.as_string()));
            }
        }
        if (item.has("methods")) {
            for (const auto& met : item["methods"].as_array()) {
                c.methods.push_back(parse_method_string(met.as_string()));
            }
        }
        if (item.has("relationships")) {
            for (const auto& rel : item["relationships"].as_array()) {
                c.relationships.push_back(rel.as_string());
            }
        }
        diag.classes.push_back(c);
    }
    return diag;
}

std::string ClassDiagramParser::to_text(const ClassDiagram& diagram) {
    std::ostringstream ss;
    ss << "Class Diagram:\n";
    for (size_t i = 0; i < diagram.classes.size(); ++i) {
        const auto& c = diagram.classes[i];
        ss << "  Class: " << c.name << "\n";

        ss << "    Attributes: ";
        if (c.attributes.empty()) {
            ss << "none";
        } else {
            for (size_t j = 0; j < c.attributes.size(); ++j) {
                ss << c.attributes[j].name << ": " << c.attributes[j].raw_type;
                if (j + 1 < c.attributes.size()) ss << ", ";
            }
        }
        ss << "\n";

        ss << "    Methods: ";
        if (c.methods.empty()) {
            ss << "none";
        } else {
            for (size_t j = 0; j < c.methods.size(); ++j) {
                ss << c.methods[j].name << "()";
                if (j + 1 < c.methods.size()) ss << ", ";
            }
        }
        ss << "\n";

        if (!c.relationships.empty()) {
            ss << "    Relationships: ";
            for (size_t j = 0; j < c.relationships.size(); ++j) {
                ss << c.relationships[j];
                if (j + 1 < c.relationships.size()) ss << ", ";
            }
            ss << "\n";
        }
        if (i + 1 < diagram.classes.size()) ss << "\n";
    }
    return ss.str();
}
