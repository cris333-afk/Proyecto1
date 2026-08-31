# Análisis de Complejidad (Big-O) — Módulo Usuarios
### Sistema Gestor de Flujos de Trabajo — EIF207 Estructuras de Datos
**Autor:** César · **Estructura:** Lista doblemente enlazada (`ListaUsuarios`)

Se define **n** como la cantidad de usuarios almacenados en la lista al momento de
ejecutar la operación. La lista mantiene punteros a `cabeza` y `cola`, además de un
contador `cantidad` que se actualiza en cada inserción/eliminación.

## Tabla resumen

| Operación | Complejidad temporal | Justificación |
|---|---|---|
| `insertar()` | **O(n)** | Se debe recorrer la lista para verificar que el ID no esté duplicado (`existeId`), lo cual domina el costo. La inserción en sí, al final de la lista, es O(1) gracias al puntero `cola`. |
| `eliminar()` | **O(n)** | Depende de `buscar()` para localizar el nodo (O(n) en el peor caso). Una vez localizado, desconectar el nodo ajustando los punteros vecinos es O(1), sin importar si es cabeza, cola o nodo intermedio. |
| `buscar()` | **O(n)** | En el peor caso (usuario inexistente, o ubicado al final) se recorre la lista completa desde `cabeza`. |
| `modificar()` | **O(n)** | Reutiliza `buscar()` (O(n)); la actualización de los campos ya localizados es O(1). |
| `mostrarAdelante()` | **O(n)** | Se visita cada nodo una vez, de `cabeza` a `cola`. |
| `mostrarAtras()` | **O(n)** | Se visita cada nodo una vez, de `cola` a `cabeza`, usando los punteros `anterior`. |
| `existeId()` | **O(n)** | Recorrido lineal equivalente a `buscar()`. |
| `contar()` | **O(1)** | Se mantiene un contador `cantidad` actualizado incrementalmente; no se recorre la lista. |
| `estaVacia()` | **O(1)** | Solo compara `cabeza == nullptr`. |
| `autenticar()` | **O(n)** | Reutiliza `buscar()` (O(n)); la comparación de contraseña es O(1). |
| `guardarCSV()` | **O(n)** | Recorre la lista completa una sola vez para escribir cada usuario. |
| `cargarCSV()` | **O(n²)** en el peor caso | Si el archivo tiene *m* líneas, se llama `insertar()` *m* veces; cada llamada valida duplicados recorriendo la porción ya construida de la lista, acumulando 1+2+...+m ≈ O(m²). |

## Complejidad espacial

Cada nodo `Usuario` ocupa espacio constante adicional (dos punteros `anterior`/`siguiente`,
un entero `id`, y las cadenas `nombre`/`contrasena`/`rol`). La lista completa ocupa **O(n)**
en memoria. No se usan arreglos auxiliares de tamaño fijo: toda la memoria se reserva
dinámicamente con `new` dentro de `insertar()` y se libera explícitamente en `eliminar()` y
en el destructor `~ListaUsuarios()`, evitando fugas de memoria (verificado con pruebas y
sin arrojar errores de acceso en la ejecución de `PruebasModuloUsuarios.cpp`).

## Notas de diseño

- Mantener el puntero `cola` permite que "enganchar" un nuevo usuario al final sea O(1),
  evitando recorrer toda la lista solo para insertar.
- El costo O(n) de `insertar()`, `eliminar()`, `buscar()` y `modificar()` es inherente a
  una lista enlazada (a diferencia de un arreglo indexado o una tabla hash): no existe
  acceso aleatorio por ID, siempre hay que recorrer nodo por nodo.
- Posible optimización futura para `cargarCSV()`: usar una tabla hash auxiliar solo
  durante la carga masiva para detectar duplicados en O(1) promedio, bajando el costo
  total de O(n²) a O(n). No se implementó para mantener el diseño apegado estrictamente
  a la lista doblemente enlazada que pide el enunciado del proyecto.
