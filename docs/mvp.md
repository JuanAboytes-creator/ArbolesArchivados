# Minimum Viable Product (MVP)

Fecha: 9 de Diciembre del 2025
Autores: Aboytes Mena Juan Jaime -- Luque Orozco Alan Giovanny

## Funcionalidades del MVP:

### 1. Árbol de directorios
- Nodos con tipo: CARPETA o ARCHIVO
- Sistema jerárquico como sistema de archivos real

### 2. Operaciones básicas:
- Crear carpeta/archivo
- Eliminar (a papelera temporal)
- Mostrar ruta completa
- listar hijos
- Listar contenido

### 3. Persistencia en JSON
- Guardar todo el árbol en archivo
- Cargar al iniciar el programa

### 4. Interfaz de consola
- Menú interactivo
- Comandos simples tipo Linux

Funcionalidades mínimas para el MVP:

1. CREACIÓN:
   - mkdir <ruta>: Crear carpeta
   - touch <ruta>: Crear archivo

2. NAVEGACIÓN Y CONSULTA:
   - ls <ruta>: Listar contenido
   - pwd: Mostrar ruta actual

3. MANIPULACIÓN:
   - mv <origen> <destino>: Mover nodo
   - rename <ruta> <nuevo_nombre>: Renombrar
   - rm <ruta>: Eliminar a papelera

4. BÚSQUEDA:
   - search <prefijo>: Buscar por prefijo
   - autocomplete <prefijo>: Sugerencias

5. EXPORTACIÓN:
   - export_preorder <archivo>: Exportar recorrido

6. PERSISTENCIA:
   - save: Guardar estado
   - load: Cargar estado