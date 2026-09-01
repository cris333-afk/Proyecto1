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
    // NOTA DE PROPIEDAD (ownership):
    // Este puntero es una referencia PRESTADA (borrowed reference), no una
    // posesión. NodoTarea NO es dueño de la Tarea apuntada: no la crea ni la
    // libera. La responsabilidad de hacer delete de cada Tarea es de quien la
    // construyó con new (en este proyecto, normalmente ArbolSubtareas u otro
    // creador de tareas). Liberarla aquí provocaría un double free.
    Tarea* tarea;      // Puntero (prestado) a la tarea almacenada
    NodoTarea* siguiente; // Puntero al siguiente nodo

public:
    /**
     * Constructor del nodo.
     * @param tarea Puntero a la tarea que se almacena en el nodo.
     */
    NodoTarea(Tarea* tarea);

    // Destructor: libera únicamente este NodoTarea.
    // NO hace delete de la Tarea apuntada (ver comentario de propiedad sobre
    // el atributo tarea: es una referencia prestada, no una posesión).
    ~NodoTarea();

    // Devuelve el puntero a la tarea del nodo
    Tarea* getTarea() const;

    // Devuelve el puntero al siguiente nodo
    NodoTarea* getSiguiente() const;

    // Establece el puntero al siguiente nodo
    void setSiguiente(NodoTarea* siguiente);
};

#endif // NODOTAREA_H
