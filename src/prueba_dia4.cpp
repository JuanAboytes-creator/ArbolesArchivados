// prueba_dia4.cpp
#include <iostream>
#include <memory>
#include "FileSystemTree.hpp"
#include "JsonHandler.hpp"

using namespace std;

int main() {
    cout << "=== PRUEBA DIA 4: PERSISTENCIA JSON ===" << endl;
    
    // Crear un árbol simple
    auto tree = make_shared<FileSystemTree>();
    
    try {
        tree->createNode("/root", "carpeta1", NodeType::FOLDER);
        tree->createNode("/root", "carpeta2", NodeType::FOLDER);
        tree->createNode("/root/carpeta1", "archivo1.txt", NodeType::FILE, "Contenido 1");
        tree->createNode("/root/carpeta2", "archivo2.txt", NodeType::FILE, "Contenido 2");
        
        cout << "Árbol creado. Número de nodos: " << tree->calculateSize() << endl;
        
        // Guardar
        if (JsonHandler::saveTree(tree, "test_dia4.json")) {
            cout << "Guardado exitoso." << endl;
        } else {
            cout << "Error al guardar." << endl;
        }
        
        // Cargar en un nuevo árbol
        auto tree2 = make_shared<FileSystemTree>();
        if (JsonHandler::loadTree(tree2, "test_dia4.json")) {
            cout << "Carga exitosa. Número de nodos: " << tree2->calculateSize() << endl;
        } else {
            cout << "Error al cargar." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}