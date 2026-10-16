#include "code_generator.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

static std::string to_upper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    return s;
}

std::vector<std::string> CodeGenerator::generate(
    const ClassDiagram& diagram,
    const std::string& outDir,
    const std::vector<std::shared_ptr<CodeRule>>& rules
) {
    std::vector<std::string> generatedFiles;

    for (const auto& c : diagram.classes) {
        std::string guard = to_upper(c.name) + "_H";
        std::string headerPath = outDir + "/" + c.name + ".h";
        std::string sourcePath = outDir + "/" + c.name + ".cpp";

        // Генерация заголовка (.h)
        std::ostringstream h;
        h << "#ifndef " << guard << "\n";
        h << "#define " << guard << "\n\n";

        std::set<std::string> includes;
        for (const auto& attr : c.attributes) {
            if (!attr.extra_include.empty()) {
                includes.insert(attr.extra_include);
            }
        }
        for (const auto& inc : includes) {
            h << inc;
        }
        if (!includes.empty()) h << "\n";

        h << "class " << c.name << " {\n";
        h << "public:\n";

        for (const auto& attr : c.attributes) {
            h << "    " << attr.cpp_type << " " << attr.name << ";\n";
        }
        if (!c.attributes.empty() && !c.methods.empty()) {
            h << "\n";
        }

        for (const auto& m : c.methods) {
            h << "    " << m.return_type << " " << m.name << "();\n";
        }
        h << "};\n\n";
        h << "#endif // " << guard << "\n";

        std::string headerCode = h.str();

        // Генерация реализации (.cpp)
        std::ostringstream s;
        s << "#include \"" << c.name << ".h\"\n";
        s << "#include <iostream>\n\n";

        for (const auto& m : c.methods) {
            s << m.return_type << " " << c.name << "::" << m.name << "() {\n";
            s << "    // Auto-generated method stub\n";
            s << "}\n\n";
        }

        std::string sourceCode = s.str();

        // Применение динамических плагинов
        for (const auto& rule : rules) {
            rule->modifyHeader(c, headerCode);
            rule->modifySource(c, sourceCode);
        }

        // Запись на диск
        std::ofstream hf(headerPath);
        hf << headerCode;
        hf.close();
        generatedFiles.push_back(headerPath);

        std::ofstream sf(sourcePath);
        sf << sourceCode;
        sf.close();
        generatedFiles.push_back(sourcePath);
    }

    return generatedFiles;
}
