#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Enumeración para tipos de nodo
enum class NodeType {
    FOLDER,
    FILE
};

// Estructura base de un nodo - hereda de enable_shared_from_this
struct TreeNode : public enable_shared_from_this<TreeNode> {
    int id;
    string name;
    NodeType type;
    string content;  // Solo para archivos
    vector<shared_ptr<TreeNode>> children;
    weak_ptr<TreeNode> parent;  // Referencia al padre
    
    // Constructor
    TreeNode(int nodeId, const string& nodeName, NodeType nodeType, 
             const string& nodeContent = "");
    
    // Método para verificar si es carpeta
    bool isFolder() const;
    
    // Método para verificar si es archivo
    bool isFile() const;
    
    // Método para agregar hijo
    void addChild(shared_ptr<TreeNode> child);
    
    // Método para encontrar hijo por nombre
    shared_ptr<TreeNode> findChild(const string& childName);
    
    // Método para eliminar hijo por nombre
    bool removeChild(const string& childName);
};

#endif // TREENODE_HPP