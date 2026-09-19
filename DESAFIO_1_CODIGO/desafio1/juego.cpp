#include "juego.h"
#include "tablero.h"
#include <iostream>
#include <cstdlib>

using namespace std;

char obtenerCaracterFicha(unsigned char ficha) {
    switch(ficha) {
    case 0: return '#'; // 000
    case 1: return '%'; // 001
    case 2: return 'x'; // 010
    case 3: return '&'; // 011
    case 4: return '?'; // 100
    case 5: return '~'; // 101
    case 6: return 'o'; // 110 (Estado libre / vacío)
    default: return 'C'; // 111 (Estado especial)
    }
}

bool detectarYEliminar(unsigned char* tablero, int filas, int columnas, bool* marcados, int* acumFichas, int* acumCombos) {
    if (!tablero || !marcados || filas < 3 || columnas < 3) return false;

    int total_casillas = filas * columnas;
    for (int i = 0; i < total_casillas; i++) marcados[i] = false;

    int combosEnRonda = 0;

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas - 2; c++) {
            unsigned char f1 = obtenerFicha(tablero, columnas, f, c);
            unsigned char f2 = obtenerFicha(tablero, columnas, f, c + 1);
            unsigned char f3 = obtenerFicha(tablero, columnas, f, c + 2);

            if (f1 < 6 && f1 == f2 && f2 == f3) {
                marcados[f * columnas + c] = true;
                marcados[f * columnas + (c + 1)] = true;
                marcados[f * columnas + (c + 2)] = true;
                combosEnRonda++;
            }
        }
    }
    for (int f = 0; f < filas - 2; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char f1 = obtenerFicha(tablero, columnas, f, c);
            unsigned char f2 = obtenerFicha(tablero, columnas, f + 1, c);
            unsigned char f3 = obtenerFicha(tablero, columnas, f + 2, c);

            if (f1 < 6 && f1 == f2 && f2 == f3) {
                marcados[f * columnas + c] = true;
                marcados[(f + 1) * columnas + c] = true;
                marcados[(f + 2) * columnas + c] = true;
                combosEnRonda++;
            }
        }
    }
    int fichasBorradasRonda = 0;
    if (combosEnRonda > 0) {
        for (int i = 0; i < total_casillas; i++) {
            if (marcados[i]) {
                fichasBorradasRonda++;
                int f = i / columnas;
                int c = i % columnas;
                guardarFicha(tablero, columnas, f, c, 6); // 6 = Vacio
            }
        }
    }

    if (acumFichas) *acumFichas += fichasBorradasRonda;
    if (acumCombos) *acumCombos += combosEnRonda;

    return (combosEnRonda > 0);
}

void aplicarGravedad(unsigned char* tablero, int filas, int columnas) {
    if (!tablero || filas <= 0 || columnas <= 0) return;

    for (int c = 0; c < columnas; c++) {
        int destino = filas - 1;
        for (int f = filas - 1; f >= 0; f--) {
            unsigned char ficha = obtenerFicha(tablero, columnas, f, c);
            if (ficha != 6) {
                if (f != destino) {
                    guardarFicha(tablero, columnas, destino, c, ficha);
                    guardarFicha(tablero, columnas, f, c, 6);
                }
                destino--;
            }
        }
    }
}

void rellenarVacios(unsigned char* tablero, int filas, int columnas) {
    if (!tablero || filas <= 0 || columnas <= 0) return;

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (obtenerFicha(tablero, columnas, f, c) == 6) {
                unsigned char nueva_ficha = (unsigned char)(rand() % 6);
                guardarFicha(tablero, columnas, f, c, nueva_ficha);
            }
        }
    }
}

int ejecutarCascadas(unsigned char* tablero, int filas, int columnas, int* acumFichas, int* acumCombos) {
    if (!tablero || filas < 3 || columnas < 3) return 0;

    int total_casillas = filas * columnas;
    bool* marcados = new bool[total_casillas]();
    int contador_cascadas = 0;

    aplicarGravedad(tablero, filas, columnas);
    rellenarVacios(tablero, filas, columnas);

    while (detectarYEliminar(tablero, filas, columnas, marcados, acumFichas, acumCombos)) {
        contador_cascadas++;
        aplicarGravedad(tablero, filas, columnas);
        rellenarVacios(tablero, filas, columnas);
    }

    delete[] marcados;
    return contador_cascadas;
}

void inicializarTableroAleatorio(unsigned char* tablero, int filas, int columnas) {
    if (!tablero) return;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char ficha = (unsigned char)(rand() % 6);
            guardarFicha(tablero, columnas, f, c, ficha);
        }
    }
}

void mostrarTableroFichas(const unsigned char* tablero, int filas, int columnas) {
    cout << "\n=== TABLERO (FICHAS) ===\n";
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char ficha = obtenerFicha(tablero, columnas, f, c);
            if (ficha == 6) cout << "[ . ] ";
            else cout << "[ " << (int)ficha << " ] ";
        }
        cout << "\n";
    }
}

void mostrarTableroBinario(const unsigned char* tablero, int filas, int columnas) {
    cout << "\n=== TABLERO (BINARIO 3 BITS) ===\n";
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char ficha = obtenerFicha(tablero, columnas, f, c);

            char b2 = ((ficha >> 2) & 1) + '0';
            char b1 = ((ficha >> 1) & 1) + '0';
            char b0 = (ficha & 1) + '0';

            cout << "[" << b2 << b1 << b0 << "] ";
        }
        cout << "\n";
    }
}

void mostrarTableroNumerado(const unsigned char* tablero, int filas, int columnas) {
    if (!tablero) return;

    cout << "\n    ";
    for (int c = 1; c <= columnas; c++) {
        if (c < 10) cout << " C" << c << " ";
        else cout << "C" << c << " ";
    }
    cout << "\n   +";
    for (int c = 0; c < columnas; c++) cout << "---+";
    cout << "\n";

    for (int f = 0; f < filas; f++) {
        int numFila = f + 1;
        if (numFila < 10) cout << "F" << numFila << " |";
        else cout << "F" << numFila << "|";

        for (int c = 0; c < columnas; c++) {
            unsigned char ficha = obtenerFicha(tablero, columnas, f, c);
            if (ficha == 6) cout << " . |";
            else cout << " " << obtenerCaracterFicha(ficha) << " |";
        }
        cout << "\n   +";
        for (int c = 0; c < columnas; c++) cout << "---+";
        cout << "\n";
    }
}