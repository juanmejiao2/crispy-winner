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
    delete[] tablero;
}

unsigned char obtenerFicha(const unsigned char* tablero, int columnas, int fila, int columna){
    int posicion_ficha= fila * columnas + columna;
    int bit_inicial= posicion_ficha * 3;
    int byte_buscado= bit_inicial / 8;
    int desplazamiento_bit= bit_inicial % 8;

    unsigned char resultado= 0;

    if (desplazamiento_bit<=5){
        resultado= (tablero[byte_buscado]>>(5-desplazamiento_bit)) & 0x07;
    } else if(desplazamiento_bit==6){
        unsigned char bits_alto= (tablero[byte_buscado] & 0x03) << 1;
        unsigned char bit_bajo= (tablero[byte_buscado+1] >> 7) & 0x01;
        resultado= bits_alto | bit_bajo;
    } else {
        unsigned char bit_alto= (tablero[byte_buscado] & 0x01)<<2;
        unsigned char bits_bajo= (tablero[byte_buscado+1] >> 6) & 0x03;
        resultado= bit_alto | bits_bajo;
    }
    return resultado;
}

void guardarFicha(unsigned char* tablero, int columnas, int fila, int columna, unsigned char valor){

}