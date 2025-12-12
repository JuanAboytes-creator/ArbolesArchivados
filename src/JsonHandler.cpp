#include "JsonHandler.hpp"
#include "FileSystemTree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool JsonHandler::saveTree(shared_ptr<FileSystemTree> tree, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << " para escribir." << endl;
        return false;
    }
    
    // Por ahora guardamos un placeholder
    // En Día 5 implementaremos JSON real con nlohmann/json
    
    file << "{\n";
    file << "  \"version\": \"1.0\",\n";
    file << "  \"type\": \"filesystem_tree\",\n";
    file << "  \"nodes\": " << tree->calculateSize() << ",\n";
    file << "  \"height\": "
