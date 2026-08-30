// ==========================================================
// Implementacion del modulo Autenticacion.
// ==========================================================

#include "Autenticacion.h"

#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

// ==========================================================
// Compilacion condicional (#ifdef _WIN32)
// ----------------------------------------------------------
// El equipo trabaja tanto en Windows como en Linux, y cada sistema
// operativo exige una primitiva distinta para ocultar la contrasena
// mientras el usuario la escribe:
//
//   - Windows:  conio.h, con la funcion _getch(); lee tecla por
//               tecla sin que la consola haga eco.
//   - Linux/Mac: termios.h; desactiva el eco de la terminal antes
//               de leer (y lo reactiva al terminar).
//
// #ifdef _WIN32 decide en tiempo de compilacion que bloque se usa,
// de modo que el mismo codigo sirve para ambos sistemas.
// ==========================================================

#ifdef _WIN32

// ---------- Implementacion para Windows ----------
#include <conio.h>

string leerContrasenaOculta() {
    string contrasena;
    int tecla;

    while (true) {
        tecla = _getch(); // lee una tecla sin mostrarla en pantalla

        if (tecla == '\r') {       // Enter finaliza la contrasena
            cout << endl;
            break;
        }

        if (tecla == '\b') {       // Backspace borra el ultimo caracter
            if (!contrasena.empty()) {
                contrasena.pop_back();
            }
            continue;
        }

        contrasena.push_back(static_cast<char>(tecla));
    }

    return contrasena;
}

#else

// ---------- Implementacion para Linux / macOS ----------
#include <termios.h>
#include <unistd.h>

namespace {

/**
 * Guard RAII de la terminal: desactiva el eco en el constructor y
 * lo restaura en el destructor. Al vivir en la pila, su destructor
 * se ejecuta siempre (tambien durante el desapilamiento causado por
 * una excepcion), de modo que la terminal nunca queda sin eco ni se
 * queda con el eco apagado involuntariamente.
 */
class DesactivadorEco {
private:
    termios estadoOriginal;
    bool ecoDesactivado;

public:
    DesactivadorEco() : estadoOriginal(), ecoDesactivado(false) {
        // tcgetattr falla si la entrada estandar no es una terminal
        // (por ejemplo, con entrada redirigida); en ese caso no hay
        // eco que ocultar y se lee la linea normalmente.
        if (tcgetattr(STDIN_FILENO, &estadoOriginal) != 0) {
            return;
        }

        termios estadoOculto = estadoOriginal;
        estadoOculto.c_lflag &= ~ECHO; // desactiva el eco de la terminal

        if (tcsetattr(STDIN_FILENO, TCSANOW, &estadoOculto) == 0) {
            ecoDesactivado = true;
        }
    }

    ~DesactivadorEco() {
        if (ecoDesactivado) {
            // Vuelve a activar el eco tal como estaba antes de leer.
            tcsetattr(STDIN_FILENO, TCSANOW, &estadoOriginal);
        }
    }

    // No se permite copiar el guard: cada instancia administra el
    // estado de la terminal de forma unica.
    DesactivadorEco(const DesactivadorEco&) = delete;
    DesactivadorEco& operator=(const DesactivadorEco&) = delete;
};

} // namespace anonimo

string leerContrasenaOculta() {
    DesactivadorEco ocultarEco; // desactiva el eco; se restaura al salir

    string contrasena;
    getline(cin, contrasena);

    // Como el eco esta apagado, el Enter que presiono el usuario no
    // se ve reflejado: se emite un salto de linea para que el
    // siguiente mensaje quede en una linea limpia.
    cout << endl;

    return contrasena;
}

#endif // _WIN32

// ==========================================================
// Clase Autenticacion (comun a ambos sistemas operativos)
// ==========================================================

Usuario* Autenticacion::iniciarSesion(ListaUsuarios& usuarios) {
    cout << "ID de usuario: ";
    int id;
    cin >> id;

    // Descarta el salto de linea que dejo 'cin >> id', para que el
    // getline() interno de leerContrasenaOculta() lea la contrasena
    // desde su inicio y no una linea vacia.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Contrasena: ";
    string contrasena = leerContrasenaOculta();

    Usuario* usuario = usuarios.buscar(id);
    if (usuario == nullptr || !usuario->validarContrasena(contrasena)) {
        // Error generico a proposito: no se informa si lo que fallo
        // fue el ID o la contrasena (seguridad).
        cout << "ID o contrasena incorrectos." << endl;
        return nullptr;
    }

    return usuario;
}

void verificarPermiso(Usuario* usuario, Rol rolRequerido) {
    if (usuario == nullptr || usuario->getRol() != rolRequerido) {
        cout << "Acceso denegado." << endl;

        // Interrumpe el flujo para que la accion protegida no
        // continue; quien llama debe capturarla con try/catch.
        throw runtime_error("Acceso denegado.");
    }
}