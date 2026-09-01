#pragma once
#include <stack>
#include <string>
#include <cstddef>
#include "Accion.h"

// Administra el historial mediante dos pilas LIFO: una con las acciones disponibles
// para deshacer y otra con las disponibles para rehacer. Su función es guardar las
// operaciones ya realizadas y entregarlas cuando se pida revertirlas o reaplicarlas,
// dejando en manos de quien use el módulo (por ejemplo, el Gestor) aplicar los cambios.
class PilaHistorial {

private:
    // Acciones disponibles para deshacer.
    std::stack<Accion> pilaDeshacer;

    // Acciones disponibles para rehacer.
    std::stack<Accion> pilaRehacer;

    // Genera un texto con todas las acciones de una pila para mostrarlas al usuario.
    // Recibe una copia (no la original) para que consultarla no afecte el historial real.
    std::string mostrarPila(std::stack<Accion> pila, const std::string& titulo) const;

public:

    // Constructor por defecto.
    PilaHistorial();

    // Agrega la acción nueva a la pila de deshacer y vacía la de rehacer,
    // porque las acciones que estaban allí pertenecían a una línea de cambios anterior.
    void registrarAccion(const Accion& accion);

   // Mueve la acción de la cima de deshacer hacia rehacer.
    bool deshacer(Accion& accion);

   // Mueve la acción de la cima de rehacer hacia deshacer.
    bool rehacer(Accion& accion);

    // Verifica si la pila de deshacer está vacía.
    bool estaVaciaDeshacer() const;

    // Verifica si la pila de rehacer está vacía.
    bool estaVaciaRehacer() const;

    // Retorna la cantidad de acciones disponibles para deshacer.
    size_t tamanoDeshacer() const;

    // Retorna la cantidad de acciones disponibles para rehacer.
    size_t tamanoRehacer() const;

    // Copia la acción de la cima de deshacer sin eliminarla.
    // Retorna false si la pila está vacía.
    bool cimaDeshacer(Accion& accion) const;

    // Copia la acción de la cima de rehacer sin eliminarla.
    // Retorna false si la pila está vacía.
    bool cimaRehacer(Accion& accion) const;

    // Vacía ambas pilas por completo.
    void limpiar();

    // Permite consultar las acciones disponibles en ambas pilas(rehacer y deshacer)
    // sin modificar su contenido.
    std::string mostrarHistorial() const;
};
