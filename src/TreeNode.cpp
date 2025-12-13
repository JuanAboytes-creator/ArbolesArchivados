#include "TreeNode.hpp"
#include <algorithm>

// Constructor
TreeNode::TreeNode(int nodeId, const string& nodeName, NodeType nodeType, 
                   const string& nodeContent)
    : id(nodeId), name(nodeName), type(nodeType), content(nodeContent) {}

// Método para verificar si es carpeta
bool TreeNode::isFolder() const {
    return type == NodeType::FOLDER;
}

// Método para verificar si es archivo
bool TreeNode::isFile() const {
    return type == NodeType::FILE;
}

// Método para agregar hijo
void TreeNode::addChild(shared_ptr<TreeNode> child) {
    child->parent = weak_ptr<TreeNode>(shared_from_this());
    children.push_back(child);
}

// Método para encontrar hijo por nombre
shared_ptr<TreeNode> TreeNode::findChild(const string& childName) {
    for (auto& child : children) {
        if (child->name == childName) {
            return child;
        }
    }
    return nullptr;
}

// Método para eliminar hijo por nombre
bool TreeNode::removeChild(const string& childName) {
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