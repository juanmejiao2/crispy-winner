#include "juego.h"
#include "tablero.h"
#include <iostream>
#include <cstdlib>

using namespace std;

bool detectarYEliminar(unsigned char* tablero, int filas, int columnas, bool* marcados) {
    if (!tablero || !marcados || filas < 3 || columnas < 3) return false;

    int total_casillas = filas * columnas;

    for (int i = 0; i < total_casillas; i++) {
        marcados[i] = false;
    }

    bool hubo_combinacion = false;

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas - 2; c++) {
            unsigned char f1 = obtenerFicha(tablero, columnas, f, c);
            unsigned char f2 = obtenerFicha(tablero, columnas, f, c + 1);
            unsigned char f3 = obtenerFicha(tablero, columnas, f, c + 2);

            if (f1 < 6 && f1 == f2 && f2 == f3) {
                int i1 = f * columnas + c;
                int i2 = i1 + 1;
                int i3 = i1 + 2;

                if (i3 < total_casillas) {
                    marcados[i1] = true;
                    marcados[i2] = true;
                    marcados[i3] = true;
                    hubo_combinacion = true;
                }
            }
        }
    }
    for (int f = 0; f < filas - 2; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char f1 = obtenerFicha(tablero, columnas, f, c);
            unsigned char f2 = obtenerFicha(tablero, columnas, f + 1, c);
            unsigned char f3 = obtenerFicha(tablero, columnas, f + 2, c);

            if (f1 < 6 && f1 == f2 && f2 == f3) {
                int i1 = f * columnas + c;
                int i2 = (f + 1) * columnas + c;
                int i3 = (f + 2) * columnas + c;

                if (i3 < total_casillas) {
                    marcados[i1] = true;
                    marcados[i2] = true;
                    marcados[i3] = true;
                    hubo_combinacion = true;
                }
            }
        }
    }
    if (hubo_combinacion) {
        for (int i = 0; i < total_casillas; i++) {
            if (marcados[i]) {
                int f = i / columnas;
                int c = i % columnas;
                guardarFicha(tablero, columnas, f, c, 6); // 6 = Vacio
            }
        }
    }

    return hubo_combinacion;
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

int ejecutarCascadas(unsigned char* tablero, int filas, int columnas) {
    if (!tablero || filas < 3 || columnas < 3) return 0;

    int total_casillas = filas * columnas;
    bool* marcados = new bool[total_casillas]();
    int contador_cascadas = 0;

    aplicarGravedad(tablero, filas, columnas);
    rellenarVacios(tablero, filas, columnas);

    while (detectarYEliminar(tablero, filas, columnas, marcados)) {
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
            if (ficha == 6) {
                cout << "[ . ] ";
            } else {
                cout << "[ " << (int)ficha << " ] ";
            }
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
    cout << "\n";
    cout << "   +";
    for (int c = 0; c < columnas; c++) {
        cout << "---+";
    }
    cout << "\n";

    for (int f = 0; f < filas; f++) {
        int numFila = f + 1;
        if (numFila < 10) cout << "F" << numFila << " |";
        else cout << "F" << numFila << "|";

        for (int c = 0; c < columnas; c++) {
            unsigned char ficha = obtenerFicha(tablero, columnas, f, c);
            if (ficha == 6) {
                cout << " . |";
            } else {
                cout << " " << (int)ficha << " |";
            }
        }
        cout << "\n";
        cout << "   +";
        for (int c = 0; c < columnas; c++) {
            cout << "---+";
        }
        cout << "\n";
    }
}