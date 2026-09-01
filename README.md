

# Documentación de uso de Herramientas de IA en el desarrollo del Módulo Historial
## Prompt #1: Implementación de Accion.h y Accion.cpp

**Herramienta:** Claude/Cline

**Estructura/función específica:**  
Crear las clases `Accion.h` y `Accion.cpp` que registren operaciones de agregar, actualizar y eliminar sobre usuarios y tareas, sin incluir lógica de ejecución de deshacer/rehacer.

**Prompt utilizado:**

> Actúa como desarrollador C++ siguiendo estrictamente el diseño indicado. Crea las clases `Accion.h` y `Accion.cpp` que registren operaciones de agregar, actualizar y eliminar sobre usuarios y tareas. No incluyas lógica de ejecución de deshacer/rehacer ni dependencias directas con Usuario o Tarea.

### Validación humana

- **Compilación correcta sin warnings:** Se ejecutó `g++ -std=c++17 -c Accion.cpp`. El archivo compiló correctamente. Los `#include <string>` se resolvieron y los enums fueron validados. Como resultado, se generó el archivo objeto `Accion.o`.

- **Desacoplamiento de Usuario y Tarea verificado:** Se realizó una revisión línea por línea de `Accion.h`. No existen `#include "Usuario.h"` ni `#include "Tarea.h"`. El único include es `<string>`. `idEntidad` es de tipo `int` y los datos se almacenan como strings. Se confirmó que el módulo permanece completamente independiente.

- **Getters y setters funcionan correctamente:** Se crearon cinco objetos `Accion` con diferentes combinaciones de `TipoAccion` y `TipoEntidad`. Se invocó cada getter y se verificó el retorno de los valores esperados. El constructor por defecto inicializa `AGREGAR`, `USUARIO` e ID `0`.

- **Enumeraciones bien diseñadas:** Se verificó que `TipoAccion` contiene `AGREGAR`, `ACTUALIZAR` y `ELIMINAR`, mientras que `TipoEntidad` contiene `USUARIO` y `TAREA`. No se incluyeron valores innecesarios.

- **Estados almacenados como strings sin parseo:** Se confirmó que `estadoAnterior` y `estadoPosterior` se almacenan sin interpretación. La interpretación corresponde al módulo Gestor cuando deba aplicar deshacer/rehacer. Esto permite mantener el desacoplamiento del módulo.

**Conclusión:** APROBADO


## Prompt #2: Comentarios a Accion.h y Accion.cpp

**Estructura/función específica:**  
Mejorar la documentación de `Accion.h` y `Accion.cpp` sin modificar el código funcional.

**Prompt utilizado:**

> Realiza una segunda pasada sobre `Accion.h` y `Accion.cpp` únicamente para mejorar la documentación del código. Revisa comentarios de atributos, métodos, firmas y arquitectura. Reglas: comentarios breves, claros, explicando POR QUÉ se usa cada elemento y sin modificar lógica ni código existente.

### Validación humana

- **Comentarios claros sin ser verbosos:** Cada comentario explica por qué existe cada componente. Por ejemplo, antes de `tipoAccion` se indica que representa la operación que modificó la entidad. Los comentarios tienen entre 1 y 3 líneas para mantener la legibilidad.

- **Documentación de enumeraciones completa:** La documentación de `TipoAccion` explica que representa el tipo de operación, mientras que `TipoEntidad` identifica la entidad afectada. También se explica por qué se utilizan dos enums separados.

- **Explicación de desacoplamiento documentada:** Se agregó un comentario explicativo sobre el desacoplamiento mediante `idEntidad` numérico y estados almacenados como strings. Esto permite que el módulo permanezca independiente de implementaciones concretas.

- **No se modificó arquitectura ni lógica:** Se realizó una comparación entre la versión original y la modificada, comprobando que solamente se agregaron comentarios. No hubo cambios en constructores, getters, setters ni lógica. El archivo objeto funciona de manera idéntica.

- **Compilación exitosa después de cambios:** Se ejecutó `g++ -std=c++17 -c Accion.cpp` después de agregar los comentarios. La compilación fue exitosa y sin warnings.

**Conclusión:** APROBADO


## Prompt #3: Implementación PilaHistorial.h y PilaHistorial.cpp

**Estructura/función específica:**  
Implementar `PilaHistorial` con dos `std::stack<Accion>` para gestionar deshacer y rehacer.

**Prompt utilizado:**

> Implementa `PilaHistorial.h` y `PilaHistorial.cpp` utilizando dos estructuras `std::stack` para gestionar el historial de deshacer y rehacer. Asegura el cumplimiento del comportamiento LIFO y la regla donde una nueva acción registrada vacía automáticamente la pila de rehacer.

### Validación humana

- **Comportamiento LIFO correcto:** Se crearon tres acciones (A: ID=1, B: ID=2, C: ID=3) y se registraron en ese orden. Al realizar tres operaciones de deshacer, se obtuvieron C (ID=3), B (ID=2) y A (ID=1), confirmando que la última acción registrada es la primera en deshacer.

- **Nueva acción vacía la pila de rehacer automáticamente:** Se registró A, luego B, se deshizo B y posteriormente se registró C. El resultado fue Deshacer=2 y Rehacer=0, confirmando que una nueva acción elimina las acciones disponibles para rehacer.

- **Deshacer/Rehacer retornan `false` con pila vacía:** Se verificó que `deshacer()` y `rehacer()` retornan `false` cuando sus respectivas pilas están vacías. Los casos límite se manejan de forma segura sin producir errores.

- **Métodos de consulta no modifican las pilas:** Con tres acciones registradas, se invocó `cimaDeshacer()` y se comprobó que la acción superior se obtiene correctamente sin reducir el tamaño de la pila. Una segunda consulta devolvió la misma acción.

- **Limpiar vacía ambas pilas:** Se registraron múltiples acciones y se realizaron algunos deshacer. Después se ejecutó `limpiar()` y se verificó que ambas pilas quedaron vacías.

- **`std::stack` usado correctamente:** Se verificó el uso de `push()`, `pop()`, `top()`, `empty()` y `size()`. No se utiliza acceso mediante índices ni se exponen iteradores, manteniendo la encapsulación de la estructura.

**Conclusión:** APROBADO

## Prompt #4: `mostrarPila()` como método privado

**Estructura/función específica:**  
Convertir la función global `mostrarPila()` en un método privado de `PilaHistorial`.

**Prompt utilizado:**

> Refactoriza el código para mover la función global `mostrarPila()` y convertirla en un método privado dentro de `PilaHistorial.h` y `PilaHistorial.cpp`. Mantén intacta la firma lógica y asegúrate de que no haya acceso externo.

### Validación humana

- **`mostrarPila()` en la sección `private` de `PilaHistorial.h`:** Se verificó que `mostrarPila()` se encuentra dentro de `private:` y no tiene acceso externo. Su firma es `void mostrarPila(std::stack<Accion> pila, const std::string& titulo) const;`.

- **Implementación movida a `PilaHistorial.cpp`:** La función global fue eliminada y se implementó como `PilaHistorial::mostrarPila(...) const`. La firma y el cuerpo mantienen la misma lógica.

- **`mostrarHistorial()` llama al método privado:** `mostrarHistorial()` utiliza `mostrarPila(pilaDeshacer, ...)` y `mostrarPila(pilaRehacer, ...)` en lugar de una función global.

- **Lógica interna sin cambios:** Se verificó que `mostrarPila()` recorre una copia temporal de la pila, por lo que no modifica las pilas originales. También mantiene la misma representación de las acciones mediante `tipoAccionToString()` y `tipoEntidadToString()`.

- **Compilación exitosa:** Se compiló el proyecto sin errores de vinculación. `mostrarHistorial()` continúa funcionando correctamente y las pilas originales no son modificadas.

**Conclusión:** APROBADO


## Prompt #5: Test de Pruebas Unitarias (Primera Versión)

**Estructura/función específica:**  
Crear `test_pruebasUnitarias.cpp` con pruebas interactivas de `Accion` y `PilaHistorial` sin utilizar frameworks externos.

**Prompt utilizado:**

> Crea `test_pruebasUnitarias.cpp` para validar `Accion` y `PilaHistorial` en C++ puro sin librerías externas. Debe incluir pruebas para estado inicial, LIFO, vaciado de rehacer, cimas y manejo de pilas vacías.

### Validación humana

- **13 pruebas unitarias implementadas:** Se implementaron pruebas para estado inicial, registrar acciones, comportamiento LIFO, deshacer, rehacer, cima de deshacer, cima de rehacer, nueva acción elimina rehacer, limpiar, deshacer con pila vacía, rehacer con pila vacía, conservación de estados y verificación de que `mostrarHistorial()` no modifica las pilas. Resultado: **13/13 PASSED**.

- **Menú interactivo funciona:** Se ejecutó el programa y se verificó el funcionamiento del menú. Al seleccionar la opción para probar `Accion`, el sistema solicitó los datos correspondientes, mostró correctamente la información y regresó al menú sin producir errores.

- **Compilación exitosa:** Se ejecutó `g++ -std=c++17 -o test_pruebasUnitarias.exe Accion.cpp PilaHistorial.cpp test_pruebasUnitarias.cpp`. El ejecutable se generó correctamente y sin warnings.

- **Sin frameworks externos:** Se utilizaron únicamente `iostream`, `string`, `Accion` y `PilaHistorial`. No se utilizaron Google Test, Catch2 ni otros frameworks. Las pruebas fueron desarrolladas utilizando C++ estándar.

**Conclusión:** APROBADO


## Prompt #6: Menú Interactivo Mejorado (Segunda Versión)

**Estructura/función específica:**  
Agregar una demostración interactiva completa con menú para registrar, deshacer, rehacer y visualizar el historial.

**Prompt utilizado:**

> Actualiza la suite de pruebas para incluir un menú interactivo en consola que permita al usuario registrar acciones personalizadas, ejecutar deshacer/rehacer y visualizar el estado del historial en tiempo real.

### Validación humana

- **Demostración interactiva funciona:** Se seleccionó la opción 11 correspondiente a la demostración. El sistema mostró el menú, permitió seleccionar la opción para registrar una acción, solicitar los datos correspondientes y confirmar el registro. El menú continuó disponible después de la operación.

- **Registrar múltiples acciones:** Se registraron las acciones A, B y C. Al mostrar el estado del historial se verificó Deshacer=3 y Rehacer=0.

- **Deshacer y rehacer individuales:** Con tres acciones registradas, se realizó un deshacer, obteniendo C y dejando Deshacer=2 y Rehacer=1. Luego se realizó otro deshacer, obteniendo B y dejando Deshacer=1 y Rehacer=2. Finalmente, se realizó un rehacer, obteniendo B y dejando Deshacer=2 y Rehacer=1. Cada operación se ejecutó individualmente bajo control del usuario.

- **Demostración de la regla crítica:** Se registraron A y B, se deshizo B y posteriormente se registró C. El sistema indicó que la nueva acción eliminó la pila de rehacer. El resultado fue Deshacer=2 y Rehacer=0.

- **Mostrar historial sin modificar:** Se mostró el historial y se verificó que los tamaños de las pilas permanecieron idénticos antes y después de la consulta.

**Conclusión:** APROBADO


## Prompt #7: Menú Más Interactivo (Tercera Versión)

**Estructura/función específica:**  
Mejorar la demostración interactiva para múltiples deshacer/rehacer manuales, manteniendo las pruebas automáticas.

**Prompt utilizado:**

> Mejora la interfaz del menú interactivo para permitir ejecuciones consecutivas individuales de deshacer y rehacer según la decisión del usuario, mostrando detalles explícitos del movimiento de acciones entre pilas.

### Validación humana

- **Pruebas automáticas conservadas y mejoradas:** `ejecutarTodasLasPruebas()` muestra 13 pruebas con resultado `[OK]`. Se verificaron estado inicial, registrar, LIFO, deshacer, rehacer, cimas, nueva acción elimina rehacer, limpiar, pilas vacías, conservación de estados y `mostrarHistorial()`. Resultado: **13/13 PASSED**.

- **Deshacer múltiples veces sin automatización:** Se registraron A, B y C. Al seleccionar consecutivamente la opción de deshacer, se obtuvo C, luego B y finalmente A. El usuario decide cuándo ejecutar cada operación.

- **Rehacer múltiples veces sin automatización:** Después de realizar los deshacer correspondientes, se seleccionó consecutivamente la opción de rehacer y se obtuvieron A, B y C. El usuario controla cada operación.

- **Mensajes demostrativos detallados:** Después de cada deshacer se muestra la información de la acción, incluyendo tipo, entidad, ID y estados, además del tamaño actual de las pilas Deshacer y Rehacer. Esto permite observar el movimiento de las acciones entre ambas pilas.

- **Manejo de casos límite:** Al intentar deshacer con la pila vacía se muestra un mensaje indicando que no hay acciones disponibles. Lo mismo ocurre al intentar rehacer con la pila vacía. El programa no presenta errores ni se bloquea.

**Conclusión:** APROBADO


## Prompt #8: Agregar `idUsuario` a `Accion`

**Estructura/función específica:**  
Agregar el atributo `idUsuario` a `Accion` para registrar quién ejecutó cada operación (Aclaratoria #3).

**Prompt utilizado:**

> Modifica la clase `Accion` para incorporar el atributo `int idUsuario`. Actualiza constructores, getters, setters y la representación en string para incluir quién ejecutó la acción.


### Validación humana

- **`idUsuario` agregado a `Accion.h`:** Se verificó que `int idUsuario` fue agregado como atributo privado. También se agregó el getter `int getIdUsuario() const` y el setter `void setIdUsuario(int)`. Su ubicación y tipo son consistentes con `idEntidad`.

- **Constructor actualizado:** El constructor parametrizado recibe `idUsuario` y lo inicializa mediante la lista de inicialización. El constructor por defecto establece su valor en `0`. La compilación fue exitosa.

- **Getter y setter implementados:** Se verificó que `getIdUsuario() const` retorna correctamente el identificador y que `setIdUsuario(int id)` permite actualizarlo correctamente.

- **`toString()` incluye `idUsuario`:** `toString()` genera una representación textual que incluye tipo de acción, entidad, `idEntidad`, `idUsuario` y estados, proporcionando información útil para auditoría.

- **Pruebas unitarias funcionan:** `test_pruebasUnitarias.cpp` fue actualizado para incluir `idUsuario` en las llamadas al constructor y verificar su getter. Las **13 pruebas continúan pasando**.

- **Lógica de deshacer/rehacer sin cambios:** `registrarAccion()`, `deshacer()`, `rehacer()` y `mostrarHistorial()` mantienen su funcionamiento. El nuevo atributo es pasivo y no modifica la lógica de las pilas.

**Conclusión:** APROBADO


## Prompt #9: Cambio de `cout` a `toString()` - Desacoplamiento

**Estructura/función específica:**  
Refactorizar `mostrarHistorial()` para devolver un `std::string` en lugar de utilizar `cout`, mejorando el desacoplamiento.

**Prompt utilizado:**

> Refactoriza `PilaHistorial` para que `mostrarHistorial()` devuelva un `std::string` con la información formateada en lugar de usar `std::cout`. La responsabilidad de imprimir en pantalla debe recaer totalmente en la interfaz de usuario.

### Validación humana

- **`Accion::toString()` implementado:** Se implementó el método `std::string toString() const`, que genera una representación textual con el tipo de acción, entidad, `idEntidad`, `idUsuario` y estados. El método no imprime en consola, sino que devuelve un string.

- **`mostrarPila()` retorna `std::string`:** La firma cambió de `void mostrarPila(...)` a `std::string mostrarPila(...) const`. El método construye y devuelve la información de la pila. Recorre una copia temporal, por lo que no modifica la pila original.

- **`mostrarHistorial()` retorna `std::string`:** La firma cambió de `void mostrarHistorial() const` a `std::string mostrarHistorial() const`. El método utiliza `mostrarPila()` para ambas pilas, concatena los resultados y devuelve el string.

- **`test_pruebasUnitarias.cpp` actualizado:** La demostración utiliza `std::string resultado = historial.mostrarHistorial();` y posteriormente `std::cout << resultado;`. Las pruebas fueron actualizadas para utilizar la nueva versión del método. La compilación fue exitosa.

- **Desacoplamiento logrado:** Anteriormente `PilaHistorial` era responsable de imprimir mediante `cout`. Ahora el módulo genera la información y la interfaz decide cómo presentarla. Esto permite utilizar la información posteriormente en archivos, redes, interfaces gráficas u otros medios sin modificar `PilaHistorial`.

**Conclusión:** APROBADO


## Prompt #10: Mejorar Documentación del Código

**Estructura/función específica:**  
Mejorar los comentarios en `Accion.h`, `Accion.cpp`, `PilaHistorial.h` y `PilaHistorial.cpp` para explicar responsabilidades y flujo.

**Prompt utilizado:**

> Añade comentarios académicos finales a todas las cabeceras y archivos de implementación. Explica la separación de responsabilidades entre `GestorSistema`, `PilaHistorial` y `Accion`, así como las garantías del diseño LIFO.


### Validación humana

- **Documentación de la clase `Accion` mejorada:** Se agregó una explicación sobre el propósito de `Accion` como representación de una operación ya realizada. Se especifica que la clase no ejecuta, deshace ni rehace operaciones y que utiliza `idEntidad` y estados como strings para mantener el desacoplamiento. La interpretación de estos estados corresponde al módulo Gestor.

- **Documentación de atributos clara:** Cada atributo cuenta con una explicación de su propósito:
  - `tipoAccion`: identifica la operación realizada (`AGREGAR`, `ACTUALIZAR`, `ELIMINAR`).
  - `tipoEntidad`: identifica la entidad modificada (`USUARIO`, `TAREA`).
  - `idEntidad`: identifica el ID de la entidad modificada.
  - `idUsuario`: identifica al usuario que ejecutó la operación y permite realizar auditoría.
  - `estadoAnterior` y `estadoPosterior`: representan el estado de la entidad antes y después de la operación.

- **Documentación de responsabilidades separadas:** Se documentó el flujo de responsabilidades: el `GestorSistema` captura los estados, crea una `Accion`, `PilaHistorial` la almacena y, durante un deshacer, devuelve la acción para que el `GestorSistema` interprete `estadoAnterior`. Se deja claro que `PilaHistorial` no reconstruye entidades y que `Accion` no modifica directamente usuarios ni tareas.

- **Documentación de `PilaHistorial` mejorada:** Se documentaron sus responsabilidades, el comportamiento LIFO, el uso de las dos pilas (`deshacer` y `rehacer`) y la regla según la cual registrar una nueva acción vacía la pila de rehacer. También se explicó que `PilaHistorial` administra el historial, pero no aplica directamente los cambios sobre las entidades.

- **Compilación exitosa después de los cambios:** Se ejecutó `g++ -std=c++17 -c Accion.cpp && g++ -std=c++17 -c PilaHistorial.cpp`. La compilación fue exitosa y sin warnings.

- **Funcionalidad sin cambios:** Se verificó que los cambios correspondieron únicamente a documentación. No se modificaron constructores, métodos, atributos, lógica ni enums. Las **13 pruebas continúan pasando**.

**Conclusión:** APROBADO
