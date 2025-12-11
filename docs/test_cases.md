# Casos de Prueba - Días 2-3

## Pruebas de Funcionalidad Básica

### 1. Creación de Nodos
    Input: createNode("/root", "Documents", FOLDER)
    Output: Nodo creado: ID=1, Nombre=Documents, Tipo=CARPETA

    Input: createNode("/root/Documents", "notes.txt", FILE, "Mis notas")
    Output: Nodo creado: ID=2, Nombre=notes.txt, Tipo=ARCHIVO


### 2. Manejo de Errores en Creación
    Input: createNode("/root/nonexistent", "test", FOLDER)
Output: Error: Ruta no encontrada

    Input: createNode("/root", "Documents", FOLDER) (duplicado)
    Output: Error: Ya existe un nodo con ese nombre


### 3. Movimiento de Nodos
    Input: moveNode("/root/Documents/notes.txt", "/root")
    Output: Nodo movido: /root/Documents/notes.txt -> /root

    Input: moveNode("/root/Documents", "/root/Documents/notes.txt")
    Output: Error: El destino debe ser una carpeta


### 4. Renombrado
    Input: renameNode("/root/notes.txt", "new_notes.txt")
    Output: Nodo renombrado: notes.txt -> new_notes.txt

    Input: renameNode("/root", "newroot")
    Output: Error: No se puede renombrar la raíz

### 5. Eliminación
    Input: deleteNode("/root/Documents")
    Output: Nodo eliminado: /root/Documents (ID=1)

    Input: deleteNode("/root")
    Output: Error: No se puede eliminar la raíz

## Pruebas de Propiedades del Árbol

### 6. Cálculo de Altura
Árbol: root -> A -> B -> C
Altura calculada: 3


### 7. Cálculo de Tamaño
Árbol con 7 nodos
Tamaño calculado: 7

### 8. Recorrido Preorden

Entrada: Árbol con estructura compleja
Salida: Lista en orden: root, A, B, C, D, E, F
Verificación: Orden correcto de visita raíz-izquierda-derecha

## Pruebas de Consistencia

### 9. Verificación de Relaciones Padre-Hijo

Después de mover nodo A de padre X a padre Y:

A.parent debe ser Y

Y.children debe contener A

X.children no debe contener A


### 10. Verificación de IDs Únicos

Después de múltiples operaciones:

Todos los nodos deben tener IDs únicos

nextId debe ser mayor que cualquier ID existente


## Pruebas de Rendimiento

### 11. Árboles Grandes

1000 nodos creados secuencialmente
Tiempo de creación: < 1 segundo
Consistencia verificada: OK


### 12. Operaciones Anidadas Profundas

10 niveles de anidación
moveNode en nivel 9: Ejecución exitosa
Recorrido preorden: Completo sin errores


