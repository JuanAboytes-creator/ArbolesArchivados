#!/bin/bash

echo "=== COMPILANDO PROYECTO (CORRECCIÓN) ==="
echo

# Descargar nlohmann/json si no existe
if [ ! -f "include/nlohmann/json.hpp" ]; then
    echo "Descargando nlohmann/json..."
    mkdir -p include/nlohmann
    curl -o include/nlohmann/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
fi

echo "Compilando archivos individualmente..."

# Compilar cada archivo .cpp
g++ -std=c++17 -I./src -I./include -c src/TreeNode.cpp -o TreeNode.o
g++ -std=c++17 -I./src -I./include -c src/FileSystemTree.cpp -o FileSystemTree.o
g++ -std=c++17 -I./src -I./include -c src/Trie.cpp -o Trie.o
g++ -std=c++17 -I./src -I./include -c src/SearchEngine.cpp -o SearchEngine.o
g++ -std=c++17 -I./src -I./include -c src/JsonHandler.cpp -o JsonHandler.o
g++ -std=c++17 -I./src -I./include -c src/ConsoleInterface.cpp -o ConsoleInterface.o

echo "Compilando aplicación principal..."
g++ -std=c++17 -I./src -I./include \
    src/main.cpp \
    TreeNode.o FileSystemTree.o Trie.o SearchEngine.o JsonHandler.o ConsoleInterface.o \
    -o arboles_archivados

if [ -f "arboles_archivados" ]; then
    echo "✓ Aplicación principal compilada: arboles_archivados"
else
    echo "✗ Error al compilar aplicación principal"
fi

echo "Compilando pruebas días 5-6..."
g++ -std=c++17 -I./src -I./include \
    src/main_dia5_6.cpp \
    TreeNode.o FileSystemTree.o Trie.o SearchEngine.o JsonHandler.o ConsoleInterface.o \
    -o prueba_dia5_6

if [ -f "prueba_dia5_6" ]; then
    echo "✓ Pruebas días 5-6 compiladas: prueba_dia5_6"
else
    echo "✗ Error al compilar pruebas días 5-6"
fi

# Limpiar archivos objeto
rm -f *.o

echo
echo "=== COMPILACIÓN COMPLETADA ==="
echo "Para ejecutar la aplicación principal: ./arboles_archivados"
echo "Para ejecutar las pruebas días 5-6: ./prueba_dia5_6"