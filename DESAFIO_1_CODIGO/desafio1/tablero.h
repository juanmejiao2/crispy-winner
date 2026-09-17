#ifndef TABLERO_H
#define TABLERO_H

unsigned char* crearTablero(int filas, int columnas);

void destruirTablero(unsigned char* tablero);

unsigned char obtenerFicha(const unsigned char* tablero, int columnas, int fila, int columna);

void guardarFicha(unsigned char* tablero, int columnas, int fila, int columna, unsigned char valor);

void agregarFila(unsigned char** tablero, int* filas, int columnas, int posicion_fila);

void eliminarFila(unsigned char** tablero, int* filas, int columnas, int posicion_fila);

void agregarColumna(unsigned char** tablero, int filas, int* columnas, int posicion_columna);

void eliminarColumna(unsigned char** tablero, int filas, int* columnas, int posicion_columna);

#endif // TABLERO_H