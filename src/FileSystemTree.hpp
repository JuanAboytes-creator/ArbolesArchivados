#ifndef FILESYSTEMTREE_HPP
#define FILESYSTEMTREE_HPP

#include "TreeNode.hpp"
#include <vector>
#include <string>
#include <memory>

using namespace std;

class FileSystemTree {
private:
    shared_ptr<TreeNode> root;
    int nextId;
    
    // Helper: dividir ruta en componentes
    vector<string> splitPath(const string& path);
    
    // Helper: encontrar nodo por ID (recursivo)
    shared_ptr<TreeNode> findNodeById(shared_ptr<TreeNode> node, int id);
    
    // Helper: verificar si nombre ya existe en hijos
    bool nameExists(shared_ptr<TreeNode> parent, const string& name);
    
    // Helper recursivo para altura
    int calculateHeightRecursive(shared_ptr<TreeNode> node);
    
    // Helper recursivo para tamaño
    int calculateSizeRecursive(shared_ptr<TreeNode> node);
    
    // Helper recursivo para consistencia
    bool verifyConsistencyRecursive(shared_ptr<TreeNode> node, shared_ptr<TreeNode> expectedParent);
    
    // Helper para imprimir árbol
    void printTreeRecursive(shared_ptr<TreeNode> node, int depth);

public:
    FileSystemTree();
    
    // Operaciones básicas
    shared_ptr<TreeNode> createNode(const string& path, const string& name, 
                                   NodeType type, const string& content = "");
    bool moveNode(const string& sourcePath, const string& destPath);
    bool renameNode(const string& path, const string& newName);
    bool deleteNode(const string& path);
    vector<string> listChildren(const string& path);
    string getFullPath(shared_ptr<TreeNode> node);
    vector<string> preorderTraversal();
    
    // Búsqueda de nodos
    shared_ptr<TreeNode> findNodeByPath(const string& path);  // MOVIDO A PÚBLICO
    
    // Propiedades del árbol
    int calculateHeight();
    int calculateSize();
    shared_ptr<TreeNode> findNodeById(int id);
    bool verifyTreeConsistency();
    
    // Getters
    shared_ptr<TreeNode> getRoot() const;
    int getNextId() const;
    
    // Utilidades
    void printTree();

    bool isNodeInTree(std::shared_ptr<TreeNode> node);
};

#endif // FILESYSTEMTREE_HPP