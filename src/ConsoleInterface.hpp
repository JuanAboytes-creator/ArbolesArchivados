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
    shared_ptr<FileSystemTree> fileSystem;
    shared_ptr<SearchEngine> searchEngine;
    string currentPath;
    vector<shared_ptr<TreeNode>> trashBin;
    
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
    
    // Helpers
    vector<string> parseArguments(const string& input);
    string trim(const string& str);
    string getAbsolutePath(const string& relativePath);

public:
    ConsoleInterface();
    void run();
};

#endif // CONSOLEINTERFACE_HPP
