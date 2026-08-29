#include "ColaPrioridad.h"

// Constructor: inicializa la cola vacía
ColaPrioridad::ColaPrioridad() {
    frenteNodo = nullptr;
    cantidad = 0;
}

// Destructor: libera la memoria de los NodoTarea.
// NOTA (ownership): este destructor hace delete SOLO de los nodos
// (delete temp, donde temp es un NodoTarea). NO hace delete de las Tarea
// que esos nodos apuntan: las Tarea son propiedad de quien las creó con new
// (normalmente ArbolSubtareas) y se liberan por separado. Esto es consistente
// con NodoTarea, que guarda una referencia prestada y no es su propietario.
ColaPrioridad::~ColaPrioridad() {
    while (frenteNodo != nullptr) {
        NodoTarea* temp = frenteNodo;
        frenteNodo = frenteNodo->getSiguiente();
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
    NodoTarea* nuevo = new NodoTarea(tarea);
    int valorNuevo = prioridadValor(tarea->getPrioridad());

    // Si la cola está vacía o la nueva tarea tiene mayor prioridad que el frente
    if (estaVacia() || valorNuevo < prioridadValor(frenteNodo->getTarea()->getPrioridad())) {
        nuevo->setSiguiente(frenteNodo);
        frenteNodo = nuevo;
    } else {
        // Recorre la cola para encontrar la posición correcta
        NodoTarea* actual = frenteNodo;
        while (actual->getSiguiente() != nullptr &&
               valorNuevo >= prioridadValor(actual->getSiguiente()->getTarea()->getPrioridad())) {
            actual = actual->getSiguiente();
        }
        nuevo->setSiguiente(actual->getSiguiente());
        actual->setSiguiente(nuevo);
    }
    cantidad++;
}

// Elimina la tarea de mayor prioridad del frente
void ColaPrioridad::desencolar() {
    if (estaVacia()) {
        // No hacer nada si la cola está vacía
        return;
    }
    NodoTarea* temp = frenteNodo;
    frenteNodo = frenteNodo->getSiguiente();
    delete temp;
    cantidad--;
}

// Devuelve la tarea de mayor prioridad sin eliminarla
Tarea* ColaPrioridad::frente() {
    if (estaVacia()) {
        // Devuelve nullptr si la cola está vacía
        return nullptr;
    }
    return frenteNodo->getTarea();
}

// Indica si la cola está vacía
bool ColaPrioridad::estaVacia() {
    return frenteNodo == nullptr;
}