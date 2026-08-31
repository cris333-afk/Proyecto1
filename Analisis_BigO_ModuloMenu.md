# Análisis de Complejidad (Big-O) — Módulo Menú (CLI)
### Sistema Gestor de Flujos de Trabajo — EIF207 Estructuras de Datos
**Módulo:** Ángel (Sistema e Integración) · **Archivos:** `CLI`, `GestorSistema`, `ValidacionEntrada`, `EntradaSegura`

Se define **n** como la cantidad de usuarios almacenados en `ListaUsuarios`
(la única estructura de datos que este módulo toca de verdad por ahora). A
diferencia del Módulo Usuarios, el CLI no recorre estructuras propias: sus
funciones o bien hacen trabajo constante (mostrar texto, validar un dato), o
bien **delegan** en `GestorSistema`, que a su vez delega en `ListaUsuarios`.

## ValidacionEntrada y EntradaSegura (lectura de consola)

| Función | Complejidad | Justificación |
|---|---|---|
| `leerEnteroValidado()` | **O(r)** | *r* = número de intentos hasta que la persona escriba algo válido. No depende de *n*; en el mejor caso (primer intento válido) es O(1). |
| `leerTextoNoVacio()` | **O(r)** | Mismo razonamiento: depende de cuántas veces la persona deja el campo vacío antes de escribir algo. |
| `leerContrasenaOculta()` | **O(L)** | *L* = cantidad de caracteres que la persona escribe en la contraseña; se procesa uno por uno para mostrar el asterisco. En la práctica, L es pequeño y constante. |

Estas tres funciones son las únicas cuyo costo depende del **usuario que está
tecleando**, no del tamaño de los datos del sistema — es un tipo de
complejidad distinto al de recorrer una lista.

## GestorSistema (la fachada)

| Método | Complejidad | Justificación |
|---|---|---|
| `GestorSistema()` (constructor) | **O(1)** | Revisa `estaVacia()` (O(1)) y, si aplica, inserta un único admin en una lista vacía (`insertar` sobre 0 elementos). |
| `iniciarSesion()` | **O(n)** | Delega en `ListaUsuarios::buscar()`. |
| `cerrarSesion()` / `getUsuarioActual()` | **O(1)** | Solo leen/asignan un puntero. |
| `agregarUsuario()` | **O(n)** | Delega en `ListaUsuarios::insertar()` (valida ID duplicado). |
| `actualizarUsuario()` | **O(n)** | Delega en `ListaUsuarios::modificar()`. |
| `eliminarUsuario()` | **O(n)** | Delega en `ListaUsuarios::eliminar()`. |
| `listarUsuarios()` | **O(n)** | Delega en `ListaUsuarios::mostrarAdelante()`. |
| `buscarUsuario()` | **O(n)** | Delega en `ListaUsuarios::buscar()`. |
| `agregarTarea`, `actualizarTarea`, `eliminarTarea`, `listarTareasPendientes`, `buscarTarea`, `cambiarEstadoTarea`, `deshacer`, `rehacer`, `guardarTodo`, `cargarTodo`, `mostrarReporteOrdenado` | **O(1)** | Hoy solo imprimen un aviso de "pendiente de integración" y retornan; no hacen trabajo real todavía. **Este análisis va a cambiar** en cuanto Cris, Liseth y Ángel conecten sus estructuras reales — por ejemplo, `mostrarReporteOrdenado()` pasará a depender del algoritmo de ordenamiento que use `Ordenador`. |

## CLI (las pantallas y menús)

| Método | Complejidad | Justificación |
|---|---|---|
| `mostrarBienvenida()` | **O(1)** | Imprime un texto fijo. |
| `iniciarSesion()` | **O(n)** | Trabajo propio O(1) + `leerEnteroValidado`/`leerContrasenaOculta` (O(r), O(L)) + `gestor.iniciarSesion()` (O(n)). El término que domina es el de la búsqueda en la lista. |
| `menuAdministrador()`, `menuUsuarioNormal()`, `submenuGestionUsuarios()`, `submenuGestionTareas()` | **O(1) por vuelta del menú, más el costo de la opción elegida** | Cada iteración del bucle solo imprime un número fijo de opciones y lee una elección (O(1)/O(r)). El costo real depende de qué opción se llame: O(n) si es una operación de usuarios, O(1) si es una de las pendientes de integración. El número de vueltas depende de cuántas veces la persona use el menú antes de salir — no de *n*. |
| `ejecutar()` | **O(n)** en el peor caso de una sesión | Une el login (O(n)) con el menú correspondiente; el total de una sesión depende de cuántas acciones ejecute la persona, cada una con su propio costo. |

## Diferencia clave con el Módulo Usuarios

En `ListaUsuarios`, *n* mide el tamaño de una estructura que el propio módulo
recorre. Aquí, el CLI **no recorre nada por su cuenta**: todo el O(n) que
aparece arriba es "heredado" de las llamadas a `ListaUsuarios` a través de
`GestorSistema`. Si `ListaUsuarios` cambiara de implementación (por ejemplo, a
una tabla hash con búsqueda O(1) promedio), el CLI heredaría esa mejora
automáticamente sin que su propio código cambie — señal de que la separación
entre capas (CLI → GestorSistema → ListaUsuarios) está bien planteada.

## Complejidad espacial

`CLI` y `GestorSistema` no usan memoria adicional que crezca con *n*: solo
guardan un puntero al usuario con sesión iniciada y una instancia de
`ListaUsuarios` (cuyo propio costo en memoria, O(n), ya se documentó en el
análisis del Módulo Usuarios). El espacio propio de este módulo es **O(1)**.
