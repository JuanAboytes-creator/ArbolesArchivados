# Makefile para compilación manual
CXX = g++
CXXFLAGS = -std=c++17 -I./src -I./include
LDFLAGS = 
TARGET = arboles_archivados
TEST_TARGET = prueba_dia5_6
TEST4_TARGET = prueba_dia4

SRCS = src/main.cpp \
       src/TreeNode.cpp \
       src/FileSystemTree.cpp \
       src/Trie.cpp \
       src/SearchEngine.cpp \
       src/JsonHandler.cpp \
       src/ConsoleInterface.cpp

TEST_SRCS = src/main_dia5_6.cpp \
            src/TreeNode.cpp \
            src/FileSystemTree.cpp \
            src/Trie.cpp \
            src/SearchEngine.cpp \
            src/JsonHandler.cpp \
            src/ConsoleInterface.cpp

TEST4_SRCS = src/prueba_dia4.cpp \
             src/TreeNode.cpp \
             src/FileSystemTree.cpp \
             src/Trie.cpp \
             src/SearchEngine.cpp \
             src/JsonHandler.cpp \
             src/ConsoleInterface.cpp

OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST4_OBJS = $(TEST4_SRCS:.cpp=.o)

# Descargar nlohmann/json si no existe
JSON_INCLUDE = include/nlohmann/json.hpp

$(JSON_INCLUDE):
	mkdir -p include/nlohmann
	curl -o $(JSON_INCLUDE) https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

all: $(JSON_INCLUDE) $(TARGET) $(TEST_TARGET) $(TEST4_TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(LDFLAGS)

$(TEST4_TARGET): $(TEST4_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST4_TARGET) $(TEST4_OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TEST4_OBJS) $(TARGET) $(TEST_TARGET) $(TEST4_TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

test4: $(TEST4_TARGET)
	./$(TEST4_TARGET)

.PHONY: all clean run test test4