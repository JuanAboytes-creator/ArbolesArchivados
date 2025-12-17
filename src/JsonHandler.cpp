#include "JsonHandler.hpp"
#include "FileSystemTree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

bool JsonHandler::saveTree(shared_ptr<FileSystemTree> tree, const string& filename) {
    try {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: No se pudo abrir " << filename << " para escritura" << endl;
            return false;
        }
        
        // Función recursiva para convertir nodo a JSON
        function<json(shared_ptr<TreeNode>)> toJson = [&](shared_ptr<TreeNode> node) -> json {
            json j;
            j["id"] = node->id;
            j["name"] = node->name;
            j["type"] = node->isFolder() ? "FOLDER" : "FILE";
            
            if (node->isFile() && !node->content.empty()) {
                j["content"] = node->content;
            }
            
            if (!node->children.empty()) {
                json children = json::array();
                for (auto& child : node->children) {
                    children.push_back(toJson(child));
                }
                j["children"] = children;
            }
            
            return j;
        };
        
        // Crear objeto JSON completo
        json rootJson;
        
        // Metadatos
        json metadata;
        metadata["version"] = "1.0";
        metadata["type"] = "filesystem_tree";
        metadata["nextId"] = tree->getNextId();
        metadata["nodes"] = tree->calculateSize();
        metadata["height"] = tree->calculateHeight();
        
        // Árbol
        json treeJson = toJson(tree->getRoot());
        
        rootJson["metadata"] = metadata;
        rootJson["tree"] = treeJson;
        
        // Escribir al archivo
        file << rootJson.dump(2);
        file.close();
        
        cout << "Árbol guardado exitosamente en " << filename << endl;
        cout << "  Nodos guardados: " << tree->calculateSize() << endl;
        
        return true;
        
    } catch (const exception& e) {
        cerr << "Error al guardar: " << e.what() << endl;
        return false;
    }
}

bool JsonHandler::loadTree(shared_ptr<FileSystemTree> tree, const string& filename) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: No se pudo abrir " << filename << endl;
            return false;
        }
        
        // Leer todo el archivo
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        // Parsear JSON
        json rootJson = json::parse(buffer.str());
        
        // Verificar estructura
        if (!rootJson.contains("tree")) {
            cerr << "Error: Formato JSON inválido - falta 'tree'" << endl;
            return false;
        }
        
        // Función recursiva para construir árbol desde JSON
        function<shared_ptr<TreeNode>(const json&)> fromJson;
        fromJson = [&](const json& nodeJson) -> shared_ptr<TreeNode> {
            if (nodeJson.is_null()) return nullptr;
            
            int id = nodeJson["id"];
            string name = nodeJson["name"];
            string typeStr = nodeJson["type"];
            NodeType type = (typeStr == "FOLDER") ? NodeType::FOLDER : NodeType::FILE;
            
            string content = "";
            if (nodeJson.contains("content") && nodeJson["content"].is_string()) {
                content = nodeJson["content"];
            }
            
            auto node = make_shared<TreeNode>(id, name, type, content);
            
            // Procesar hijos
            if (nodeJson.contains("children") && nodeJson["children"].is_array()) {
                for (const auto& childJson : nodeJson["children"]) {
                    auto child = fromJson(childJson);
                    if (child) {
                        node->addChild(child);
                    }
                }
            }
            
            return node;
        };
        
        // Construir árbol
        json treeJson = rootJson["tree"];
        auto newRoot = fromJson(treeJson);
        
        if (!newRoot) {
            cerr << "Error: No se pudo construir el árbol desde JSON" << endl;
            return false;
        }
        
        // Reemplazar la raíz del árbol
        tree->setRoot(newRoot);
        
        cout << "Árbol cargado exitosamente desde " << filename << endl;
        
        // Mostrar metadatos si existen
        if (rootJson.contains("metadata")) {
            json metadata = rootJson["metadata"];
            if (metadata.contains("nodes")) {
                cout << "  Nodos cargados: " << metadata["nodes"] << endl;
            }
        }
        
        return true;
        
    } catch (const json::exception& e) {
        cerr << "Error JSON: " << e.what() << endl;
        return false;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return false;
    }
}