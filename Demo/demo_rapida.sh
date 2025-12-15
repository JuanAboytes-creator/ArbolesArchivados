#!/bin/bash
# demo_rapida.sh - Demo simplificada del Sistema de Árboles Archivados

echo "========================================"
echo "   DEMO RÁPIDA - ÁRBOLES ARCHIVADOS    "
echo "========================================"
echo ""

# Ir al directorio src
cd src 2>/dev/null || { 
    echo "Error: No se encontró la carpeta src/"
    echo "   Ejecuta desde la raíz del proyecto"
    exit 1 
}

echo "1. Compilando proyecto..."
make clean > /dev/null 2>&1
make all > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "  Error en compilación"
    exit 1
fi
echo "  Compilado exitosamente"
echo ""

echo "2. Probando sistema básico..."
echo "--------------------------------"
echo "mkdir PruebaDemo" | ./arboles_archivados 2>&1 | tail -3
echo "cd PruebaDemo" | ./arboles_archivados 2>&1 | tail -2
echo "touch archivo.txt 'Contenido demo'" | ./arboles_archivados 2>&1 | tail -2
echo "ls" | ./arboles_archivados 2>&1 | tail -5
echo ""

echo "3.  Probando papelera..."
echo "---------------------------"
echo "rm archivo.txt" | ./arboles_archivados 2>&1 | tail -3
echo "trash" | ./arboles_archivados 2>&1 | tail -10
echo ""

echo "4.  Probando búsqueda..."
echo "--------------------------"
echo "touch documento.pdf 'PDF importante'" | ./arboles_archivados 2>&1 | tail -2
echo "search doc" | ./arboles_archivados 2>&1 | tail -5
echo ""

echo "5.  Finalizando..."
echo "-------------------"
echo "tree" | ./arboles_archivados 2>&1 | head -15
echo "stats" | ./arboles_archivados 2>&1 | tail -5
echo ""

echo "========================================"
echo "      DEMO RÁPIDA COMPLETADA           "
echo "========================================"
echo ""
echo "  Para demo completa, clona el repositorio"
echo "   y ejecuta: ./demo/run_demo_completo.sh"
echo ""
echo "  Comando para probar manualmente:"
echo "   ./arboles_archivados"
