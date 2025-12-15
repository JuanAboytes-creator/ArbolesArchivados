# Guía de Instalación Detallada

## Tabla de Contenidos
1. [Requisitos del Sistema](#requisitos-del-sistema)
2. [Instalación en Linux](#instalación-en-linux)
3. [Instalación en macOS](#instalación-en-macos)
4. [Instalación en Windows](#instalación-en-windows)
5. [Compilación desde Código Fuente](#compilación-desde-código-fuente)
6. [Solucion de Problemas](#solución-de-problemas)

## Requisitos del Sistema

### Mínimos
- **Sistema operativo**: Linux, macOS, o Windows con WSL2
- **Memoria RAM**: 512 MB mínimo
- **Espacio disco**: 100 MB

### Recomendados
- **Procesador**: 1 GHz o superior
- **Memoria RAM**: 1 GB o más
- **Espacio disco**: 200 MB

### Dependencias de Software
- **Compilador C++17**: g++ 7+, clang 5+, o MSVC 2017+
- **CMake** (opcional pero recomendado): versión 3.10+
- **Git** (para clonar repositorio)

## Instalación en Linux

### Ubuntu/Debian
```bash
# 1. Actualizar sistema
sudo apt update
sudo apt upgrade -y

# 2. Instalar dependencias
sudo apt install -y build-essential git cmake

# 3. Clonar repositorio
git clone <url-del-repositorio>
cd ArbolesArchivados

# 4. Compilar
chmod +x compile_fix.sh
./compile_fix.sh

# 5. Ejecutar
./arboles_archivados
```

### Fedora/RHEL/CentOS
# 1. Instalar dependencias
```bash
sudo dnf install -y gcc-c++ git cmake make

# 2. Seguir pasos 3-5 de Ubuntu
```
### Arch Linux/Manjaro

```bash
# 1. Instalar dependencias
sudo pacman -Syu gcc git cmake make

# 2. Seguir pasos 3-5 de Ubuntu
```
### Instalación en macOS
#### Con homebrew (recomendado)
```bash
# 1. Instalar Homebrew si no está instalado
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. Instalar dependencias
brew install gcc git cmake

# 3. Clonar y compilar
git clone <url-del-repositorio>
cd ArbolesArchivados
chmod +x compile_fix.sh
./compile_fix.sh

# 4. Ejecutar
./arboles_archivados
```
#### Con Xcode Command Line Tools
```bash
# 1. Instalar Xcode Command Line Tools
xcode-select --install

# 2. Clonar y compilar
git clone <url-del-repositorio>
cd ArbolesArchivados
chmod +x compile_fix.sh
./compile_fix.sh

# 3. Ejecutar
./arboles_archivados
```

### Instalación en Windows

#### Opción 1: Windows Subsystem for Linux 2 (WSL2) - Recomendado
```bash
# 1. Habilitar WSL2 (en PowerShell como administrador)
wsl --install

# 2. Instalar distribución Ubuntu desde Microsoft Store

# 3. En Ubuntu WSL, seguir instrucciones de Linux
```
#### Opción 2: MSYS2/MinGW
```bash
# 1. Descargar e instalar MSYS2 desde https://www.msys2.org/

# 2. En terminal MSYS2
pacman -Syu
pacman -S mingw-w64-x86_64-gcc git make

# 3. Clonar y compilar
git clone <url-del-repositorio>
cd ArbolesArchivados
./compile_fix.sh
```
#### Opción 3: Visual Studio
1. Instalar Visual Studio 2019 o superior con soporte C++

2. Abrir proyecto como "Carpeta" en Visual Studio

3. Configurar para usar CMake
