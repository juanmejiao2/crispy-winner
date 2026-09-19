#ifndef JUEGO_H
#define JUEGO_H

char obtenerCaracterFicha(unsigned char ficha);
bool detectarYEliminar(unsigned char* tablero, int filas, int columnas, bool* marcados, int* acumFichas, int* acumCombos);
void aplicarGravedad(unsigned char* tablero, int filas, int columnas);
void rellenarVacios(unsigned char* tablero, int filas, int columnas);
int ejecutarCascadas(unsigned char* tablero, int filas, int columnas, int* acumFichas = nullptr, int* acumCombos = nullptr);

void inicializarTableroAleatorio(unsigned char* tablero, int filas, int columnas);
void mostrarTableroFichas(const unsigned char* tablero, int filas, int columnas);
void mostrarTableroBinario(const unsigned char* tablero, int filas, int columnas);
void mostrarTableroNumerado(const unsigned char* tablero, int filas, int columnas);

#endif