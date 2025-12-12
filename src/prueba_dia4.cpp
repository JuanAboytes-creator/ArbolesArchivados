#include <iostream>
#include <memory>
#include "FileSystemTree.hpp"
#include "JsonHandler.hpp"

using namespace std;

int main() {
    cout << "=== DIA 4: PRUEBA DE PERSISTENCIA JSON ===" << endl;
    
    // Crear arbol de prueba
    auto tree = make_shared<FileSystemTree>();
    
    try {
        // Crear estructura basica
        tree->createNode("/root", "Documentos", NodeType::FOLDER);
        tree->createNode("/root", "Imagenes", NodeType::FOLDER);
        tree->createNode("/root/Documentos", "notas.txt", NodeType::FILE, "Contenido de prueba");
        
        cout << "\nArbol creado exitosamente" << endl;
        cout << "Total nodos: " << tree->calculateSize() << endl;
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    // Probar guardado
    cout << "\n--- Probando guardado ---" << endl;
    if (JsonHandler::saveTree(tree, "test_dia4.json")) {
        cout << "Guardado exitoso en test_dia4.json" << endl;
    } else {
        cout << "Error en guardado" << endl;
    }
    
    // Probar carga
    cout << "\n--- Probando carga ---" << endl;
    auto tree2 = make_shared<FileSystemTree>();
    if (JsonHandler::loadTree(tree2, "test_dia4.json")) {
        cout << "Carga exitosa desde test_dia4.json" << endl;
    } else {
        cout << "Error en carga" << endl;
    }
    
    cout << "\n=== DIA 4 COMPLETADO ===" << endl;
    cout << "Nota: JSON completo con nlohmann/json en Dia 5" << endl;
    
    return 0;
}
