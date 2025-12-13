#include "ConsoleInterface.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

ConsoleInterface::ConsoleInterface() 
    : fileSystem(make_shared<FileSystemTree>()), 
      currentPath("/root") {
    searchEngine = make_shared<SearchEngine>(fileSystem);
}

void ConsoleInterface::run() {
    cout << "=== SISTEMA DE ARCHIVOS JERÁRQUICO ===" << endl;
    cout << "Comandos disponibles: help, cd, ls, mkdir, touch, mv, rm, rename" << endl;
    cout << "                    search, autocomplete, export, save, load, tree, pwd, exit" << endl;
    
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
    cout << "cd <ruta>            - Cambiar directorio actual" << endl;
    cout << "ls [ruta]            - Listar contenido del directorio" << endl;
    cout << "mkdir <nombre>       - Crear nuevo directorio" << endl;
    cout << "touch <nombre> [cont] - Crear nuevo archivo" << endl;
    cout << "mv <origen> <destino> - Mover nodo" << endl;
    cout << "rm <ruta>            - Eliminar nodo" << endl;
    cout << "rename <ruta> <nuevo> - Renombrar nodo" << endl;
    cout << "search <consulta>    - Buscar nodos por nombre" << endl;
    cout << "autocomplete <prefijo> - Sugerencias de autocompletado" << endl;
    cout << "export <archivo>     - Exportar recorrido preorden" << endl;
    cout << "save [archivo]       - Guardar estado en JSON" << endl;
    cout << "load [archivo]       - Cargar estado desde JSON" << endl;
    cout << "tree                 - Mostrar estructura completa" << endl;
    cout << "pwd                  - Mostrar ruta actual" << endl;
    cout << "help                 - Mostrar esta ayuda" << endl;
    cout << "exit                 - Salir del sistema" << endl;
    cout << "=========================\n" << endl;
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
        // Primero remover del índice
        auto node = fileSystem->findNodeByPath(absPath);
        if (node) {
            searchEngine->removeNodeFromIndex(node);
        }
        
        // Luego eliminar del árbol
        if (fileSystem->deleteNode(absPath)) {
            cout << "Nodo eliminado exitosamente" << endl;
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