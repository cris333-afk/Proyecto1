#ifndef ORDENADOR_H
#define ORDENADOR_H

/**
 * Comparador<T>: alias del tipo de la funcion de comparacion que reciben los
 * algoritmos de ordenamiento.
 *
 * Recibe dos elementos del arreglo (por referencia constante) y devuelve true
 * si "a" debe quedar antes que "b" segun el criterio deseado. Asi, Ordenador
 * no necesita conocer la estructura interna de los elementos que ordena
 * (por ejemplo Tarea): quien invoca es quien define el criterio.
 *
 * Ejemplo de criterio por id de Tarea:
 *   bool menorPorId(const Tarea& a, const Tarea& b) { return a.getId() < b.getId(); }
 */
template <typename T>
using Comparador = bool (*)(const T&, const T&);

/**
 * Clase Ordenador<T>: ofrece tres algoritmos de ordenamiento reutilizables
 * (Bubble Sort, Merge Sort y Quick Sort) sobre arreglos de elementos genericos.
 *
 * 1) Es una PLANTILLA sobre el tipo T del arreglo: sirve para int, std::string,
 *    punteros a Tarea (Tarea*), etc.
 * 2) Esta DESACOPLADA de las clases del proyecto: no conoce el criterio de orden
 *    ni llama a metodos de Tarea. Cada algoritmo recibe un "comparador" que
 *    decide si un elemento precede a otro.
 * 3) Manejo manual de memoria: Merge Sort utiliza new[]/delete[] para el arreglo
 *    temporal que necesita en la fase de mezcla.
 * 4) La implementacion vive en Ordenador.cpp. Por ello, cada tipo T con el que
 *    se use la clase debe estar declarado en las "instanciaciones explicitas"
 *    de ese .cpp (por defecto estan int, std::string y Tarea*).
 *
 * Complejidad temporal de cada algoritmo:
 *   - burbuja():   mejor caso O(n)   (arreglo ya ordenado, gracias a la bandera)
 *                  promedio y peor  O(n^2)
 *   - mergeSort(): O(n log n) en los tres casos (divide y vence + mezcla)
 *   - quickSort(): promedio y mejor O(n log n);  peor caso O(n^2)
 */
template <typename T>
class Ordenador {
public:
    /**
     * Ordena el arreglo con Bubble Sort.
     * Recorre el arreglo en pasadas: en cada pasada compara elementos contiguos
     * y los intercambia si estan desordenados, "flotando" el mayor hacia el
     * final. Si en una pasada no hubo intercambios, el arreglo ya esta ordenado
     * y se detiene (mejor caso O(n)).
     * @param arreglo    Puntero al primer elemento (arreglo a ordenar, in-place).
     * @param tamano     Cantidad de elementos del arreglo.
     * @param comparador Criterio de orden: true si "a" precede a "b".
     */
    void burbuja(T* arreglo, int tamano, Comparador<T> comparador);

    /**
     * Ordena el arreglo con Merge Sort (divide y vence, O(n log n)).
     * Divide el arreglo en mitades hasta listas de un solo elemento, y luego
     * las mezcla de a pares en orden. Necesita memoria extra (new[]/delete[]).
     * @param arreglo    Puntero al primer elemento (se ordena in-place).
     * @param tamano     Cantidad de elementos del arreglo.
     * @param comparador Criterio de orden: true si "a" precede a "b".
     */
    void mergeSort(T* arreglo, int tamano, Comparador<T> comparador);

    /**
     * Ordena el arreglo con Quick Sort (pivote + particion + recursion).
     * Elige un pivote (el elemento central), particiona el arreglo de modo que
     * los menores queden a la izquierda y los mayores a la derecha, y recursa
     * sobre cada particion. Promedio O(n log n), peor O(n^2).
     * @param arreglo    Puntero al primer elemento (se ordena in-place).
     * @param tamano     Cantidad de elementos del arreglo.
     * @param comparador Criterio de orden: true si "a" precede a "b".
     */
    void quickSort(T* arreglo, int tamano, Comparador<T> comparador);

private:
    // Divide recursivamente [inicio,fin] en mitades y llama a mezclar() (Merge Sort).
    void ordenarMezcla(T* arreglo, int inicio, int fin, T* temporal, Comparador<T> comparador);

    // Mezcla dos sublistas contiguas ya ordenadas [inicio..medio] y [medio+1..fin].
    void mezclar(T* arreglo, int inicio, int medio, int fin, T* temporal, Comparador<T> comparador);

    // Particiona con pivote y recursa sobre las dos partes (Quick Sort).
    void ordenarParticion(T* arreglo, int inicio, int fin, Comparador<T> comparador);

    // Coloca el pivote en su posicion final y devuelve su indice (Lomuto).
    int particion(T* arreglo, int inicio, int fin, Comparador<T> comparador);
};

#endif // ORDENADOR_H