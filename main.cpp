// main.cpp - Día 1: Definición de estructuras y MVP
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

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
struct TreeNode {
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
};

// Clase principal del árbol
class FileSystemTree {
private:
    shared_ptr<TreeNode> root;
    int nextId;
    
public:
    FileSystemTree() : nextId(1) {
        // Crear nodo raíz
        root = make_shared<TreeNode>(0, "root", NodeType::FOLDER);
    }
    
    // Métodos a implementar (declaraciones)
    shared_ptr<TreeNode> createNode(const string& path, const string& name, 
                                    NodeType type, const string& content = "");
    bool moveNode(const string& sourcePath, const string& destPath);
    bool renameNode(const string& path, const string& newName);
    bool deleteNode(const string& path);
    vector<string> listChildren(const string& path);
    string getFullPath(shared_ptr<TreeNode> node);
    vector<string> preorderTraversal();
    
    // Persistencia
    bool saveToJSON(const string& filename);
    bool loadFromJSON(const string& filename);
    
    // Getters
    shared_ptr<TreeNode> getRoot() const { return root; }
    int getNextId() const { return nextId; }
};

// Estructura para el Trie (autocompletado)
struct TrieNode {
    map<char, shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    vector<int> nodeIds;  // IDs de nodos que terminan con este prefijo
    
    TrieNode() : isEndOfWord(false) {}
};

// Clase para el Trie
class Trie {
private:
    shared_ptr<TrieNode> root;
    
public:
    Trie() : root(make_shared<TrieNode>()) {}
    
    // Métodos a implementar
    void insert(const string& word, int nodeId);
    vector<int> search(const string& prefix);
    vector<string> autocomplete(const string& prefix);
};

// ====================
// ESQUELETO DE LA INTERFAZ DE CONSOLA
// ====================

class ConsoleInterface {
private:
    FileSystemTree fileSystem;
    Trie searchIndex;
    string currentPath;
    
public:
    ConsoleInterface() : currentPath("/root") {}
    
    void run() {
        cout << "=== SISTEMA DE ARCHIVOS JERÁRQUICO ===" << endl;
        cout << "Comandos disponibles:" << endl;
        cout << "  mkdir, touch, ls, mv, rename, rm" << endl;
        cout << "  search, autocomplete, export_preorder" << endl;
        cout << "  save, load, exit" << endl;
        
        // Bucle principal (a implementar en días posteriores)
        // while (true) {
        //     string command;
        //     cout << currentPath << "> ";
        //     getline(cin, command);
        //     processCommand(command);
        // }
    }
    
    void processCommand(const string& command) {
        // Implementación pendiente
    }
};

// ====================
// FUNCIÓN PRINCIPAL
// ====================

int main() {
    cout << "=== DÍA 1: DEFINICIÓN DE ESTRUCTURAS Y MVP ===" << endl;
    cout << endl;
    
    // Demostración básica de las estructuras
    FileSystemTree fs;
    cout << "Árbol del sistema de archivos inicializado." << endl;
    cout << "ID raíz: " << fs.getRoot()->id << endl;
    cout << "Nombre raíz: " << fs.getRoot()->name << endl;
    cout << "Próximo ID disponible: " << fs.getNextId() << endl;
    
    cout << endl;
    cout << "Estructuras definidas correctamente:" << endl;
    cout << "1. TreeNode - Nodo del árbol" << endl;
    cout << "2. FileSystemTree - Árbol principal" << endl;
    cout << "3. TrieNode/Trie - Para autocompletado" << endl;
    cout << "4. ConsoleInterface - Interfaz de usuario" << endl;
    
    cout << endl;
    cout << "Formato JSON definido para persistencia." << endl;
    cout << "MVP con 6 categorías de funcionalidades establecido." << endl;
    
    return 0;
}