// main.cpp - Punto de entrada principal del proyecto
#include <iostream>
#include "ConsoleInterface.hpp"

using namespace std;

int main() {
    cout << "=========================================" << endl;
    cout << "    SISTEMA DE ARCHIVOS JERÁRQUICO" << endl;
    cout << "      Proyecto 1 - Estructura de Datos" << endl;
    cout << "           Tema: Árboles" << endl;
    cout << "=========================================" << endl;
    cout << endl;
    
    try {
        ConsoleInterface console;
        console.run();
    } catch (const exception& e) {
        cerr << "Error crítico: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nGracias por usar el sistema de archivos jerárquico!" << endl;
    return 0;
}