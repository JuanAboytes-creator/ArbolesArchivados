cmake_minimum_required(VERSION 3.10)
project(ArbolesArchivados)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Archivos fuente principales
set(SOURCES
    src/main.cpp
    src/TreeNode.cpp
    src/FileSystemTree.cpp
    src/Trie.cpp
    src/SearchEngine.cpp
    src/JsonHandler.cpp
    src/ConsoleInterface.cpp
)

# Archivos de cabecera
set(HEADERS
    src/TreeNode.hpp
    src/FileSystemTree.hpp
    src/Trie.hpp
    src/SearchEngine.hpp
    src/JsonHandler.hpp
    src/ConsoleInterface.hpp
)

# Ejecutable principal
add_executable(arboles_archivados ${SOURCES} ${HEADERS})
target_include_directories(arboles_archivados PRIVATE src)
target_link_libraries(arboles_archivados nlohmann_json::nlohmann_json)

# Ejecutable de pruebas días 5-6
add_executable(prueba_dia5_6 src/main_dia5_6.cpp ${SOURCES} ${HEADERS})
target_include_directories(prueba_dia5_6 PRIVATE src)
target_link_libraries(prueba_dia5_6 nlohmann_json::nlohmann_json)

# Ejecutable de prueba día 4
add_executable(prueba_dia4 src/prueba_dia4.cpp ${SOURCES} ${HEADERS})
target_include_directories(prueba_dia4 PRIVATE src)
target_link_libraries(prueba_dia4 nlohmann_json::nlohmann_json)