# Días 5-6: Implementación de Trie y Sistema de Búsqueda

## Estructura del Proyecto Actualizada
ArbolesArchivados/
├── docs/
├── src/
│ ├── main.cpp # Punto de entrada principal
│ ├── main_dia5_6.cpp # Pruebas específicas días 5-6
│ ├── TreeNode.hpp # Definición del nodo (con enable_shared_from_this)
│ ├── FileSystemTree.hpp # Clase del árbol con operaciones
│ ├── FileSystemTree.cpp # Implementación del árbol
│ ├── Trie.hpp # Estructura Trie para autocompletado
│ ├── Trie.cpp # Implementación Trie
│ ├── SearchEngine.hpp # Motor de búsqueda integrado
│ ├── SearchEngine.cpp # Implementación motor de búsqueda
│ ├── JsonHandler.hpp # Interfaz para persistencia JSON
│ ├── JsonHandler.cpp # Implementación JSON con nlohmann/json
│ └── ConsoleInterface.hpp # Interfaz de consola (pendiente)
├── CMakeLists.txt # Configuración de compilación
└── README_dia5_6.md # Este documento

## Nuevas Funcionalidades Implementadas

### 1. **Trie Completo**
- **Inserción**: `insert(word, nodeId)` - Indexa palabras con sus IDs
- **Búsqueda exacta**: `searchExact(word)` - Encuentra IDs de palabra exacta
- **Búsqueda por prefijo**: `searchByPrefix(prefix)` - Todas las palabras que empiezan con...
- **Autocompletado**: `autocomplete(prefix, maxSuggestions)` - Sugerencias interactivas
- **Eliminación**: `remove(word, nodeId)` - Remueve palabra específica
- **Estadísticas**: `countWords()`, `startsWith()`, `getAllWords()`

### 2. **Motor de Búsqueda Integrado**
- **Indexación automática**: Reconstruye índice completo del árbol
- **Búsqueda avanzada**: Exacta, por prefijo, por contenido (substring)
- **Autocompletado**: Integrado con árbol de archivos
- **Verificación de integridad**: Asegura coherencia entre árbol e índice
- **Estadísticas**: Información detallada del índice

### 3. **Persistencia JSON Mejorada**
- **Metadatos**: Versión, tamaño, altura, nextId
- **Estructura jerárquica**: Preserva relaciones padre-hijo
- **Tipos diferenciados**: Carpeta vs Archivo con contenido
- **Manejo de errores**: Validación de estructura JSON

## Compilación y Ejecución

### Requisitos:
- C++17 o superior
- CMake 3.10+
- Biblioteca nlohmann/json (se descarga automáticamente)

### Compilar:
```bash
mkdir build
cd build
cmake ..
make

./prueba_dia5_6

./arboles_archivados