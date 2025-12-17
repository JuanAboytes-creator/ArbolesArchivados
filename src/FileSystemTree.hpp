#ifndef FILESYSTEMTREE_HPP
#define FILESYSTEMTREE_HPP

#include "TreeNode.hpp"
#include <vector>
#include <string>
#include <memory>
#include <chrono>

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
    
    // Helper para estadísticas
    void collectStatsRecursive(shared_ptr<TreeNode> node, int depth, 
                              int& folderCount, int& fileCount, int& maxDepth, int& totalNodes);

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
    shared_ptr<TreeNode> findNodeByPath(const string& path);
    shared_ptr<TreeNode> findNodeById(int id);
    
    // Verificar si nodo está en árbol
    bool isNodeInTree(shared_ptr<TreeNode> node);
    
    // Propiedades del árbol
    int calculateHeight();
    int calculateSize();
    bool verifyTreeConsistency();
    
    // Estadísticas avanzadas
    struct TreeStats {
        int totalNodes;
        int folderCount;
        int fileCount;
        int maxDepth;
        int minDepth;
        double avgDepth;
        int treeHeight;
    };
    
    TreeStats getTreeStats();
    
    // Pruebas de rendimiento
    void generateLargeTree(int levels, int childrenPerLevel);
    double measureTraversalTime();
    double measureSearchTime(const string& query);
    
    // Casos límite y validación
    bool validateTreeStructure();
    vector<string> findOrphanNodes();
    vector<string> findCycles();
    
    // Getters
    shared_ptr<TreeNode> getRoot() const;
    int getNextId() const;
    
    // Utilidades
    void printTree();
    void printTreeStats();
    // Método para reconstruir el árbol desde una raíz
    void rebuildTree(shared_ptr<TreeNode> newRoot, int newNextId);

    // Método para limpiar el árbol
void clear();
// Método para establecer nueva raíz
void setRoot(shared_ptr<TreeNode> newRoot);
};

#endif // FILESYSTEMTREE_HPP