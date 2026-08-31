#include "ListaUsuarios.h"
#include <iostream>

using namespace std;

ListaUsuarios::ListaUsuarios()
    : cabeza(nullptr), cola(nullptr), cantidad(0) {
}

ListaUsuarios::~ListaUsuarios() {
    Usuario* actual = cabeza;

    while (actual != nullptr) {
        Usuario* siguiente = actual->getSiguiente();
        delete actual;
        actual = siguiente;
    }

    cabeza = nullptr;
    cola = nullptr;
    cantidad = 0;
}

bool ListaUsuarios::existeId(int id) const {
    Usuario* actual = cabeza;

    while (actual != nullptr) {
        if (actual->getId() == id) {
            return true;
        }

        actual = actual->getSiguiente();
    }

    return false;
}

bool ListaUsuarios::insertar(int id,
                             const string& nombre,
                             const string& contrasena,
                             Rol rol) {
    // No se permiten IDs duplicados.
    if (existeId(id)) {
        return false;
    }

    Usuario* nuevo = new Usuario(id, nombre, contrasena, rol);

    // Caso 1: lista vacia.
    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    }
    // Caso 2: la lista ya tiene elementos.
    else {
        nuevo->setAnterior(cola);
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }

    cantidad++;

    return true;
}

bool ListaUsuarios::eliminar(int id) {
    Usuario* actual = buscar(id);

    // El usuario no existe.
    if (actual == nullptr) {
        return false;
    }

    Usuario* anterior = actual->getAnterior();
    Usuario* siguiente = actual->getSiguiente();

    // Si tiene un nodo anterior, este debe apuntar
    // al siguiente. Si no tiene, era la cabeza.
    if (anterior != nullptr) {
        anterior->setSiguiente(siguiente);
    } else {
        cabeza = siguiente;
    }

    // Si tiene un nodo siguiente, este debe apuntar
    // al anterior. Si no tiene, era la cola.
    if (siguiente != nullptr) {
        siguiente->setAnterior(anterior);
    } else {
        cola = anterior;
    }

    delete actual;
    cantidad--;

    return true;
}

Usuario* ListaUsuarios::buscar(int id) const {
    Usuario* actual = cabeza;

    while (actual != nullptr) {
        if (actual->getId() == id) {
            return actual;
        }

        actual = actual->getSiguiente();
    }

    return nullptr;
}

bool ListaUsuarios::modificar(int id, const string& nuevoNombre) {
    Usuario* usuario = buscar(id);

    if (usuario == nullptr) {
        return false;
    }

    usuario->setNombre(nuevoNombre);

    return true;
}

bool ListaUsuarios::modificar(int id,
                              const string& nuevoNombre,
                              Rol nuevoRol) {
    Usuario* usuario = buscar(id);

    if (usuario == nullptr) {
        return false;
    }

    usuario->setNombre(nuevoNombre);
    usuario->setRol(nuevoRol);

    return true;
}

void ListaUsuarios::mostrarAdelante() const {
    if (cabeza == nullptr) {
        cout << "(Lista de usuarios vacia)" << endl;
        return;
    }

    cout << "--- Usuarios (cabeza -> cola) ---" << endl;

    Usuario* actual = cabeza;

    while (actual != nullptr) {
        cout << actual->toString() << endl;
        actual = actual->getSiguiente();
    }
}

void ListaUsuarios::mostrarAtras() const {
    if (cola == nullptr) {
        cout << "(Lista de usuarios vacia)" << endl;
        return;
    }

    cout << "--- Usuarios (cola -> cabeza) ---" << endl;

    Usuario* actual = cola;

    while (actual != nullptr) {
        cout << actual->toString() << endl;
        actual = actual->getAnterior();
    }
}

int ListaUsuarios::contar() const {
    return cantidad;
}

bool ListaUsuarios::estaVacia() const {
    return cabeza == nullptr;
}