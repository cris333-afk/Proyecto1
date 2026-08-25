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

// Muestra las acciones de una pila recorriendo una copia temporal.
// La copia se crea al pasar la pila por valor.
std::string PilaHistorial::mostrarPila(std::stack<Accion> pila, const std::string& titulo) const {
    std::string texto = "--- " + titulo + " ---\n";

    if (pila.empty()) {
        texto += "Vacia\n";
        return texto;
    }

    while (!pila.empty()) {
        Accion accion = pila.top();
        pila.pop();

        texto += "Accion:\n";
        texto += accion.toString();
        texto += "\n";
    }

    return texto;
}

// Muestra las acciones almacenadas en ambas pilas.
// Recorre copias temporales para no modificar las pilas originales.
std::string PilaHistorial::mostrarHistorial() const {
    std::string texto = "================ HISTORIAL ================\n";

    texto += mostrarPila(pilaDeshacer, "ACCIONES PARA DESHACER");
    texto += mostrarPila(pilaRehacer, "ACCIONES PARA REHACER");

    texto += "============================================\n";

    return texto;
}
