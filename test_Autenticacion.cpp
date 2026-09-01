// ==========================================================
// Pruebas unitarias del modulo Autenticacion.
// Curso: EIF207 - Estructuras de Datos
//
// Usa la clase Usuario REAL (con contrasena, rol y
// validarContrasena()) y una ListaUsuarios de prueba cargada en
// memoria con 3 usuarios.
//
// Casos cubiertos:
//   1. Login exitoso con credenciales correctas.
//   2. Login fallido con contrasena incorrecta.
//   3. Login fallido con ID de usuario inexistente.
//   4. verificarPermiso rechaza a USUARIO_NORMAL cuando se
//      requiere ADMINISTRADOR (y acepta a un administrador).
//
// Nota: NO se prueban automaticamente leerContrasenaOculta() ni
// leerOpcionMenu(), porque dependen de la entrada interactiva del
// teclado; ambas se prueban de forma manual corriendo el programa.
// ==========================================================

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <functional>
#include <cassert>

#include "Usuario.h"
#include "ListaUsuarios.h"
#include "Autenticacion.h"

using namespace std;

// ==========================================================
// Utilidades de flujo (RAII: restauran cin/cout aunque haya
// excepcion, para que una prueba fallida no deje el estado
// corrupto).
// ==========================================================

// Redirige temporalmente cin hacia una cadena con el "script" de
// entrada del login: la primera linea es el ID y la segunda la
// contrasena (igual que escribirlas en consola).
class EntradaSimulada {
private:
    istringstream script;
    streambuf* cinAnterior;

public:
    EntradaSimulada(const string& texto)
        : script(texto), cinAnterior(cin.rdbuf(script.rdbuf())) {}

    ~EntradaSimulada() { cin.rdbuf(cinAnterior); }

    // No se copia: cada instancia posee el rdbuf de cin.
    EntradaSimulada(const EntradaSimulada&) = delete;
    EntradaSimulada& operator=(const EntradaSimulada&) = delete;
};

// Ejecuta una accion capturando todo lo que escriba en cout.
string capturarSalida(const function<void()>& accion) {
    stringstream buffer;
    streambuf* coutAnterior = cout.rdbuf(buffer.rdbuf());
    try {
        accion();
    } catch (...) {
        cout.rdbuf(coutAnterior); // restaura antes de propagar
        throw;
    }
    cout.rdbuf(coutAnterior);
    return buffer.str();
}

// Redirige cout hacia un buffer mientras la instancia viva. A
// diferencia de capturarSalida(), el texto capturado se puede leer
// aunque dentro se lance una excepcion: el buffer conserva lo que se
// escribio antes del throw.
class CapturaSalida {
private:
    stringstream buffer;
    streambuf* coutAnterior;

public:
    CapturaSalida() : coutAnterior(cout.rdbuf(buffer.rdbuf())) {}
    ~CapturaSalida() { cout.rdbuf(coutAnterior); }

    string str() const { return buffer.str(); }

    CapturaSalida(const CapturaSalida&) = delete;
    CapturaSalida& operator=(const CapturaSalida&) = delete;
};

// ==========================================================
// Construccion de la lista de prueba (3 usuarios en memoria).
// ==========================================================
void construirListaDePrueba(ListaUsuarios& lista) {
    assert(lista.insertar(1, "Ana", "clave123", Rol::ADMINISTRADOR));
    assert(lista.insertar(2, "Beto", "otraClave", Rol::USUARIO_NORMAL));
    assert(lista.insertar(3, "Carla", "claveABC", Rol::USUARIO_NORMAL));
    assert(lista.contar() == 3);
}

// ==========================================================
// Caso 1: login exitoso con credenciales correctas.
// ==========================================================
void probarLoginExitoso(Autenticacion& auth, ListaUsuarios& lista) {
    EntradaSimulada entrada("1\nclave123\n");

    Usuario* usuario = nullptr;
    // Silencia los prompts "ID de usuario: / Contrasena:" de la salida.
    capturarSalida([&]() { usuario = auth.iniciarSesion(lista); });

    assert(usuario != nullptr);
    assert(usuario->getId() == 1);
    assert(usuario->getNombre() == "Ana");
    assert(usuario->getRol() == Rol::ADMINISTRADOR);
    cout << "[OK]  login exitoso: credenciales correctas devuelven al Usuario" << endl;
}

// ==========================================================
// Caso 2: login fallido con contrasena incorrecta.
// ==========================================================
void probarLoginContrasenaIncorrecta(Autenticacion& auth, ListaUsuarios& lista) {
    EntradaSimulada entrada("1\nclaveEquivocada\n");

    Usuario* usuario = nullptr;
    string salida = capturarSalida([&]() { usuario = auth.iniciarSesion(lista); });

    assert(usuario == nullptr);
    // Mensaje de error generico: no debe revelar si fallo el ID o
    // la contrasena.
    assert(salida.find("ID o contrasena incorrectos") != string::npos);
    cout << "[OK]  login fallido: contrasena incorrecta devuelve nullptr" << endl;
}
// ==========================================================
// Caso 3: login fallido con ID de usuario inexistente.
// ==========================================================
void probarLoginIdInexistente(Autenticacion& auth, ListaUsuarios& lista) {
    EntradaSimulada entrada("999\ncualquierClave\n");

    Usuario* usuario = nullptr;
    string salida = capturarSalida([&]() { usuario = auth.iniciarSesion(lista); });

    assert(usuario == nullptr);
    assert(salida.find("ID o contrasena incorrectos") != string::npos);
    cout << "[OK]  login fallido: ID inexistente devuelve nullptr" << endl;
}

// ==========================================================
// Caso 4: verificarPermiso rechaza a un USUARIO_NORMAL cuando se
// requiere ADMINISTRADOR. La accion protegida no debe ejecutarse.
// ==========================================================
void probarPermisoDenegado(ListaUsuarios& lista) {
    Usuario* normal = lista.buscar(2); // Beto, USUARIO_NORMAL
    assert(normal != nullptr);
    assert(normal->getRol() == Rol::USUARIO_NORMAL);

    bool lanzoExcepcion = false;
    bool accionEjecutada = false; // bandera de la accion protegida
    string salida;

    {
        CapturaSalida captura;
        try {
            verificarPermiso(normal, Rol::ADMINISTRADOR);
            accionEjecutada = true; // no se debe llegar aqui
        } catch (const runtime_error&) {
            lanzoExcepcion = true;
        }
        salida = captura.str(); // conserva lo escrito antes del throw
    }

    assert(lanzoExcepcion);
    assert(!accionEjecutada); // la accion protegida NO continuo
    assert(salida.find("Acceso denegado") != string::npos);
    cout << "[OK]  verificarPermiso: USUARIO_NORMAL rechazado para rol ADMINISTRADOR" << endl;
}

// ==========================================================
// Caso complementario: verificarPermiso SÍ deja pasar a un
// administrador cuando el rol requerido es ADMINISTRADOR.
// ==========================================================
void probarPermisoConcedido(ListaUsuarios& lista) {
    Usuario* admin = lista.buscar(1); // Ana, ADMINISTRADOR
    assert(admin != nullptr);
    assert(admin->getRol() == Rol::ADMINISTRADOR);

    bool accionEjecutada = false;
    string salida;

    {
        CapturaSalida captura;
        try {
            verificarPermiso(admin, Rol::ADMINISTRADOR);
            accionEjecutada = true;
        } catch (const runtime_error&) {
            // No deberia ocurrir: al administrador se le permite pasar.
        }
        salida = captura.str();
    }

    assert(accionEjecutada);
    assert(salida.find("Acceso denegado") == string::npos);
    cout << "[OK]  verificarPermiso: a ADMINISTRADOR se le permite continuar" << endl;
}

// ==========================================================
// main
// ==========================================================
int main() {
    ListaUsuarios lista;
    construirListaDePrueba(lista);

    Autenticacion auth;

#ifdef _WIN32
    // En Windows, leerContrasenaOculta() usa _getch(), que lee del
    // teclado real y no puede simularse redirigiendo cin; esos casos
    // de login se prueban de forma manual (igual que en la nota del
    // enunciado). verificarPermiso no depende de la entrada.
    cout << "[--]  casos de iniciarSesion omitidos en Windows (entrada manual)" << endl;
#else
    probarLoginExitoso(auth, lista);
    probarLoginContrasenaIncorrecta(auth, lista);
    probarLoginIdInexistente(auth, lista);
#endif

    probarPermisoDenegado(lista);
    probarPermisoConcedido(lista);

    cout << endl << "Todos los tests de Autenticacion pasaron correctamente." << endl;
    return 0;
}