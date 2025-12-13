# Proyecto 1: Sistema de Archivos Jerárquico con Árboles

## Descripción
Implementación en C++ de un sistema de archivos jerárquico que incluye:
- Árbol general para representar carpetas y archivos
- Trie para autocompletado y búsqueda eficiente
- Persistencia en formato JSON
- Interfaz de consola interactiva

## Estructura del Proyecto
ArbolesArchivados/
├── src/
│ ├── main.cpp # Punto de entrada principal
│ ├── TreeNode.hpp/.cpp # Definición e implementación del nodo
│ ├── FileSystemTree.hpp/.cpp # Clase del árbol de archivos
│ ├── Trie.hpp/.cpp # Estructura para autocompletado
│ ├── SearchEngine.hpp/.cpp # Motor de búsqueda integrado
│ ├── JsonHandler.hpp/.cpp # Persistencia JSON
│ ├── ConsoleInterface.hpp/.cpp # Interfaz de consola
│ ├── main_dia5_6.cpp # Pruebas días 5-6
│ └── prueba_dia4.cpp # Prueba día 4
├── include/ # Dependencias (nlohmann/json)
├── build/ # Directorio de compilación
├── CMakeLists.txt # Configuración CMake
├── Makefile # Makefile alternativo
├── compile.sh # Script de compilación
└── README.md # Este archivo


## Requisitos
- C++17 o superior
- CMake 3.10+ (recomendado) o g++ 7+
- Biblioteca nlohmann/json (se descarga automáticamente)

## Compilación

### Método 1: Usando CMake (recomendado)
```bash
# Ejecutar script de compilación
chmod +x compile.sh
./compile.sh

### Metodo 2: Manual con Cmake 
mkdir build
cd build
cmake ..
make

### Metodo 3: usando Makefile
# Descarga automática de dependencias y compilación
make

## Ejecucion
###Apliciacion principal
./build/arboles_archivados

###Pruebas especificas
# Pruebas días 5-6 (Trie y búsqueda)
./build/prueba_dia5_6

# Prueba día 4 (JSON)
./build/prueba_dia4

### Comandos Disponibles

cd <ruta>            - Cambiar directorio actual
ls [ruta]            - Listar contenido
mkdir <nombre>       - Crear directorio
touch <nombre> [cont] - Crear archivo
mv <origen> <destino> - Mover nodo
rm <ruta>            - Eliminar nodo
rename <ruta> <nuevo> - Renombrar nodo
search <consulta>    - Buscar por nombre
autocomplete <prefijo> - Sugerencias
export <archivo>     - Exportar recorrido preorden
save [archivo]       - Guardar estado en JSON
load [archivo]       - Cargar estado desde JSON
tree                 - Mostrar estructura completa
pwd                  - Mostrar ruta actual
help                 - Mostrar ayuda
exit                 - Salir

## Ejemplos de uso

### Crear estructura de archivos
mkdir Documentos
cd Documentos
touch notas.txt "Estas son mis notas"
mkdir Proyectos
cd Proyectos
touch main.cpp "#include <iostream>"

### Busqueda y autocompletado
# Buscar archivos que empiezan con "not"
search not

# Obtener sugerencias para "pro"
autocomplete pro

### Persistencia

# Guardar estado actual
save sistema.json

# Cargar estado posteriormente
load sistema.json

### Exportar recorrido

# Exportar recorrido preorden a archivo
export recorrido.txt