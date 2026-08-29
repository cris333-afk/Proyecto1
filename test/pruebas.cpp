#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <functional>

#include "../Tarea.h"
#include "../ColaTareas.h"
#include "../ColaPrioridad.h"
#include "../ArbolSubtareas.h"

using namespace std;

// Ejecuta una función capturando la salida de cout en un string
string capturarSalida(function<void()> fn) {
    stringstream buffer;
    streambuf* old = cout.rdbuf(buffer.rdbuf());
    fn();
    cout.rdbuf(old);
    return buffer.str();
}

// Extrae los IDs de las líneas "Tarea ID: X" de la salida
vector<int> obtenerIds(const string& salida) {
    vector<int> ids;
    istringstream iss(salida);
    string token;
    while (iss >> token) {
        if (token == "ID:") {
            int id;
            iss >> id;
            ids.push_back(id);
        }
    }
    return ids;
}

// ===== Pruebas de ColaTareas (FIFO) =====

void probarColaVacia() {
    ColaTareas cola;
    // La cola debe estar vacía al inicio
    assert(cola.estaVacia());
    // frente() debe devolver nullptr en cola vacía
    assert(cola.frente() == nullptr);
    // desencolar() en cola vacía no debe crashear
    cola.desencolar();
    assert(cola.estaVacia());
    cout << "[OK] ColaTareas: cola vacía manejada sin crashear" << endl;
}

void probarOrdenFIFO() {
    ColaTareas cola;
    Tarea* t1 = new Tarea(1, "ALTA", "pendiente");
    Tarea* t2 = new Tarea(2, "MEDIA", "pendiente");
    Tarea* t3 = new Tarea(3, "BAJA", "pendiente");

    cola.encolar(t1);
    cola.encolar(t2);
    cola.encolar(t3);

    // El frente debe ser la primera tarea encolada (FIFO)
    assert(cola.frente() == t1);
    assert(cola.tamano() == 3);

    // Salen en el mismo orden en que entraron: t1, t2, t3
    cola.desencolar();
    assert(cola.frente() == t2);
    cola.desencolar();
    assert(cola.frente() == t3);
    cola.desencolar();
    assert(cola.estaVacia());

    // Libera la memoria de las tareas (la cola solo gestiona los nodos)
    delete t1;
    delete t2;
    delete t3;

    cout << "[OK] ColaTareas: orden FIFO correcto" << endl;
}

// ===== Pruebas de ColaPrioridad =====

void probarOrdenPrioridad() {
    ColaPrioridad cola;
    Tarea* baja = new Tarea(1, "BAJA", "pendiente");
    Tarea* alta = new Tarea(2, "ALTA", "pendiente");
    Tarea* media = new Tarea(3, "MEDIA", "pendiente");
    Tarea* alta2 = new Tarea(4, "ALTA", "pendiente");

    // Se encolan en desorden
    cola.encolar(baja);
    cola.encolar(alta);
    cola.encolar(media);
    cola.encolar(alta2);

    // Debe salir primero ALTA (2), luego ALTA (4), luego MEDIA, luego BAJA
    assert(cola.frente() == alta);
    cola.desencolar();
    assert(cola.frente() == alta2);
    cola.desencolar();
    assert(cola.frente() == media);
    cola.desencolar();
    assert(cola.frente() == baja);
    cola.desencolar();
    assert(cola.estaVacia());

    // cola vacía: frente() devuelve nullptr, desencolar() no crashea
    assert(cola.frente() == nullptr);
    cola.desencolar();

    // Libera la memoria de las tareas (la cola solo gestiona los nodos)
    delete baja;
    delete alta;
    delete media;
    delete alta2;

    cout << "[OK] ColaPrioridad: orden ALTA > MEDIA > BAJA correcto" << endl;
}

// ===== Pruebas de ArbolSubtareas =====

void probarArbol() {
    // Construcción de un árbol con 5 niveles de profundidad:
    // Raíz 1 -> 2 -> 3 -> 4 -> 5  (5 niveles)
    //        \-> 6
    Tarea* raiz = new Tarea(1, "ALTA", "pendiente");
    Tarea* n2 = new Tarea(2, "MEDIA", "pendiente");
    Tarea* n3 = new Tarea(3, "BAJA", "pendiente");
    Tarea* n4 = new Tarea(4, "ALTA", "pendiente");
    Tarea* n5 = new Tarea(5, "MEDIA", "pendiente");
    Tarea* n6 = new Tarea(6, "BAJA", "pendiente");

    ArbolSubtareas arbol(raiz);

    assert(arbol.agregarSubtarea(1, n2));
    assert(arbol.agregarSubtarea(2, n3));
    assert(arbol.agregarSubtarea(3, n4));
    assert(arbol.agregarSubtarea(4, n5));
    assert(arbol.agregarSubtarea(1, n6));

    // Conteo de nodos: 1, 2, 3, 4, 5, 6 = 6 nodos
    assert(arbol.contarNodos() == 6);

    // Altura: 5 niveles -> altura 4
    assert(arbol.altura() == 4);

    // Buscar subtarea existente
    Tarea* encontrada = arbol.buscar(5);
    assert(encontrada != nullptr);
    assert(encontrada->getId() == 5);

    // Buscar subtarea inexistente
    assert(arbol.buscar(99) == nullptr);
    assert(arbol.buscar(0) == nullptr);

    // Recorrido pre-orden: raíz primero, luego hijos
    string salidaPre = capturarSalida([&]() { arbol.recorrerPreOrden(); });
    vector<int> idsPre = obtenerIds(salidaPre);
    assert(idsPre.size() == 6);
    assert(idsPre[0] == 1 && idsPre[1] == 2 && idsPre[2] == 3 &&
           idsPre[3] == 4 && idsPre[4] == 5 && idsPre[5] == 6);

    // Recorrido post-orden: hijos primero, luego raíz
    string salidaPost = capturarSalida([&]() { arbol.recorrerPostOrden(); });
    vector<int> idsPost = obtenerIds(salidaPost);
    assert(idsPost.size() == 6);
    assert(idsPost[0] == 5 && idsPost[1] == 4 && idsPost[2] == 3 &&
           idsPost[3] == 2 && idsPost[4] == 6 && idsPost[5] == 1);

    // Eliminar nodo hoja (id=6)
    assert(arbol.eliminar(6));
    assert(arbol.contarNodos() == 5);
    assert(arbol.buscar(6) == nullptr);

    // Eliminar nodo con hijos (id=3): borra 3, 4 y 5
    assert(arbol.eliminar(3));
    assert(arbol.contarNodos() == 2);  // Quedan 1 y 2
    assert(arbol.buscar(3) == nullptr);
    assert(arbol.buscar(4) == nullptr);
    assert(arbol.buscar(5) == nullptr);
    assert(arbol.buscar(2) != nullptr);

    // Eliminar nodo inexistente devuelve false
    assert(!arbol.eliminar(99));

    cout << "[OK] ArbolSubtareas: 5 niveles, búsqueda, eliminación, recorridos, conteo y altura" << endl;
}

int main() {
    probarColaVacia();
    probarOrdenFIFO();
    probarOrdenPrioridad();
    probarArbol();

    cout << endl << "Todos los tests pasaron correctamente." << endl;
    return 0;
}