#ifndef NODOTAREA_H
#define NODOTAREA_H

#include "Tarea.h"

using namespace std;

/**
 * Clase NodoTarea: nodo enlazado que almacena una Tarea y
 * un puntero al siguiente nodo. Usado por ColaTareas y ColaPrioridad.
 */
class NodoTarea {
private:
    Tarea* tarea;      // Puntero a la tarea almacenada
    NodoTarea* siguiente; // Puntero al siguiente nodo

public:
    /**
     * Constructor del nodo.
     * @param tarea Puntero a la tarea que se almacena en el nodo.
     */
    NodoTarea(Tarea* tarea);

    // Devuelve el puntero a la tarea del nodo
    Tarea* getTarea() const;

    // Devuelve el puntero al siguiente nodo
    NodoTarea* getSiguiente() const;

    // Establece el puntero al siguiente nodo
    void setSiguiente(NodoTarea* siguiente);
};

#endif // NODOTAREA_H
