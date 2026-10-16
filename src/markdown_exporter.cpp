#include "markdown_exporter.hpp"
#include "diagram_parsers.hpp"
#include <fstream>

bool MarkdownExporter::exportUseCase(const UseCaseDiagram& diagram, const std::string& outPath) {
    std::ofstream out(outPath);
    if (!out.is_open()) return false;

    out << "# Use Case Diagram: " << diagram.system_name << "\n\n";
    out << "```text\n";
    out << UseCaseParser::to_text(diagram);
    out << "```\n\n";

    out << "## Спецификация вариантов использования\n\n";
    out << "| Актёр | Вариант использования (Прецедент) |\n";
    out << "| :--- | :--- |\n";
    for (const auto& actor : diagram.actors) {
        for (const auto& uc : actor.use_cases) {
            out << "| " << actor.name << " | " << uc << " |\n";
        }
    }
    return true;
}

bool MarkdownExporter::exportClassDiagram(const ClassDiagram& diagram, const std::string& outPath) {
    std::ofstream out(outPath);
    if (!out.is_open()) return false;

    out << "# Class Diagram\n\n";
    out << "```text\n";
    out << ClassDiagramParser::to_text(diagram);
    out << "```\n\n";

    out << "## Детали классов и компонентов\n\n";
    for (const auto& c : diagram.classes) {
        out << "### Класс `" << c.name << "`\n";
        out << "- **Атрибуты:**\n";
        if (c.attributes.empty()) out << "  - *нет атрибутов*\n";
        for (const auto& a : c.attributes) {
            out << "  - `" << a.name << "`: `" << a.raw_type << "` (C++: `" << a.cpp_type << "`)\n";
        }
        out << "- **Методы:**\n";
        if (c.methods.empty()) out << "  - *нет методов*\n";
        for (const auto& m : c.methods) {
            out << "  - `" << m.name << "()` -> `" << m.return_type << "`\n";
        }
        if (!c.relationships.empty()) {
            out << "- **Связи (Relationships):**\n";
            for (const auto& r : c.relationships) {
                out << "  - Ассоциация с `" << r << "`\n";
            }
        }
        out << "\n";
    }
    return true;
}
