#ifndef TRIE_HPP
#define TRIE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>

using namespace std;

// Nodo del Trie para búsqueda por prefijo
struct TrieNode {
    map<char, shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    vector<int> nodeIds;  // IDs de nodos que terminan con este prefijo
    
    TrieNode() : isEndOfWord(false) {}
    
    // Método para verificar si tiene un carácter específico como hijo
    bool hasChild(char c) const {
        return children.find(c) != children.end();
    }
    
    // Método para obtener hijo
    shared_ptr<TrieNode> getChild(char c) {
        auto it = children.find(c);
        return it != children.end() ? it->second : nullptr;
    }
};

// Clase Trie para autocompletado de nombres
class Trie {
private:
    shared_ptr<TrieNode> root;
    
    // Helper para búsqueda recursiva de todos los nodos
    void collectAllWords(shared_ptr<TrieNode> node, string currentPrefix, 
                         vector<string>& results) {
        if (node->isEndOfWord) {
            // Agregar todas las palabras que terminan aquí
            for (int i = 0; i < node->nodeIds.size(); i++) {
                results.push_back(currentPrefix);
            }
        }
        
        for (const auto& pair : node->children) {
            collectAllWords(pair.second, currentPrefix + pair.first, results);
        }
    }
    
    // Helper para búsqueda recursiva de prefijo
    void findWordsWithPrefix(shared_ptr<TrieNode> node, string currentPrefix, 
                            vector<string>& results) {
        if (node->isEndOfWord) {
            // Solo agregar una vez cada palabra única
            if (!results.empty() && results.back() != currentPrefix) {
                results.push_back(currentPrefix);
            } else if (results.empty()) {
                results.push_back(currentPrefix);
            }
        }
        
        for (const auto& pair : node->children) {
            findWordsWithPrefix(pair.second, currentPrefix + pair.first, results);
        }
    }

public:
    Trie() : root(make_shared<TrieNode>()) {}
    
    // 1. Insertar una palabra con su ID de nodo
    void insert(const string& word, int nodeId) {
        shared_ptr<TrieNode> current = root;
        
        for (char c : word) {
            if (!current->hasChild(c)) {
                current->children[c] = make_shared<TrieNode>();
            }
            current = current->getChild(c);
        }
        
        current->isEndOfWord = true;
        // Agregar el ID del nodo a la lista
        if (find(current->nodeIds.begin(), current->nodeIds.end(), nodeId) == current->nodeIds.end()) {
            current->nodeIds.push_back(nodeId);
        }
    }
    
    // 2. Buscar una palabra exacta
    vector<int> searchExact(const string& word) {
        shared_ptr<TrieNode> current = root;
        
        for (char c : word) {
            if (!current->hasChild(c)) {
                return vector<int>();  // Palabra no encontrada
            }
            current = current->getChild(c);
        }
        
        if (current->isEndOfWord) {
            return current->nodeIds;
        }
        
        return vector<int>();  // Prefijo encontrado pero no palabra completa
    }
    
    // 3. Buscar por prefijo
    vector<string> searchByPrefix(const string& prefix) {
        vector<string> results;
        
        // Primero navegar hasta el nodo del prefijo
        shared_ptr<TrieNode> current = root;
        for (char c : prefix) {
            if (!current->hasChild(c)) {
                return results;  // Prefijo no encontrado
            }
            current = current->getChild(c);
        }
        
        // Ahora recolectar todas las palabras desde este nodo
        findWordsWithPrefix(current, prefix, results);
        
        return results;
    }
    
    // 4. Autocompletado: sugerencias para un prefijo
    vector<string> autocomplete(const string& prefix, int maxSuggestions = 5) {
        vector<string> suggestions;
        
        // Navegar hasta el nodo del prefijo
        shared_ptr<TrieNode> current = root;
        for (char c : prefix) {
            if (!current->hasChild(c)) {
                return suggestions;  // Prefijo no encontrado
            }
            current = current->getChild(c);
        }
        
        // Recolectar sugerencias
        vector<string> allWords;
        findWordsWithPrefix(current, prefix, allWords);
        
        // Limitar el número de sugerencias
        int count = min(static_cast<int>(allWords.size()), maxSuggestions);
        suggestions.insert(suggestions.end(), allWords.begin(), allWords.begin() + count);
        
        return suggestions;
    }
    
    // 5. Eliminar una palabra
    bool remove(const string& word, int nodeId) {
        return removeRecursive(root, word, 0, nodeId);
    }
    
    bool removeRecursive(shared_ptr<TrieNode> current, const string& word, 
                        int index, int nodeId) {
        if (index == word.length()) {
            if (!current->isEndOfWord) {
                return false;
            }
            
            // Eliminar el ID específico de la lista
            auto it = find(current->nodeIds.begin(), current->nodeIds.end(), nodeId);
            if (it != current->nodeIds.end()) {
                current->nodeIds.erase(it);
                
                // Si no hay más IDs, marcar como no fin de palabra
                if (current->nodeIds.empty()) {
                    current->isEndOfWord = false;
                }
                return true;
            }
            return false;
        }
        
        char c = word[index];
        shared_ptr<TrieNode> child = current->getChild(c);
        if (!child) {
            return false;
        }
        
        bool shouldDeleteChild = removeRecursive(child, word, index + 1, nodeId);
        
        // Si el hijo debe ser eliminado y no tiene hijos ni es fin de palabra
        if (shouldDeleteChild && !child->isEndOfWord && child->children.empty()) {
            current->children.erase(c);
        }
        
        return false;  // No eliminar nodos intermedios
    }
    
    // 6. Obtener todas las palabras en el Trie
    vector<string> getAllWords() {
        vector<string> words;
        collectAllWords(root, "", words);
        return words;
    }
    
    // 7. Verificar si un prefijo existe
    bool startsWith(const string& prefix) {
        shared_ptr<TrieNode> current = root;
        
        for (char c : prefix) {
            if (!current->hasChild(c)) {
                return false;
            }
            current = current->getChild(c);
        }
        
        return true;
    }
    
    // 8. Contar palabras en el Trie
    int countWords() {
        return countWordsRecursive(root);
    }
    
    int countWordsRecursive(shared_ptr<TrieNode> node) {
        int count = node->isEndOfWord ? 1 : 0;
        
        for (const auto& pair : node->children) {
            count += countWordsRecursive(pair.second);
        }
        
        return count;
    }
};

#endif // TRIE_HPP