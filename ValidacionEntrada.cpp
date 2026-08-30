// ==========================================================
// Implementacion del modulo ValidacionEntrada.
// ==========================================================

#include "ValidacionEntrada.h"

#include <iostream>
#include <limits>

using namespace std;

int leerOpcionMenu(int min, int max) {
    // Si los limites llegan invertidos, se intercambian para no
    // quedar con un rango vacio (que volveria la entrada invalida
    // para siempre y causaria un bucle infinito).
    if (min > max) {
        int aux = min;
        min = max;
        max = aux;
    }

    // El prompt mantiene el formato original "Opcion (min-max): ".
    return leerEnteroValidado("Opcion (" + to_string(min) + "-" + to_string(max) + "): ", min, max);
}

int leerEnteroValidado(const string& mensaje, int min) {
    int numero;

    while (true) {
        cout << mensaje;

        if (!(cin >> numero)) {
            // La entrada no fue un numero (letras, simbolos, etc.).

            if (cin.eof()) {
                // La entrada se agoto (p. ej. Ctrl+D): sin un valor
                // legible no tiene sentido seguir repitiendo, asi que
                // se sale del bucle con el limite inferior como
                // valor por defecto.
                cout << endl;
                cout << "Entrada finalizada sin valor valido; se usa " << min << "." << endl;
                return min;
            }

            cin.clear();                                         // limpia el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta el resto de la linea

            cout << "Entrada invalida: ingrese un numero entero." << endl;
            continue;
        }

        // Como la entrada fue un numero valido, se descarta lo que
        // quede pendiente en la linea (normalmente el Enter).
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (numero < min) {
            cout << "Numero fuera de rango: ingrese un valor mayor o igual a " << min << "." << endl;
            continue;
        }

        return numero;
    }
}

int leerEnteroValidado(const string& mensaje, int min, int max) {
    // Si los limites llegan invertidos, se intercambian para no
    // quedar con un rango vacio (que volveria la entrada invalida
    // para siempre y causaria un bucle infinito).
    if (min > max) {
        int aux = min;
        min = max;
        max = aux;
    }

    int numero;

    while (true) {
        cout << mensaje;

        if (!(cin >> numero)) {
            // La entrada no fue un numero (letras, simbolos, etc.).

            if (cin.eof()) {
                // La entrada se agoto (p. ej. Ctrl+D): sin un valor
                // legible no tiene sentido seguir repitiendo, asi que
                // se sale del bucle con el limite inferior como
                // valor por defecto.
                cout << endl;
                cout << "Entrada finalizada sin valor valido; se usa " << min << "." << endl;
                return min;
            }

            cin.clear();                                         // limpia el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta el resto de la linea

            cout << "Entrada invalida: ingrese un numero entero." << endl;
            continue;
        }

        // Como la entrada fue un numero valido, se descarta lo que
        // quede pendiente en la linea (normalmente el Enter).
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (numero < min || numero > max) {
            cout << "Numero fuera de rango: ingrese un valor entre "
                 << min << " y " << max << "." << endl;
            continue;
        }

        return numero;
    }
}

string leerTextoNoVacio(const string& mensaje) {
    while (true) {
        cout << mensaje;

        string texto;
        getline(cin, texto);

        if (cin.eof()) {
            // La entrada se agoto (p. ej. Ctrl+D): sin una linea
            // valida no tiene sentido seguir repitiendo, asi que se
            // sale del bucle con una cadena vacia.
            cout << endl;
            cout << "Entrada finalizada sin valor valido; se devuelve una cadena vacia." << endl;
            return string();
        }

        // Se considera vacia una cadena que este vacia o que
        // contenga unicamente espacios en blanco.
        bool soloEspacios = true;
        for (size_t i = 0; i < texto.size(); ++i) {
            if (texto[i] != ' ' && texto[i] != '\t') {
                soloEspacios = false;
                break;
            }
        }

        if (texto.empty() || soloEspacios) {
            cout << "Entrada invalida: el texto no puede estar vacio." << endl;
            continue;
        }

        return texto;
    }
}