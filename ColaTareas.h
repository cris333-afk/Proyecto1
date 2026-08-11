#ifndef COLATAREAS_H
#define COLATAREAS_H

#include "Tarea.h"

using namespace std;

/**
 * Clase ColaTareas: cola FIFO de tareas implementada con nodos enlazados.
 */
class ColaTareas {
private:
    // Nodo interno de la cola
    struct Nodo {
        Tarea* tarea;   // Puntero a la tarea
        Nodo* siguiente; // Puntero al siguiente nodo
        Nodo(Tarea* tarea) : tarea(tarea), siguiente(nullptr) {}
    };

    Nodo* frenteNodo; // Nodo al frente de la cola
    Nodo* finalNodo;  // Nodo al final de la cola
    int cantidad;     // Cantidad de elementos en la cola

public:
    // Constructor
    ColaTareas();

    // Destructor
    ~ColaTareas();

    // Agrega una tarea al final de la cola
    void encolar(Tarea* tarea);

    // Elimina la tarea del frente de la cola
    void desencolar();

    // Devuelve la tarea del frente sin eliminarla
    Tarea* frente();

    // Indica si la cola está vacía
    bool estaVacia();

    // Devuelve el número de tareas en la cola
    int tamano();
};

#endif // COLATAREAS_H