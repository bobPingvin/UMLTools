# UML Tool — Учебный проект ТРПО (ИСП-43)

Консольное приложение для моделирования систем учёта с помощью UML-диаграмм (Use Case, Class Diagram) и генерации C++ кода по диаграмме классов.

Разработано для **ALT Linux** на C++17 без сторонних библиотек.

---

## 1. Сборка и запуск тестов

```bash
chmod +x tests/run_tests.sh
./tests/run_tests.sh
```

---

## 2. Команды приложения

### Создание Use Case диаграммы
```bash
./uml-tool create-use-case --input data/use-case.json --output output/use-case.md
```

### Создание Class Diagram
```bash
./uml-tool create-class-diagram --input data/class-diagram.json --output output/class-diagram.md
```

### Генерация базового кода C++
```bash
./uml-tool generate-code --input data/class-diagram.json --output output/generated-code --plugins plugins
```

---

## 3. Архитектура и плагины

- **Собственный JSON-парсер:** метод рекурсивного спуска, не требующий внешних зависимостей.
- **Динамические плагины (.so):** загружаются во время выполнения через `dlopen()` и `dlsym()`.
  - `librule_cpp_style.so` — стандартизирует оформление кода.
  - `librule_doc_comments.so` — добавляет Doxygen-комментарии к классам.
- **Экспорт в Markdown:** автоматическое формирование отчётных файлов в папке `output/`.
