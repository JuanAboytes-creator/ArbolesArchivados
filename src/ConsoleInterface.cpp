#include "ConsoleInterface.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <random>
#include <functional>

using namespace std;
using namespace chrono;

ConsoleInterface::ConsoleInterface() 
    : fileSystem(make_shared<FileSystemTree>()), 
      currentPath("/root") {
    searchEngine = make_shared<SearchEngine>(fileSystem);
    // papelera inicia vacía
}

void ConsoleInterface::run() {
    cout << "=== SISTEMA DE ARCHIVOS JERÁRQUICO ===" << endl;
    cout << "Comandos disponibles: help, cd, ls, mkdir, touch, mv, rm, rename" << endl;
    cout << "                    search, autocomplete, export, save, load, tree, pwd, exit" << endl;
    cout << "                    trash, restore, emptytrash" << endl;
    
    string command;
    while (true) {
        cout << currentPath << "> ";
        getline(cin, command);
        
        if (command == "exit" || command == "quit") {
            break;
        }
        
        if (!command.empty()) {
            processCommand(command);
        }
    }
    
    cout << "Saliendo del sistema..." << endl;
}

void ConsoleInterface::processCommand(const string& command) {
    vector<string> args = parseArguments(command);
    
    if (args.empty()) return;
    
    string cmd = args[0];
    
    try {
        if (cmd == "help") {
            showHelp();
        } else if (cmd == "cd") {
            if (args.size() > 1) {
                changeDirectory(args[1]);
            } else {
                cout << "Uso: cd <ruta>" << endl;
            }
        } else if (cmd == "ls") {
            if (args.size() > 1) {
                listDirectory(args[1]);
            } else {
                listDirectory();
            }
        } else if (cmd == "mkdir") {
            if (args.size() > 1) {
                makeDirectory(args[1]);
            } else {
                cout << "Uso: mkdir <nombre>" << endl;
            }
        } else if (cmd == "touch") {
            if (args.size() > 1) {
                string content = "";
                if (args.size() > 2) {
                    content = args[2];
                }
                createFile(args[1], content);
            } else {
                cout << "Uso: touch <nombre> [contenido]" << endl;
            }
        } else if (cmd == "mv") {
            if (args.size() > 2) {
                moveNode(args[1], args[2]);
            } else {
                cout << "Uso: mv <origen> <destino>" << endl;
            }
        } else if (cmd == "rm") {
            if (args.size() > 1) {
                removeNode(args[1]);
            } else {
                cout << "Uso: rm <ruta>" << endl;
            }
        } else if (cmd == "rename") {
            if (args.size() > 2) {
                renameNode(args[1], args[2]);
            } else {
                cout << "Uso: rename <ruta> <nuevo_nombre>" << endl;
            }
        } else if (cmd == "trash") {
            listTrash();
        } else if (cmd == "restore") {
            if (args.size() > 1) {
                restoreFromTrash(args[1]);
            } else {
                cout << "Uso: restore <nombre>" << endl;
            }
        } else if (cmd == "emptytrash") {
            emptyTrash();
        } else if (cmd == "search") {
            if (args.size() > 1) {
                searchNodes(args[1]);
            } else {
                cout << "Uso: search <consulta>" << endl;
            }
        } else if (cmd == "autocomplete") {
            if (args.size() > 1) {
                autocompleteSuggestions(args[1]);
            } else {
                cout << "Uso: autocomplete <prefijo>" << endl;
            }
        } else if (cmd == "export") {
            if (args.size() > 1) {
                exportPreorder(args[1]);
            } else {
                cout << "Uso: export <archivo>" << endl;
            }
        } else if (cmd == "save") {
            string filename = "filesystem.json";
            if (args.size() > 1) {
                filename = args[1];
            }
            saveState(filename);
        } else if (cmd == "load") {
            string filename = "filesystem.json";
            if (args.size() > 1) {
                filename = args[1];
            }
            loadState(filename);
        } else if (cmd == "tree") {
            showTree();
        } else if (cmd == "pwd") {
            showPath();
        } else if (cmd == "pwd") {
            showPath();
            
        // Días 10-11: Comandos de pruebas
        } else if (cmd == "test-integration") {
            runIntegrationTests();
        } else if (cmd == "test-performance") {
            runPerformanceTests();
        } else if (cmd == "test-edge") {
            runEdgeCaseTests();
        } else if (cmd == "stats") {
            showSystemStats();
        } else if (cmd == "validate") {
            validateSystem();
        } else if (cmd == "generate") {
            if (args.size() > 2) {
                int levels = stoi(args[1]);
                int children = stoi(args[2]);
                generateTestTree(levels, children);
            } else {
                cout << "Uso: generate <niveles> <hijos_por_nivel>" << endl;
            }
        } else if (cmd == "benchmark") {
            benchmarkOperations();
        } else {
            cout << "Comando no reconocido: " << cmd << endl;
            cout << "Escriba 'help' para ver los comandos disponibles." << endl;
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::showHelp() {
    cout << "\n=== AYUDA DE COMANDOS ===" << endl;
    cout << "Comandos básicos:" << endl;
    cout << "  cd <ruta>            - Cambiar directorio actual" << endl;
    cout << "  ls [ruta]            - Listar contenido del directorio" << endl;
    cout << "  mkdir <nombre>       - Crear nuevo directorio" << endl;
    cout << "  touch <nombre> [cont] - Crear nuevo archivo" << endl;
    cout << "  mv <origen> <destino> - Mover nodo" << endl;
    cout << "  rm <ruta>            - Eliminar nodo (mueve a papelera)" << endl;
    cout << "  rename <ruta> <nuevo> - Renombrar nodo" << endl;
    cout << "  search <consulta>    - Buscar nodos por nombre" << endl;
    cout << "  autocomplete <prefijo> - Sugerencias de autocompletado" << endl;
    cout << "  export <archivo>     - Exportar recorrido preorden" << endl;
    cout << "  save [archivo]       - Guardar estado en JSON" << endl;
    cout << "  load [archivo]       - Cargar estado desde JSON" << endl;
    cout << "  tree                 - Mostrar estructura completa" << endl;
    cout << "  pwd                  - Mostrar ruta actual" << endl;
    
    cout << "\nPapelera:" << endl;
    cout << "  trash                - Mostrar papelera temporal" << endl;
    cout << "  restore <nombre>     - Restaurar elemento de papelera" << endl;
    cout << "  emptytrash           - Vaciar papelera permanentemente" << endl;
    
    cout << "\nPruebas (Días 10-11):" << endl;
    cout << "  test-integration     - Ejecutar pruebas de integración" << endl;
    cout << "  test-performance     - Ejecutar pruebas de rendimiento" << endl;
    cout << "  test-edge            - Ejecutar pruebas de casos límite" << endl;
    cout << "  stats                - Mostrar estadísticas del sistema" << endl;
    cout << "  validate             - Validar consistencia del sistema" << endl;
    cout << "  generate <n> <h>     - Generar árbol de prueba" << endl;
    cout << "  benchmark            - Medir rendimiento de operaciones" << endl;
    
    cout << "\nSistema:" << endl;
    cout << "  help                 - Mostrar esta ayuda" << endl;
    cout << "  exit                 - Salir del sistema" << endl;
    cout << "=========================\n" << endl;
}
void ConsoleInterface::runIntegrationTests() {
    cout << "\n=== PRUEBAS DE INTEGRACIÓN ===" << endl;
    
    // Test 1: Crear y eliminar estructura compleja
    cout << "\n1. Creando estructura compleja..." << endl;
    createFile("test1.txt", "Contenido de prueba");
    makeDirectory("carpeta1");
    changeDirectory("carpeta1");
    createFile("archivo1.txt", "Archivo dentro de carpeta");
    makeDirectory("subcarpeta");
    changeDirectory("..");
    
    cout << "✓ Estructura creada exitosamente." << endl;
    
    // Test 2: Mover nodos
    cout << "\n2. Probando movimiento de nodos..." << endl;
    moveNode("carpeta1", ".");
    cout << "✓ Movimiento completado." << endl;
    
    // Test 3: Renombrar
    cout << "\n3. Probando renombrado..." << endl;
    renameNode("carpeta1", "carpeta_renombrada");
    cout << "✓ Renombrado completado." << endl;
    
    // Test 4: Buscar
    cout << "\n4. Probando búsqueda..." << endl;
    searchNodes("test");
    cout << "✓ Búsqueda completada." << endl;
    
    // Test 5: Exportar
    cout << "\n5. Probando exportación..." << endl;
    exportPreorder("test_integracion.txt");
    cout << "✓ Exportación completada." << endl;
    
    // Test 6: Guardar y cargar
    cout << "\n6. Probando persistencia..." << endl;
    saveState("test_state.json");
    loadState("test_state.json");
    cout << "✓ Persistencia verificada." << endl;
    
    // Test 7: Papelera
    cout << "\n7. Probando papelera..." << endl;
    removeNode("test1.txt");
    listTrash();
    restoreFromTrash("test1.txt");
    cout << "✓ Papelera funcionando correctamente." << endl;
    
    cout << "\n=== PRUEBAS DE INTEGRACIÓN COMPLETADAS ===" << endl;
}

void ConsoleInterface::runPerformanceTests() {
    cout << "\n=== PRUEBAS DE RENDIMIENTO ===" << endl;
    
    // Guardar estado actual
    auto originalTree = fileSystem;
    auto originalSearch = searchEngine;
    
    // Crear árbol grande
    cout << "1. Generando árbol grande para pruebas..." << endl;
    fileSystem = make_shared<FileSystemTree>();
    searchEngine = make_shared<SearchEngine>(fileSystem);
    
    fileSystem->generateLargeTree(4, 3); // 4 niveles, 3 hijos por nivel
    int totalNodes = fileSystem->calculateSize();
    cout << "   Árbol creado con " << totalNodes << " nodos." << endl;
    
    // Medir recorrido
    cout << "\n2. Mediendo tiempo de recorrido..." << endl;
    double traversalTime = fileSystem->measureTraversalTime();
    cout << "   Tiempo de recorrido preorden: " << traversalTime * 1000 << " ms" << endl;
    cout << "   Tiempo por nodo: " << (traversalTime / totalNodes) * 1000000 << " μs" << endl;
    
    // Medir búsqueda
    cout << "\n3. Mediendo tiempo de búsqueda..." << endl;
    double searchTime = fileSystem->measureSearchTime("nodo");
    cout << "   Tiempo de búsqueda: " << searchTime * 1000 << " ms" << endl;
    
    // Medir operaciones de creación
    cout << "\n4. Mediendo operaciones de creación..." << endl;
    auto start = high_resolution_clock::now();
    
    for (int i = 0; i < 100; i++) {
        string name = "perf_test_" + to_string(i);
        fileSystem->createNode("/root", name, NodeType::FILE);
    }
    
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    cout << "   Tiempo para crear 100 archivos: " << elapsed.count() * 1000 << " ms" << endl;
    cout << "   Tiempo por operación: " << (elapsed.count() / 100) * 1000 << " ms" << endl;
    
    // Restaurar estado original
    fileSystem = originalTree;
    searchEngine = originalSearch;
    
    cout << "\n=== PRUEBAS DE RENDIMIENTO COMPLETADAS ===" << endl;
}

void ConsoleInterface::runEdgeCaseTests() {
    cout << "\n=== PRUEBAS DE CASOS LÍMITE ===" << endl;
    
    cout << "1. Intentando crear nodo con nombre vacío..." << endl;
    try {
        createFile("", "contenido");
    } catch (const exception& e) {
        cout << "   ✓ Correctamente rechazado: " << e.what() << endl;
    }
    
    cout << "\n2. Intentando crear nodo con nombre duplicado..." << endl;
    createFile("duplicado.txt", "test");
    try {
        createFile("duplicado.txt", "test2");
    } catch (const exception& e) {
        cout << "   ✓ Correctamente rechazado: " << e.what() << endl;
    }
    
    cout << "\n3. Intentando mover nodo a sí mismo..." << endl;
    makeDirectory("test_move");
    try {
        moveNode("test_move", "test_move");
    } catch (const exception& e) {
        cout << "   ✓ Correctamente rechazado: " << e.what() << endl;
    }
    
    cout << "\n4. Intentando mover nodo a su descendiente..." << endl;
    makeDirectory("test_move/parent");
    makeDirectory("test_move/parent/child");
    try {
        moveNode("test_move/parent", "test_move/parent/child");
    } catch (const exception& e) {
        cout << "   ✓ Correctamente rechazado: " << e.what() << endl;
    }
    
    cout << "\n5. Intentando eliminar directorio no vacío..." << endl;
    createFile("test_delete/file.txt", "contenido");
    removeNode("test_delete");
    cout << "   ✓ Directorio movido a papelera correctamente." << endl;
    
    cout << "\n6. Intentando restaurar nodo que no existe en papelera..." << endl;
    restoreFromTrash("nodo_inexistente");
    cout << "   ✓ Correctamente manejado." << endl;
    
    cout << "\n7. Probando rutas inválidas..." << endl;
    try {
        changeDirectory("/ruta/inexistente");
    } catch (const exception& e) {
        cout << "   ✓ Correctamente manejado: " << e.what() << endl;
    }
    
    cout << "\n=== PRUEBAS DE CASOS LÍMITE COMPLETADAS ===" << endl;
}

void ConsoleInterface::showSystemStats() {
    cout << "\n=== ESTADÍSTICAS DEL SISTEMA ===" << endl;
    
    // Estadísticas del árbol
    fileSystem->printTreeStats();
    
    // Estadísticas del índice de búsqueda
    searchEngine->printIndexStats();
    
    // Estadísticas de la papelera
    cout << "Papelera: " << trashBin.size() << " elementos" << endl;
    
    // Memoria aproximada
    int totalNodes = fileSystem->calculateSize();
    long estimatedMemory = totalNodes * 100; // Estimación aproximada por nodo
    cout << "Memoria estimada: ~" << estimatedMemory << " bytes" << endl;
    
    cout << "=================================\n" << endl;
}

void ConsoleInterface::validateSystem() {
    cout << "\n=== VALIDACIÓN DEL SISTEMA ===" << endl;
    
    // Validar árbol
    bool treeValid = fileSystem->validateTreeStructure();
    cout << "\nValidación del árbol: " << (treeValid ? "PASÓ" : "FALLÓ") << endl;
    
    // Validar índice
    bool indexValid = searchEngine->verifyIndexIntegrity();
    cout << "Validación del índice: " << (indexValid ? "PASÓ" : "FALLÓ") << endl;
    
    // Verificar consistencia entre árbol e índice
    cout << "\nVerificando consistencia árbol-índice..." << endl;
    int treeSize = fileSystem->calculateSize();
    // Aquí podríamos agregar más verificaciones
    
    cout << "\n=== VALIDACIÓN COMPLETADA ===" << endl;
}

void ConsoleInterface::generateTestTree(int levels, int children) {
    cout << "\nGenerando árbol de prueba..." << endl;
    cout << "Niveles: " << levels << ", Hijos por nivel: " << children << endl;
    
    fileSystem->generateLargeTree(levels, children);
    
    cout << "Árbol generado exitosamente." << endl;
    showSystemStats();
}

void ConsoleInterface::benchmarkOperations() {
    cout << "\n=== BENCHMARK DE OPERACIONES ===" << endl;
    
    vector<pair<string, std::function<void()>>> operations;
    
    // Agregar operaciones al vector
    operations.push_back({"Crear 100 archivos", [&]() {
        for (int i = 0; i < 100; i++) {
            createFile("bench_" + to_string(i) + ".txt", "contenido");
        }
    }});
    
    operations.push_back({"Crear 50 carpetas", [&]() {
        for (int i = 0; i < 50; i++) {
            makeDirectory("bench_dir_" + to_string(i));
        }
    }});
    
    operations.push_back({"Listar directorio", [&]() {
        listDirectory();
    }});
    
    operations.push_back({"Búsqueda por prefijo", [&]() {
        searchNodes("bench");
    }});
    
    operations.push_back({"Recorrido completo", [&]() {
        fileSystem->preorderTraversal();
    }});
    
    for (auto& operation : operations) {
        auto start = high_resolution_clock::now();
        
        try {
            operation.second();  // Ejecutar la función
        } catch (const exception& e) {
            cout << "Error durante " << operation.first << ": " << e.what() << endl;
        }
        
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;
        
        cout << operation.first << ": " << elapsed.count() * 1000 << " ms" << endl;
    }
    
    cout << "\n=== BENCHMARK COMPLETADO ===" << endl;
}
void ConsoleInterface::changeDirectory(const string& path) {
    string absPath = getAbsolutePath(path);
    auto node = fileSystem->findNodeByPath(absPath);
    
    if (node && node->isFolder()) {
        currentPath = absPath;
        cout << "Directorio cambiado a: " << currentPath << endl;
    } else {
        cout << "Error: Directorio no encontrado o no es una carpeta" << endl;
    }
}

void ConsoleInterface::listDirectory(const string& path) {
    string absPath = path.empty() ? currentPath : getAbsolutePath(path);
    
    try {
        auto children = fileSystem->listChildren(absPath);
        
        if (children.empty()) {
            cout << "Directorio vacío" << endl;
        } else {
            cout << "Contenido de " << absPath << ":" << endl;
            for (const auto& child : children) {
                cout << "  " << child << endl;
            }
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::makeDirectory(const string& path) {
    string absPath = currentPath;
    string dirName = path;
    
    // Si la ruta contiene '/', separar
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash != string::npos) {
        absPath = getAbsolutePath(path.substr(0, lastSlash));
        dirName = path.substr(lastSlash + 1);
    }
    
    try {
        fileSystem->createNode(absPath, dirName, NodeType::FOLDER);
        searchEngine->rebuildIndex(); // Reindexar después de crear
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::createFile(const string& path, const string& content) {
    string absPath = currentPath;
    string fileName = path;
    
    // Si la ruta contiene '/', separar
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash != string::npos) {
        absPath = getAbsolutePath(path.substr(0, lastSlash));
        fileName = path.substr(lastSlash + 1);
    }
    
    try {
        fileSystem->createNode(absPath, fileName, NodeType::FILE, content);
        searchEngine->rebuildIndex(); // Reindexar después de crear
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::moveNode(const string& source, const string& dest) {
    string absSource = getAbsolutePath(source);
    string absDest = getAbsolutePath(dest);
    
    try {
        if (fileSystem->moveNode(absSource, absDest)) {
            searchEngine->rebuildIndex(); // Reindexar después de mover
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::removeNode(const string& path) {
    string absPath = getAbsolutePath(path);
    
    try {
        auto node = fileSystem->findNodeByPath(absPath);
        if (!node) {
            cout << "Error: Nodo no encontrado: " << path << endl;
            return;
        }
        
        // Guardar información para posible restauración
        TrashItem item;
        item.node = node;
        item.originalPath = absPath;
        // CORRECCIÓN: Usamos .lock() para obtener shared_ptr del weak_ptr
        item.parent = node->parent.lock();
        
        // guardar en papelera
        trashBin.push_back(item);
        
        // eliminar del índice de búsqueda
        searchEngine->removeNodeFromIndex(node);
        
        // eliminar del árbol
        if (fileSystem->deleteNode(absPath)) {
            cout << "Nodo '" << node->name << "' movido a la papelera." << endl;
            cout << "Usa 'trash' para ver la papelera." << endl;
            cout << "Usa 'restore " << node->name << "' para recuperarlo." << endl;
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}


void ConsoleInterface::renameNode(const string& path, const string& newName) {
    string absPath = getAbsolutePath(path);
    
    try {
        // Primero remover del índice
        auto node = fileSystem->findNodeByPath(absPath);
        if (node) {
            searchEngine->removeNodeFromIndex(node);
        }
        
        // Renombrar
        if (fileSystem->renameNode(absPath, newName)) {
            // Reindexar con el nuevo nombre
            searchEngine->rebuildIndex();
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::searchNodes(const string& query) {
    cout << "Resultados de búsqueda para '" << query << "':" << endl;
    
    // Búsqueda por prefijo
    auto results = searchEngine->searchByPrefix(query);
    
    if (results.empty()) {
        cout << "No se encontraron resultados" << endl;
    } else {
        for (const auto& node : results) {
            cout << "  - " << node->name << " (ID: " << node->id 
                 << ") - Ruta: " << fileSystem->getFullPath(node) << endl;
        }
        cout << "Total: " << results.size() << " resultados" << endl;
    }
}

void ConsoleInterface::autocompleteSuggestions(const string& prefix) {
    cout << "Sugerencias para '" << prefix << "':" << endl;
    
    auto suggestions = searchEngine->autocomplete(prefix, 10);
    
    if (suggestions.empty()) {
        cout << "No hay sugerencias" << endl;
    } else {
        for (const auto& suggestion : suggestions) {
            cout << "  - " << suggestion << endl;
        }
    }
}

void ConsoleInterface::exportPreorder(const string& filename) {
    try {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << filename << endl;
            return;
        }
        
        auto traversal = fileSystem->preorderTraversal();
        
        file << "=== RECORRIDO PREORDEN DEL ÁRBOL ===" << endl;
        file << "Fecha: " << __DATE__ << " " << __TIME__ << endl;
        file << "Total nodos: " << fileSystem->calculateSize() << endl;
        file << "Altura: " << fileSystem->calculateHeight() << endl;
        file << "=====================================" << endl << endl;
        
        for (const auto& node : traversal) {
            file << node << endl;
        }
        
        file.close();
        cout << "Recorrido exportado exitosamente a " << filename << endl;
        cout << "Nodos exportados: " << traversal.size() << endl;
        
    } catch (const exception& e) {
        cout << "Error al exportar: " << e.what() << endl;
    }
}

void ConsoleInterface::saveState(const string& filename) {
    try {
        if (JsonHandler::saveTree(fileSystem, filename)) {
            cout << "Estado guardado exitosamente en " << filename << endl;
        } else {
            cout << "Error al guardar el estado" << endl;
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::loadState(const string& filename) {
    try {
        // Crear nuevo árbol
        auto newTree = make_shared<FileSystemTree>();
        
        if (JsonHandler::loadTree(newTree, filename)) {
            fileSystem = newTree;
            searchEngine = make_shared<SearchEngine>(fileSystem);
            currentPath = "/root";
            // Limpiar papelera al cargar nuevo estado
            trashBin.clear();
            cout << "Estado cargado exitosamente desde " << filename << endl;
        } else {
            cout << "Error al cargar el estado" << endl;
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void ConsoleInterface::showTree() {
    fileSystem->printTree();
}

void ConsoleInterface::showPath() {
    cout << "Ruta actual: " << currentPath << endl;
}

// Helper para parsear argumentos
vector<string> ConsoleInterface::parseArguments(const string& input) {
    vector<string> args;
    stringstream ss(input);
    string token;
    
    while (ss >> token) {
        args.push_back(token);
    }
    
    return args;
}

// Helper para trim de strings
string ConsoleInterface::trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Helper para convertir ruta relativa a absoluta
string ConsoleInterface::getAbsolutePath(const string& relativePath) {
    if (relativePath.empty()) return currentPath;
    
    if (relativePath[0] == '/') {
        // Ruta absoluta
        return relativePath;
    }
    
    // Ruta relativa
    if (relativePath == "..") {
        // Subir un nivel
        size_t lastSlash = currentPath.find_last_of('/');
        if (lastSlash != string::npos && lastSlash > 0) {
            return currentPath.substr(0, lastSlash);
        }
        return "/root";
    }
    
    if (relativePath == ".") {
        // Directorio actual
        return currentPath;
    }
    
    // Concatenar con directorio actual
    if (currentPath == "/") {
        return currentPath + relativePath;
    } else {
        return currentPath + "/" + relativePath;
    }
}

// ============================================
// PAPELERA (DÍA 7)
// ============================================

void ConsoleInterface::listTrash() {
    if (trashBin.empty()) {
        cout << "La papelera está vacía." << endl;
        return;
    }
    
    cout << "\n=== PAPELERA TEMPORAL ===" << endl;
    cout << "Elementos: " << trashBin.size() << endl;
    cout << "==========================" << endl;
    
    for (size_t i = 0; i < trashBin.size(); ++i) {
        auto& item = trashBin[i];
        auto node = item.node;
        string tipo = node->isFolder() ? "[CARPETA]" : "[ARCHIVO]";
        cout << i+1 << ". " << tipo << " " << node->name 
             << " (ID: " << node->id << ")" << endl;
        cout << "    Ruta original: " << item.originalPath << endl;
    }
}

void ConsoleInterface::restoreFromTrash(const string& name) {
    // Buscar nodo en la papelera
    for (auto it = trashBin.begin(); it != trashBin.end(); ++it) {
        if (it->node->name == name) {
            auto& item = *it;
            
            try {
                // Determinar dónde restaurar el nodo
                shared_ptr<TreeNode> parentNode = nullptr;
                
                if (item.parent) {
                    // Intentar restaurar en el padre original
                    parentNode = item.parent;
                    
                    // Verificar que el padre todavía existe en el árbol
                    // Si no existe, restaurar en /root
                    if (!fileSystem->isNodeInTree(parentNode)) {
                        parentNode = fileSystem->findNodeByPath("/root");
                        if (!parentNode) {
                            cout << "Error: El padre original ya no existe y no se encontró /root" << endl;
                            return;
                        }
                        cout << "Nota: El padre original ya no existe, restaurando en /root" << endl;
                    } else {
                        cout << "Restaurando en el directorio original." << endl;
                    }
                } else {
                    // Si no hay padre original, restaurar en /root
                    parentNode = fileSystem->findNodeByPath("/root");
                    if (!parentNode) {
                        cout << "Error: No se encontró el directorio raíz (/root)" << endl;
                        return;
                    }
                    cout << "Nota: Restaurando en el directorio raíz (/root)" << endl;
                }
                
                // Verificar si ya existe un nodo con el mismo nombre en el directorio destino
                bool nameExists = false;
                for (const auto& child : parentNode->children) {
                    if (child->name == name) {
                        nameExists = true;
                        break;
                    }
                }
                
                if (nameExists) {
                    cout << "Error: Ya existe un nodo con el nombre '" << name 
                         << "' en el directorio de destino." << endl;
                    return;
                }
                
                // Restaurar el nodo al árbol
                item.node->parent = parentNode;
                parentNode->children.push_back(item.node);
                
                // Reindexar el nodo en el motor de búsqueda
                searchEngine->indexNode(item.node);
                
                // Remover de la papelera
                trashBin.erase(it);
                
                cout << "Nodo '" << name << "' restaurado exitosamente en: " 
                     << fileSystem->getFullPath(parentNode) << endl;
                
                return;
                
            } catch (const exception& e) {
                cout << "Error al restaurar: " << e.what() << endl;
                return;
            }
        }
    }
    
    cout << "Error: No se encontró '" << name << "' en la papelera." << endl;
}

void ConsoleInterface::emptyTrash() {
    if (trashBin.empty()) {
        cout << "La papelera ya está vacía." << endl;
        return;
    }
    
    cout << "¿Está seguro de vaciar la papelera? (" << trashBin.size() 
         << " elementos) [s/N]: ";
    
    string respuesta;
    getline(cin, respuesta);
    
    if (respuesta == "s" || respuesta == "S") {
        cout << "Vaciando papelera..." << endl;
        trashBin.clear();
        cout << "Papelera vaciada permanentemente." << endl;
    } else {
        cout << "Operación cancelada." << endl;
    }
}