# Formato JSON

**Fecha:** 9 de diciembre de 2025  
**Autor:** Luque Orozco Alan Giovanny  

## Ejemplo:
Formato JSON propuesto:

{
  "metadata": {
    "nextId": 10,
    "created": "2024-01-15T10:30:00Z"
  },
  "tree": {
    "id": 0,
    "name": "root",
    "type": "FOLDER",
    "content": "",
    "children": [
      {
        "id": 1,
        "name": "Documents",
        "type": "FOLDER",
        "content": "",
        "children": [
          {
            "id": 2,
            "name": "report.txt",
            "type": "FILE",
            "content": "Contenido del reporte",
            "children": []
          }
        ]
      },
      {
        "id": 3,
        "name": "Images",
        "type": "FOLDER",
        "content": "",
        "children": []
      }
    ]
  }
}
