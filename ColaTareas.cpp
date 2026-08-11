#include "ColaTareas.h"

// Constructor: inicializa la cola vacía
ColaTareas::ColaTareas() {
    frenteNodo = nullptr;
    finalNodo = nullptr;
    cantidad = 0;
}

// Destructor: libera la memoria de los nodos
ColaTareas::~ColaTareas() {
    while (frenteNodo != nullptr) {
        Nodo* temp = frenteNodo;
        frenteNodo = frenteNodo->siguiente;
        delete temp;
    }
}

// Agrega una tarea al final de la cola
void ColaTareas::encolar(Tarea* tarea) {
    Nodo* nuevo = new Nodo(tarea);
    if (estaVacia()) {
        // Si la cola está vacía, el nuevo nodo es frente y final
        frenteNodo = nuevo;
        finalNodo = nuevo;
    } else {
        // Si no, se enlaza al final y se actualiza el final
        finalNodo->siguiente = nuevo;
        finalNodo = nuevo;
    }
    cantidad++;
}

// Elimina la tarea del frente de la cola
void ColaTareas::desencolar() {
    if (estaVacia()) {
        // No hacer nada si la cola está vacía
        return;
    }
    Nodo* temp = frenteNodo;
    frenteNodo = frenteNodo->siguiente;
    delete temp;
    cantidad--;
    // Si la cola quedó vacía, el final también debe ser nullptr
    if (frenteNodo == nullptr) {
        finalNodo = nullptr;
    }
}

// Devuelve la tarea del frente sin eliminarla
Tarea* ColaTareas::frente() {
    if (estaVacia()) {
        // Devuelve nullptr si la cola está vacía
        return nullptr;
    }
    return frenteNodo->tarea;
}

// Indica si la cola está vacía
bool ColaTareas::estaVacia() {
    return frenteNodo == nullptr;
}

// Devuelve el número de tareas en la cola
int ColaTareas::tamano() {
    return cantidad;
}