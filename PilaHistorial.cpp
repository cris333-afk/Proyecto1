#include "PilaHistorial.h"
#include <iostream>

// Convierte TipoAccion a texto legible.
static std::string tipoAccionToString(TipoAccion tipo) {
    switch (tipo) {
        case TipoAccion::AGREGAR: return "AGREGAR";
        case TipoAccion::ACTUALIZAR: return "ACTUALIZAR";
        case TipoAccion::ELIMINAR: return "ELIMINAR";
    }
    return "DESCONOCIDO";
}

// Convierte TipoEntidad a texto legible.
static std::string tipoEntidadToString(TipoEntidad tipo) {
    switch (tipo) {
        case TipoEntidad::USUARIO: return "USUARIO";
        case TipoEntidad::TAREA: return "TAREA";
    }
    return "DESCONOCIDO";
}

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
void PilaHistorial::mostrarPila(std::stack<Accion> pila, const std::string& titulo) const {
    std::cout << "--- " << titulo << " ---" << std::endl;

    if (pila.empty()) {
        std::cout << "Vacía" << std::endl;
        return;
    }

    while (!pila.empty()) {
        Accion accion = pila.top();
        pila.pop();

        std::cout << "Acción:" << std::endl;
        std::cout << "Tipo: " << tipoAccionToString(accion.getTipoAccion()) << std::endl;
        std::cout << "Entidad: " << tipoEntidadToString(accion.getTipoEntidad()) << std::endl;
        std::cout << "ID: " << accion.getIdEntidad() << std::endl;
        std::cout << "Estado anterior: " << accion.getEstadoAnterior() << std::endl;
        std::cout << "Estado posterior: " << accion.getEstadoPosterior() << std::endl;
        std::cout << std::endl;
    }
}

// Muestra las acciones almacenadas en ambas pilas.
// Recorre copias temporales para no modificar las pilas originales.
void PilaHistorial::mostrarHistorial() const {
    std::cout << "================ HISTORIAL ================" << std::endl;

    mostrarPila(pilaDeshacer, "ACCIONES PARA DESHACER");
    mostrarPila(pilaRehacer, "ACCIONES PARA REHACER");

    std::cout << "============================================" << std::endl;
}
