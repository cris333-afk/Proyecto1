#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Ordenador.h"

using namespace std;

// Comparador de ejemplo: orden ascendente por valor (int).
bool asc(const int& a, const int& b) { return a < b; }

// Comparador de ejemplo: orden descendente por valor (int).
bool mayor(const int& a, const int& b) { return a > b; }

// Declaracion adelantada: Tarea es de otro companero, NO se incluye ni se
// modifica aqui. Basta para ordenar un arreglo de punteros Tarea*.
class Tarea;

// Comparador sobre punteros a Tarea: en la prueba los punteros realmente
// apuntan a ints, asi que se lee el valor apuntado. En la integracion real,
// GestorSistema usara aqui a->getId(), getPrioridad(), etc. de Tarea.
bool menorPtr(Tarea* const& a, Tarea* const& b) {
    return *reinterpret_cast<int*>(a) < *reinterpret_cast<int*>(b);
}

// Copia el arreglo origen en el destino (n elementos).
void copiar(const int* origen, int* destino, int n) {
    for (int i = 0; i < n; i++) destino[i] = origen[i];
}

// Indica si el arreglo esta ordenado ascendentemente.
bool estaOrdenado(const int* arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

// Verifica que dos arreglos sean identicos (los tres algoritmos deben coincidir).
bool iguales(const int* a, const int* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Prueba un caso con los tres algoritmos y comprueba que todos den el mismo
// resultado ordenado correctamente.
void probarCaso(const char* nombre, const int* datos, int n) {
    int arrB[64], arrM[64], arrQ[64];
    copiar(datos, arrB, n);
    copiar(datos, arrM, n);
    copiar(datos, arrQ, n);

    Ordenador<int> ordenador;
    ordenador.burbuja(arrB, n, asc);
    ordenador.mergeSort(arrM, n, asc);
    ordenador.quickSort(arrQ, n, asc);

    bool okBurbuja = estaOrdenado(arrB, n);
    bool okMerge   = estaOrdenado(arrM, n);
    bool okQuick   = estaOrdenado(arrQ, n);
    bool coinciden = iguales(arrB, arrM, n) && iguales(arrM, arrQ, n);

    cout << (okBurbuja && okMerge && okQuick && coinciden ? "[OK]  " : "[FALLO] ")
         << nombre;
    if (!okBurbuja) cout << " (burbuja)";
    if (!okMerge)   cout << " (merge)";
    if (!okQuick)   cout << " (quick)";
    if (!coinciden) cout << " (resultados difieren)";
    cout << endl;
}

int main() {
    // Coleccion vacia (0 elementos).
    probarCaso("vacia", nullptr, 0);

    // Un solo elemento.
    int uno[] = {7};
    probarCaso("un elemento", uno, 1);

    // Varios elementos desordenados.
    int desord[] = {5, 2, 9, 1, 7, 3, 6};
    probarCaso("desordenado", desord, 7);

    // Elementos repetidos.
    int repet[] = {4, 2, 4, 1, 4, 2, 1};
    probarCaso("repetidos", repet, 7);

    // Ya ordenados (mejor caso de burbuja).
    int yaOrd[] = {1, 2, 3, 4, 5};
    probarCaso("ya ordenado", yaOrd, 5);

    // Orden inverso.
    int inverso[] = {9, 7, 5, 3, 1};
    probarCaso("orden inverso", inverso, 5);

    // Datos aleatorios (cobertura "aleatorias" de los requisitos).
    srand(static_cast<unsigned>(time(nullptr)));
    int aleat[40];
    for (int i = 0; i < 40; i++) aleat[i] = rand() % 100;
    probarCaso("aleatorias (40)", aleat, 40);

    // Orden descendente (comprobacion de que el comparador define el criterio).
    Ordenador<int> ordDesc;
    int desc[] = {3, 1, 4, 1, 5};
    int tamDesc = 5;
    ordDesc.quickSort(desc, tamDesc, mayor);
    bool okDesc = true;
    for (int i = 1; i < tamDesc; i++) if (desc[i - 1] < desc[i]) okDesc = false;
    cout << (okDesc ? "[OK]  desc descendente (criterio definido por comparador)"
                    : "[FALLO] desc descendente") << endl;

    // Ordenar un arreglo de Tarea* sin necesitar la definicion de Tarea:
    // Ordenador solo mueve punteros usando el comparador (desacoplamiento real).
    int valores[4] = {30, 10, 40, 20};
    Tarea* ptrs[4];
    for (int i = 0; i < 4; i++) {
        ptrs[i] = reinterpret_cast<Tarea*>(&valores[i]);  // Direcciones validas.
    }

    Ordenador<Tarea*> ordPtr;
    ordPtr.mergeSort(ptrs, 4, menorPtr);

    int esperado[4] = {10, 20, 30, 40};
    bool okPtr = true;
    for (int i = 0; i < 4; i++) {
        if (*reinterpret_cast<int*>(ptrs[i]) != esperado[i]) okPtr = false;
    }
    cout << (okPtr ? "[OK]  Tarea* ordenado sin incluir Tarea.h (desacoplado)"
                   : "[FALLO] Tarea*") << endl;

    return 0;
}