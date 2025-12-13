#include "SearchEngine.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

// Helper recursivo para indexar
void SearchEngine::indexNodeRecursive(shared_ptr<TreeNode> node) {
    if (!node) return;
    
    // Indexar el nombre del nodo actual
    nameIndex.insert(node->name, node->id);
    
    // Indexar hijos recursivamente
    for (auto& child : node->children) {
        indexNodeRecursive(child);
    }
}

// Helper recursivo para desindexar
void SearchEngine::removeIndexRecursive(shared_ptr<TreeNode> node) {
    if (!node) return;
    
    // Remover el nombre del nodo actual
    nameIndex.remove(node->name, node->id);
    
    // Remover hijos recursivamente
    for (auto& child : node->children) {
        removeIndexRecursive(child);
    }
}

SearchEngine::SearchEngine(shared_ptr<FileSystemTree> fs) : fileSystem(fs) {
    // Indexar árbol existente
    rebuildIndex();
}

// 1. Reconstruir índice completo
void SearchEngine::rebuildIndex() {
    nameIndex = Trie();  // Reiniciar índice
    indexNodeRecursive(fileSystem->getRoot());
}

// 2. Indexar un nodo específico (y sus hijos)
void SearchEngine::indexNode(shared_ptr<TreeNode> node) {
    if (node) {
        indexNodeRecursive(node);
    }
}

// 3. Remover nodo del índice
void SearchEngine::removeNodeFromIndex(shared_ptr<TreeNode> node) {
    if (node) {
        removeIndexRecursive(node);
    }
}

// 4. Búsqueda exacta por nombre
vector<shared_ptr<TreeNode>> SearchEngine::searchExact(const string& name) {
    vector<shared_ptr<TreeNode>> results;
    vector<int> nodeIds = nameIndex.searchExact(name);
    
    for (int id : nodeIds) {
        auto node = fileSystem->findNodeById(id);
        if (node) {
            results.push_back(node);
        }
    }
    
    return results;
}

// 5. Búsqueda por prefijo
vector<shared_ptr<TreeNode>> SearchEngine::searchByPrefix(const string& prefix) {
    vector<shared_ptr<TreeNode>> results;
    vector<string> names = nameIndex.searchByPrefix(prefix);
    
    for (const string& name : names) {
        vector<int> nodeIds = nameIndex.searchExact(name);
        for (int id : nodeIds) {
            auto node = fileSystem->findNodeById(id);
            if (node) {
                results.push_back(node);
            }
        }
    }
    
    return results;
}

// 6. Autocompletado
vector<string> SearchEngine::autocomplete(const string& prefix, int maxSuggestions) {
    return nameIndex.autocomplete(prefix, maxSuggestions);
}

// 7. Búsqueda avanzada: nombres que contienen un substring
vector<shared_ptr<TreeNode>> SearchEngine::searchContains(const string& substring) {
    vector<shared_ptr<TreeNode>> results;
    vector<string> allNames = nameIndex.getAllWords();
    
    for (const string& name : allNames) {
        if (name.find(substring) != string::npos) {
            vector<int> nodeIds = nameIndex.searchExact(name);
            for (int id : nodeIds) {
                auto node = fileSystem->findNodeById(id);
                if (node) {
                    results.push_back(node);
                }
            }
        }
    }
    
    return results;
}

// 8. Estadísticas del índice
void SearchEngine::printIndexStats() {
    cout << "=== ESTADÍSTICAS DEL ÍNDICE ===" << endl;
    cout << "Palabras indexadas: " << nameIndex.countWords() << endl;
    cout << "Nodos en el sistema: " << fileSystem->calculateSize() << endl;
    
    // Mostrar algunas palabras indexadas
    vector<string> sampleWords = nameIndex.getAllWords();
    int sampleSize = min(10, static_cast<int>(sampleWords.size()));
    
    if (sampleSize > 0) {
        cout << "\nMuestra de palabras indexadas (" << sampleSize << " de " << sampleWords.size() << "):" << endl;
        for (int i = 0; i < sampleSize; i++) {
            cout << "  " << sampleWords[i] << endl;
        }
    }
}

// 9. Verificar integridad del índice
bool SearchEngine::verifyIndexIntegrity() {
    // Verificar que todos los nodos del árbol están indexados
    return verifyIntegrityRecursive(fileSystem->getRoot());
}

bool SearchEngine::verifyIntegrityRecursive(shared_ptr<TreeNode> node) {
    if (!node) return true;
    
    // Verificar que el nodo está indexado
    vector<int> ids = nameIndex.searchExact(node->name);
    if (find(ids.begin(), ids.end(), node->id) == ids.end()) {
        return false;
    }
    
    // Verificar hijos recursivamente
    for (auto& child : node->children) {
        if (!verifyIntegrityRecursive(child)) {
            return false;
        }
    }
    
    return true;
}