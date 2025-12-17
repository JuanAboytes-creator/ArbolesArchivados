#include "FileSystemTree.hpp"
#include <sstream>
#include <algorithm>
#include <stack>
#include <queue>
#include <iostream>
#include <chrono>
#include <cmath>
#include <unordered_set>
#include <climits>
#include <limits>

using namespace std;
using namespace chrono;

FileSystemTree::FileSystemTree() : nextId(1) {
    // Crear nodo raíz
    root = make_shared<TreeNode>(0, "root", NodeType::FOLDER);
}

// Helper: dividir ruta en componentes
vector<string> FileSystemTree::splitPath(const string& path) {
    vector<string> components;
    stringstream ss(path);
    string component;
    
    while (getline(ss, component, '/')) {
        if (!component.empty()) {
            components.push_back(component);
        }
    }
    
    return components;
}

// Helper: encontrar nodo por ruta
shared_ptr<TreeNode> FileSystemTree::findNodeByPath(const string& path) {
    if (path == "/" || path.empty()) {
        return root;
    }
    
    vector<string> components = splitPath(path);
    shared_ptr<TreeNode> current = root;
    
    for (const string& component : components) {
        if (component == "root") continue;
        
        bool found = false;
        for (auto& child : current->children) {
            if (child->name == component) {
                current = child;
                found = true;
                break;
            }
        }
        
        if (!found) {
            return nullptr;
        }
    }
    
    return current;
}

// Helper: encontrar nodo por ID (recursivo)
shared_ptr<TreeNode> FileSystemTree::findNodeById(shared_ptr<TreeNode> node, int id) {
    if (node->id == id) {
        return node;
    }
    
    for (auto& child : node->children) {
        auto result = findNodeById(child, id);
        if (result != nullptr) {
            return result;
        }
    }
    
    return nullptr;
}

// Helper: verificar si nombre ya existe en hijos
bool FileSystemTree::nameExists(shared_ptr<TreeNode> parent, const string& name) {
    for (auto& child : parent->children) {
        if (child->name == name) {
            return true;
        }
    }
    return false;
}

// 1. Crear nodo en ruta específica
shared_ptr<TreeNode> FileSystemTree::createNode(const string& path, const string& name, 
                                               NodeType type, const string& content) {
    // Validar entrada
    if (name.empty()) {
        throw invalid_argument("El nombre no puede estar vacío");
    }
    
    auto parentNode = findNodeByPath(path);
    if (!parentNode) {
        throw invalid_argument("Ruta no encontrada: " + path);
    }
    
    if (!parentNode->isFolder()) {
        throw invalid_argument("No se puede crear dentro de un archivo");
    }
    
    if (nameExists(parentNode, name)) {
        throw invalid_argument("Ya existe un nodo con ese nombre");
    }
    
    // Crear nuevo nodo
    auto newNode = make_shared<TreeNode>(nextId++, name, type, content);
    parentNode->addChild(newNode);
    
    cout << "Nodo creado: ID=" << newNode->id 
         << ", Nombre=" << name 
         << ", Tipo=" << (type == NodeType::FOLDER ? "CARPETA" : "ARCHIVO")
         << endl;
    
    return newNode;
}

// 2. Mover nodo de una ruta a otra
bool FileSystemTree::moveNode(const string& sourcePath, const string& destPath) {
    auto sourceNode = findNodeByPath(sourcePath);
    if (!sourceNode) {
        throw invalid_argument("Nodo origen no encontrado: " + sourcePath);
    }
    
    auto destNode = findNodeByPath(destPath);
    if (!destNode) {
        throw invalid_argument("Ruta destino no encontrada: " + destPath);
    }
    
    if (!destNode->isFolder()) {
        throw invalid_argument("El destino debe ser una carpeta");
    }
    
    // Verificar si el destino es descendiente del origen
    auto current = destNode;
    while (auto parent = current->parent.lock()) {
        if (parent == sourceNode) {
            throw invalid_argument("No se puede mover un nodo dentro de sus propios descendientes");
        }
        current = parent;
    }
    
    // Remover del padre actual
    auto sourceParent = sourceNode->parent.lock();
    if (sourceParent) {
        sourceParent->removeChild(sourceNode->name);
    }
    
    // Agregar al nuevo padre
    destNode->addChild(sourceNode);
    
    cout << "Nodo movido: " << sourcePath << " -> " << destPath << endl;
    return true;
}

// 3. Renombrar nodo
bool FileSystemTree::renameNode(const string& path, const string& newName) {
    if (newName.empty()) {
        throw invalid_argument("El nuevo nombre no puede estar vacío");
    }
    
    auto node = findNodeByPath(path);
    if (!node) {
        throw invalid_argument("Nodo no encontrado: " + path);
    }
    
    // Verificar si es la raíz
    if (node == root) {
        throw invalid_argument("No se puede renombrar la raíz");
    }
    
    // Verificar si el nombre ya existe en el mismo nivel
    auto parent = node->parent.lock();
    if (parent && nameExists(parent, newName)) {
        throw invalid_argument("Ya existe un nodo con ese nombre en esta ubicación");
    }
    
    string oldName = node->name;
    node->name = newName;
    
    cout << "Nodo renombrado: " << oldName << " -> " << newName << endl;
    return true;
}

// 4. Eliminar nodo (recursivo)
bool FileSystemTree::deleteNode(const string& path) {
    auto node = findNodeByPath(path);
    if (!node) {
        throw invalid_argument("Nodo no encontrado: " + path);
    }
    
    // Verificar si es la raíz
    if (node == root) {
        throw invalid_argument("No se puede eliminar la raíz");
    }
    
    // Remover del padre
    auto parent = node->parent.lock();
    if (parent) {
        parent->removeChild(node->name);
        cout << "Nodo eliminado: " << path << " (ID=" << node->id << ")" << endl;
        return true;
    }
    
    return false;
}

// 5. Listar hijos de un nodo
vector<string> FileSystemTree::listChildren(const string& path) {
    auto node = findNodeByPath(path);
    if (!node) {
        throw invalid_argument("Nodo no encontrado: " + path);
    }
    
    vector<string> result;
    for (auto& child : node->children) {
        string typeStr = child->isFolder() ? "[DIR] " : "[FILE] ";
        result.push_back(typeStr + child->name + " (ID: " + to_string(child->id) + ")");
    }
    
    return result;
}

// 6. Mostrar ruta completa de un nodo
string FileSystemTree::getFullPath(shared_ptr<TreeNode> node) {
    if (!node) return "";
    
    vector<string> pathComponents;
    shared_ptr<TreeNode> current = node;
    
    while (current && current != root) {
        pathComponents.push_back(current->name);
        if (auto parent = current->parent.lock()) {
            current = parent;
        } else {
            break;
        }
    }
    
    // Agregar la raíz
    pathComponents.push_back("root");
    
    // Construir la ruta desde la raíz
    string path = "/";
    for (auto it = pathComponents.rbegin(); it != pathComponents.rend(); ++it) {
        path += *it;
        if (it + 1 != pathComponents.rend()) {
            path += "/";
        }
    }
    
    return path;
}

// 7. Recorrido en preorden
vector<string> FileSystemTree::preorderTraversal() {
    vector<string> result;
    if (!root) return result;
    
    stack<shared_ptr<TreeNode>> s;
    s.push(root);
    
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        
        // Procesar nodo actual
        string nodeInfo = "ID: " + to_string(current->id) + 
                         ", Nombre: " + current->name +
                         ", Tipo: " + (current->isFolder() ? "CARPETA" : "ARCHIVO");
        result.push_back(nodeInfo);
        
        // Agregar hijos en orden inverso para mantener orden correcto
        for (auto it = current->children.rbegin(); it != current->children.rend(); ++it) {
            s.push(*it);
        }
    }
    
    return result;
}

// 8. Calcular altura del árbol
int FileSystemTree::calculateHeight() {
    return calculateHeightRecursive(root) - 1; // Restar 1 porque la raíz tiene altura 0
}

int FileSystemTree::calculateHeightRecursive(shared_ptr<TreeNode> node) {
    if (!node) return 0;
    
    int maxHeight = 0;
    for (auto& child : node->children) {
        int childHeight = calculateHeightRecursive(child);
        maxHeight = max(maxHeight, childHeight);
    }
    
    return maxHeight + 1;
}

// 9. Calcular tamaño (número de nodos)
int FileSystemTree::calculateSize() {
    return calculateSizeRecursive(root);
}

int FileSystemTree::calculateSizeRecursive(shared_ptr<TreeNode> node) {
    if (!node) return 0;
    
    int size = 1; // Contar el nodo actual
    for (auto& child : node->children) {
        size += calculateSizeRecursive(child);
    }
    
    return size;
}

// 10. Buscar nodo por ID
shared_ptr<TreeNode> FileSystemTree::findNodeById(int id) {
    return findNodeById(root, id);
}

// 11. Verificar consistencia del árbol
bool FileSystemTree::verifyTreeConsistency() {
    return verifyConsistencyRecursive(root, nullptr);
}

bool FileSystemTree::verifyConsistencyRecursive(shared_ptr<TreeNode> node, shared_ptr<TreeNode> expectedParent) {
    if (!node) return true;
    
    // Verificar que el padre sea el esperado
    auto actualParent = node->parent.lock();
    if (expectedParent == nullptr) {
        if (actualParent != nullptr) {
            return false;
        }
    } else if (actualParent != expectedParent) {
        return false;
    }
    
    // Verificar recursivamente los hijos
    for (auto& child : node->children) {
        if (!verifyConsistencyRecursive(child, node)) {
            return false;
        }
    }
    
    return true;
}

// Getters
shared_ptr<TreeNode> FileSystemTree::getRoot() const { 
    return root; 
}

int FileSystemTree::getNextId() const { 
    return nextId; 
}

// Método para imprimir árbol
void FileSystemTree::printTree() {
    cout << "\n=== ESTRUCTURA DEL ÁRBOL ===" << endl;
    printTreeRecursive(root, 0);
}

void FileSystemTree::printTreeRecursive(shared_ptr<TreeNode> node, int depth) {
    if (!node) return;
    
    // Imprimir sangría
    for (int i = 0; i < depth; i++) {
        cout << "  ";
    }
    
    // Imprimir información del nodo
    cout << "├─ ";
    if (node->isFolder()) {
        cout << "[DIR] ";
    } else {
        cout << "[FILE] ";
    }
    cout << node->name << " (ID: " << node->id << ")";
    
    if (node->isFile() && !node->content.empty()) {
        cout << " - Contenido: \"" << node->content.substr(0, 20) 
             << (node->content.length() > 20 ? "..." : "") << "\"";
    }
    cout << endl;
    
    // Imprimir hijos
    for (auto& child : node->children) {
        printTreeRecursive(child, depth + 1);
    }
}
// Verificar si nodo está en árbol
bool FileSystemTree::isNodeInTree(shared_ptr<TreeNode> node) {
    return findNodeById(node->id) != nullptr;
}

// Helper para estadísticas
void FileSystemTree::collectStatsRecursive(shared_ptr<TreeNode> node, int depth, 
                                          int& folderCount, int& fileCount, int& maxDepth, int& totalNodes) {
    if (!node) return;
    
    totalNodes++;
    if (node->isFolder()) {
        folderCount++;
    } else {
        fileCount++;
    }
    
    maxDepth = max(maxDepth, depth);
    
    for (auto& child : node->children) {
        collectStatsRecursive(child, depth + 1, folderCount, fileCount, maxDepth, totalNodes);
    }
}

// Obtener estadísticas del árbol
FileSystemTree::TreeStats FileSystemTree::getTreeStats() {
    TreeStats stats;
    stats.totalNodes = 0;
    stats.folderCount = 0;
    stats.fileCount = 0;
    stats.maxDepth = 0;
    stats.minDepth = INT_MAX;  // Usar INT_MAX que ahora está disponible
    stats.avgDepth = 0.0;
    stats.treeHeight = 0;
    
    if (!root) return stats;
    
    // Usar BFS para calcular profundidades mínimas y promedio
    queue<pair<shared_ptr<TreeNode>, int>> q;
    q.push({root, 0});
    
    int totalLeafDepth = 0;
    int leafCount = 0;
    
    while (!q.empty()) {
        auto currentPair = q.front();
        auto current = currentPair.first;
        int depth = currentPair.second;
        q.pop();
        
        stats.totalNodes++;
        if (current->isFolder()) {
            stats.folderCount++;
        } else {
            stats.fileCount++;
        }
        
        // Si es hoja
        if (current->children.empty()) {
            leafCount++;
            totalLeafDepth += depth;
            stats.minDepth = min(stats.minDepth, depth);
        }
        
        for (auto& child : current->children) {
            q.push({child, depth + 1});
        }
    }
    
    stats.maxDepth = calculateHeight() - 1; // Altura - 1 para profundidad máxima
    stats.treeHeight = calculateHeight();
    
    if (leafCount > 0) {
        stats.avgDepth = static_cast<double>(totalLeafDepth) / leafCount;
    } else {
        stats.minDepth = 0;
        stats.avgDepth = 0.0;
    }
    
    return stats;
}

// Generar árbol grande para pruebas de rendimiento
void FileSystemTree::generateLargeTree(int levels, int childrenPerLevel) {
    cout << "Generando árbol grande con " << levels << " niveles y " 
         << childrenPerLevel << " hijos por nivel..." << endl;
    
    // Limpiar árbol existente
    root = make_shared<TreeNode>(0, "root", NodeType::FOLDER);
    nextId = 1;
    
    // Usar BFS para generar el árbol
    queue<shared_ptr<TreeNode>> q;
    q.push(root);
    int currentLevel = 0;
    
    while (!q.empty() && currentLevel < levels) {
        int levelSize = q.size();
        
        for (int i = 0; i < levelSize; i++) {
            auto current = q.front();
            q.pop();
            
            // Crear hijos para este nodo
            for (int j = 0; j < childrenPerLevel; j++) {
                string name = "nodo_L" + to_string(currentLevel + 1) + 
                            "_P" + to_string(i) + "_H" + to_string(j);
                NodeType type = (currentLevel == levels - 1) ? NodeType::FILE : NodeType::FOLDER;
                
                try {
                    auto newNode = make_shared<TreeNode>(nextId++, name, type);
                    current->addChild(newNode);
                    
                    if (currentLevel < levels - 1) {
                        q.push(newNode);
                    }
                } catch (const exception& e) {
                    // Ignorar errores en generación
                }
            }
        }
        
        currentLevel++;
    }
    
    cout << "Árbol generado. Total de nodos: " << calculateSize() << endl;
}

// Medir tiempo de recorrido
double FileSystemTree::measureTraversalTime() {
    auto start = high_resolution_clock::now();
    
    // Realizar recorrido preorden
    auto traversal = preorderTraversal();
    
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    
    return elapsed.count();
}

// Medir tiempo de búsqueda
double FileSystemTree::measureSearchTime(const string& query) {
    auto start = high_resolution_clock::now();
    
    // Realizar búsqueda básica
    vector<shared_ptr<TreeNode>> results;
    queue<shared_ptr<TreeNode>> q;
    q.push(root);
    
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        
        if (current->name.find(query) != string::npos) {
            results.push_back(current);
        }
        
        for (auto& child : current->children) {
            q.push(child);
        }
    }
    
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    
    return elapsed.count();
}

// Validar estructura del árbol
bool FileSystemTree::validateTreeStructure() {
    cout << "=== VALIDACIÓN DE ESTRUCTURA DEL ÁRBOL ===" << endl;
    
    bool isValid = true;
    
    // 1. Verificar que la raíz no tenga padre
    if (root->parent.lock()) {
        cout << "ERROR: La raíz tiene un padre asignado." << endl;
        isValid = false;
    }
    
    // 2. Verificar consistencia padre-hijo
    stack<shared_ptr<TreeNode>> s;
    s.push(root);
    
    int inconsistencyCount = 0;
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        
        for (auto& child : current->children) {
            // Verificar que el padre del hijo sea correcto
            auto parent = child->parent.lock();
            if (parent != current) {
                cout << "ERROR: Inconsistencia padre-hijo en nodo '" 
                     << child->name << "' (ID: " << child->id << ")" << endl;
                inconsistencyCount++;
                isValid = false;
            }
            s.push(child);
        }
    }
    
    if (inconsistencyCount == 0) {
        cout << "✓ Relaciones padre-hijo consistentes." << endl;
    }
    
    // 3. Verificar IDs únicos
    unordered_set<int> ids;
    s.push(root);
    int duplicateCount = 0;
    
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        
        if (ids.find(current->id) != ids.end()) {
            cout << "ERROR: ID duplicado encontrado: " << current->id 
                 << " (Nodo: " << current->name << ")" << endl;
            duplicateCount++;
            isValid = false;
        }
        ids.insert(current->id);
        
        for (auto& child : current->children) {
            s.push(child);
        }
    }
    
    if (duplicateCount == 0) {
        cout << "✓ Todos los IDs son únicos." << endl;
    }
    
    // 4. Verificar nombres duplicados en mismo nivel
    s.push(root);
    int duplicateNameCount = 0;
    
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        
        unordered_set<string> names;
        for (auto& child : current->children) {
            if (names.find(child->name) != names.end()) {
                cout << "ADVERTENCIA: Nombre duplicado '" << child->name 
                     << "' en hijos de '" << current->name << "'" << endl;
                duplicateNameCount++;
                // No marcamos como error porque técnicamente es permitido
            }
            names.insert(child->name);
            s.push(child);
        }
    }
    
    if (duplicateNameCount == 0) {
        cout << "✓ No hay nombres duplicados en mismo nivel." << endl;
    }
    
    cout << "===========================================" << endl;
    return isValid;
}

// Encontrar nodos huérfanos
vector<string> FileSystemTree::findOrphanNodes() {
    vector<string> orphans;
    unordered_set<shared_ptr<TreeNode>> visited;
    
    // Recorrer todo el árbol marcando nodos visitados
    stack<shared_ptr<TreeNode>> s;
    s.push(root);
    
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        
        visited.insert(current);
        
        for (auto& child : current->children) {
            s.push(child);
        }
    }
    
    // Buscar en todos los nodos creados (simulado)
    // En un sistema real, tendríamos un registro de todos los nodos
    
    return orphans;
}

// Encontrar ciclos
vector<string> FileSystemTree::findCycles() {
    vector<string> cycles;
    
    // Usar DFS para detectar ciclos
    unordered_set<int> visited;
    unordered_set<int> recursionStack;
    stack<pair<shared_ptr<TreeNode>, vector<string>>> s;
    
    s.push({root, {}});
    
    while (!s.empty()) {
        auto [current, path] = s.top();
        s.pop();
        
        if (recursionStack.find(current->id) != recursionStack.end()) {
            // Ciclo detectado
            string cyclePath = "Ciclo encontrado: ";
            for (const auto& nodeName : path) {
                cyclePath += nodeName + " -> ";
            }
            cyclePath += current->name;
            cycles.push_back(cyclePath);
            continue;
        }
        
        if (visited.find(current->id) != visited.end()) {
            continue;
        }
        
        visited.insert(current->id);
        recursionStack.insert(current->id);
        
        vector<string> newPath = path;
        newPath.push_back(current->name);
        
        for (auto& child : current->children) {
            s.push({child, newPath});
        }
        
        recursionStack.erase(current->id);
    }
    
    return cycles;
}

// Imprimir estadísticas del árbol
void FileSystemTree::printTreeStats() {
    TreeStats stats = getTreeStats();
    
    cout << "\n=== ESTADÍSTICAS DEL ÁRBOL ===" << endl;
    cout << "Nodos totales: " << stats.totalNodes << endl;
    cout << "Carpetas: " << stats.folderCount << endl;
    cout << "Archivos: " << stats.fileCount << endl;
    cout << "Altura del árbol: " << stats.treeHeight << endl;
    cout << "Profundidad máxima: " << stats.maxDepth << endl;
    cout << "Profundidad mínima (hojas): " << stats.minDepth << endl;
    cout << "Profundidad promedio (hojas): " << stats.avgDepth << endl;
    
    // Calcular factor de ramificación promedio
    if (stats.folderCount > 0) {
        double avgBranching = static_cast<double>(stats.totalNodes - 1) / stats.folderCount;
        cout << "Factor de ramificación promedio: " << avgBranching << endl;
    }
    
    cout << "===============================\n" << endl;
}
void FileSystemTree::rebuildTree(shared_ptr<TreeNode> newRoot, int newNextId) {
    root = newRoot;
    nextId = newNextId;
}
// Método para limpiar el árbol
void FileSystemTree::clear() {
    root = make_shared<TreeNode>(0, "root", NodeType::FOLDER);
    nextId = 1;
}

// Método para establecer nueva raíz
void FileSystemTree::setRoot(shared_ptr<TreeNode> newRoot) {
    root = newRoot;
    
    // Encontrar el máximo ID para establecer nextId correctamente
    int maxId = 0;
    stack<shared_ptr<TreeNode>> s;
    s.push(root);
    
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        
        if (current->id > maxId) {
            maxId = current->id;
        }
        
        for (auto& child : current->children) {
            s.push(child);
        }
    }
    
    nextId = maxId + 1;
}
