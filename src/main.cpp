#include <iostream>
#include <string>
#include <sys/stat.h>
#include "logger.hpp"
#include "diagram_parsers.hpp"
#include "code_generator.hpp"
#include "markdown_exporter.hpp"
#include "plugin_loader.hpp"

static void printHelp() {
    std::cout << "UML Tool — Генератор кода и анализатор UML диаграмм\n\n"
              << "Команды:\n"
              << "  create-use-case --input <file.json> [--output <file.md>]\n"
              << "  create-class-diagram --input <file.json> [--output <file.md>]\n"
              << "  generate-code --input <file.json> --output <dir> [--plugins <dir>]\n"
              << "  --help\n";
}

int main(int argc, char* argv[]) {
    Logger::init("logs/uml_tool.log");

    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string command = argv[1];
    if (command == "--help" || command == "-h") {
        printHelp();
        return 0;
    }

    std::string inputFile;
    std::string outputFile;
    std::string pluginDir = "plugins";

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--input" && i + 1 < argc) inputFile = argv[++i];
        else if (arg == "--output" && i + 1 < argc) outputFile = argv[++i];
        else if (arg == "--plugins" && i + 1 < argc) pluginDir = argv[++i];
    }

    try {
        if (command == "create-use-case") {
            if (inputFile.empty()) {
                std::cerr << "Ошибка: Укажите --input <use-case.json>\n";
                return 1;
            }
            if (outputFile.empty()) outputFile = "output/use-case.md";

            std::cout << "\n=== UML Tool ===\n";
            std::cout << "Режим: создание Use Case Diagram\n";
            std::cout << "Входной файл: " << inputFile << "\n\n";

            auto diag = UseCaseParser::parse_file(inputFile);
            std::cout << UseCaseParser::to_text(diag) << "\n";

            MarkdownExporter::exportUseCase(diag, outputFile);
            std::cout << "Диаграмма сохранена в файл: " << outputFile << "\n";
            return 0;

        } else if (command == "create-class-diagram") {
            if (inputFile.empty()) {
                std::cerr << "Ошибка: Укажите --input <class-diagram.json>\n";
                return 1;
            }
            if (outputFile.empty()) outputFile = "output/class-diagram.md";

            std::cout << "\n=== UML Tool ===\n";
            std::cout << "Режим: создание Class Diagram\n";
            std::cout << "Входной файл: " << inputFile << "\n\n";

            auto diag = ClassDiagramParser::parse_file(inputFile);
            std::cout << ClassDiagramParser::to_text(diag) << "\n";

            MarkdownExporter::exportClassDiagram(diag, outputFile);
            std::cout << "Диаграмма сохранена в файл: " << outputFile << "\n";
            return 0;

        } else if (command == "generate-code") {
            if (inputFile.empty()) {
                std::cerr << "Ошибка: Укажите --input <class-diagram.json>\n";
                return 1;
            }
            if (outputFile.empty()) outputFile = "output/generated-code";

            mkdir(outputFile.c_str(), 0777);

            std::cout << "\n=== UML Tool ===\n";
            std::cout << "Режим: Генерация C++ кода по диаграмме классов\n";
            std::cout << "Входной файл: " << inputFile << "\n";

            auto diag = ClassDiagramParser::parse_file(inputFile);

            PluginLoader loader;
            loader.loadPlugins(pluginDir);

            auto files = CodeGenerator::generate(diag, outputFile, loader.getRules());

            std::cout << "\n=== Генерация кода ===\n";
            std::cout << "Сгенерировано файлов: " << files.size() << "\n";
            for (const auto& f : files) {
                std::cout << "  - " << f << "\n";
            }
            std::cout << "\nКод сохранён в директорию: " << outputFile << "/\n";

            return 0;
        } else {
            std::cerr << "Неизвестная команда: " << command << "\n";
            printHelp();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "\n[ОШИБКА]: " << e.what() << "\n";
        Logger::error(e.what());
        return 1;
    }
}
