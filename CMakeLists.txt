cmake_minimum_required(VERSION 3.10)
project(uml_tool CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

include_directories(include)

# Главный исполняемый файл
add_executable(uml-tool
    src/main.cpp
    src/json_parser.cpp
    src/use_case_parser.cpp
    src/class_diagram_parser.cpp
    src/code_generator.cpp
    src/markdown_exporter.cpp
    src/plugin_loader.cpp
)
target_link_libraries(uml-tool ${CMAKE_DL_LIBS})
set_target_properties(uml-tool PROPERTIES ENABLE_EXPORTS TRUE)

# Плагины (.so)
add_library(rule_cpp_style SHARED plugins/rule_cpp_style.cpp)
set_target_properties(rule_cpp_style PROPERTIES PREFIX "lib" SUFFIX ".so")

add_library(rule_doc_comments SHARED plugins/rule_doc_comments.cpp)
set_target_properties(rule_doc_comments PROPERTIES PREFIX "lib" SUFFIX ".so")
