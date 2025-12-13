#ifndef SEARCHENGINE_HPP
#define SEARCHENGINE_HPP

#include "FileSystemTree.hpp"
#include "Trie.hpp"
#include <memory>
#include <vector>
#include <string>

using namespace std;

class SearchEngine {
private:
    shared_ptr<FileSystemTree> fileSystem;
    Trie nameIndex;
    
    // Helper para indexar recursivamente
    void indexNodeRecursive(shared_ptr<TreeNode> node);
    
    // Helper para desindexar recursivamente
    void removeIndexRecursive(shared_ptr<TreeNode> node);
    
    // Helper para verificar integridad recursivamente
    bool verifyIntegrityRecursive(shared_ptr<TreeNode> node);

public:
    SearchEngine(shared_ptr<FileSystemTree> fs);
    
    // 1. Reconstruir índice completo
    void rebuildIndex();
    
    // 2. Indexar un nodo específico (y sus hijos)
    void indexNode(shared_ptr<TreeNode> node);
    
    // 3. Remover nodo del índice
    void removeNodeFromIndex(shared_ptr<TreeNode> node);
    
    // 4. Búsqueda exacta por nombre
    vector<shared_ptr<TreeNode>> searchExact(const string& name);
    
    // 5. Búsqueda por prefijo
    vector<shared_ptr<TreeNode>> searchByPrefix(const string& prefix);
    
    // 6. Autocompletado
    vector<string> autocomplete(const string& prefix, int maxSuggestions = 5);
    
    // 7. Búsqueda avanzada: nombres que contienen un substring
    vector<shared_ptr<TreeNode>> searchContains(const string& substring);
    
    // 8. Estadísticas del índice
    void printIndexStats();
    
    // 9. Verificar integridad del índice
    bool verifyIndexIntegrity();
};

#endif // SEARCHENGINE_HPP