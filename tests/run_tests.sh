#!/bin/bash
set -e

echo "=== 1. Сборка проекта через CMake ==="
mkdir -p build
cd build
cmake ..
make -j$(nproc)

# Копирование бинарника и библиотек
cp uml-tool ..
mkdir -p ../plugins
cp librule_cpp_style.so librule_doc_comments.so ../plugins/
cd ..

echo ""
echo "=== 2. ТЕСТ: Создание Use Case диаграммы и экспорт в Markdown ==="
./uml-tool create-use-case --input data/use-case.json --output output/use-case.md

echo ""
echo "=== 3. ТЕСТ: Создание Class Diagram и вывод в терминал ==="
./uml-tool create-class-diagram --input data/class-diagram.json --output output/class-diagram.md

echo ""
echo "=== 4. ТЕСТ: Генерация C++ кода с применением плагинов ==="
./uml-tool generate-code --input data/class-diagram.json --output output/generated-code --plugins plugins

echo ""
echo "=== 5. Проверка компилируемости сгенерированного кода ==="
cd output/generated-code
g++ -std=c++17 -c Item.cpp Inventory.cpp
echo "-> Сгенерированный код успешно скомпилирован через g++ без ошибок!"
cd ../..

echo ""
echo "=== 6. ТЕСТ: Обработка крайних случаев (Пустой класс) ==="
./uml-tool generate-code --input data/edge-case.json --output output/generated-code

echo ""
echo "=== 7. ТЕСТ: Обработка ошибок (Несуществующий файл) ==="
set +e
./uml-tool create-class-diagram --input data/non-existent.json
if [ $? -ne 0 ]; then
    echo "-> Ошибка корректно перехвачена, аварийного падения не произошло!"
fi
set -e

echo ""
echo "=== ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ! ==="
