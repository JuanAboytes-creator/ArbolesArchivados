# Proyecto 1: Sistema de Archivos Jerárquico con Árboles

## Descripción
Implementación en C++ de un sistema de archivos jerárquico que incluye:
- Árbol general para representar carpetas y archivos
- Trie para autocompletado y búsqueda eficiente
- Persistencia en formato JSON
- Interfaz de consola interactiva
- Sistema de papelera temporal
- Pruebas de integración, rendimiento y casos límite

# Caracteristicas Principales

- ✅ Árbol jerárquico completo con operaciones CRUD

- ✅ Búsqueda y autocompletado usando Trie

- ✅ Persistencia JSON para guardar/cargar estado

- ✅ Papelera temporal con restauración

- ✅ Interfaz de consola interactiva

- ✅ Pruebas exhaustivas (integración, rendimiento, casos límite)

- ✅ Validación y estadísticas del sistema

- ✅ Generación de árboles para pruebas

- ✅ Benchmark de operaciones

## Estructura del Proyecto
```bash
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
├── compile_fix.sh # Script de compilación
└── README.md # Este archivo

```
## Requisitos
- C++17 o superior
- CMake 3.10+ (recomendado) o g++ 7+
- Biblioteca nlohmann/json (se descarga automáticamente)

## Instalación y Configuración

### Método 1: Usando scipt de compilación (recomendado)

```bash
# Dar permisos de ejecución
chmod +x compile_fix.sh

# Compilar el proyecto
./compile_fix.sh
```
### Metodo 2: Usando Cmake 
```bash
mkdir build
cd build
cmake ..
make
```
### Metodo 3: usando Makefile
```bash
# Descarga automática de dependencias y compilación
make
```
## Ejecucion
```bash
# Apliciacion principal
./arboles_archivados
```

### Pruebas especificas
```bash
# Pruebas días 5-6 (Trie y búsqueda)
./prueba_dia5_6
```
```bash
# Prueba día 4 (JSON)
./build/prueba_dia4
```

### Comandos Disponibles

#### Comandos Básicos
```bash
help                 - Mostrar ayuda completa
cd <ruta>            - Cambiar directorio actual
ls [ruta]            - Listar contenido del directorio
mkdir <nombre>       - Crear nuevo directorio
touch <nombre> [cont] - Crear nuevo archivo con contenido opcional
mv <origen> <destino> - Mover nodo entre directorios
rm <ruta>            - Eliminar nodo (envía a papelera)
rename <ruta> <nuevo> - Renombrar nodo
pwd                  - Mostrar ruta actual
tree                 - Mostrar estructura completa del árbol
```
#### Búsqueda y Navegación
```bash
search <consulta>    - Buscar nodos por nombre (prefijo)
autocomplete <prefijo> - Obtener sugerencias de autocompletado
```
#### Persistencia y Exporcaión
```bash
save [archivo]       - Guardar estado en JSON (default: filesystem.json)
load [archivo]       - Cargar estado desde JSON (default: filesystem.json)
export <archivo>     - Exportar recorrido preorden a archivo de texto
```
#### Sistema de Papelera
```bash
trash                - Mostrar contenido de la papelera temporal
restore <nombre>     - Restaurar elemento desde la papelera
emptytrash           - Vaciar papelera permanentemente (con confirmación)
```
#### Pruebas y Validación (Días 10-11)
```bash
test-integration     - Ejecutar pruebas de integración completas
test-performance     - Ejecutar pruebas de rendimiento
test-edge            - Ejecutar pruebas de casos límite
stats                - Mostrar estadísticas del sistema
validate             - Validar consistencia del sistema
generate <n> <h>     - Generar árbol de prueba con n niveles y h hijos
benchmark            - Medir rendimiento de operaciones
```
## Ejemplos de uso

```bash
# Crear estructura de archivos
mkdir Documentos
cd Documentos
touch notas.txt "Estas son mis notas"
mkdir Proyectos
cd Proyectos
touch main.cpp "#include <iostream>"

# Regresar al directorio anterior
cd ..

# Listar contenido
ls
```
### Búsqueda y autocompletado
```bash
# Buscar archivos que empiezan con "not"
search not

# Obtener sugerencias para "pro"

autocomplete pro
```
### Operaciones de Sistema
```bash
# Mover un archivo
mv Documentos/notas.txt Documentos/Proyectos/

# Renombrar un directorio
rename Documentos/Proyectos MiProyecto

# Eliminar un archivo (va a papelera)
rm MiProyecto/README.md

# Ver papelera
trash

# Restaurar archivo eliminado
restore README.md
```
### Persistencia

```bash
# Guardar estado actual
save mi_sistema.json

# Cargar estado posteriormente

load mi_sistema.json
```
### Exportar y Visualizar
```bash
# Exportar recorrido preorden
export recorrido.txt

# Ver estructura completa
tree

# Ver estadísticas
stats
```
### Pruebas y Benchmark
```bash
# Ejecutar pruebas de integración
test-integration

# Probar rendimiento con árbol grande
test-performance

# Generar árbol de prueba
generate 4 3

# Medir tiempos de operaciones
benchmark
```

## Pruebas del Sistema
### Pruebas de integración
#### Comprueban el funcionamiento conjunto de todos los componentes:
- Creación de estructura compleja

- Movimiento y renombrado de nodos

- Búsqueda y autocompletado

- Persistencia (guardar/cargar)

- Sistema de papelera

### Pruebas de Rendimiento
#### Miden el desempeño con estructuras grandes:
- Tiempos de recorrido preorden

- Velocidad de búsqueda

- Operaciones de creación masiva

### Pruebas de Casos Límite
#### Validan manejo de situaciones especiales:

- Nombres vacíos o duplicados

- Movimientos inválidos

- Rutas inexistentes

- Recuperación de errores

### Dependencias
#### nlohmann/json
Biblioteca para manejo de JSON en C++. Se descarga automáticamente durante la compilación.

### Sistemas Compatibles
- Linux (Ubuntu/Debian, Fedora, Arch)

- macOS (con Xcode Command Line Tools)

- Windows (WSL2, MinGW, Cygwin)