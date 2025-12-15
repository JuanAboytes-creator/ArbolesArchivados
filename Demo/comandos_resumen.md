#   Comandos del Sistema

##   BÁSICOS
`help` - Muestra ayuda completa  
`exit` - Sale del sistema  
`pwd` - Muestra ruta actual  

##   ARCHIVOS
`ls [ruta]` - Lista directorio  
`cd <ruta>` - Cambia directorio  
`mkdir <nombre>` - Crea directorio  
`touch <nombre>` - Crea archivo  
`mv <origen> <destino>` - Mueve  
`rm <ruta>` - Elimina (va a papelera)  
`rename <viejo> <nuevo>` - Renombra  

##   PAPELERA
`trash` - Muestra papelera  
`restore <nombre>` - Restaura archivo  
`emptytrash` - Vacía papelera  

##   BÚSQUEDA
`search <texto>` - Busca archivos  
`autocomplete <prefijo>` - Sugerencias  

##   GUARDAR
`save [archivo]` - Guarda en JSON  
`load [archivo]` - Carga desde JSON  
`export <archivo>` - Exporta estructura  

##   VISUALIZAR
`tree` - Muestra árbol completo  

##   PRUEBAS
`test-integration` - Pruebas integración  
`test-performance` - Pruebas rendimiento  
`stats` - Estadísticas sistema  
`validate` - Valida consistencia  

##   EJEMPLOS RÁPIDOS

### Crear proyecto:
```bash
mkdir MiProyecto
cd MiProyecto
touch main.cpp
touch README.md
ls
