#include "ArbolSubtareas.h"
#include <iostream>

using namespace std;

// Constructor por defecto: árbol vacío
ArbolSubtareas::ArbolSubtareas() {
    raiz = nullptr;
}

// Constructor con raíz inicial
ArbolSubtareas::ArbolSubtareas(Tarea* raiz) {
    this->raiz = raiz;
}

// Destructor: libera toda la memoria del árbol.
// delete raiz invoca el destructor de Tarea, que borra recursivamente todas sus subtareas.
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

// Busca una tarea por id, devuelve nullptr si no existe
Tarea* ArbolSubtareas::buscar(int id) {
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

// Elimina la tarea con el id indicado (junto con sus descendientes)
bool ArbolSubtareas::eliminar(int id) {
    if (raiz == nullptr) {
        return false;
    }

    // Si la raíz es la que se elimina, se borra todo el árbol
    if (raiz->getId() == id) {
        delete raiz;  // El destructor de Tarea borra recursivamente las subtareas
        raiz = nullptr;
        return true;
    }

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

// Recorrido pre-orden del árbol
void ArbolSubtareas::recorrerPreOrden() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío." << endl;
        return;
    }
    recorrerPreOrdenRec(raiz);
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

// Recorrido post-orden del árbol
void ArbolSubtareas::recorrerPostOrden() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío." << endl;
        return;
    }
    recorrerPostOrdenRec(raiz);
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

// Devuelve el número total de nodos del árbol
int ArbolSubtareas::contarNodos() {
    return contarNodosRec(raiz);
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

// Devuelve la altura del árbol (-1 si está vacío)
int ArbolSubtareas::altura() {
    return alturaRec(raiz);
}