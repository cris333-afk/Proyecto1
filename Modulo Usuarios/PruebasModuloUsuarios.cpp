// ==========================================================
// Modulo Usuarios - Sistema Gestor de Flujos de Trabajo
// Curso: EIF207 - Estructuras de Datos
// Autor: Cesar
//
// Pruebas manuales (con assert) que cubren exactamente los casos
// de prueba listados en la tarjeta del proyecto:
//  1. Lista vacia
//  2. Agregar primer usuario / agregar multiples
//  3. Eliminar cabeza, cola y nodo intermedio
//  4. Buscar usuario existente e inexistente
//  5. Modificar usuario
//  6. Recorrer en ambos sentidos
//  7. Intentar insertar un ID repetido.
//
// Bitacora de IA: estas pruebas fueron generadas con asistencia de
// Claude (Anthropic) como parte del cumplimiento del criterio
// "Integracion de IA (QA)" de la rubrica.
// ==========================================================

#include "ListaUsuarios.h"
#include <iostream>
#include <cassert>

using namespace std;

void separador(const string& titulo) {
    cout << "\n=== " << titulo << " ===" << endl;
}

// ============================================================
// PRUEBA 1: Lista vacia
// ============================================================

void prueba_listaVacia() {
    separador("Prueba 1: Lista vacia");

    ListaUsuarios lista;

    assert(lista.estaVacia() == true);
    assert(lista.contar() == 0);
    assert(lista.buscar(100) == nullptr);
    assert(lista.eliminar(100) == false);

    cout << "OK: una lista recien creada esta vacia y maneja "
            "busquedas/eliminaciones sin fallar."
         << endl;
}

// ============================================================
// PRUEBA 2: Agregar usuarios
// ============================================================

void prueba_agregarUsuarios() {
    separador("Prueba 2: Agregar primer usuario y multiples usuarios");

    ListaUsuarios lista;

    assert(lista.insertar(101, "Ana Rojas") == true);
    assert(lista.estaVacia() == false);
    assert(lista.contar() == 1);

    assert(lista.insertar(102, "Bruno Vega") == true);
    assert(lista.insertar(103, "Carla Mora") == true);

    assert(lista.contar() == 3);

    // Verificar el orden y enlaces
    Usuario* usuario101 = lista.buscar(101);
    Usuario* usuario102 = lista.buscar(102);
    Usuario* usuario103 = lista.buscar(103);

    assert(usuario101 != nullptr);
    assert(usuario102 != nullptr);
    assert(usuario103 != nullptr);

    // Adelante
    assert(usuario101->getSiguiente() == usuario102);
    assert(usuario102->getSiguiente() == usuario103);
    assert(usuario103->getSiguiente() == nullptr);

    // Atras
    assert(usuario103->getAnterior() == usuario102);
    assert(usuario102->getAnterior() == usuario101);
    assert(usuario101->getAnterior() == nullptr);

    cout << "OK: se agregaron 3 usuarios y los enlaces "
            "anterior/siguiente son correctos."
         << endl;

    lista.mostrarAdelante();
}

// ============================================================
// PRUEBA 3: ID duplicado
// ============================================================

void prueba_idDuplicado() {
    separador("Prueba 3: Intentar insertar un ID repetido");

    ListaUsuarios lista;

    assert(lista.insertar(201, "Diego Soto") == true);

    bool resultado = lista.insertar(201, "Otro Usuario");

    assert(resultado == false);
    assert(lista.contar() == 1);

    // Verificar que el usuario original sigue intacto
    Usuario* usuario = lista.buscar(201);

    assert(usuario != nullptr);
    assert(usuario->getNombre() == "Diego Soto");

    cout << "OK: el sistema rechazo el ID duplicado 201."
         << endl;
}

// ============================================================
// PRUEBA 4: Buscar
// ============================================================

void prueba_buscar() {
    separador("Prueba 4: Buscar usuario existente e inexistente");

    ListaUsuarios lista;

    lista.insertar(301, "Elena Duarte");

    Usuario* encontrado = lista.buscar(301);

    assert(encontrado != nullptr);
    assert(encontrado->getNombre() == "Elena Duarte");

    Usuario* noEncontrado = lista.buscar(999);

    assert(noEncontrado == nullptr);

    cout << "OK: busqueda de usuario existente e inexistente "
            "funciona correctamente."
         << endl;
}

// ============================================================
// PRUEBA 5: Modificar
// ============================================================

void prueba_modificar() {
    separador("Prueba 5: Modificar usuario");

    ListaUsuarios lista;

    lista.insertar(401, "Fabian Leon");

    bool resultado =
        lista.modificar(401, "Fabian Leon Jimenez", Rol::ADMINISTRADOR);

    assert(resultado == true);

    Usuario* usuario = lista.buscar(401);

    assert(usuario != nullptr);
    assert(usuario->getNombre() == "Fabian Leon Jimenez");
    assert(usuario->getRol() == Rol::ADMINISTRADOR);

    // Intentar modificar un usuario inexistente
    assert(lista.modificar(999, "No Existe") == false);

    cout << "OK: los datos del usuario 401 se modificaron "
            "correctamente."
         << endl;
}

// ============================================================
// PRUEBA 6: Eliminar cabeza, cola e intermedio
// ============================================================

void prueba_eliminarCabezaColaIntermedio() {
    separador("Prueba 6: Eliminar cabeza, cola y nodo intermedio");

    // --------------------------------------------------------
    // Eliminar cabeza
    // --------------------------------------------------------

    ListaUsuarios lista;

    lista.insertar(501, "Usuario Cabeza");
    lista.insertar(502, "Usuario Intermedio");
    lista.insertar(503, "Usuario Cola");

    assert(lista.eliminar(501) == true);

    assert(lista.buscar(501) == nullptr);
    assert(lista.contar() == 2);

    Usuario* usuario502 = lista.buscar(502);
    Usuario* usuario503 = lista.buscar(503);

    assert(usuario502 != nullptr);
    assert(usuario503 != nullptr);

    // 502 debe ser ahora la cabeza
    assert(usuario502->getAnterior() == nullptr);
    assert(usuario502->getSiguiente() == usuario503);

    // --------------------------------------------------------
    // Eliminar cola
    // --------------------------------------------------------

    assert(lista.eliminar(503) == true);

    assert(lista.buscar(503) == nullptr);
    assert(lista.contar() == 1);

    usuario502 = lista.buscar(502);

    assert(usuario502 != nullptr);
    assert(usuario502->getAnterior() == nullptr);
    assert(usuario502->getSiguiente() == nullptr);

    // --------------------------------------------------------
    // Eliminar intermedio
    // --------------------------------------------------------

    ListaUsuarios lista2;

    lista2.insertar(601, "A");
    lista2.insertar(602, "B");
    lista2.insertar(603, "C");

    assert(lista2.eliminar(602) == true);

    assert(lista2.buscar(602) == nullptr);
    assert(lista2.contar() == 2);

    Usuario* usuario601 = lista2.buscar(601);
    Usuario* usuario603 = lista2.buscar(603);

    assert(usuario601 != nullptr);
    assert(usuario603 != nullptr);

    assert(usuario601->getSiguiente() == usuario603);
    assert(usuario603->getAnterior() == usuario601);

    cout << "OK: eliminacion de cabeza, cola y nodo intermedio "
            "funciona correctamente."
         << endl;
}

// ============================================================
// PRUEBA 7: Recorrido en ambos sentidos
// ============================================================

void prueba_recorridoAmbosSentidos() {
    separador("Prueba 7: Recorrer en ambos sentidos");

    ListaUsuarios lista;

    lista.insertar(701, "Uno");
    lista.insertar(702, "Dos");
    lista.insertar(703, "Tres");

    Usuario* uno = lista.buscar(701);
    Usuario* dos = lista.buscar(702);
    Usuario* tres = lista.buscar(703);

    assert(uno != nullptr);
    assert(dos != nullptr);
    assert(tres != nullptr);

    // Recorrido hacia adelante
    assert(uno->getSiguiente() == dos);
    assert(dos->getSiguiente() == tres);
    assert(tres->getSiguiente() == nullptr);

    // Recorrido hacia atras
    assert(tres->getAnterior() == dos);
    assert(dos->getAnterior() == uno);
    assert(uno->getAnterior() == nullptr);

    cout << "Recorrido hacia adelante:" << endl;
    lista.mostrarAdelante();

    cout << "Recorrido hacia atras:" << endl;
    lista.mostrarAtras();

    cout << "OK: los enlaces permiten recorrer la lista "
            "correctamente en ambos sentidos."
         << endl;
}

// ============================================================
// PRUEBA 8: Eliminar el unico elemento
// ============================================================

void prueba_eliminarUltimoUsuario() {
    separador("Prueba 8: Eliminar el unico usuario de la lista");

    ListaUsuarios lista;

    assert(lista.insertar(801, "Usuario Unico") == true);
    assert(lista.contar() == 1);
    assert(lista.estaVacia() == false);

    assert(lista.eliminar(801) == true);

    assert(lista.contar() == 0);
    assert(lista.estaVacia() == true);
    assert(lista.buscar(801) == nullptr);

    cout << "OK: al eliminar el unico usuario, la lista vuelve "
            "correctamente a un estado vacio."
         << endl;
}

// ============================================================
// PRUEBA 9: Eliminar usuario inexistente
// ============================================================

void prueba_eliminarInexistente() {
    separador("Prueba 9: Eliminar usuario inexistente");

    ListaUsuarios lista;

    lista.insertar(901, "Usuario Existente");

    assert(lista.eliminar(999) == false);
    assert(lista.contar() == 1);

    assert(lista.buscar(901) != nullptr);

    cout << "OK: intentar eliminar un usuario inexistente "
            "no modifica la lista."
         << endl;
}

// ============================================================
// MAIN
// ============================================================

int main() {
    prueba_listaVacia();
    prueba_agregarUsuarios();
    prueba_idDuplicado();
    prueba_buscar();
    prueba_modificar();
    prueba_eliminarCabezaColaIntermedio();
    prueba_recorridoAmbosSentidos();
    prueba_eliminarUltimoUsuario();
    prueba_eliminarInexistente();

    cout << "\n========================================" << endl;
    cout << "TODAS LAS PRUEBAS PASARON CORRECTAMENTE" << endl;
    cout << "========================================" << endl;

    return 0;
}
