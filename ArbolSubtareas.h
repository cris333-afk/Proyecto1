#ifndef ARBOLSUBTAREAS_H
#define ARBOLSUBTAREAS_H

#include "Tarea.h"

using namespace std;

/**
 * Clase ArbolSubtareas: árbol general (no binario) de tareas,
 * donde cada Tarea puede tener múltiples subtareas hijas.
 */
class ArbolSubtareas {
private:
    Tarea* raiz;  // Nodo raíz del árbol

    // Métodos recursivos privados
    bool agregarSubtareaRec(Tarea* nodo, int idPadre, Tarea* nueva);
    Tarea* buscarRec(Tarea* nodo, int id);
    bool eliminarRec(Tarea* padre, int id);
    void recorrerPreOrdenRec(Tarea* nodo);
    void recorrerPostOrdenRec(Tarea* nodo);
    int contarNodosRec(Tarea* nodo);
    int alturaRec(Tarea* nodo);

public:
    // Constructores
    ArbolSubtareas();
    ArbolSubtareas(Tarea* raiz);

    // Destructor
    ~ArbolSubtareas();

    // Getters / Setters básicos de la raíz
    Tarea* getRaiz() const;
    void setRaiz(Tarea* raiz);

    // Agrega una subtarea al padre con el id indicado
    bool agregarSubtarea(int idPadre, Tarea* nueva);

    // Busca una tarea por id, devuelve nullptr si no existe
    Tarea* buscar(int id);

    // Elimina la tarea con el id indicado (junto con sus descendientes)
    bool eliminar(int id);

    // Recorridos del árbol
    void recorrerPreOrden();
    void recorrerPostOrden();

    // Estadísticas del árbol
    int contarNodos();
    int altura();
};

#endif // ARBOLSUBTAREAS_H