#ifndef ARBOLSUBTAREAS_H
#define ARBOLSUBTAREAS_H

#include "Tarea.h"

#include <functional>

using namespace std;

/**
 * Clase ArbolSubtareas: bosque de tareas representado como un árbol general
 * (no binario), donde cada Tarea puede tener múltiples subtareas hijas.
 *
 * Para permitir varias tareas de nivel superior independientes se usa una
 * "raíz virtual": una Tarea invisible con id = 0 creada automáticamente por
 * el constructor, cuyos hijos directos son las tareas reales de nivel
 * superior. La raíz virtual no se muestra en los recorridos y queda excluida
 * de buscar(), eliminar(), contarNodos() y altura().
 */
class ArbolSubtareas {
private:
    Tarea* raiz;  // Raíz virtual (id = 0); sus hijos son las tareas de nivel superior

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
    ArbolSubtareas();                 // Crea automáticamente la raíz virtual (id = 0)
    ArbolSubtareas(Tarea* raiz);      // La tarea dada pasa a ser la primera de nivel superior

    // Destructor
    ~ArbolSubtareas();

    // Getters / Setters básicos de la raíz
    Tarea* getRaiz() const;
    void setRaiz(Tarea* raiz);

    // Agrega una tarea de nivel superior (hija directa de la raíz virtual)
    bool agregarTarea(Tarea* nueva);

    // Agrega una subtarea al padre con el id indicado
    bool agregarSubtarea(int idPadre, Tarea* nueva);

    // Busca una tarea por id, devuelve nullptr si no existe.
    // El id 0 (raíz virtual) nunca se devuelve.
    Tarea* buscar(int id);

    // Elimina la tarea con el id indicado (junto con sus descendientes);
    // la raíz virtual (id 0) nunca puede eliminarse.
    bool eliminar(int id);

    // Recorridos del bosque (no imprimen la raíz virtual)
    void recorrerPreOrden();
    void recorrerPostOrden();

    // Nuevo recorrido de filtrado: recorre el bosque (excluye la raíz
    // virtual) y ejecuta "accion" sobre cada Tarea* real que cumpla el
    // predicado "filtro". No altera recorrerPreOrden()/recorrerPostOrden().
    void recorrerConFiltro(const std::function<bool(Tarea*)>& filtro,
                           const std::function<void(Tarea*)>& accion);

    // Estadísticas del bosque (sin contar la raíz virtual)
    int contarNodos();
    int altura();
};

#endif // ARBOLSUBTAREAS_H