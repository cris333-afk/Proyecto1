#pragma once
#include <stack>
#include "Accion.h"

// Administra dos pilas de acciones para permitir deshacer y rehacer.
// No modifica usuarios ni tareas: solo guarda y mueve acciones.
class PilaHistorial {
private:
    // Acciones disponibles para deshacer.
    std::stack<Accion> pilaDeshacer;

    // Acciones disponibles para rehacer.
    std::stack<Accion> pilaRehacer;

public:
    // Constructor por defecto.
    PilaHistorial();

    // Registra una nueva acción en la pila de deshacer.
    // Al registrar, la pila de rehacer se vacía por completo.
    void registrarAccion(const Accion& accion);

    // Mueve la acción de la cima de deshacer hacia rehacer.
    // Copia la acción al parámetro y retorna true si se pudo deshacer.
    bool deshacer(Accion& accion);

    // Mueve la acción de la cima de rehacer hacia deshacer.
    // Copia la acción al parámetro y retorna true si se pudo rehacer.
    bool rehacer(Accion& accion);

    // Indica si la pila de deshacer está vacía.
    bool estaVaciaDeshacer() const;

    // Indica si la pila de rehacer está vacía.
    bool estaVaciaRehacer() const;

    // Cantidad de acciones en la pila de deshacer.
    size_t tamanoDeshacer() const;

    // Cantidad de acciones en la pila de rehacer.
    size_t tamanoRehacer() const;

    // Copia la acción de la cima de deshacer sin eliminarla.
    // Retorna false si la pila está vacía.
    bool cimaDeshacer(Accion& accion) const;

    // Copia la acción de la cima de rehacer sin eliminarla.
    // Retorna false si la pila está vacía.
    bool cimaRehacer(Accion& accion) const;

    // Vacía ambas pilas por completo.
    void limpiar();
};