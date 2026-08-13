#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H

#include "Tarea.h"
#include "NodoTarea.h"

using namespace std;

/**
 * Clase ColaPrioridad: cola de tareas ordenada por prioridad
 * (ALTA primero, luego MEDIA, luego BAJA) con nodos enlazados.
 */
class ColaPrioridad {
private:
    NodoTarea* frenteNodo; // Nodo al frente de la cola (mayor prioridad)
    int cantidad;          // Cantidad de elementos en la cola

    // Devuelve el valor numérico de la prioridad (ALTA=0, MEDIA=1, BAJA=2)
    int prioridadValor(const string& prioridad) const;

public:
    // Constructor
    ColaPrioridad();

    // Destructor
    ~ColaPrioridad();

    // Inserta una tarea ordenada por prioridad
    void encolar(Tarea* tarea);

    // Elimina la tarea de mayor prioridad del frente
    void desencolar();

    // Devuelve la tarea de mayor prioridad sin eliminarla
    Tarea* frente();

    // Indica si la cola está vacía
    bool estaVacia();
};

#endif // COLAPRIORIDAD_H