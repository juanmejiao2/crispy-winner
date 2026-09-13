#ifndef TABLERO_H
#define TABLERO_H

unsigned char* crearTablero(int filas, int columnas);

void destruirTablero(unsigned char* tablero);

unsigned char obtenerFicha(const unsigned char* tablero, int columnas, int fila, int columna);

void guardarFicha(unsigned char* tablero, int columnas, int fila, int columna, unsigned char valor);

#endif // TABLERO_H