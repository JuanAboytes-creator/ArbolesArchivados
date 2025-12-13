#include "FileSystemTree.hpp"
#include <sstream>
#include <algorithm>
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

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