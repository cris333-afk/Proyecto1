#include "ColaTareas.h"

// Constructor: inicializa la cola vacía
ColaTareas::ColaTareas() {
    frenteNodo = nullptr;
    finalNodo = nullptr;
    cantidad = 0;
}

// Destructor: libera la memoria de los NodoTarea.
// NOTA (ownership): este destructor hace delete SOLO de los nodos
// (delete temp, donde temp es un NodoTarea). NO hace delete de las Tarea
// que esos nodos apuntan: las Tarea son propiedad de quien las creó con new
// (normalmente ArbolSubtareas) y se liberan por separado. Esto es consistente
// con NodoTarea, que guarda una referencia prestada y no es su propietario.
ColaTareas::~ColaTareas() {
    while (frenteNodo != nullptr) {
        NodoTarea* temp = frenteNodo;
        frenteNodo = frenteNodo->getSiguiente();
        delete temp;
    }
}

// Agrega una tarea al final de la cola
void ColaTareas::encolar(Tarea* tarea) {
    NodoTarea* nuevo = new NodoTarea(tarea);
    if (estaVacia()) {
        // Si la cola está vacía, el nuevo nodo es frente y final
        frenteNodo = nuevo;
        finalNodo = nuevo;
    } else {
        // Si no, se enlaza al final y se actualiza el final
        finalNodo->setSiguiente(nuevo);
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
    NodoTarea* temp = frenteNodo;
    frenteNodo = frenteNodo->getSiguiente();
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
    return frenteNodo->getTarea();
}

// Indica si la cola está vacía
bool ColaTareas::estaVacia() {
    return frenteNodo == nullptr;
}

// Devuelve el número de tareas en la cola
int ColaTareas::tamano() {
    return cantidad;
}