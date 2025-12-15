#!/bin/bash

# demo.sh - Demo automatizada del Sistema de Archivos Jerárquico
# Día 13: Preparación de demo y script de ejecución

# ============================================================================
# CONFIGURACIÓN
# ============================================================================
APP_NAME="Sistema de Archivos Jerárquico"
VERSION="1.0.0"
AUTHOR="Equipo de Estructura de Datos"
APP_BINARY="./arboles_archivados"
EXAMPLE_JSON="ejemplo_completo.json"
DEMO_JSON="demo_estado.json"
DEMO_EXPORT="demo_recorrido.txt"

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# ============================================================================
# FUNCIONES DE UTILIDAD
# ============================================================================

print_header() {
    echo -e "${CYAN}"
    echo "╔══════════════════════════════════════════════════════════════╗"
    echo "║                    $APP_NAME                     ║"
    echo "║                   Día 13: DEMO COMPLETA                      ║"
    echo "╚══════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

print_section() {
    echo -e "\n${GREEN}=== $1 ===${NC}"
}

print_step() {
    echo -e "${YELLOW}▶ Paso $1: $2${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

pause() {
    echo -e "\n${MAGENTA}Presiona Enter para continuar...${NC}"
    read
}

clear_screen() {
    clear
}

# ============================================================================
# COMPROBACIONES INICIALES
# ============================================================================

check_dependencies() {
    print_section "COMPROBACIÓN DE DEPENDENCIAS"
    
    # Comprobar compilador C++
    if command -v g++ &> /dev/null; then
        print_success "g++ encontrado: $(g++ --version | head -n1)"
    else
        print_error "g++ no encontrado. Se requiere compilador C++"
        exit 1
    fi
    
    # Comprobar Make
    if command -v make &> /dev/null; then
        print_success "make encontrado"
    else
        print_warning "make no encontrado (se usará compilación directa)"
    fi
    
    # Comprobar CMake (opcional)
    if command -v cmake &> /dev/null; then
        print_success "CMake encontrado: $(cmake --version | head -n1)"
    fi
}

check_binary() {
    if [ ! -f "$APP_BINARY" ]; then
        print_info "Aplicación no compilada. Compilando..."
        
        # Buscar script de compilación
        if [ -f "compile_fix.sh" ]; then
            chmod +x compile_fix.sh
            ./compile_fix.sh
            
            if [ $? -eq 0 ] && [ -f "$APP_BINARY" ]; then
                print_success "Compilación exitosa"
            else
                print_error "Error en compilación"
                exit 1
            fi
        else
            print_error "No se encontró compile_fix.sh"
            exit 1
        fi
    else
        print_success "Aplicación ya compilada"
    fi
}

# ============================================================================
# DEMOSTRACIONES
# ============================================================================

demo_install() {
    clear_screen
    print_header
    print_section "INSTALACIÓN Y COMPILACIÓN"
    
    print_step "1" "Comprobando sistema..."
    echo "Sistema operativo: $(uname -s)"
    echo "Arquitectura: $(uname -m)"
    echo "Directorio actual: $(pwd)"
    
    print_step "2" "Comprobando dependencias..."
    check_dependencies
    
    print_step "3" "Comprobando binario..."
    check_binary
    
    print_success "Sistema listo para la demo"
    pause
}

demo_basic_operations() {
    clear_screen
    print_header
    print_section "DEMO 1: OPERACIONES BÁSICAS"
    
    print_info "En esta demo veremos:"
    echo "  • Crear directorios y archivos"
    echo "  • Navegar entre directorios"
    echo "  • Listar contenido"
    echo "  • Mostrar estructura completa"
    
    pause
    
    # Crear archivo de comandos para la demo
    cat > demo_commands.txt << 'EOF'
# ============================================
# DEMO: OPERACIONES BÁSICAS
# ============================================
echo "=== Creando estructura básica ==="
mkdir ProyectoEDD
cd ProyectoEDD
touch README.md "# Proyecto de Estructura de Datos"
mkdir src
mkdir docs
mkdir tests

echo "=== Creando archivos en src ==="
cd src
touch main.cpp "// Punto de entrada principal"
touch TreeNode.cpp "// Implementación de nodos"
touch FileSystemTree.cpp "// Implementación del árbol"

echo "=== Regresando y listando ==="
cd ..
ls

echo "=== Mostrando árbol completo ==="
tree

echo "=== Mostrando ruta actual ==="
pwd

echo "=== Demo completada ==="
EOF
    
    print_step "1" "Mostrando comandos de la demo..."
    cat demo_commands.txt
    
    pause
    
    print_step "2" "Ejecutando aplicación con comandos básicos..."
    echo "Nota: La demo se ejecutará en modo interactivo"
    echo "      Usa los comandos del archivo demo_commands.txt"
    
    print_info "Para probar manualmente, ejecuta: ./arboles_archivados"
    print_info "Luego usa los comandos del archivo demo_commands.txt"
    
    pause
}

demo_search_autocomplete() {
    clear_screen
    print_header
    print_section "DEMO 2: BÚSQUEDA Y AUTOCOMPLETADO"
    
    print_info "En esta demo veremos:"
    echo "  • Búsqueda por prefijo"
    echo "  • Autocompletado inteligente"
    echo "  • Búsqueda por contenido"
    
    pause
    
    cat > demo_search_commands.txt << 'EOF'
# ============================================
# DEMO: BÚSQUEDA Y AUTOCOMPLETADO
# ============================================
echo "=== Creando archivos para búsqueda ==="
touch algoritmo_avl.cpp "// Implementación AVL"
touch algoritmo_rb.cpp "// Implementación Red-Black"
touch estructura_heap.cpp "// Implementación Heap"
touch prueba_busqueda.cpp "// Pruebas de búsqueda"
touch documentacion.txt "// Documentación del proyecto"

echo "=== Búsqueda por prefijo 'algoritmo' ==="
search algoritmo

echo "=== Autocompletado para 'est' ==="
autocomplete est

echo "=== Búsqueda por contenido 'implementación' ==="
# Nota: Asumiendo que search también busca en contenido
search implementación

echo "=== Búsqueda de todos los archivos .cpp ==="
search .cpp

echo "=== Demo de búsqueda completada ==="
EOF
    
    print_step "1" "Comandos de búsqueda y autocompletado..."
    cat demo_search_commands.txt
    
    pause
    
    print_info "El Trie permite:"
    echo "  • Búsqueda en O(k) donde k es la longitud de la palabra"
    echo "  • Autocompletado con sugerencias relevantes"
    echo "  • Búsqueda por prefijo eficiente"
    
    pause
}

demo_trash_system() {
    clear_screen
    print_header
    print_section "DEMO 3: SISTEMA DE PAPELERA"
    
    print_info "En esta demo veremos:"
    echo "  • Eliminación segura a papelera"
    echo "  • Listado de papelera"
    echo "  • Restauración de archivos"
    echo "  • Vaciar papelera"
    
    pause
    
    cat > demo_trash_commands.txt << 'EOF'
# ============================================
# DEMO: SISTEMA DE PAPELERA
# ============================================
echo "=== Creando archivos temporales ==="
touch temp1.txt "Archivo temporal 1"
touch temp2.txt "Archivo temporal 2"
touch temp3.txt "Archivo temporal 3"
mkdir temp_dir

echo "=== Eliminando archivos (van a papelera) ==="
rm temp1.txt
rm temp2.txt
rm temp_dir

echo "=== Mostrando contenido de la papelera ==="
trash

echo "=== Restaurando un archivo ==="
restore temp1.txt

echo "=== Mostrando papelera después de restaurar ==="
trash

echo "=== Vaciar papelera (con confirmación) ==="
emptytrash

echo "=== Verificando papelera vacía ==="
trash

echo "=== Demo de papelera completada ==="
EOF
    
    print_step "1" "Comandos del sistema de papelera..."
    cat demo_trash_commands.txt
    
    pause
    
    print_info "Características de la papelera:"
    echo "  • Eliminación temporal (no permanente)"
    echo "  • Restauración completa con metadatos"
    echo "  • Confirmación para vaciado permanente"
    echo "  • Persistencia de ruta original"
    
    pause
}

demo_persistence() {
    clear_screen
    print_header
    print_section "DEMO 4: PERSISTENCIA Y EXPORTACIÓN"
    
    print_info "En esta demo veremos:"
    echo "  • Guardar estado en JSON"
    echo "  • Cargar estado desde JSON"
    echo "  • Exportar recorrido preorden"
    echo "  • Cargar ejemplo preconfigurado"
    
    pause
    
    cat > demo_persistence_commands.txt << 'EOF'
# ============================================
# DEMO: PERSISTENCIA Y EXPORTACIÓN
# ============================================
echo "=== Creando estructura de ejemplo ==="
mkdir Proyectos
cd Proyectos
mkdir Personal
mkdir Universidad
touch lista_tareas.txt "1. Estudiar árboles\n2. Implementar Trie"

echo "=== Guardando estado actual ==="
save estado_actual.json

echo "=== Exportando recorrido preorden ==="
export recorrido_preorden.txt

echo "=== Cargando ejemplo preconfigurado ==="
load ejemplo_completo.json

echo "=== Mostrando estructura cargada ==="
tree

echo "=== Volviendo al estado guardado ==="
load estado_actual.json

echo "=== Demo de persistencia completada ==="
EOF
    
    print_step "1" "Comandos de persistencia..."
    cat demo_persistence_commands.txt
    
    pause
    
    if [ -f "$EXAMPLE_JSON" ]; then
        print_success "Archivo de ejemplo encontrado: $EXAMPLE_JSON"
        print_info "Contenido del ejemplo:"
        echo "------------------------"
        head -20 $EXAMPLE_JSON
        echo "..."
        echo "------------------------"
    else
        print_warning "Archivo de ejemplo no encontrado"
    fi
    
    pause
}

demo_testing() {
    clear_screen
    print_header
    print_section "DEMO 5: PRUEBAS Y VALIDACIÓN"
    
    print_info "En esta demo veremos:"
    echo "  • Pruebas de integración"
    echo "  • Pruebas de rendimiento"
    echo "  • Pruebas de casos límite"
    echo "  • Estadísticas del sistema"
    echo "  • Validación de consistencia"
    
    pause
    
    cat > demo_test_commands.txt << 'EOF'
# ============================================
# DEMO: PRUEBAS Y VALIDACIÓN
# ============================================
echo "=== Ejecutando pruebas de integración ==="
test-integration

echo "=== Mostrando estadísticas del sistema ==="
stats

echo "=== Validando consistencia del sistema ==="
validate

echo "=== Ejecutando pruebas de rendimiento ==="
test-performance

echo "=== Ejecutando pruebas de casos límite ==="
test-edge

echo "=== Generando árbol de prueba ==="
generate 3 2

echo "=== Ejecutando benchmark ==="
benchmark

echo "=== Demo de pruebas completada ==="
EOF
    
    print_step "1" "Comandos de pruebas y validación..."
    cat demo_test_commands.txt
    
    pause
    
    print_info "Cobertura de pruebas:"
    echo "  • Integración: Comprueba flujos completos"
    echo "  • Rendimiento: Mide tiempos con árboles grandes"
    echo "  • Casos límite: Valida manejo de errores"
    echo "  • Validación: Verifica consistencia interna"
    
    pause
}

quick_demo() {
    clear_screen
    print_header
    print_section "DEMO RÁPIDA (2 minutos)"
    
    print_info "Esta demo rápida muestra las funciones principales:"
    
    # Comandos que se ejecutarían en una demo real
    echo -e "${YELLOW}Comandos que se ejecutarían:${NC}"
    echo "1.  mkdir DemoRapida"
    echo "2.  cd DemoRapida"
    echo "3.  touch archivo1.txt 'Contenido 1'"
    echo "4.  touch archivo2.txt 'Contenido 2'"
    echo "5.  search archivo"
    echo "6.  autocomplete arc"
    echo "7.  rm archivo1.txt"
    echo "8.  trash"
    echo "9.  restore archivo1.txt"
    echo "10. save demo_rapida.json"
    echo "11. export demo_export.txt"
    echo "12. stats"
    
    pause
    
    print_success "Demo rápida completada"
    print_info "Tiempo estimado: 2 minutos"
}

# ============================================================================
# MENÚ PRINCIPAL
# ============================================================================

show_menu() {
    clear_screen
    print_header
    
    echo -e "${GREEN}Selecciona una opción:${NC}"
    echo ""
    echo "  1) 📦 Comprobación e instalación"
    echo "  2) 📁 Demo 1: Operaciones básicas"
    echo "  3) 🔍 Demo 2: Búsqueda y autocompletado"
    echo "  4) 🗑️  Demo 3: Sistema de papelera"
    echo "  5) 💾 Demo 4: Persistencia y exportación"
    echo "  6) ✅ Demo 5: Pruebas y validación"
    echo "  7) ⚡ Demo rápida (2 minutos)"
    echo "  8) 🚀 Ejecutar aplicación completa"
    echo "  9) 📊 Ver archivos de ejemplo"
    echo "  0) ❌ Salir"
    echo ""
    echo -n "Tu elección [0-9]: "
}

show_example_files() {
    clear_screen
    print_header
    print_section "ARCHIVOS DE EJEMPLO"
    
    echo -e "${YELLOW}Archivos disponibles para la demo:${NC}"
    echo ""
    
    # Mostrar estructura de archivos
    echo "📁 Estructura del proyecto:"
    echo "├── 📄 arboles_archivados          (Aplicación principal)"
    echo "├── 📄 compile_fix.sh              (Script de compilación)"
    echo "├── 📄 demo.sh                     (Este script de demo)"
    echo "├── 📄 ejemplo_completo.json       (Estructura preconfigurada)"
    echo "├── 📄 README.md                   (Documentación)"
    echo "├── 📁 src/                        (Código fuente)"
    echo "│   ├── main.cpp"
    echo "│   ├── ConsoleInterface.cpp"
    echo "│   ├── FileSystemTree.cpp"
    echo "│   └── ..."
    echo "└── 📁 build/                      (Binarios compilados)"
    echo ""
    
    if [ -f "$EXAMPLE_JSON" ]; then
        print_info "Vista previa de $EXAMPLE_JSON:"
        echo "----------------------------------------"
        head -15 "$EXAMPLE_JSON"
        echo "..."
        echo "----------------------------------------"
    else
        print_warning "$EXAMPLE_JSON no encontrado"
        print_info "Puedes crear uno con: ./arboles_archivados -> save ejemplo_completo.json"
    fi
    
    pause
}

# ============================================================================
# FUNCIÓN PRINCIPAL
# ============================================================================

main() {
    # Verificar que estamos en el directorio correcto
    if [ ! -f "src/main.cpp" ] && [ ! -f "compile_fix.sh" ]; then
        print_error "No se detecta estructura del proyecto"
        print_info "Ejecuta este script desde la raíz del proyecto"
        exit 1
    fi
    
    # Comprobar binario
    check_binary
    
    # Menú principal
    while true; do
        show_menu
        read choice
        
        case $choice in
            1) demo_install ;;
            2) demo_basic_operations ;;
            3) demo_search_autocomplete ;;
            4) demo_trash_system ;;
            5) demo_persistence ;;
            6) demo_testing ;;
            7) quick_demo ;;
            8) 
                clear_screen
                print_header
                print_success "Iniciando aplicación principal..."
                echo ""
                print_info "Escribe 'help' para ver todos los comandos"
                print_info "Presiona Ctrl+C o escribe 'exit' para salir"
                echo ""
                pause
                ./arboles_archivados
                ;;
            9) show_example_files ;;
            0) 
                print_success "¡Gracias por usar la demo!"
                echo ""
                print_info "Recuerda:"
                echo "• Para más ayuda: ./arboles_archivados -> help"
                echo "• Para pruebas: ./arboles_archivados -> test-integration"
                echo "• Para exportar: ./arboles_archivados -> export <archivo>"
                echo ""
                exit 0
                ;;
            *) 
                print_error "Opción inválida"
                pause
                ;;
        esac
    done
}

# ============================================================================
# EJECUCIÓN
# ============================================================================

# Solo ejecutar si se llama directamente
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main
fi