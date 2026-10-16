#include "code_rule.hpp"

class DocCommentsRule : public CodeRule {
public:
    std::string name() const override {
        return "Doxygen Documentation Generator Rule";
    }

    void modifyHeader(const UmlClass& umlClass, std::string& headerCode) override {
        std::string needle = "class " + umlClass.name;
        size_t pos = headerCode.find(needle);
        if (pos != std::string::npos) {
            std::string doc = "/**\n"
                              " * @class " + umlClass.name + "\n"
                              " * @brief Сущность учёта, сформированная на основе Class Diagram.\n"
                              " */\n";
            headerCode.insert(pos, doc);
        }
    }

    void modifySource(const UmlClass& /*umlClass*/, std::string& /*sourceCode*/) override {
        // Дополнительные модификации не требуются
    }
};

extern "C" CodeRule* create_rule() {
    return new DocCommentsRule();
}
