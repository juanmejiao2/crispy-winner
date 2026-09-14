#include "tablero.h"

unsigned char* crearTablero(int filas, int columnas){
    int bits_totales= filas*columnas*3;
    int bytes_necesarios=(bits_totales+7)/8;
    unsigned char* tablero= new unsigned char[bytes_necesarios];

    for (int i=0; i<bytes_necesarios; ++i){
        tablero[i]=0;
    }
    return tablero;
}

void destruirTablero(unsigned char* tablero){

}

unsigned char obtenerFicha(const unsigned char* tablero, int columnas, int fila, int columna){

}

void guardarFicha(unsigned char* tablero, int columnas, int fila, int columna, unsigned char valor){

}