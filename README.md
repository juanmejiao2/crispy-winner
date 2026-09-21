# Desafio 1 Informatica 2

Este es el primer desafío de informática 2, en el que teníamos que realizar el juego "Sweet Crush" en bits (bitwise). Solo se podía usar el lenguaje C++ y en una programación modular.
Este también incluye ciertas restricciones de funciones, para poder hacer todo con operadores a nivel de bits. En el "Sweet Crush" que hicimos el jugador tiene la posibilidad de eliminar
las fichas que quiera en manera de coordenadas matriciales, para una mayor precisión en sus jugadas, aparte de que también puede agregar columnas y filas a esta matriz para redimensionar el 
tablero a su preferencia (o eliminarlas para mayor comodidad). El jugador tiene la libertad de decidir cuando quiere dejar de jugar y el sistema de puntos es perfecto para que lleve constancia
de su habilidad dentro del juego.

## Estructura del repositorio
En este repositorio se tienen los diferentes módulos del programa, los cuales están dentro de la carpeta DESAFIO_1_CODIGO y se divide de la siguiente manera:

- tablero.h -> Funciones de manipulación de memoria, creación/destrucción y redimensión del tablero.
- tablero.cpp -> Implementación de las funciones de tablero.
- juego.h -> Funciones de lógica de juego: cascadas, detección de combinaciones, gravedad, y visualización.
- juego.cpp ->Implementación de las funciones de juego.
- main.cpp -> menú principal, bucle del juego e integración.

README.md -> Archivo de documentación y breve resumen del desafío.
