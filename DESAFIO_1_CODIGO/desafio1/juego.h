#ifndef JUEGO_H
#define JUEGO_H

void inicializarTableroAleatorio(unsigned char* tablero, int filas, int columnas);

bool detectarYEliminar(unsigned char* tablero, int filas, int columnas, bool* marcados);

void aplicarGravedad(unsigned char* tablero, int filas, int columnas);

void rellenarVacios(unsigned char* tablero, int filas, int columnas);

int ejecutarCascadas(unsigned char* tablero, int filas, int columnas);

#endif // JUEGO_H