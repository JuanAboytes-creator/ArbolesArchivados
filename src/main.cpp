// main.cpp - Días 2-3: Implementación del Árbol General
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <sstream>
#include <queue>
#include <stack>

using namespace std;

// ====================
// ESTRUCTURAS DE DATOS
// ====================

// Enumeración para tipos de nodo
enum class NodeType {
    FOLDER,
    FILE
};

// Estructura base de un nodo
struct TreeNode : public enable_shared_from_this<TreeNode> {
    int id;
    string name;
    NodeType type;
    string content;  // Solo para archivos
    vector<shared_ptr<TreeNode>> children;
    weak_ptr<TreeNode> parent;  // Referencia al padre
    
    // Constructor
    TreeNode(int nodeId, const string& nodeName, NodeType nodeType, 
             const string& nodeContent = "")
        : id(nodeId), name(nodeName), type(nodeType), 
          content(nodeContent) {}
    
    // Método para verificar si es carpeta
    bool isFolder() const {
        return type == NodeType::FOLDER;
    }
    
    // Método para verificar si es archivo
    bool isFile() const {
        return type == NodeType::FILE;
    }
    
    // Método para agregar hijo
    void addChild(shared_ptr<TreeNode> child) {
        child->parent = weak_from_this();  // Usamos weak_from_this() porque heredamos de enable_shared_from_this
        children.push_back(child);
    }
    
    // Método para encontrar hijo por nombre
    shared_ptr<TreeNode> findChild(const string& childName) {
        for (auto& child : children) {
            if (child->name == childName) {
                return child;
            }
        }
        return nullptr;
    }
    
    // Método para eliminar hijo por nombre
    bool removeChild(const string& childName) {
        auto it = remove_if(children.begin(), children.end(),
            [&childName](const shared_ptr<TreeNode>& child) {
                return child->name == childName;
            });
        
        if (it != children.end()) {
            children.erase(it, children.end());
            return true;
        }
        return false;
    }
};

// Clase principal del árbol
class FileSystemTree {
private:
    shared_ptr<TreeNode> root;
    int nextId;
    
    // Helper: dividir ruta en componentes
    vector<string> splitPath(const string& path) {
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
    shared_ptr<TreeNode> findNodeByPath(const string& path) {
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
    shared_ptr<TreeNode> findNodeById(shared_ptr<TreeNode> node, int id) {
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
    bool nameExists(shared_ptr<TreeNode> parent, const string& name) {
        for (auto& child : parent->children) {
            if (child->name == name) {
                return true;
            }
        }
        return false;
    }

public:
    FileSystemTree() : nextId(1) {
        // Crear nodo raíz
        root = make_shared<TreeNode>(0, "root", NodeType::FOLDER);
    }
    
    // ====================
    // OPERACIONES BÁSICAS
    // ====================
    
    // 1. Crear nodo en ruta específica
    shared_ptr<TreeNode> createNode(const string& path, const string& name, 
                                   NodeType type, const string& content = "") {
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
    bool moveNode(const string& sourcePath, const string& destPath) {
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
    bool renameNode(const string& path, const string& newName) {
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
    bool deleteNode(const string& path) {
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
    vector<string> listChildren(const string& path) {
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
    string getFullPath(shared_ptr<TreeNode> node) {
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
    vector<string> preorderTraversal() {
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
    int calculateHeight() {
        return calculateHeightRecursive(root) - 1; // Restar 1 porque la raíz tiene altura 0
    }
    
    int calculateHeightRecursive(shared_ptr<TreeNode> node) {
        if (!node) return 0;
        
        int maxHeight = 0;
        for (auto& child : node->children) {
            int childHeight = calculateHeightRecursive(child);
            maxHeight = max(maxHeight, childHeight);
        }
        
        return maxHeight + 1;
    }
    
    // 9. Calcular tamaño (número de nodos)
    int calculateSize() {
        return calculateSizeRecursive(root);
    }
    
    int calculateSizeRecursive(shared_ptr<TreeNode> node) {
        if (!node) return 0;
        
        int size = 1; // Contar el nodo actual
        for (auto& child : node->children) {
            size += calculateSizeRecursive(child);
        }
        
        return size;
    }
    
    // 10. Buscar nodo por ID
    shared_ptr<TreeNode> findNodeById(int id) {
        return findNodeById(root, id);
    }
    
    // 11. Verificar consistencia del árbol
    bool verifyTreeConsistency() {
        return verifyConsistencyRecursive(root, nullptr);
    }
    
    bool verifyConsistencyRecursive(shared_ptr<TreeNode> node, shared_ptr<TreeNode> expectedParent) {
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
    
    // ====================
    // GETTERS Y UTILIDADES
    // ====================
    
    shared_ptr<TreeNode> getRoot() const { return root; }
    int getNextId() const { return nextId; }
    
    // Método para imprimir árbol
    void printTree() {
        cout << "\n=== ESTRUCTURA DEL ÁRBOL ===" << endl;
        printTreeRecursive(root, 0);
    }
    
private:
    void printTreeRecursive(shared_ptr<TreeNode> node, int depth) {
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
};

// ====================
// PRUEBAS UNITARIAS
// ====================

class FileSystemTreeTests {
public:
    static void runAllTests() {
        cout << "\n=== EJECUTANDO PRUEBAS UNITARIAS ===" << endl;
        
        testCreateNodes();
        testMoveNode();
        testRenameNode();
        testDeleteNode();
        testTreeTraversal();
        testTreeProperties();
        testErrorCases();
        
        cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS ===" << endl;
    }
    
private:
    static void testCreateNodes() {
        cout << "\n1. Prueba: Creación de nodos" << endl;
        
        FileSystemTree tree;
        
        try {
            // Crear carpetas
            tree.createNode("/root", "Documents", NodeType::FOLDER);
            tree.createNode("/root", "Images", NodeType::FOLDER);
            tree.createNode("/root/Documents", "Work", NodeType::FOLDER);
            
            // Crear archivos
            tree.createNode("/root/Documents", "notes.txt", NodeType::FILE, "Mis notas importantes");
            tree.createNode("/root/Documents/Work", "report.pdf", NodeType::FILE, "Reporte trimestral");
            tree.createNode("/root/Images", "photo.jpg", NodeType::FILE, "Foto de vacaciones");
            
            // Listar hijos de root
            auto rootChildren = tree.listChildren("/root");
            cout << "   Hijos de root: ";
            for (const auto& child : rootChildren) {
                cout << child << " | ";
            }
            cout << endl;
            
            // Verificar tamaño
            int size = tree.calculateSize();
            cout << "   Tamaño del árbol: " << size << " nodos" << endl;
            cout << "   ✓ Prueba de creación exitosa" << endl;
            
        } catch (const exception& e) {
            cout << "   ✗ Error: " << e.what() << endl;
        }
    }
    
    static void testMoveNode() {
        cout << "\n2. Prueba: Mover nodos" << endl;
        
        FileSystemTree tree;
        
        try {
            // Configurar árbol de prueba
            tree.createNode("/root", "Source", NodeType::FOLDER);
            tree.createNode("/root", "Dest", NodeType::FOLDER);
            tree.createNode("/root/Source", "file.txt", NodeType::FILE, "Contenido");
            
            cout << "   Antes de mover:" << endl;
            tree.printTree();
            
            // Mover archivo
            tree.moveNode("/root/Source/file.txt", "/root/Dest");
            
            cout << "   Después de mover:" << endl;
            tree.printTree();
            
            // Verificar consistencia
            if (tree.verifyTreeConsistency()) {
                cout << "   ✓ Árbol consistente después de mover" << endl;
            } else {
                cout << "   ✗ Árbol inconsistente" << endl;
            }
            
        } catch (const exception& e) {
            cout << "   ✗ Error: " << e.what() << endl;
        }
    }
    
    static void testRenameNode() {
        cout << "\n3. Prueba: Renombrar nodos" << endl;
        
        FileSystemTree tree;
        
        try {
            tree.createNode("/root", "oldname", NodeType::FOLDER);
            
            cout << "   Antes de renombrar: oldname" << endl;
            
            tree.renameNode("/root/oldname", "newname");
            
            auto children = tree.listChildren("/root");
            cout << "   Después de renombrar: ";
            for (const auto& child : children) {
                cout << child << " ";
            }
            cout << endl;
            
            cout << "   ✓ Renombrado exitoso" << endl;
            
        } catch (const exception& e) {
            cout << "   ✗ Error: " << e.what() << endl;
        }
    }
    
    static void testDeleteNode() {
        cout << "\n4. Prueba: Eliminar nodos" << endl;
        
        FileSystemTree tree;
        
        try {
            // Configurar árbol
            tree.createNode("/root", "toDelete", NodeType::FOLDER);
            tree.createNode("/root/toDelete", "child.txt", NodeType::FILE, "Hola");
            
            cout << "   Antes de eliminar:" << endl;
            tree.printTree();
            
            int initialSize = tree.calculateSize();
            
            // Eliminar nodo
            tree.deleteNode("/root/toDelete");
            
            cout << "   Después de eliminar:" << endl;
            tree.printTree();
            
            int finalSize = tree.calculateSize();
            
            if (finalSize == initialSize - 2) { // Se elimina carpeta + archivo hijo
                cout << "   ✓ Eliminación recursiva exitosa" << endl;
            } else {
                cout << "   ✗ Error en tamaño después de eliminar" << endl;
            }
            
        } catch (const exception& e) {
            cout << "   ✗ Error: " << e.what() << endl;
        }
    }
    
    static void testTreeTraversal() {
        cout << "\n5. Prueba: Recorrido en preorden" << endl;
        
        FileSystemTree tree;
        
        try {
            // Crear estructura de prueba
            tree.createNode("/root", "A", NodeType::FOLDER);
            tree.createNode("/root/A", "B", NodeType::FOLDER);
            tree.createNode("/root/A/B", "C.txt", NodeType::FILE, "Contenido C");
            tree.createNode("/root/A", "D.txt", NodeType::FILE, "Contenido D");
            tree.createNode("/root", "E", NodeType::FOLDER);
            
            auto traversal = tree.preorderTraversal();
            
            cout << "   Recorrido preorden:" << endl;
            for (const auto& node : traversal) {
                cout << "   - " << node << endl;
            }
            
            cout << "   ✓ Recorrido generado correctamente" << endl;
            
        } catch (const exception& e) {
            cout << "   ✗ Error: " << e.what() << endl;
        }
    }
    
    static void testTreeProperties() {
        cout << "\n6. Prueba: Propiedades del árbol" << endl;
        
        FileSystemTree tree;
        
        try {
            // Crear árbol profundo
            tree.createNode("/root", "level1", NodeType::FOLDER);
            tree.createNode("/root/level1", "level2", NodeType::FOLDER);
            tree.createNode("/root/level1/level2", "level3", NodeType::FOLDER);
            
            int height = tree.calculateHeight();
            int size = tree.calculateSize();
            
            cout << "   Altura del árbol: " << height << endl;
            cout << "   Tamaño del árbol: " << size << " nodos" << endl;
            
            if (height == 3 && size == 4) { // root + level1 + level2 + level3
                cout << "   ✓ Propiedades calculadas correctamente" << endl;
            } else {
                cout << "   ✗ Error en cálculo de propiedades" << endl;
            }
            
        } catch (const exception& e) {
            cout << "   ✗ Error: " << e.what() << endl;
        }
    }
    
    static void testErrorCases() {
        cout << "\n7. Prueba: Casos de error" << endl;
        
        FileSystemTree tree;
        
        // Test 1: Crear en ruta inexistente
        try {
            tree.createNode("/root/nonexistent", "test", NodeType::FOLDER);
            cout << "   ✗ Debería haber lanzado error por ruta inexistente" << endl;
        } catch (const invalid_argument&) {
            cout << "   ✓ Correctamente rechazada creación en ruta inexistente" << endl;
        }
        
        // Test 2: Nombre duplicado
        try {
            tree.createNode("/root", "test", NodeType::FOLDER);
            tree.createNode("/root", "test", NodeType::FOLDER);
            cout << "   ✗ Debería haber lanzado error por nombre duplicado" << endl;
        } catch (const invalid_argument&) {
            cout << "   ✓ Correctamente rechazado nombre duplicado" << endl;
        }
        
        // Test 3: Mover a no-carpeta
        try {
            tree.createNode("/root", "file.txt", NodeType::FILE, "test");
            tree.createNode("/root", "dest", NodeType::FOLDER);
            tree.moveNode("/root/file.txt", "/root/dest/file.txt");
            cout << "   ✗ Debería haber lanzado error por destino no-carpeta" << endl;
        } catch (const invalid_argument&) {
            cout << "   ✓ Correctamente rechazado mover a no-carpeta" << endl;
        }
    }
};

// ====================
// FUNCIÓN PRINCIPAL
// ====================

int main() {
    cout << "=== DÍAS 2-3: IMPLEMENTACIÓN DEL ÁRBOL GENERAL ===" << endl;
    cout << "Implementando operaciones básicas y pruebas unitarias" << endl;
    
    // Ejecutar pruebas unitarias
    FileSystemTreeTests::runAllTests();
    
    // Demostración interactiva
    cout << "\n=== DEMOSTRACIÓN INTERACTIVA ===" << endl;
    
    FileSystemTree demoTree;
    
    try {
        // Crear estructura de ejemplo
        demoTree.createNode("/root", "Documents", NodeType::FOLDER);
        demoTree.createNode("/root", "Pictures", NodeType::FOLDER);
        demoTree.createNode("/root/Documents", "Work", NodeType::FOLDER);
        demoTree.createNode("/root/Documents", "Personal", NodeType::FOLDER);
        demoTree.createNode("/root/Documents/Work", "report.docx", NodeType::FILE, "Informe final");
        demoTree.createNode("/root/Documents/Personal", "diary.txt", NodeType::FILE, "Entrada 1 de enero");
        demoTree.createNode("/root/Pictures", "vacation.jpg", NodeType::FILE, "Foto de playa");
        
        // Mostrar estructura
        demoTree.printTree();
        
        // Mostrar propiedades
        cout << "\nPropiedades del árbol:" << endl;
        cout << "- Altura: " << demoTree.calculateHeight() << endl;
        cout << "- Tamaño: " << demoTree.calculateSize() << " nodos" << endl;
        cout << "- Próximo ID disponible: " << demoTree.getNextId() << endl;
        
        // Demostrar recorrido
        cout << "\nRecorrido en preorden:" << endl;
        auto traversal = demoTree.preorderTraversal();
        for (const auto& node : traversal) {
            cout << "- " << node << endl;
        }
        
        // Demostrar mover y renombrar
        cout << "\n--- Operaciones adicionales ---" << endl;
        demoTree.moveNode("/root/Documents/Personal/diary.txt", "/root/Documents/Work");
        demoTree.renameNode("/root/Pictures/vacation.jpg", "beach_photo.jpg");
        
        cout << "\nEstructura después de operaciones:" << endl;
        demoTree.printTree();
        
        // Verificar consistencia
        if (demoTree.verifyTreeConsistency()) {
            cout << "\n✓ El árbol mantiene consistencia después de todas las operaciones" << endl;
        } else {
            cout << "\n✗ El árbol tiene problemas de consistencia" << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error en demostración: " << e.what() << endl;
    }
    
    cout << "\n=== IMPLEMENTACIÓN COMPLETADA ===" << endl;
    cout << "Operaciones implementadas:" << endl;
    cout << "1. createNode() - Crear nodos (carpetas/archivos)" << endl;
    cout << "2. moveNode() - Mover nodos entre ubicaciones" << endl;
    cout << "3. renameNode() - Renombrar nodos" << endl;
    cout << "4. deleteNode() - Eliminar nodos recursivamente" << endl;
    cout << "5. listChildren() - Listar hijos de un nodo" << endl;
    cout << "6. getFullPath() - Obtener ruta completa" << endl;
    cout << "7. preorderTraversal() - Recorrido en preorden" << endl;
    cout << "8. calculateHeight() - Calcular altura del árbol" << endl;
    cout << "9. calculateSize() - Calcular número de nodos" << endl;
    cout << "10. verifyTreeConsistency() - Verificar consistencia" << endl;
    
    return 0;
}
