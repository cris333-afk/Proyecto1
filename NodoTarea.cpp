#include "NodoTarea.h"

// Constructor: inicializa la tarea y deja siguiente en nullptr
NodoTarea::NodoTarea(Tarea* tarea) {
    this->tarea = tarea;
    this->siguiente = nullptr;
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
