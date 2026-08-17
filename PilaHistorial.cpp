#include "PilaHistorial.h"

// Constructor por defecto.
PilaHistorial::PilaHistorial() {}

// Registra una nueva acción en la pila de deshacer.
// Al registrar, la pila de rehacer se vacía por completo.
void PilaHistorial::registrarAccion(const Accion& accion) {
    pilaDeshacer.push(accion);

    // Al registrar una acción nueva, las acciones rehechas pierden validez.
    while (!pilaRehacer.empty()) {
        pilaRehacer.pop();
    }
}

// Mueve la acción de la cima de deshacer hacia rehacer.
bool PilaHistorial::deshacer(Accion& accion) {
    if (pilaDeshacer.empty()) {
        return false;
    }

    accion = pilaDeshacer.top();
    pilaDeshacer.pop();
    pilaRehacer.push(accion);
    return true;
}

// Mueve la acción de la cima de rehacer hacia deshacer.
bool PilaHistorial::rehacer(Accion& accion) {
    if (pilaRehacer.empty()) {
        return false;
    }

    accion = pilaRehacer.top();
    pilaRehacer.pop();
    pilaDeshacer.push(accion);
    return true;
}

bool PilaHistorial::estaVaciaDeshacer() const {
    return pilaDeshacer.empty();
}

bool PilaHistorial::estaVaciaRehacer() const {
    return pilaRehacer.empty();
}

size_t PilaHistorial::tamanoDeshacer() const {
    return pilaDeshacer.size();
}

size_t PilaHistorial::tamanoRehacer() const {
    return pilaRehacer.size();
}

// Copia la acción de la cima de deshacer sin eliminarla.
bool PilaHistorial::cimaDeshacer(Accion& accion) const {
    if (pilaDeshacer.empty()) {
        return false;
    }

    accion = pilaDeshacer.top();
    return true;
}

// Copia la acción de la cima de rehacer sin eliminarla.
bool PilaHistorial::cimaRehacer(Accion& accion) const {
    if (pilaRehacer.empty()) {
        return false;
    }

    accion = pilaRehacer.top();
    return true;
}

// Vacía ambas pilas por completo.
void PilaHistorial::limpiar() {
    while (!pilaDeshacer.empty()) {
        pilaDeshacer.pop();
    }

    while (!pilaRehacer.empty()) {
        pilaRehacer.pop();
    }
}