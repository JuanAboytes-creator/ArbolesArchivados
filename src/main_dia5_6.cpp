// main_dia5_6.cpp - Días 5-6: Pruebas de Trie y Búsqueda
#include <iostream>
#include <memory>
#include <chrono>  // ¡AGREGADO!
#include "FileSystemTree.hpp"
#include "Trie.hpp"
#include "SearchEngine.hpp"
#include "JsonHandler.hpp"

using namespace std;
using namespace chrono;  // ¡AGREGADO!

void testTrieBasic() {
    cout << "=== PRUEBA 1: OPERACIONES BÁSICAS DEL TRIE ===" << endl;
    
    Trie trie;
    
    // Insertar palabras
    trie.insert("documento", 1);
    trie.insert("documentos", 2);
    trie.insert("doc", 3);
    trie.insert("archivo", 4);
    trie.insert("archivos", 5);
    trie.insert("carpeta", 6);
    
    cout << "Palabras insertadas: documento, documentos, doc, archivo, archivos, carpeta" << endl;
    
    // Búsqueda exacta
    cout << "\n1. Búsqueda exacta:" << endl;
    vector<int> docIds = trie.searchExact("documento");
    cout << "   'documento' encontrado con IDs: ";
    for (int id : docIds) cout << id << " ";
    cout << endl;
    
    // Búsqueda por prefijo
    cout << "\n2. Búsqueda por prefijo 'doc':" << endl;
    vector<string> prefixResults = trie.searchByPrefix("doc");
    cout << "   Resultados: ";
    for (const string& word : prefixResults) cout << word << " ";
    cout << endl;
    
    // Autocompletado
    cout << "\n3. Autocompletado 'arc':" << endl;
    vector<string> suggestions = trie.autocomplete("arc", 3);
    cout << "   Sugerencias: ";
    for (const string& word : suggestions) cout << word << " ";
    cout << endl;
    
    // Eliminar palabra
    cout << "\n4. Eliminando 'documento' (ID 1):" << endl;
    if (trie.remove("documento", 1)) {
        cout << "   Eliminado exitosamente" << endl;
        
        // Verificar eliminación
        docIds = trie.searchExact("documento");
        cout << "   'documento' después de eliminar: ";
        cout << (docIds.empty() ? "No encontrado" : "Aún existe") << endl;
    }
    
    // Estadísticas
    cout << "\n5. Estadísticas del Trie:" << endl;
    cout << "   Total de palabras: " << trie.countWords() << endl;
    cout << "   ¿Comienza con 'car'?: " << (trie.startsWith("car") ? "Sí" : "No") << endl;
    cout << "   ¿Comienza con 'xyz'?: " << (trie.startsWith("xyz") ? "Sí" : "No") << endl;
}

void testSearchEngine() {
    cout << "\n\n=== PRUEBA 2: MOTOR DE BÚSQUEDA INTEGRADO ===" << endl;
    
    // Crear árbol de prueba
    auto tree = make_shared<FileSystemTree>();
    
    // Crear estructura de archivos
    tree->createNode("/root", "Documentos", NodeType::FOLDER);
    tree->createNode("/root", "Imagenes", NodeType::FOLDER);
    tree->createNode("/root", "Musica", NodeType::FOLDER);
    
    tree->createNode("/root/Documentos", "trabajo", NodeType::FOLDER);
    tree->createNode("/root/Documentos", "personal", NodeType::FOLDER);
    tree->createNode("/root/Documentos/trabajo", "reporte_final.docx", NodeType::FILE, "Contenido del reporte");
    tree->createNode("/root/Documentos/trabajo", "presentacion.ppt", NodeType::FILE, "Diapositivas");
    tree->createNode("/root/Documentos/personal", "diario.txt", NodeType::FILE, "Mi diario personal");
    tree->createNode("/root/Documentos/personal", "lista_compras.txt", NodeType::FILE, "Leche, pan, huevos");
    
    tree->createNode("/root/Imagenes", "vacaciones", NodeType::FOLDER);
    tree->createNode("/root/Imagenes/vacaciones", "playa.jpg", NodeType::FILE, "Foto en la playa");
    tree->createNode("/root/Imagenes/vacaciones", "montaña.png", NodeType::FILE, "Foto en la montaña");
    
    tree->createNode("/root/Musica", "rock", NodeType::FOLDER);
    tree->createNode("/root/Musica/rock", "cancion1.mp3", NodeType::FILE, "Música rock");
    tree->createNode("/root/Musica/rock", "cancion2.mp3", NodeType::FILE, "Más música rock");
    
    cout << "Árbol de prueba creado con " << tree->calculateSize() << " nodos" << endl;
    
    // Crear motor de búsqueda
    SearchEngine searcher(tree);
    
    // Estadísticas iniciales
    cout << "\n1. Estadísticas del índice:" << endl;
    searcher.printIndexStats();
    
    // Búsqueda exacta
    cout << "\n2. Búsqueda exacta 'reporte_final.docx':" << endl;
    auto exactResults = searcher.searchExact("reporte_final.docx");
    cout << "   Encontrados: " << exactResults.size() << " nodos" << endl;
    for (auto node : exactResults) {
        cout << "   - ID: " << node->id << ", Ruta: " << tree->getFullPath(node) << endl;
    }
    
    // Búsqueda por prefijo
    cout << "\n3. Búsqueda por prefijo 'rep':" << endl;
    auto prefixResults = searcher.searchByPrefix("rep");
    cout << "   Encontrados: " << prefixResults.size() << " nodos" << endl;
    for (auto node : prefixResults) {
        cout << "   - ID: " << node->id << ", Nombre: " << node->name 
             << ", Ruta: " << tree->getFullPath(node) << endl;
    }
    
    // Autocompletado
    cout << "\n4. Autocompletado para 'can':" << endl;
    auto suggestions = searcher.autocomplete("can", 3);
    cout << "   Sugerencias (" << suggestions.size() << "):" << endl;
    for (const string& suggestion : suggestions) {
        cout << "   - " << suggestion << endl;
    }
    
    // Búsqueda por contenido
    cout << "\n5. Búsqueda que contiene 'musica':" << endl;
    auto containsResults = searcher.searchContains("musica");
    cout << "   Encontrados: " << containsResults.size() << " nodos" << endl;
    for (auto node : containsResults) {
        cout << "   - Nombre: " << node->name 
             << ", Ruta: " << tree->getFullPath(node) << endl;
    }
    
    // Verificar integridad
    cout << "\n6. Verificación de integridad del índice:" << endl;
    if (searcher.verifyIndexIntegrity()) {
        cout << "   ✓ Índice íntegro - todos los nodos están indexados" << endl;
    } else {
        cout << "   ✗ Problemas detectados en el índice" << endl;
    }
}

void testJsonIntegration() {
    cout << "\n\n=== PRUEBA 3: INTEGRACIÓN CON JSON ===" << endl;
    
    // Crear árbol de prueba
    auto tree = make_shared<FileSystemTree>();
    
    // Crear estructura
    tree->createNode("/root", "proyecto", NodeType::FOLDER);
    tree->createNode("/root/proyecto", "src", NodeType::FOLDER);
    tree->createNode("/root/proyecto", "docs", NodeType::FOLDER);
    tree->createNode("/root/proyecto/src", "main.cpp", NodeType::FILE, "#include <iostream>");
    tree->createNode("/root/proyecto/src", "utils.hpp", NodeType::FILE, "#pragma once");
    tree->createNode("/root/proyecto/docs", "README.md", NodeType::FILE, "# Proyecto");
    
    cout << "Árbol creado con " << tree->calculateSize() << " nodos" << endl;
    
    // Crear motor de búsqueda e indexar
    SearchEngine searcher(tree);
    
    // Guardar a JSON
    cout << "\n1. Guardando árbol a JSON..." << endl;
    if (JsonHandler::saveTree(tree, "test_tree.json")) {
        cout << "   ✓ Guardado exitoso" << endl;
    } else {
        cout << "   ✗ Error al guardar" << endl;
    }
    
    // Realizar búsqueda antes de cargar
    cout << "\n2. Búsqueda antes de cargar:" << endl;
    auto results = searcher.searchByPrefix("mai");
    cout << "   Búsqueda 'mai': " << results.size() << " resultados" << endl;
    
    // Crear nuevo árbol y cargar desde JSON
    cout << "\n3. Cargando desde JSON..." << endl;
    auto loadedTree = make_shared<FileSystemTree>();
    if (JsonHandler::loadTree(loadedTree, "test_tree.json")) {
        cout << "   ✓ Carga exitosa" << endl;
        
        // Crear nuevo motor de búsqueda para el árbol cargado
        SearchEngine loadedSearcher(loadedTree);
        
        // Verificar que la búsqueda funciona igual
        cout << "\n4. Búsqueda después de cargar:" << endl;
        auto loadedResults = loadedSearcher.searchByPrefix("mai");
        cout << "   Búsqueda 'mai': " << loadedResults.size() << " resultados" << endl;
        
        if (results.size() == loadedResults.size()) {
            cout << "   ✓ Resultados consistentes después de guardar/cargar" << endl;
        } else {
            cout << "   ✗ Inconsistencia en resultados" << endl;
        }
    }
}

void testPerformance() {
    cout << "\n\n=== PRUEBA 4: RENDIMIENTO CON DATOS GRANDES ===" << endl;
    
    auto tree = make_shared<FileSystemTree>();
    
    cout << "Creando árbol con múltiples niveles..." << endl;
    
    // Crear estructura profunda
    string currentPath = "/root";
    for (int i = 1; i <= 5; i++) {
        string folderName = "nivel_" + to_string(i);
        tree->createNode(currentPath, folderName, NodeType::FOLDER);
        currentPath += "/" + folderName;
        
        // Crear algunos archivos en cada nivel
        for (int j = 1; j <= 3; j++) {
            string fileName = "archivo_" + to_string(i) + "_" + to_string(j) + ".txt";
            tree->createNode(currentPath, fileName, NodeType::FILE, "Contenido de prueba " + fileName);
        }
    }
    
    cout << "Árbol creado con " << tree->calculateSize() << " nodos" << endl;
    cout << "Altura del árbol: " << tree->calculateHeight() << endl;
    
    // Crear motor de búsqueda
    cout << "\nIndexando árbol..." << endl;
    SearchEngine searcher(tree);
    
    // Pruebas de rendimiento de búsqueda
    cout << "\nPruebas de búsqueda:" << endl;
    
    // Búsqueda exacta
    auto start = high_resolution_clock::now();
    auto results = searcher.searchExact("archivo_5_3.txt");
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "1. Búsqueda exacta: " << duration.count() << " microsegundos" << endl;
    
    // Búsqueda por prefijo
    start = high_resolution_clock::now();
    results = searcher.searchByPrefix("nivel");
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "2. Búsqueda por prefijo: " << duration.count() << " microsegundos" << endl;
    
    // Autocompletado
    start = high_resolution_clock::now();
    auto suggestions = searcher.autocomplete("arch", 10);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "3. Autocompletado: " << duration.count() << " microsegundos" << endl;
    
    // Verificar integridad
    start = high_resolution_clock::now();
    bool integrity = searcher.verifyIndexIntegrity();
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "4. Verificación de integridad: " << duration.count() << " microsegundos" << endl;
    cout << "   Resultado: " << (integrity ? "Íntegro" : "Problemas detectados") << endl;
}

int main() {
    cout << "=== DÍAS 5-6: IMPLEMENTACIÓN DE TRIE Y BÚSQUEDA ===" << endl;
    cout << "Pruebas de autocompletado, búsqueda por prefijo e integración JSON\n" << endl;
    
    try {
        // Ejecutar pruebas
        testTrieBasic();
        testSearchEngine();
        testJsonIntegration();
        testPerformance();
        
        cout << "\n\n=== RESUMEN DE DÍAS 5-6 ===" << endl;
        cout << "Funcionalidades implementadas:" << endl;
        cout << "1. Trie completo con:" << endl;
        cout << "   - Inserción de palabras con IDs" << endl;
        cout << "   - Búsqueda exacta y por prefijo" << endl;
        cout << "   - Autocompletado con límite de sugerencias" << endl;
        cout << "   - Eliminación de palabras" << endl;
        cout << "   - Estadísticas y verificación" << endl;
        
        cout << "\n2. Motor de búsqueda integrado con:" << endl;
        cout << "   - Indexación automática del árbol" << endl;
        cout << "   - Reconstrucción de índice" << endl;
        cout << "   - Búsqueda exacta y por prefijo" << endl;
        cout << "   - Autocompletado interactivo" << endl;
        cout << "   - Búsqueda por contenido (substring)" << endl;
        cout << "   - Verificación de integridad" << endl;
        
        cout << "\n3. Integración JSON completa con:" << endl;
        cout << "   - Guardado de árbol completo con metadatos" << endl;
        cout << "   - Carga de árbol desde archivo JSON" << endl;
        cout << "   - Preservación de relaciones padre-hijo" << endl;
        cout << "   - Manejo de tipos (carpetas/archivos)" << endl;
        cout << "   - Contenido de archivos" << endl;
        
        cout << "\n4. Pruebas de rendimiento con datos grandes" << endl;
        
    } catch (const exception& e) {
        cerr << "\n✗ Error durante las pruebas: " << e.what() << endl;
        return 1;
    }
    
    cout << "\n=== IMPLEMENTACIÓN COMPLETADA EXITOSAMENTE ===" << endl;
    return 0;
}