# UNIVERSIDAD NACIONAL
## Sede Regional Central Occidente
### Curso: EIF207 - Estructuras de Datos
**Proyecto de Investigación Aplicada #1: Sistema Gestor de Flujos de Trabajo (Workflow Manager)**

---

## 1. Descripción del Proyecto

El **Sistema Gestor de Flujos de Trabajo** (Workflow Manager) es una aplicación de consola desarrollada en C++17 que simula la administración, priorización y ejecución de procesos dentro de una organización. 

El sistema implementa de forma nativa (desde cero y con gestión manual de memoria) las siguientes **Estructuras de Datos (TDA)**:
- **Lista Doblemente Enlazada (`ListaUsuarios`):** Gestión completa de usuarios (Administrador y Usuario Normal) con punteros anterior y siguiente, evitando duplicados de ID.
- **Pila (`PilaHistorial` / `Accion`):** Pila LIFO doble para habilitar el mecanismo estricto de **Deshacer (Undo)** y **Rehacer (Redo)**.
- **Cola FIFO (`ColaTareas`) y Cola de Prioridad (`ColaPrioridad`):** Motor de ejecución con soporte para prioridades (`ALTA`, `MEDIA`, `BAJA`).
- **Control de SLA y Escalamiento Automático (Prevención de Inanición):** Contador de ciclos de espera por tarea. Si una tarea regular acumula 3 ciclos de espera, se escala automáticamente a la Cola de Prioridad con prioridad `ALTA`.
- **Árbol General (`ArbolSubtareas`):** Bosque de tareas con raíz virtual que permite modelar y recorrer jerarquías recursivas de subtareas con indentación.
- **Algoritmos de Ordenamiento y Análisis Asintótico (`Ordenador`):** Implementación de **Bubble Sort**, **Merge Sort** y **Quick Sort** en plantilla genérica con análisis formal Big-O.
- **Persistencia CSV y Bitácora Transaccional (`GestorArchivosCSV`):** Carga y guardado automático en `usuarios.csv`, `tareas.csv`, `subtareas.csv` y registro inmutable en `auditoria_log.csv`.
- **Seguridad en Autenticación:** Ocultamiento de contraseñas en consola en tiempo real mediante `termios` en Linux y `conio.h` en Windows.

---

## 2. Compilación y Ejecución

### Requisitos
- Compilador C++17 (`g++` o `clang++`).
- Sistema Operativo: Linux / macOS / Windows.

### Compilar y Ejecutar la Aplicación Principal
```bash
g++ -std=c++17 -Wall *.cpp -o workflow_manager
./workflow_manager
```

### Compilar y Ejecutar Pruebas Unitarias
```bash
# Pruebas de TDAs (Colas y Árbol)
g++ -std=c++17 -Wall test/pruebas.cpp Tarea.cpp ColaTareas.cpp ColaPrioridad.cpp ArbolSubtareas.cpp NodoTarea.cpp -o test_pruebas
./test_pruebas

# Pruebas de Autenticación
g++ -std=c++17 -Wall test_Autenticacion.cpp Autenticacion.cpp ListaUsuarios.cpp Usuario.cpp -o test_auth
./test_auth

# Pruebas de Algoritmos de Ordenamiento
g++ -std=c++17 -Wall test_Ordenador.cpp Ordenador.cpp -o test_ord
./test_ord
```

---

## 3. Análisis de Complejidad Asintótica (Notación Big-O)

| Estructura / Algoritmo | Operación / Algoritmo | Mejor Caso | Caso Promedio | Peor Caso | Memoria Auxiliar |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **ListaUsuarios** | Buscar / Eliminar por ID | $O(1)$ | $O(n)$ | $O(n)$ | $O(1)$ |
| **PilaHistorial** | Deshacer / Rehacer | $O(1)$ | $O(1)$ | $O(1)$ | $O(1)$ |
| **ColaPrioridad** | Encolar tarea ordenada | $O(1)$ | $O(n)$ | $O(n)$ | $O(1)$ |
| **ArbolSubtareas** | Recorrido / Búsqueda | $O(1)$ | $O(n)$ | $O(n)$ | $O(h)$ (altura) |
| **Ordenador** | **Bubble Sort** | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ |
| **Ordenador** | **Merge Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ |
| **Ordenador** | **Quick Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ |

---

## 4. Bitácora de Inteligencia Artificial (IA Log)

| Módulo / Estructura | Prompt Exacto Utilizado | Proceso de Revisión y Ajuste Humano |
| :--- | :--- | :--- |
| **`Autenticacion`** | *"Genera una función portable en C++ para leer contraseñas ocultando caracteres por pantalla en Linux (termios) y Windows (conio.h)."* | Se validó el patrón RAII `DesactivadorEco` para garantizar la restauración del estado del terminal en Linux aun ante excepciones. |
| **`Ordenador`** | *"Implementa una plantilla C++ desacoplada para Bubble Sort, Merge Sort y Quick Sort aceptando un comparador `bool(*)(const T&, const T&)`."* | Se verificó la gestión manual de memoria en `Merge Sort` (`new[]`/`delete[]`), se adaptó Lomuto central en `Quick Sort` y se agregaron instanciaciones explícitas. |
| **`ArbolSubtareas`** | *"Diseña una estructura de árbol general en C++ con raíz virtual para soportar bosques de tareas con subtareas infinitas y recorridos de filtrado."* | Se ajustó la firma del buscador y eliminador para excluir el ID 0 de la raíz virtual y evitar fugas de memoria al eliminar nodos con subtareas hijas. |

---

## 5. Gestión del Repositorio y Git Workflow

El proyecto se gestionó siguiendo las mejores prácticas de la industria:
- **`main`:** Rama principal estable y lista para producción.
- **Ramas de Funcionalidad (`feature/*`):**
  - `feature/usuarios`: Implementación de `ListaUsuarios` doblemente enlazada.
  - `feature/tareas-arbol`: Estructuras de `ColaTareas`, `ColaPrioridad` y `ArbolSubtareas`.
  - `feature/historial-pila`: Implementación de `PilaHistorial` y clase `Accion`.
  - `feature/ordenamiento`: Módulo `Ordenador` y pruebas unitarias.
  - `feature/persistencia-sla`: `GestorArchivosCSV`, control de SLA y bitácora de auditoría.
- **Pull Requests & Code Reviews:** Todas las integraciones se realizaron mediante Pull Requests previa ejecución y aprobación del suite de pruebas unitarias.
