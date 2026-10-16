#pragma once
#include <string>
#include <vector>

// Модели Use Case
struct Actor {
    std::string name;
    std::vector<std::string> use_cases;
};

struct UseCaseDiagram {
    std::string system_name;
    std::vector<Actor> actors;
};

// Модели Class Diagram
struct Attribute {
    std::string name;
    std::string raw_type;
    std::string cpp_type;
    std::string extra_include;
};

struct Method {
    std::string name;
    std::string return_type = "void";
    std::vector<std::string> params;
};

struct UmlClass {
    std::string name;
    std::vector<Attribute> attributes;
    std::vector<Method> methods;
    std::vector<std::string> relationships;
};

struct ClassDiagram {
    std::vector<UmlClass> classes;
};
