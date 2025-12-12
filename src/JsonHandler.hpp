#ifndef JSONHANDLER_HPP
#define JSONHANDLER_HPP

#include <string>
#include <memory>
#include "FileSystemTree.hpp"

using namespace std;

class JsonHandler {
public:
    static bool saveTree(shared_ptr<FileSystemTree> tree, const string& filename);
    static bool loadTree(shared_ptr<FileSystemTree> tree, const string& filename);
};

#endif
