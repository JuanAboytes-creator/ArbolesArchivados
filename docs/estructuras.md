# Estructura del Proyecto - Día 1

## Estructura de Archivos

proyecto-arboles/
├── src/
│ ├── main.cpp # Punto de entrada principal
│ ├── TreeNode.h # Definición del nodo
│ ├── FileSystemTree.h # Clase del árbol
│ ├── Trie.h # Estructura para autocompletado
│ └── ConsoleInterface.h # Interfaz de consola
├── data/
│ └── filesystem.json # Archivo de persistencia
├── tests/ # Pruebas unitarias
└── README.md # Documentación


## Dependencias
- C++17 o superior
- Biblioteca JSON: nlohmann/json (a incluir)

## MVP Definido
1. **Operaciones CRUD básicas** en árbol jerárquico
2. **Persistencia** en formato JSON
3. **Búsqueda con autocompletado** usando Trie
4. **Exportación** de recorridos preorden
5. **Interfaz de consola** interactiva

## Próximos pasos (Día 2-3)
- Implementar operaciones básicas del árbol
- Crear pruebas unitarias iniciales
- Establecer sistema de IDs único