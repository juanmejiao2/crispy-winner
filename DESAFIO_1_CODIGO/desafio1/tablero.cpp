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
    valor= valor & 0x07;
    int posicion_ficha= fila * columnas + columna;
    int bit_inicial= posicion_ficha * 3;
    int byte_buscado= bit_inicial / 8;
    int desplazamiento_bit= bit_inicial % 8;

    if (desplazamiento_bit<=5){
        int desplazamiento= 5-desplazamiento_bit;
        tablero[byte_buscado]= tablero[byte_buscado] & ~(0x07<<desplazamiento);
        tablero[byte_buscado]=tablero[byte_buscado] | (valor<<desplazamiento);

    } else if (desplazamiento_bit==6){
        unsigned char bits_alto= (valor>>1)& 0x03;
        unsigned char bit_bajo= valor & 0x01;

        tablero[byte_buscado]= tablero[byte_buscado] & 0xFC;
        tablero[byte_buscado]= tablero[byte_buscado] | bits_alto;

        tablero[byte_buscado+1]= tablero[byte_buscado+1] & 0x7F;
        tablero[byte_buscado+1]= tablero[byte_buscado+1] | (bit_bajo<<7);

    } else {
        unsigned char bit_alto= (valor>>2) & 0x01;
        unsigned char bits_bajo= valor & 0x03;

        tablero[byte_buscado]= tablero[byte_buscado] & 0xFE;
        tablero[byte_buscado]= tablero[byte_buscado] | bit_alto;

        tablero[byte_buscado+1]=tablero[byte_buscado+1] & 0x3F;
        tablero[byte_buscado+1]= tablero[byte_buscado+1] | (bits_bajo<<6);
    }
}
//0x07 es 00000111
//0x7F es 01111111
//0x3F es 00111111
//0xFC es 11111100
//0xFE es 11111110
//0x03 es 00000011
//0x01 es 00000001