#include "ArbolSubtareas.h"
#include <iostream>

using namespace std;

// Constructor por defecto: crea automáticamente la raíz virtual (id = 0).
// Esta Tarea es invisible para quien usa el árbol; sus hijos directos son
// las tareas reales de nivel superior del bosque.
ArbolSubtareas::ArbolSubtareas() {
    raiz = new Tarea(0, "SIN_PRIORIDAD", "SIN_ESTADO");
}

// Constructor con tarea inicial: la tarea indicada se convierte en la
// primera tarea de nivel superior (hija de la raíz virtual).
ArbolSubtareas::ArbolSubtareas(Tarea* raiz) {
    this->raiz = new Tarea(0, "SIN_PRIORIDAD", "SIN_ESTADO");
    if (raiz != nullptr) {
        agregarTarea(raiz);
    }
}

// Destructor: libera toda la memoria del bosque.
// delete raiz libera la raíz virtual, cuyo destructor de Tarea borra
// recursivamente a sus hijos (las tareas reales de nivel superior).
ArbolSubtareas::~ArbolSubtareas() {
    delete raiz;
}

// Getter de la raíz
Tarea* ArbolSubtareas::getRaiz() const {
    return raiz;
}

// Setter de la raíz
void ArbolSubtareas::setRaiz(Tarea* raiz) {
    this->raiz = raiz;
}

// Agrega una tarea directamente como hija de la raíz virtual, es decir,
// como una tarea real de nivel superior, sin pedir idPadre.
bool ArbolSubtareas::agregarTarea(Tarea* nueva) {
    if (nueva == nullptr) {
        return false;
    }
    vector<Tarea*> hijos = raiz->getSubtareas();
    hijos.push_back(nueva);
    raiz->setSubtareas(hijos);
    return true;
}

// Recursión: recorre cada nodo buscando el idPadre.
// Caso base: si el nodo actual es el padre, agrega la nueva tarea y devuelve true.
// Caso recursivo: si no es el padre, intenta en cada subtarea hija hasta encontrar el padre.
bool ArbolSubtareas::agregarSubtareaRec(Tarea* nodo, int idPadre, Tarea* nueva) {
    // Caso base: encontramos el nodo padre
    if (nodo->getId() == idPadre) {
        // Obtiene el vector actual de subtareas, agrega la nueva y lo asigna de vuelta
        vector<Tarea*> subtareas = nodo->getSubtareas();
        subtareas.push_back(nueva);
        nodo->setSubtareas(subtareas);
        return true;
    }

    // Caso recursivo: busca en cada hijo
    vector<Tarea*> hijos = nodo->getSubtareas();
    for (Tarea* hijo : hijos) {
        if (agregarSubtareaRec(hijo, idPadre, nueva)) {
            return true;
        }
    }
    return false;
}

// Agrega una subtarea al padre con el id indicado
bool ArbolSubtareas::agregarSubtarea(int idPadre, Tarea* nueva) {
    if (raiz == nullptr) {
        return false;
    }
    return agregarSubtareaRec(raiz, idPadre, nueva);
}

// Recursión: busca el nodo con el id indicado.
// Caso base 1: nodo nulo, no encontrado (devuelve nullptr).
// Caso base 2: el nodo actual tiene el id buscado (devuelve el nodo).
// Caso recursivo: busca en cada hijo; si alguno devuelve un nodo, lo retorna.
Tarea* ArbolSubtareas::buscarRec(Tarea* nodo, int id) {
    if (nodo == nullptr) {
        return nullptr;
    }
    if (nodo->getId() == id) {
        return nodo;
    }
    vector<Tarea*> hijos = nodo->getSubtareas();
    for (Tarea* hijo : hijos) {
        Tarea* encontrado = buscarRec(hijo, id);
        if (encontrado != nullptr) {
            return encontrado;
        }
    }
    return nullptr;
}

// Busca una tarea por id, devuelve nullptr si no existe.
// El id 0 es el de la raíz virtual: nunca se busca ni se devuelve.
Tarea* ArbolSubtareas::buscar(int id) {
    if (id == 0 || raiz == nullptr) {
        return nullptr;
    }
    return buscarRec(raiz, id);
}

// Recursión: busca en las subtareas del nodo padre la tarea con el id indicado.
// Caso base: si un hijo directo tiene el id, lo desenlaza del vector y lo elimina.
//   Al hacer delete, el destructor de Tarea borra recursivamente todos sus descendientes.
// Caso recursivo: si ningún hijo directo coincide, intenta eliminar dentro de cada hijo.
bool ArbolSubtareas::eliminarRec(Tarea* padre, int id) {
    vector<Tarea*> hijos = padre->getSubtareas();

    // Busca si algún hijo directo es el que se quiere eliminar
    for (size_t i = 0; i < hijos.size(); i++) {
        if (hijos[i]->getId() == id) {
            // Crea un nuevo vector sin la tarea a eliminar
            vector<Tarea*> nuevoVector;
            for (size_t j = 0; j < hijos.size(); j++) {
                if (j != i) {
                    nuevoVector.push_back(hijos[j]);
                }
            }
            padre->setSubtareas(nuevoVector);

            // Elimina la tarea (su destructor borra recursivamente a sus hijos)
            delete hijos[i];
            return true;
        }
    }

    // Recursión: intenta eliminar dentro de cada hijo
    for (Tarea* hijo : hijos) {
        if (eliminarRec(hijo, id)) {
            return true;
        }
    }
    return false;
}

// Elimina la tarea con el id indicado (junto con sus descendientes).
// La raíz virtual (id 0) no puede eliminarse: se destruye solo al borrar
// el bosque completo.
bool ArbolSubtareas::eliminar(int id) {
    if (id == 0 || raiz == nullptr) {
        return false;
    }

    // eliminarRec busca a partir de la raíz virtual y elimina el primer
    // nodo real con ese id (ya sea de nivel superior o una subtarea).
    return eliminarRec(raiz, id);
}

// Recursión: primero imprime el nodo actual, luego recorre cada hijo en pre-orden.
void ArbolSubtareas::recorrerPreOrdenRec(Tarea* nodo) {
    if (nodo == nullptr) {
        return;
    }

    // Visita el nodo actual primero
    cout << "Tarea ID: " << nodo->getId()
         << " | Prioridad: " << nodo->getPrioridad()
         << " | Estado: " << nodo->getEstado() << endl;

    // Luego recorre recursivamente cada subtarea (raíz -> hijos -> nietos)
    vector<Tarea*> hijos = nodo->getSubtareas();
    for (Tarea* hijo : hijos) {
        recorrerPreOrdenRec(hijo);
    }
}

// Recorrido pre-orden del bosque.
// Se recorren los hijos directos de la raíz virtual (las tareas de nivel
// superior); la propia raíz virtual (id 0) nunca se imprime.
void ArbolSubtareas::recorrerPreOrden() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío." << endl;
        return;
    }

    vector<Tarea*> hijos = raiz->getSubtareas();
    if (hijos.empty()) {
        cout << "El árbol está vacío." << endl;
        return;
    }

    for (Tarea* hijo : hijos) {
        recorrerPreOrdenRec(hijo);
    }
}

// Recursión: primero recorre cada hijo en post-orden, luego imprime el nodo actual.
void ArbolSubtareas::recorrerPostOrdenRec(Tarea* nodo) {
    if (nodo == nullptr) {
        return;
    }

    // Primero recorre recursivamente cada subtarea (hijos -> nietos -> raíz)
    vector<Tarea*> hijos = nodo->getSubtareas();
    for (Tarea* hijo : hijos) {
        recorrerPostOrdenRec(hijo);
    }

    // Imprime el nodo actual al final
    cout << "Tarea ID: " << nodo->getId()
         << " | Prioridad: " << nodo->getPrioridad()
         << " | Estado: " << nodo->getEstado() << endl;
}

// Recorrido post-orden del bosque.
// Se recorren los hijos directos de la raíz virtual (las tareas de nivel
// superior); la propia raíz virtual (id 0) nunca se imprime.
void ArbolSubtareas::recorrerPostOrden() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío." << endl;
        return;
    }

    vector<Tarea*> hijos = raiz->getSubtareas();
    if (hijos.empty()) {
        cout << "El árbol está vacío." << endl;
        return;
    }

    for (Tarea* hijo : hijos) {
        recorrerPostOrdenRec(hijo);
    }
}

// Recorrido de filtrado del bosque.
// Recorre todas las tareas reales (excluye la raíz virtual) y, por cada
// una que cumpla el predicado "filtro", ejecuta "accion" con esa Tarea*.
// Es un recorrido nuevo y genérico que no altera los recorridos de
// impresión existentes (pre-orden/post-orden).
void ArbolSubtareas::recorrerConFiltro(const std::function<bool(Tarea*)>& filtro,
                                       const std::function<void(Tarea*)>& accion) {
    if (raiz == nullptr) {
        return;
    }

    // Función recursiva local que visita un subárbol. Se declara como
    // std::function para poder llamarse a sí misma.
    std::function<void(Tarea*)> visitar = [&](Tarea* nodo) {
        if (nodo == nullptr) {
            return;
        }
        if (filtro(nodo)) {
            accion(nodo);
        }
        vector<Tarea*> hijos = nodo->getSubtareas();
        for (Tarea* hijo : hijos) {
            visitar(hijo);
        }
    };

    // Arranca desde cada hijo de la raíz virtual: no se visita la raíz virtual.
    vector<Tarea*> hijos = raiz->getSubtareas();
    for (Tarea* hijo : hijos) {
        visitar(hijo);
    }
}

// Recursión: cuenta el nodo actual (1) más la suma de los nodos de cada hijo.
// Caso base: nodo nulo, contribuye con 0.
int ArbolSubtareas::contarNodosRec(Tarea* nodo) {
    if (nodo == nullptr) {
        return 0;
    }

    int total = 1;  // Cuenta el nodo actual
    vector<Tarea*> hijos = nodo->getSubtareas();
    for (Tarea* hijo : hijos) {
        total += contarNodosRec(hijo);  // Suma recursivamente los nodos de cada hijo
    }
    return total;
}

// Devuelve el número de tareas reales del bosque.
// Se cuentan los subárboles de cada hijo de la raíz virtual, sin incluir
// a la propia raíz virtual (id 0).
int ArbolSubtareas::contarNodos() {
    if (raiz == nullptr) {
        return 0;
    }

    int total = 0;
    vector<Tarea*> hijos = raiz->getSubtareas();
    for (Tarea* hijo : hijos) {
        total += contarNodosRec(hijo);
    }
    return total;
}

// Recursión: altura = 1 + la mayor altura entre los hijos.
// Caso base: nodo nulo devuelve -1, así un nodo hoja tiene altura 0.
int ArbolSubtareas::alturaRec(Tarea* nodo) {
    if (nodo == nullptr) {
        return -1;
    }

    int alturaMax = -1;
    vector<Tarea*> hijos = nodo->getSubtareas();
    for (Tarea* hijo : hijos) {
        int alturaHijo = alturaRec(hijo);  // Calcula recursivamente la altura de cada hijo
        if (alturaHijo > alturaMax) {
            alturaMax = alturaHijo;
        }
    }
    return alturaMax + 1;
}

// Devuelve la altura real del bosque (-1 si no hay tareas).
// alturaRec cuenta también el nivel de la raíz virtual, así que se descuenta
// 1: con una sola tarea de nivel superior la altura es 0, igual que un árbol
// clásico con una única raíz real.
int ArbolSubtareas::altura() {
    if (raiz == nullptr) {
        return -1;
    }
    return alturaRec(raiz) - 1;
}