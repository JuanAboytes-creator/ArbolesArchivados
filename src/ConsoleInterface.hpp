#ifndef CONSOLEINTERFACE_HPP
#define CONSOLEINTERFACE_HPP

#include "FileSystemTree.hpp"
#include "SearchEngine.hpp"
#include "JsonHandler.hpp"
#include <memory>
#include <string>
#include <vector>
using namespace std;

class ConsoleInterface {
private:
    struct TrashItem {
        shared_ptr<TreeNode> node;
        string originalPath;
        shared_ptr<TreeNode> parent;
    };
    
    shared_ptr<FileSystemTree> fileSystem;
    shared_ptr<SearchEngine> searchEngine;
    string currentPath;
    vector<TrashItem> trashBin;
    
    // Comandos internos
    void processCommand(const string& command);
    void showHelp();
    void changeDirectory(const string& path);
    void listDirectory(const string& path = "");
    void makeDirectory(const string& path);
    void createFile(const string& path, const string& content = "");
    void moveNode(const string& source, const string& dest);
    void removeNode(const string& path);
    void renameNode(const string& path, const string& newName);
    void searchNodes(const string& query);
    void autocompleteSuggestions(const string& prefix);
    void exportPreorder(const string& filename);
    void saveState(const string& filename);
    void loadState(const string& filename);
    void showTree();
    void showPath();

    void listTrash();
    void restoreFromTrash(const string& name);
    void emptyTrash();
    
    // Días 10-11: Pruebas de integración y rendimiento
    void runIntegrationTests();
    void runPerformanceTests();
    void runEdgeCaseTests();
    void showSystemStats();
    void validateSystem();
    void generateTestTree(int levels, int children);
    void benchmarkOperations();
    
    // Helpers
    vector<string> parseArguments(const string& input);
    string trim(const string& str);
    string getAbsolutePath(const string& relativePath);

public:
    ConsoleInterface();
    void run();
};

#endif // CONSOLEINTERFACE_HPP