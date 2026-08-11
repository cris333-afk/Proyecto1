#include "ColaPrioridad.h"

// Constructor: inicializa la cola vacía
ColaPrioridad::ColaPrioridad() {
    frenteNodo = nullptr;
    cantidad = 0;
}

// Destructor: libera la memoria de los nodos
ColaPrioridad::~ColaPrioridad() {
    while (frenteNodo != nullptr) {
        Nodo* temp = frenteNodo;
        frenteNodo = frenteNodo->siguiente;
        delete temp;
    }
}

// Devuelve el valor numérico de la prioridad (ALTA=0, MEDIA=1, BAJA=2)
int ColaPrioridad::prioridadValor(const string& prioridad) const {
    if (prioridad == "ALTA") {
        return 0;
    } else if (prioridad == "MEDIA") {
        return 1;
    }
    return 2; // BAJA o cualquier otro valor
}

// Inserta una tarea ordenada por prioridad
void ColaPrioridad::encolar(Tarea* tarea) {
    Nodo* nuevo = new Nodo(tarea);
    int valorNuevo = prioridadValor(tarea->getPrioridad());

    // Si la cola está vacía o la nueva tarea tiene mayor prioridad que el frente
    if (estaVacia() || valorNuevo < prioridadValor(frenteNodo->tarea->getPrioridad())) {
        nuevo->siguiente = frenteNodo;
        frenteNodo = nuevo;
    } else {
        // Recorre la cola para encontrar la posición correcta
        Nodo* actual = frenteNodo;
        while (actual->siguiente != nullptr &&
               valorNuevo >= prioridadValor(actual->siguiente->tarea->getPrioridad())) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
    cantidad++;
}

// Elimina la tarea de mayor prioridad del frente
void ColaPrioridad::desencolar() {
    if (estaVacia()) {
        // No hacer nada si la cola está vacía
        return;
    }
    Nodo* temp = frenteNodo;
    frenteNodo = frenteNodo->siguiente;
    delete temp;
    cantidad--;
}

// Devuelve la tarea de mayor prioridad sin eliminarla
Tarea* ColaPrioridad::frente() {
    if (estaVacia()) {
        // Devuelve nullptr si la cola está vacía
        return nullptr;
    }
    return frenteNodo->tarea;
}

// Indica si la cola está vacía
bool ColaPrioridad::estaVacia() {
    return frenteNodo == nullptr;
}