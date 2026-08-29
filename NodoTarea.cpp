#include "NodoTarea.h"

// Constructor: inicializa la tarea y deja siguiente en nullptr
NodoTarea::NodoTarea(Tarea* tarea) {
    this->tarea = tarea;
    this->siguiente = nullptr;
}

// Destructor.
// IMPORTANTE (ownership): NodoTarea NO es dueño de la Tarea que apunta, por
// lo que aquí NO se hace delete sobre tarea. La Tarea sigue viva y la
// responsabilidad de liberarla es de quien la creó con new (en este proyecto,
// normalmente ArbolSubtareas o quien construye las tareas). Liberarla aquí
// provocaría un double free, porque el dueño real también la eliminaría.
NodoTarea::~NodoTarea() {
    // El nodo guarda una referencia prestada (borrowed pointer), no es su
    // propietario: intencionalmente NO se libera la Tarea.
}

// Devuelve el puntero a la tarea del nodo
Tarea* NodoTarea::getTarea() const {
    return tarea;
}

// Devuelve el puntero al siguiente nodo
NodoTarea* NodoTarea::getSiguiente() const {
    return siguiente;
}

// Establece el puntero al siguiente nodo
void NodoTarea::setSiguiente(NodoTarea* siguiente) {
    this->siguiente = siguiente;
}
