# Diseño de Estructuras

**Fecha:** 9 de diciembre del 2025
**Autor:** Luque Orozco Alan Giovanny

## 1. Enumeración NodeType
```cpp
enum class NodeType {
    FOLDER,
    FILE
};
 class Node {
    int id;
    std::string name;
    NodeType type;
    std::string content;
    std::vector<Node*> children;
    Node* parent;
};
 class Tree {
    Node* root;
    int nextId;
    std::unordered_map<int, Node*> nodesMap;
};
 class FileSystem {
    Tree fileTree;
    Node* currentDirectory;
};
