# Proyecto 1: Análisis de una red de transporte urbano
**Integrantes:**
* David Garrido - 2310610
* Kevin Gomes   - 2310170

## Detalles Técnicos
* **Lenguaje de Programación:** C++
* **Versión / Estándar:** C++11 o superior (necesario para el uso de contenedores de la STL como `std::unordered_map` y `std::set`)

## Instrucciones de Compilación
Para compilar el código fuente, abra la terminal en el directorio del proyecto y ejecute el siguiente comando utilizando el compilador GCC:

g++ -std=c++11 -O2 -o p1 p1.cpp

## Instrucciones de Ejecución
El programa está diseñado para ejecutarse mediante línea de comandos recibiendo exactamente dos parámetros obligatorios: el archivo de texto con la definición de la red y el archivo de texto con las consultas.

**En sistemas Linux o macOS:**
./p1 <red.txt> <consultas.txt>

**En sistemas Windows (PowerShell o Símbolo del sistema):**
.\p1.exe <red.txt> <consultas.txt>