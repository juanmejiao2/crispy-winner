#include "tablero.h"

unsigned char* crearTablero(int filas, int columnas){
    int bits_totales= filas*columnas*3;
    int bytes_necesarios=(bits_totales+7)/8;

    unsigned char* tablero = new unsigned char[bytes_necesarios];

    for (int i = 0; i < bytes_necesarios; i++) {
        tablero[i] = 0;
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
        unsigned char bitmask= ~(0x07<<desplazamiento);
        tablero[byte_buscado] &= bitmask;
        tablero[byte_buscado] |= (valor<<desplazamiento);

    } else if (desplazamiento_bit==6){
        unsigned char bits_alto= (valor>>1)& 0x03;
        unsigned char bit_bajo= valor & 0x01;

        tablero[byte_buscado] &= 0xFC;
        tablero[byte_buscado] |= bits_alto;
        tablero[byte_buscado+1]&= 0x7F;
        tablero[byte_buscado+1] |= (bit_bajo<<7);

    } else {
        unsigned char bit_alto= (valor>>2) & 0x01;
        unsigned char bits_bajo= valor & 0x03;

        tablero[byte_buscado] &= 0xFE;
        tablero[byte_buscado]|= bit_alto;
        tablero[byte_buscado+1] &= 0x3F;
        tablero[byte_buscado+1] |= (bits_bajo<<6);
    }
}
//0x07 es 00000111
//0x7F es 01111111
//0x3F es 00111111
//0xFC es 11111100
//0xFE es 11111110
//0x03 es 00000011
//0x01 es 00000001

void agregarFila(unsigned char** tablero, int* filas, int columnas, int posicion_fila){
    if (posicion_fila<0|| posicion_fila> *filas) return;

    int filas_nuevas= *filas + 1;
    int bytes_nuevos= (filas_nuevas * columnas * 3 + 7)/8;
    unsigned char* nuevo_tablero= new unsigned char[bytes_nuevos]();

    for (int f=0; f< posicion_fila;f++){
        for (int c=0;c<columnas;c++){
            guardarFicha(nuevo_tablero, columnas, f, c, obtenerFicha(*tablero, columnas, f ,c));
        }
    }
    for (int c = 0; c < columnas; c++) {
        guardarFicha(nuevo_tablero, columnas, posicion_fila, c, 6);
    }
    for(int f= posicion_fila; f< *filas ;f++){
        for(int c=0;c<columnas;c++){
            guardarFicha(nuevo_tablero, columnas, f+1, c, obtenerFicha(*tablero, columnas, f ,c));
        }
    }
    delete[] *tablero;
    *tablero= nuevo_tablero;
    *filas=filas_nuevas;
}

void eliminarFila(unsigned char** tablero, int* filas, int columnas, int posicion_fila){
    if (posicion_fila < 0 || posicion_fila >= *filas || *filas <= 1) return;

    int filas_nuevas = *filas - 1;
    int bytes_actuales = (*filas * columnas * 3 + 7) / 8;
    int bytes_necesarios_nuevos = (filas_nuevas * columnas * 3 + 7) / 8;

    float ocupacion = (float)bytes_necesarios_nuevos / (float)bytes_actuales;

    if (ocupacion < 0.65f) {
        unsigned char* nuevo_tablero = new unsigned char[bytes_necesarios_nuevos]();

        for (int f = 0; f < filas_nuevas; f++) {
            int f_origen = (f < posicion_fila) ? f : f + 1;
            for (int c = 0; c < columnas; c++) {
                unsigned char ficha = obtenerFicha(*tablero, columnas, f_origen, c);
                guardarFicha(nuevo_tablero, columnas, f, c, ficha);
            }
        }
        delete[] *tablero;
        *tablero = nuevo_tablero;

    } else {
        for (int f = 0; f < filas_nuevas; f++) {
            int f_origen = (f < posicion_fila) ? f : f + 1;
            for (int c = 0; c < columnas; c++) {
                unsigned char ficha = obtenerFicha(*tablero, columnas, f_origen, c);
                guardarFicha(*tablero, columnas, f, c, ficha);
            }
        }
    }

    *filas = filas_nuevas;
}

void agregarColumna(unsigned char** tablero, int filas, int* columnas, int posicion_columna){
    if (posicion_columna < 0 || posicion_columna > *columnas) return;

    int columnas_nuevas = *columnas + 1;
    int bytes_nuevos = (filas * columnas_nuevas * 3 + 7) / 8;
    unsigned char* nuevo_tablero = new unsigned char[bytes_nuevos]();

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < posicion_columna; c++) {
            guardarFicha(nuevo_tablero, columnas_nuevas, f, c, obtenerFicha(*tablero, *columnas, f, c));
        }
        guardarFicha(nuevo_tablero, columnas_nuevas, f, posicion_columna, 6);
        for (int c = posicion_columna; c < *columnas; c++) {
            guardarFicha(nuevo_tablero, columnas_nuevas, f, c + 1, obtenerFicha(*tablero, *columnas, f, c));
        }
    }
    delete[] *tablero;
    *tablero= nuevo_tablero;
    *columnas= columnas_nuevas;
}

void eliminarColumna(unsigned char** tablero, int filas, int* columnas, int posicion_columna) {
    if (posicion_columna < 0 || posicion_columna >= *columnas || *columnas <= 1) return;

    int columnas_nuevas = *columnas - 1;
    int bytes_actuales = (filas * *columnas * 3 + 7) / 8;
    int bytes_necesarios_nuevos = (filas * columnas_nuevas * 3 + 7) / 8;

    float ocupacion = (float)bytes_necesarios_nuevos / (float)bytes_actuales;

    if (ocupacion < 0.65f) {
        unsigned char* nuevo_tablero = new unsigned char[bytes_necesarios_nuevos]();

        for (int f = 0; f < filas; f++) {
            for (int c = 0; c < posicion_columna; c++) {
                guardarFicha(nuevo_tablero, columnas_nuevas, f, c, obtenerFicha(*tablero, *columnas, f, c));
            }
            for (int c = posicion_columna + 1; c < *columnas; c++) {
                guardarFicha(nuevo_tablero, columnas_nuevas, f, c - 1, obtenerFicha(*tablero, *columnas, f, c));
            }
        }
        delete[] *tablero;
        *tablero = nuevo_tablero;

    } else {
        for(int f=0; f<filas ;f++){
            for (int c = 0; c < columnas_nuevas; c++) {
                int c_origen = (c < posicion_columna) ? c : c + 1;
                unsigned char ficha = obtenerFicha(*tablero, *columnas, f, c_origen);
                guardarFicha(*tablero, *columnas, f, c, ficha);
            }
        }
    }
    *columnas= columnas_nuevas;
}