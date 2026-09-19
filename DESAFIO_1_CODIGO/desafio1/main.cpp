#include <iostream>
#include <cstdlib>
#include <ctime>
#include "tablero.h"
#include "juego.h"

using namespace std;

int main() {
    srand(time(NULL));

    int filas = 0, columnas = 0;
    int jugadasUsuario = 0;
    int totalFichasEliminadas = 0;
    int totalCombinaciones = 0;
    int cascadasUltimaJugada = 0;
    int puntos = 0;

    cout << "========================================\n";
    cout << "      BIENVENIDO A SWEET CRUSH\n";
    cout << "========================================\n";

    do {
        cout << "Ingrese el numero de filas (minimo 3): ";
        cin >> filas;
        cout << "Ingrese el numero de columnas (minimo 3): ";
        cin >> columnas;

        if (filas < 3 || columnas < 3) {
            cout << "[ERROR] El tablero debe ser de al menos 3x3. Intente de nuevo.\n\n";
        }
    } while (filas < 3 || columnas < 3);

    unsigned char* tablero = crearTablero(filas, columnas);
    inicializarTableroAleatorio(tablero, filas, columnas);
    ejecutarCascadas(tablero, filas, columnas);

    int opcion = 0;

    while (opcion != 4) {
        cout << "\n========================================";
        cout << "\n          ESTADO DE LA PARTIDA          ";
        cout << "\n========================================";
        cout << "\n- Dimensiones del tablero     : " << filas << " x " << columnas;
        cout << "\n- Eliminaciones del usuario   : " << jugadasUsuario;
        cout << "\n- Total fichas eliminadas     : " << totalFichasEliminadas;
        cout << "\n- Combinaciones detectadas    : " << totalCombinaciones;
        cout << "\n- Cascadas en la ultima jugada: " << cascadasUltimaJugada;
        cout << "\n- Puntuacion total            : " << puntos << " PTS";
        cout << "\n----------------------------------------";

        mostrarTableroNumerado(tablero, filas, columnas);

        cout << "\nOPCIONES:\n";
        cout << "1. Eliminar una ficha (Realizar jugada)\n";
        cout << "2. Ver estado interno en BINARIO (3 bits)\n";
        cout << "3. Redimensionar tablero (Filas / Columnas)\n";
        cout << "4. Salir del juego\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int f, c;
            cout << "\n--- ELIMINAR FICHA ---\n";
            cout << "\nIngrese Fila (1 a " << filas << "): ";
            cin >> f;
            cout << "Ingrese Columna (1 a " << columnas << "): ";
            cin >> c;

            if (f >= 1 && f <= filas && c >= 1 && c <= columnas) {
                guardarFicha(tablero, columnas, f - 1, c - 1, 6);
                jugadasUsuario++;
                totalFichasEliminadas++;
                cascadasUltimaJugada = ejecutarCascadas(tablero, filas, columnas, &totalFichasEliminadas, &totalCombinaciones);

                int puntosTurno = (cascadasUltimaJugada * 100) + 20;
                puntos += puntosTurno;

                cout << "\n[OK] Jugada procesada exitosamente.\n";
                cout << "\n- Cascadas generadas: " << cascadasUltimaJugada;
                cout << "\n- Puntos obtenidos  : +" << puntosTurno << " PTS\n";
            } else {
                cout << "\n[ERROR] Coordenadas fuera de rango.\n";
            }

        } else if (opcion == 2) {
            mostrarTableroBinario(tablero, filas, columnas);

        } else if (opcion == 3) {
            int subOpcion;
            cout << "\n--- REDIMENSIONAR TABLERO ---\n";
            cout << "\n1. Agregar Fila\n2. Eliminar Fila\n3. Agregar Columna\n4. Eliminar Columna\nSeleccione: ";
            cin >> subOpcion;

            if (subOpcion == 2 && filas <= 3) {
                cout << "\n[ERROR] Requiere minimo 3 filas.\n";
                continue;
            }
            if (subOpcion == 4 && columnas <= 3) {
                cout << "\n[ERROR] Requiere minimo 3 columnas.\n";
                continue;
            }

            int pos;
            if (subOpcion == 1 || subOpcion == 2) {
                int maxPos = (subOpcion == 1) ? filas + 1 : filas;
                cout << "Ingrese posicion de la fila (1 a " << maxPos << "): ";
                cin >> pos;

                if (pos >= 1 && pos <= maxPos) {
                    if (subOpcion == 1) agregarFila(&tablero, &filas, columnas, pos - 1);
                    else eliminarFila(&tablero, &filas, columnas, pos - 1);
                } else {
                    cout << "\n[ERROR] Posicion invalida.\n";
                }

            } else if (subOpcion == 3 || subOpcion == 4) {
                int maxPos = (subOpcion == 3) ? columnas + 1 : columnas;
                cout << "Ingrese posicion de la columna (1 a " << maxPos << "): ";
                cin >> pos;

                if (pos >= 1 && pos <= maxPos) {
                    if (subOpcion == 3) agregarColumna(&tablero, filas, &columnas, pos - 1);
                    else eliminarColumna(&tablero, filas, &columnas, pos - 1);
                } else {
                    cout << "\n[ERROR] Posicion invalida.\n";
                }
            }
            cascadasUltimaJugada = ejecutarCascadas(tablero, filas, columnas, &totalFichasEliminadas, &totalCombinaciones);

        } else if (opcion == 4) {
            cout << "\nFinalizando partida. Puntuacion final: " << puntos << " PTS\n";
        } else {
            cout << "\n[ERROR] Opcion no valida.\n";
        }
    }
    destruirTablero(tablero);
    cout << "[OK] Memoria liberada correctamente. Partida terminada.\n";

    return 0;
}