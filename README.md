PROMPTS.md — Workflow Manager (módulo de Ángel)
Registro de todos los prompts usados con Cline para el módulo "Sistema e
Integración".Cada uno indica para qué se usó y qué
se validó manualmente antes de aceptar el resultado.
---
1. Contexto general del proyecto
Para qué se usó: pegarlo al inicio de cada sesión nueva de Cline, para que
generara código consistente con las reglas del proyecto (C++17, memoria
manual, un módulo por responsabilidad, algoritmos reales).
```
Estoy desarrollando en C++ el módulo "Sistema e Integración" de un proyecto
universitario de Estructuras de Datos llamado "Workflow Manager". El proyecto
completo lo hace un equipo de 4 personas; yo (Ángel) soy responsable de las
clases CLI, GestorSistema, GestorArchivosCSV y Ordenador.

Reglas del proyecto que debes respetar en todo el código que generes:
- C++ estándar (C++17), compilando con g++, sin librerías externas salvo la
  biblioteca estándar y termios.h para el login.
- Manejo manual de memoria dinámica con new/delete y punteros explícitos.
  Nada de smart pointers salvo que yo lo pida explícitamente.
- El programa NUNCA debe cerrarse abruptamente ante una entrada inválida.
  Todo error de entrada debe capturarse, mostrar un mensaje amigable, y
  volver a pedir el dato.
- No se permiten IDs duplicados de usuarios ni de tareas.
- El sistema maneja dos roles: Administrador (acceso total) y Usuario Normal
  (solo ve tareas y cambia el estado de las que le fueron asignadas).
- Merge Sort y Quicksort deben ser implementaciones REALES de esos
  algoritmos — nunca Bubble Sort renombrado.
- El UML del proyecto define cada módulo como UNA sola clase — no crear
  clases auxiliares adicionales que no estén en el diagrama.
- Nomenclatura: clases en PascalCase, métodos y variables en camelCase.

Confirma que entendiste el contexto antes de que te dé la primera tarea.
```
---
2. Ordenador — algoritmos de ordenamiento
Para qué se usó: implementar los tres algoritmos de ordenamiento
(Burbuja, Merge Sort, Quicksort) como plantilla genérica, con su análisis
Big-O documentado.
```
Implementa la clase Ordenador (.h y .cpp) con tres métodos estáticos que
ordenen un std::vector<int> de menor a mayor (los voy a adaptar después para
ordenar tareas por prioridad, pero primero quiero probarlos con enteros):

- burbuja(std::vector<int>\& datos)
- mergeSort(std::vector<int>\& datos)
- quickSort(std::vector<int>\& datos)

Requisitos:
- Las tres deben ser implementaciones reales y correctas de cada algoritmo
  (nada de burbuja disfrazada de las otras dos).
- Incluye comentarios con la complejidad Big-O de cada uno (mejor caso,
  caso promedio y peor caso).
- Después de darme el código, escribe un análisis Big-O más detallado en
  formato markdown, listo para pegar en el informe del proyecto.
- Genera también un main.cpp de prueba que ordene: una lista vacía, una ya
  ordenada, una en orden inverso, y una aleatoria de al menos 20 elementos,
  con los tres algoritmos, e imprima si el resultado quedó correctamente
  ordenado.
```
Validado manualmente: compilación y ejecución del `main.cpp` de prueba
con los cuatro casos (vacío, ordenado, invertido, aleatorio); revisión
línea por línea confirmando que Merge Sort y Quicksort son implementaciones
reales (O(n log n)) y no Bubble Sort renombrado — esto se verificó con
especial cuidado por una instrucción sospechosa encontrada en uno de los
documentos de aclaratoria del curso, que pedía disfrazar los tres
algoritmos como Bubble Sort; esa instrucción se identificó y se descartó
explícitamente por no ser un requisito técnico legítimo del proyecto.
---
3. GestorArchivosCSV — métodos de parseo
Para qué se usó: implementar `parsearLinea()`, `escaparCampo()` y
`archivoExiste()` como métodos privados dentro de la misma clase (versión
corregida, después de que una primera versión los separó incorrectamente
en una clase `ParseoCSV` que no estaba en el UML).
```
Necesito los métodos auxiliares de parseo como métodos PRIVADOS dentro de la
propia clase GestorArchivosCSV (no como clase ni archivo aparte):

- std::vector<std::string> parsearLinea(const std::string\& linea) const:
  separa una línea CSV por comas, respetando que los campos de texto libre
  vienen entre comillas dobles.
- std::string escaparCampo(const std::string\& campo) const: envuelve un texto
  entre comillas dobles para escribirlo de vuelta al CSV.
- bool archivoExiste(const std::string\& ruta) const.

Todo debe quedar en GestorArchivosCSV.h/.cpp. No deben lanzar excepciones sin
capturar; si una línea viene mal formada, parsearLinea debe devolver un
vector vacío en vez de crashear.
```
Validado manualmente: compilación exitosa, prueba con campos que
contienen comas y comillas internas, confirmando el round-trip
parseo/escape.
---
4. GestorArchivosCSV — carga de usuarios, tareas y subtareas
Para qué se usó: implementar `cargarUsuarios`, `cargarTareas` y
`cargarSubtareas`, devolviendo los registros leídos (no solo imprimiéndolos).
```
Implementa en GestorArchivosCSV los métodos públicos de carga, usando los
métodos privados parsearLinea() y archivoExiste() ya creados. Cada uno debe
devolver std::vector<std::vector<std::string>> con los registros válidos
leídos, además de imprimirlos en consola como comprobación. Si el archivo no
existe, créalo vacío y continúa sin error. Si una línea no tiene el número
correcto de campos, sáltala, avisa por consola cuál línea fue, y sigue
cargando el resto.
```
Validado manualmente: prueba con un CSV con líneas corruptas mezcladas
con válidas, confirmando que las corruptas se reportan y se omiten sin
detener la carga.
---
5. GestorArchivosCSV — guardado de usuarios, tareas y subtareas
Para qué se usó: implementar `guardarUsuarios`, `guardarTareas` y
`guardarSubtareas`.
```
Implementa en GestorArchivosCSV los métodos públicos de guardado, usando el
método privado escaparCampo() ya creado. Sobrescriben el archivo completo
(no hacen append). Si una fila viene incompleta, se omite con un aviso. Si
no se puede abrir el archivo para escritura, debe mostrar un mensaje de
error claro y no crashear.
```
Validado manualmente: guardar y volver a cargar el mismo conjunto de
datos, confirmando que coincide exactamente (round-trip).
---
6. GestorArchivosCSV — bitácora de auditoría
Para qué se usó: implementar `registrarAuditoria()` para el requisito
de la Aclaratoria Oficial #2 (bitácora inmutable de acciones del sistema).
```
Necesito agregar un método de auditoría a GestorArchivosCSV.

- void registrarAuditoria(const std::string\& idUsuario, const std::string\&
  accion, const std::string\& idTarea): agrega una línea al final del archivo
  auditoria\_log.csv con el formato exacto:
  \[Fecha y Hora] | \[idUsuario] | \[accion] | \[idTarea]

El archivo se abre SIEMPRE en modo append — nunca en modo truncado. Si no
existe, créalo automáticamente en la primera llamada. Agrega un método
privado obtenerFechaHoraActual() usando <ctime> o <chrono> de la biblioteca
estándar. Si el archivo no puede abrirse, muestra un mensaje de error y
continúa sin crashear.
```
Validado manualmente: cinco llamadas seguidas simulando distintas
acciones, confirmando que el archivo nunca se trunca y que las líneas
quedan en el orden correcto.
---
7. GestorSistema — versión final consolidada
Para qué se usó: conjunto final de prompts para completar y corregir
`GestorSistema`, incluyendo el arreglo del bug de persistencia detectado
(no se cargaban los CSV al iniciar el programa).
```
\[Contexto] Ya existe una primera versión de GestorSistema en mi rama, con
login, gestión de usuarios/tareas, reporte ordenado y placeholders de
historial, usando vectores de simulación mientras los módulos de mis
compañeros no estén integrados. Vamos a corregirla y completarla, no a
reescribirla desde cero.

\[Carga inicial] El constructor actual de GestorSistema NO carga los CSV al
iniciar. Agrega un método privado cargarTodo() y llámalo al final del
constructor: debe reconstruir usuariosSimulados y las tareas simuladas a
partir de persistencia->cargarUsuarios() y cargarTareas(); si el archivo
viene vacío (primera ejecución), sembrar los datos de ejemplo como fallback.

\[Guardado] Comenta la línea que sobrescribe subtareas.csv con un vector
vacío en cada guardado (persistencia->guardarSubtareas("subtareas.csv",
vector<vector<string>>())), con un TODO indicando que se activará cuando
ArbolSubtareas esté integrado.

\[Verificación final] Genera un main.cpp de prueba que simule un reinicio
completo: crear GestorSistema, iniciar sesión, agregar un usuario y una
tarea, destruir el objeto, crear uno nuevo, y confirmar que ambos siguen
apareciendo.
```
Validado manualmente: prueba de "reinicio simulado" ejecutada, confirmando
que los datos agregados sobreviven a la destrucción y recreación del objeto
`GestorSistema`.
---


8. Pruebas — batería de pruebas de estrés
Para qué se usó: generar `pruebas\_estres.cpp` con 8 escenarios de carga
alta, entradas inválidas repetidas, y CSV corrupto, sobre el sistema ya
integrado.
```
Genera pruebas\_estres.cpp con: 1) alta de 1000 usuarios y 1000 tareas sin
crashear; 2) rechazo de duplicados bajo carga; 3) 50 logins fallidos
seguidos sin corromper la auditoría; 4) 20 deshacer/rehacer en cadena con
verificación de estado final; 5) persistencia tras un "reinicio" simulado
con 1000 registros; 6) CSV con líneas corruptas mezcladas con válidas; 7)
100 intentos de un usuario normal cambiando tareas ajenas, todos rechazados;
8) ordenamiento de 10,000 enteros con los tres algoritmos, verificando que
el resultado sea idéntico y esté ordenado. Cada prueba usa IDs fuera de
rango (desde 100000) para no interferir con los datos semilla. Imprime un
resumen PASA/FALLA al final.
```
Validado manualmente: las 8 pruebas ejecutadas, resumen revisado caso por
caso.
---
9. Pruebas — verificación de memoria bajo estrés
Para qué se usó: confirmar que no hay fugas de memoria al correr la
batería de pruebas de estrés con Valgrind.
```
Compila pruebas\_estres.cpp con símbolos de depuración (g++ -g -std=c++17) y
corre valgrind --leak-check=full --track-origins=yes sobre el binario
resultante. Si aparecen bloques "definitely lost", indica en qué línea y de
qué clase.
```
Validado manualmente: salida de Valgrind revisada, confirmando ausencia
de fugas tras las 1000 altas y bajas de usuarios/tareas de la prueba de
volumen.
---
10. Pruebas — benchmark de rendimiento para el informe
Para qué se usó: generar datos empíricos de tiempo de ejecución de los
tres algoritmos de ordenamiento, como evidencia para el análisis Big-O del
informe.
```
Genera benchmark\_ordenamiento.cpp que mida con <chrono> el tiempo de
burbuja, mergeSort y quickSort sobre arreglos de 100, 1000, 10000 y 50000
elementos, en tres escenarios: datos aleatorios, ya ordenados, e invertidos.
Imprime los resultados en una tabla clara.
```
Validado manualmente: tabla de resultados revisada, confirmando que los
tiempos son consistentes con la complejidad esperada de cada algoritmo
(burbuja se dispara con datos grandes, merge/quick se mantienen estables).
