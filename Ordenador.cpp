#include <string>

#include "Ordenador.h"

// Bubble Sort (Burbuja)
// Estrategia: en cada pasada se comparan elementos adyacentes y se intercambian
// si estan desordenados; el mayor "flota" hacia el final. La bandera
// "intercambio" detecta si el arreglo quedo ordenado antes de tiempo
// (mejor caso O(n) para un arreglo ya ordenado).
// ---------------------------------------------------------------------------
template <typename T>
void Ordenador<T>::burbuja(T* arreglo, int tamano, Comparador<T> comparador) {
    bool intercambio;  // Indica si en la pasada actual hubo algun intercambio.
    // Cada iteracion de "i" deja posicionado el i-esimo mayor en su sitio final.
    for (int i = 0; i < tamano - 1; i++) {
        intercambio = false;
        // Se recorre solo hasta la zona aun desordenada (los ultimos ya estan ordenados).
        for (int j = 0; j < tamano - 1 - i; j++) {
            // comparador(a, b) es true cuando "a" debe ir ANTES que "b".
            // Si el de la derecha (j+1) debe ir antes que el de la izquierda (j), se intercambian.
            if (comparador(arreglo[j + 1], arreglo[j])) {
                T temp = arreglo[j];
                arreglo[j] = arreglo[j + 1];
                arreglo[j + 1] = temp;
                intercambio = true;
            }
        }
        // Si no hubo intercambios, el arreglo esta ordenado: se detiene (optimizacion).
        if (!intercambio) {
            break;
        }
    }
}

// Merge Sort (Ordenamiento por mezcla)
// Estrategia: "dividir y vencer". Se divide el arreglo en mitades recursivamente
// hasta sublistas de un solo elemento (por definicion ordenadas), y luego se
// mezclan las mitades ya ordenadas. Mezclar dos listas de O(n) elementos cuesta
// O(n), y la division produce log(n) niveles => O(n log n) en todos los casos.
// Requiere memoria auxiliar (arreglo temporal) para la fase de mezcla.
// ---------------------------------------------------------------------------

// Ordena Merge Sort sobre la porcion [0, tamano-1].
template <typename T>
void Ordenador<T>::mergeSort(T* arreglo, int tamano, Comparador<T> comparador) {
    if (tamano <= 1) {
        return;  // Caso trivial: 0 o 1 elementos => ya esta ordenado.
    }
    // Arreglo temporal usado exclusivamente en la mezcla; se libera siempre.
    T* temporal = new T[tamano];
    ordenarMezcla(arreglo, 0, tamano - 1, temporal, comparador);
    delete[] temporal;
}

// Recursion que divide [inicio,fin] a la mitad y luego mezcla las partes.
template <typename T>
void Ordenador<T>::ordenarMezcla(T* arreglo, int inicio, int fin, T* temporal, Comparador<T> comparador) {
    if (inicio >= fin) {
        return;  // Caso base: sublista de 0 o 1 elementos => ordenada.
    }
    int medio = (inicio + fin) / 2;  // Punto de division (parte baja).
    ordenarMezcla(arreglo, inicio, medio, temporal, comparador);      // Ordena la mitad izquierda.
    ordenarMezcla(arreglo, medio + 1, fin, temporal, comparador);     // Ordena la mitad derecha.
    mezclar(arreglo, inicio, medio, fin, temporal, comparador);       // Mezcla ambas mitades.
}

// Mezcla dos sublistas ORDENADAS contiguas:
//   - izquierda en [inicio..medio]
//   - derecha en [medio+1..fin]
// usando un arreglo temporal, y luego copia el resultado al arreglo original.
template <typename T>
void Ordenador<T>::mezclar(T* arreglo, int inicio, int medio, int fin, T* temporal, Comparador<T> comparador) {
    int i = inicio;     // Puntero de lectura en la sublista izquierda.
    int j = medio + 1;  // Puntero de lectura en la sublista derecha.
    int k = inicio;     // Puntero de escritura en el arreglo temporal.

    // Mientras haya elementos en ambas sublistas, se toma siempre el menor de los frentes.
    while (i <= medio && j <= fin) {
        if (comparador(arreglo[i], arreglo[j])) {
            temporal[k++] = arreglo[i++];  // Izquierda es el candidato al menor.
        } else {
            temporal[k++] = arreglo[j++];  // Derecha es el candidato al menor.
        }
    }
    while (i <= medio) {          // Copia lo que quedo en la sublista izquierda.
        temporal[k++] = arreglo[i++];
    }
    while (j <= fin) {            // Copia lo que quedo en la sublista derecha.
        temporal[k++] = arreglo[j++];
    }
    for (int m = inicio; m <= fin; m++) {  // Proyecta el segmento ordenado al arreglo original.
        arreglo[m] = temporal[m];
    }
}

// Quick Sort (Ordenamiento rapido)
// Estrategia: se elige un "pivote" y se reordena el arreglo de modo que los
// elementos menores que el pivote queden a la izquierda y los mayores a la
// derecha (una sola pasada: la "particion"). Luego se recursa sobre cada mitad.
// Promedio O(n log n); peor caso O(n^2). Aqui el pivote es el elemento central
// (heuristica eficaz para evitar el peor caso en arreglos ya ordenados).
// ---------------------------------------------------------------------------

// Ordena Quick Sort sobre la porcion [0, tamano-1].
template <typename T>
void Ordenador<T>::quickSort(T* arreglo, int tamano, Comparador<T> comparador) {
    if (tamano <= 1) {
        return;  // Caso trivial: 0 o 1 elementos.
    }
    ordenarParticion(arreglo, 0, tamano - 1, comparador);
}

// Recursion de Quick Sort sobre el intervalo [inicio,fin].
template <typename T>
void Ordenador<T>::ordenarParticion(T* arreglo, int inicio, int fin, Comparador<T> comparador) {
    if (inicio >= fin) {
        return;  // Caso base: 0 o 1 elementos => ordenado.
    }
    // "particion" coloca el pivote en su posicion final y devuelve ese indice.
    int pivote = particion(arreglo, inicio, fin, comparador);
    ordenarParticion(arreglo, inicio, pivote - 1, comparador);  // Ordena lo estrictamente menor.
    ordenarParticion(arreglo, pivote + 1, fin, comparador);     // Ordena lo mayor o igual.
}

// Particion de Lomuto con pivote central:
// 1) Se escoge el pivote (elemento central) y se guarda su valor.
// 2) Se intercambia el pivote con el ultimo elemento del intervalo.
// 3) Se recorre [inicio, fin-1] manteniendo "i" como frontera de los elementos
//    que deben quedar ANTES del pivote; los que cumplen el criterio se intercambian
//    hacia esa frontera.
// 4) Finalmente se coloca el pivote justo despues de la frontera => posicion final.
template <typename T>
int Ordenador<T>::particion(T* arreglo, int inicio, int fin, Comparador<T> comparador) {
    int medio = (inicio + fin) / 2;   // Indice del pivote: elemento central.
    T pivoteValor = arreglo[medio];   // Valor del pivote (se guarda antes de moverlo).

    // Se lleva el pivote al final para aplicar el esquema de Lomuto.
    T temp = arreglo[medio];
    arreglo[medio] = arreglo[fin];
    arreglo[fin] = temp;

    int i = inicio;  // Frontera: [inicio, i-1] son los que van antes del pivote.
    for (int j = inicio; j < fin; j++) {
        // Si el elemento actual debe ir antes que el pivote, se intercambia con la frontera.
        if (comparador(arreglo[j], pivoteValor)) {
            T temp2 = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = temp2;
            i++;
        }
    }
    // Se coloca el pivote justo despues de los elementos menores => posicion final.
    T temp3 = arreglo[i];
    arreglo[i] = arreglo[fin];
    arreglo[fin] = temp3;
    return i;  // Indice final (correcto) del pivote.
}

// Instanciaciones explicitas.
// Como la implementacion de la plantilla esta en este .cpp, el compilador debe
// generar aqui el codigo para cada tipo concreto que se usara. Si en el futuro
// se necesita ordenar otro tipo (por ejemplo Usuario*), agregar la linea aqui.
template class Ordenador<int>;
template class Ordenador<std::string>;

// Tarea se usa unicamente como puntero (arreglo de Tarea*): basta una
// declaracion adelantada (forward declaration). NO se incluye ni se modifica
// Tarea.h, que pertenece al companero Cris.
class Tarea;
template class Ordenador<Tarea*>;
