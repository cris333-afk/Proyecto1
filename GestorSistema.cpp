#include <iostream>   // cout/endl para mensajes al usuario (nunca crashea).
#include <string>
#include <vector>
#include <functional> // std::function para el visitante recursivo del aplanado.

#include "GestorSistema.h"
#include "Usuario.h"     // Real: Usuario y el enum Rol.
#include "Ordenador.h"   // Real: Bubble Sort, Merge Sort y Quick Sort del repo.
#include "ArbolSubtareas.h"   // Real: bosque de tareas con raíz virtual (Cris).
#include "ColaTareas.h"       // Real: cola FIFO de tareas (Cris).
#include "ColaPrioridad.h"    // Real: cola por prioridad de tareas (Cris).

using namespace std;

//
// GestorSistema: clase fachada del modulo "Sistema e Integracion".
// Contiene login/sesion, CRUD de usuarios y tareas, cambio de estado con
// validacion de rol, reporte ordenado (Ordenador real), historial
// (deshacer/rehacer) y AHORA carga real de datos al iniciar. Los modulos
// de los companeros que aun no estan en esta rama se simulan con vectores
// de respaldo; al integrarlos se reemplazan por las estructuras reales
// (listaUsuarios, colaFIFO, ...).
//

// Conversion entre el texto guardado en el CSV y el enum Rol: NO se define
// aqui. rolATexto() y textoARol() provienen del modulo Usuarios real
// (Usuario.h / Usuario.cpp), la misma interfaz que usara ListaUsuarios al
// integrarse, de modo que guardarTodo() y cargarTodo() escriben y leen el
// CSV con la codificacion canonica ("Administrador" / "Usuario Normal").

// ---------------------------------------------------------------------
// Constructor: instancia los modulos REALES ya integrados (las tres
// estructuras de tareas -ArbolSubtareas, ColaTareas y ColaPrioridad- y la
// persistencia) y deja en nullptr los punteros a los modulos que aun no
// entregan los companeros (ListaUsuarios y PilaHistorial): para esos,
// todavia solo existen sus declaraciones adelantadas (tipos incompletos) y
// C++ no permite hacer new de un tipo cuyo tamano no se conoce. Al final
// llama a cargarTodo() para reconstruir el estado desde los CSV (o sembrar
// datos de ejemplo si es la primera ejecucion).
// ---------------------------------------------------------------------
GestorSistema::GestorSistema()
    : listaUsuarios(nullptr),             // Pendiente: ListaUsuarios (Cesar).
      historial(nullptr),                 // Pendiente: PilaHistorial (modulo historial).
      colaFIFO(new ColaTareas()),         // Real: ColaTareas (Cris).
      colaPrioridad(new ColaPrioridad()), // Real: ColaPrioridad (Cris).
      arbolTareas(new ArbolSubtareas()),  // Real: ArbolSubtareas (Cris).
      persistencia(new GestorArchivosCSV()),
      usuarioActual(nullptr) {
    cargarTodo();
}

// ---------------------------------------------------------------------
// cargarTodo: reconstruye usuariosSimulados y las tareas simuladas a
// partir de usuarios.csv y tareas.csv. Si un archivo viene vacio (primera
// ejecucion del programa, o CSV borrado), siembra los datos de ejemplo
// como respaldo, igual que hacia antes el constructor.
//
// Los campos numericos (id, idUsuarioResponsable) se convierten con
// stoi() dentro de un try/catch: si un CSV fue editado a mano y quedo con
// un valor no numerico, esa fila se descarta con un aviso en vez de
// crashear el programa.
// ---------------------------------------------------------------------
void GestorSistema::cargarTodo() {
    // --- Usuarios ---
    vector<vector<string>> filasUsuarios = persistencia->cargarUsuarios("usuarios.csv");

    if (!filasUsuarios.empty()) {
        for (size_t i = 0; i < filasUsuarios.size(); ++i) {
            const vector<string>& fila = filasUsuarios[i];
            try {
                int id = stoi(fila[0]);
                string nombre = fila[1];
                Rol rol = textoARol(fila[2]);
                string contrasena = fila[3];
                usuariosSimulados.push_back(new Usuario(id, nombre, contrasena, rol));
            } catch (const exception&) {
                cout << "Aviso: fila de usuarios.csv con ID invalido; se omite." << endl;
            }
        }
    } else {
        // Primera ejecucion (o archivo vacio): sembrar usuarios de prueba.
        usuariosSimulados.push_back(new Usuario(1, "Administrador", "admin123", Rol::ADMINISTRADOR));
        usuariosSimulados.push_back(new Usuario(2, "Usuario Normal", "user123", Rol::USUARIO_NORMAL));
    }

    // --- Tareas ---
    vector<vector<string>> filasTareas = persistencia->cargarTareas("tareas.csv");

    if (!filasTareas.empty()) {
        for (size_t i = 0; i < filasTareas.size(); ++i) {
            const vector<string>& fila = filasTareas[i];
            try {
                int id = stoi(fila[0]);
                string prioridad = fila[1];
                int idResponsable = stoi(fila[2]);
                string descripcion = fila[3];
                string estado = fila[4];

                idsTareasSimuladas.push_back(id);
                prioridadesSimuladas.push_back(prioridad);
                responsablesSimulados.push_back(idResponsable);
                descripcionesSimuladas.push_back(descripcion);
                estadosSimuladas.push_back(estado);
            } catch (const exception&) {
                cout << "Aviso: fila de tareas.csv con ID invalido; se omite." << endl;
            }
        }
    } else {
        // Primera ejecucion (o archivo vacio): sembrar tareas de ejemplo.
        idsTareasSimuladas.push_back(10);
        prioridadesSimuladas.push_back("ALTA");
        responsablesSimulados.push_back(1);
        estadosSimuladas.push_back("Pendiente");
        descripcionesSimuladas.push_back("Tarea de ejemplo del administrador");

        idsTareasSimuladas.push_back(11);
        prioridadesSimuladas.push_back("MEDIA");
        responsablesSimulados.push_back(2);
        estadosSimuladas.push_back("Pendiente");
        descripcionesSimuladas.push_back("Tarea de ejemplo del usuario normal");
    }

    // --- Subtareas (reconstruye el bosque real de tareas) ---
    // Formato de subtareas.csv: (id, idTareaPadre, descripcion, estado).
    // Cada fila valida se convierte a una Tarea* y se inserta en el arbol
    // real con agregarSubtarea(idTareaPadre, nueva). Un idTareaPadre = 0
    // significa que es una tarea de nivel superior (hija de la raiz
    // virtual). El CSV no trae prioridad ni responsable, por lo que las
    // tareas se reconstruyen con valores neutros.
    vector<vector<string>> filasSubtareas = persistencia->cargarSubtareas("subtareas.csv");

    for (size_t i = 0; i < filasSubtareas.size(); ++i) {
        const vector<string>& fila = filasSubtareas[i];
        try {
            int id = stoi(fila[0]);
            int idPadre = stoi(fila[1]);
            string descripcion = fila[2];
            string estado = fila[3];

            Tarea* nueva = new Tarea(id, "SIN_PRIORIDAD", estado, descripcion, -1);
            if (!arbolTareas->agregarSubtarea(idPadre, nueva)) {
                // El padre no existe en el arbol: la tarea no se inserta y
                // el arbol no la tomo, asi que quien la creo con new debe
                // liberarla aqui para no fugarla.
                cout << "Aviso: fila de subtareas.csv con idTareaPadre "
                     << "inexistente; se omite." << endl;
                delete nueva;
            }
        } catch (const exception&) {
            cout << "Aviso: fila de subtareas.csv con ID invalido; se omite." << endl;
        }
    }
}

// ---------------------------------------------------------------------
// Destructor: libera con delete cada puntero del cual la clase es duena.
//
// ORDEN DE ELIMINACION (evita acceder a memoria ya liberada):
//   - Las colas (colaFIFO, colaPrioridad) solo administran sus NodoTarea y
//     guardan una referencia PRESTADA a las Tarea; al destruirse liberan
//     unicamente sus nodos y no tocan las Tarea.
//   - arbolTareas es el DUEÑO de las Tarea de nivel superior (su destructor
//     las libera recursivamente, junto con la raíz virtual). Por eso se
//     destruyen primero las "observadoras" (las colas) y al final el
//     "propietario" (el árbol): así ningún destructor queda con punteros a
//     memoria que ya fue liberada.
//   - persistencia es independiente de las anteriores.
//
// Los punteros a los modulos pendientes de integracion (historial y
// listaUsuarios) siguen valiendo nullptr y NO se liberan aqui: hacer delete
// de un tipo incompleto (solo declarado adelante) es un comportamiento
// indefinido, y se activaran cuando cada companero integre su modulo.
//
// usuarioActual NO se libera: apunta a un nodo dentro de usuariosSimulados
// (o, cuando se integre, dentro de listaUsuarios), y ese vector ya se
// encarga de liberar sus elementos abajo. Hacerle delete aqui provocaria
// una doble liberacion de memoria (undefined behavior).
// ---------------------------------------------------------------------
GestorSistema::~GestorSistema() {
    // Primero las colas (solo nodos; referencias prestadas a las Tarea)...
    delete colaFIFO;
    delete colaPrioridad;

    // ... y al final el árbol, propietario real de las Tarea.
    delete arbolTareas;

    delete persistencia;  // Independiente de las estructuras de tareas.

    // Pendientes de integracion (siguen en nullptr; liberar al integrar):
    // delete historial;
    // delete listaUsuarios;

    // Limpieza de la SIMULACION temporal (se elimina al integrar las listas
    // reales, que ya administran su propia memoria).
    usuarioActual = nullptr;  // Evita punteros colgantes al liberar el respaldo.
    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        delete usuariosSimulados[i];
    }
    usuariosSimulados.clear();
}

// ---------------------------------------------------------------------
// iniciarSesion: valida un id y contrasena para abrir una sesion.
//
// Cuando la clase ListaUsuarios exista en el repo, este metodo buscara al
// usuario ahi. Por ahora se busca en el registro simulado (usuariosSimulados,
// ya reconstruido desde el CSV por cargarTodo()), de manera que un usuario
// agregado en una sesion anterior tambien pueda iniciar sesion despues de
// reabrir el programa.
// Devuelve nullptr (nunca crashea) si el id no existe o la contrasena es
// incorrecta, y deja usuarioActual en nullptr.
// ---------------------------------------------------------------------
Usuario* GestorSistema::iniciarSesion(int id, const std::string& contrasena) {
    // Cierra cualquier sesion previa antes de validar la nueva.
    usuarioActual = nullptr;

    // --- SIMULACION TEMPORAL (reemplazar por listaUsuarios al integrar) ---
    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        if (usuariosSimulados[i]->getId() == id) {
            // Id encontrado: debe coincidir ademas la contrasena.
            if (usuariosSimulados[i]->validarContrasena(contrasena)) {
                usuarioActual = usuariosSimulados[i];
                return usuarioActual;
            }
            return nullptr;  // Id correcto pero contrasena incorrecta.
        }
    }

    return nullptr;   // No existe ningun usuario con ese id.
}

// ---------------------------------------------------------------------
// getUsuarioActual: devuelve el usuario con la sesion activa, o nullptr
// si todavia no ha iniciado sesion nadie.
// ---------------------------------------------------------------------
Usuario* GestorSistema::getUsuarioActual() {
    return usuarioActual;
}

// =====================================================================
//                       GESTION DE USUARIOS
// (Pendiente de conectar con ListaUsuarios; hoy usan la simulacion local,
//  que ahora SI persiste correctamente entre ejecuciones gracias a
//  cargarTodo() + guardarTodo())
// =====================================================================

bool GestorSistema::agregarUsuario(int id, const std::string& nombre, Rol rol, const std::string& contrasena) {
    cout << "  [Pendiente de conectar con ListaUsuarios] usando simulacion local." << endl;

    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        if (usuariosSimulados[i]->getId() == id) {
            cout << "Error: ya existe un usuario con el ID " << id << "." << endl;
            return false;                    // Sin duplicados.
        }
    }
    if (nombre.empty() || contrasena.empty()) {
        cout << "Error: el nombre y la contrasena no pueden estar vacios." << endl;
        return false;
    }

    usuariosSimulados.push_back(new Usuario(id, nombre, contrasena, rol));
    cout << "Usuario '" << nombre << "' (ID " << id << ") agregado correctamente." << endl;

    guardarTodo();
    string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAud, "agregarUsuario", to_string(id));
    return true;
}

bool GestorSistema::actualizarUsuario(int id, const std::string& nombre, Rol rol) {
    cout << "  [Pendiente de conectar con ListaUsuarios] usando simulacion local." << endl;

    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        if (usuariosSimulados[i]->getId() == id) {
            if (nombre.empty()) {
                cout << "Error: el nuevo nombre no puede estar vacio." << endl;
                return false;
            }
            usuariosSimulados[i]->setNombre(nombre);
            usuariosSimulados[i]->setRol(rol);
            cout << "Usuario ID " << id << " actualizado correctamente." << endl;

            guardarTodo();
            string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
            persistencia->registrarAuditoria(idAud, "actualizarUsuario", to_string(id));
            return true;
        }
    }

    cout << "No se encontro un usuario con el ID " << id << "." << endl;
    return false;
}

bool GestorSistema::eliminarUsuario(int id) {
    cout << "  [Pendiente de conectar con ListaUsuarios] usando simulacion local." << endl;

    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        if (usuariosSimulados[i]->getId() == id) {
            // Se registra quien realiza la accion ANTES de que la sesion pueda
            // quedar sin usuario (caso: alguien elimina su propia cuenta).
            string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";

            if (usuariosSimulados[i] == usuarioActual) {
                usuarioActual = nullptr;
                cout << "Aviso: la sesion activa se cerro (su usuario fue eliminado)." << endl;
            }
            delete usuariosSimulados[i];
            usuariosSimulados.erase(usuariosSimulados.begin() + i);

            cout << "Usuario ID " << id << " eliminado correctamente." << endl;

            guardarTodo();
            persistencia->registrarAuditoria(idAud, "eliminarUsuario", to_string(id));
            return true;
        }
    }

    cout << "No se encontro un usuario con el ID " << id << "." << endl;
    return false;
}

void GestorSistema::listarUsuarios() {
    cout << "  [Pendiente de conectar con ListaUsuarios] usando simulacion local." << endl;

    if (usuariosSimulados.empty()) {
        cout << "No hay usuarios registrados." << endl;
        return;
    }

    cout << "--- Lista de usuarios ---" << endl;
    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        cout << "  " << usuariosSimulados[i]->toString() << endl;
    }
}

Usuario* GestorSistema::buscarUsuario(int id) {
    cout << "  [Pendiente de conectar con ListaUsuarios] usando simulacion local." << endl;

    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        if (usuariosSimulados[i]->getId() == id) {
            return usuariosSimulados[i];
        }
    }
    return nullptr;
}

// =====================================================================
//                       GESTION DE TAREAS
// (Ya conectada con las estructuras reales: ArbolSubtareas (dueño de las
//  Tarea), ColaTareas y ColaPrioridad. Los vectores de simulacion se
//  conservan temporalmente en la clase, pero este bloque ya no los usa;
//  se eliminaran por completo cuando todos los metodos esten migrados.)
// =====================================================================

void GestorSistema::agregarTarea(int id, const std::string& prioridad, int idResponsable, const std::string& descripcion) {
    if (arbolTareas->buscar(id) != nullptr) {
        cout << "Error: ya existe una tarea con el ID " << id << "." << endl;
        return;                          // Sin duplicados.
    }
    if (descripcion.empty()) {
        cout << "Error: la descripcion no puede estar vacia." << endl;
        return;
    }

    // Crea la Tarea real (el árbol es su DUEÑO y la liberará), la agrega
    // como tarea de nivel superior y la encola en ambas colas, que guardan
    // una referencia PRESTADA a la misma Tarea (no la liberan).
    Tarea* tarea = new Tarea(id, prioridad, "Pendiente", descripcion, idResponsable);
    arbolTareas->agregarTarea(tarea);
    colaFIFO->encolar(tarea);
    colaPrioridad->encolar(tarea);

    cout << "Tarea ID " << id << " agregada correctamente." << endl;

    guardarTodo();
    string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAud, "agregarTarea", to_string(id));
}

void GestorSistema::actualizarTarea(int id, const std::string& descripcion) {
    cout << "  [Pendiente de conectar con ColaTareas] usando simulacion local." << endl;

    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        if (idsTareasSimuladas[i] == id) {
            if (descripcion.empty()) {
                cout << "Error: la nueva descripcion no puede estar vacia." << endl;
                return;
            }
            descripcionesSimuladas[i] = descripcion;
            cout << "Tarea ID " << id << " actualizada correctamente." << endl;

            guardarTodo();
            string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
            persistencia->registrarAuditoria(idAud, "actualizarTarea", to_string(id));
            return;
        }
    }

    cout << "No se encontro una tarea con el ID " << id << "." << endl;
}

void GestorSistema::eliminarTarea(int id) {
    if (!arbolTareas->eliminar(id)) {
        cout << "No se encontro una tarea con el ID " << id << "." << endl;
        return;
    }

    cout << "Tarea ID " << id << " eliminada correctamente." << endl;

    guardarTodo();
    string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAud, "eliminarTarea", to_string(id));
}

void GestorSistema::listarTareasPendientes() {
    bool hayPendientes = false;

    // Recorre el bosque real con el nuevo recorrido de filtrado de
    // ArbolSubtareas y muestra solo las tareas cuyo estado no sea
    // "Completada", con el mismo formato que usaba la version simulada.
    arbolTareas->recorrerConFiltro(
        [](Tarea* tarea) { return tarea->getEstado() != "Completada"; },
        [&](Tarea* tarea) {
            cout << "  ID " << tarea->getId()
                 << " | Prioridad: " << tarea->getPrioridad()
                 << " | Responsable: " << tarea->getIdUsuarioResponsable()
                 << " | Estado: " << tarea->getEstado()
                 << " | " << tarea->getDescripcion() << endl;
            hayPendientes = true;
        });

    if (!hayPendientes) {
        cout << "No hay tareas pendientes." << endl;
    }
}

void GestorSistema::buscarTarea(int id) {
    Tarea* tarea = arbolTareas->buscar(id);
    if (tarea == nullptr) {
        cout << "No se encontro una tarea con el ID " << id << "." << endl;
        return;
    }

    cout << "Tarea ID " << tarea->getId()
         << " | Prioridad: " << tarea->getPrioridad()
         << " | Responsable: " << tarea->getIdUsuarioResponsable()
         << " | Estado: " << tarea->getEstado()
         << " | " << tarea->getDescripcion() << endl;
}

// ---------------------------------------------------------------------
// cambiarEstadoTarea: cambia el estado de una tarea.
// Regla de rol: si usuarioActual es USUARIO_NORMAL, el cambio solo se
// permite cuando idUsuario coincide con el responsable asignado a la
// tarea. Un ADMINISTRADOR puede cambiar cualquier estado.
// ---------------------------------------------------------------------
bool GestorSistema::cambiarEstadoTarea(int idTarea, int idUsuario, const std::string& nuevoEstado) {
    if (usuarioActual == nullptr) {
        cout << "Error: no hay una sesion activa para cambiar estados." << endl;
        return false;
    }

    Tarea* tarea = arbolTareas->buscar(idTarea);
    if (tarea == nullptr) {
        cout << "No se encontro una tarea con el ID " << idTarea << "." << endl;
        return false;
    }

    // Validacion de rol para usuarios normales.
    if (usuarioActual->getRol() == Rol::USUARIO_NORMAL) {
        if (tarea->getIdUsuarioResponsable() != idUsuario) {
            cout << "Error: un usuario normal solo puede cambiar el estado de las "
                    "tareas que le fueron asignadas." << endl;
            return false;
        }
        cout << "Permiso validado: la tarea esta asignada al usuario " << idUsuario << "." << endl;
    }

    tarea->setEstado(nuevoEstado);
    cout << "Estado de la tarea " << idTarea << " cambiado a '" << nuevoEstado << "'." << endl;

    guardarTodo();
    string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAud, "cambioEstado", to_string(idTarea));
    return true;
}

// =====================================================================
//                          PERSISTENCIA
// =====================================================================

// ---------------------------------------------------------------------
// aplanarArbolParaGuardar: recorre el bosque real de tareas excluyendo la
// raíz virtual y arma una fila por cada tarea con el formato que espera
// subtareas.csv: (id, idTareaPadre, descripcion, estado).
//
// El recorrido es pre-orden, así que cada tarea queda escrita ANTES que sus
// subtareas, lo cual permite reconstruir el árbol al cargar sin depender
// del orden en el archivo. Las tareas de nivel superior llevan idTareaPadre
// = 0 (el id de la raíz virtual), que es justo lo que espera
// arbolTareas->agregarSubtarea() para volver a insertarlas como hijas
// directas de la raíz virtual.
// ---------------------------------------------------------------------
vector<vector<string>> GestorSistema::aplanarArbolParaGuardar() {
    vector<vector<string>> filas;

    Tarea* raizVirtual = arbolTareas->getRaiz();
    if (raizVirtual == nullptr) {
        return filas;
    }

    // Visitante recursivo: empuja la fila del nodo actual (con su padre) y
    // luego recorre sus subtareas. Se usa std::function para poder
    // llamarse a sí mismo.
    function<void(Tarea*, Tarea*)> visitar = [&](Tarea* padre, Tarea* nodo) {
        if (nodo == nullptr) {
            return;
        }

        vector<string> fila;
        fila.push_back(to_string(nodo->getId()));
        fila.push_back(to_string(padre->getId()));
        fila.push_back(nodo->getDescripcion());
        fila.push_back(nodo->getEstado());
        filas.push_back(fila);

        vector<Tarea*> subtareas = nodo->getSubtareas();
        for (Tarea* subtarea : subtareas) {
            visitar(nodo, subtarea);
        }
    };

    // Arranca desde cada hijo de la raíz virtual: la propia raíz virtual
    // (id 0) NO se escribe como fila.
    vector<Tarea*> nivelSuperior = raizVirtual->getSubtareas();
    for (Tarea* tarea : nivelSuperior) {
        visitar(raizVirtual, tarea);
    }

    return filas;
}

// ---------------------------------------------------------------------
// guardarTodo: guarda el estado actual en los CSV (lo llama CLI como
// gestor.guardarTodo()).
//
// Usuarios y tareas se escriben desde los vectores de simulacion todavia
// usados por los modulos pendientes; las subtareas se aplanan desde el
// arbol real (ArbolSubtareas), que ya es quien posee las tareas.
// ---------------------------------------------------------------------
void GestorSistema::guardarTodo() {
    cout << "Guardando cambios en los archivos CSV..." << endl;

    // Usuarios: columnas (id, nombre, rol, password) segun guardarUsuarios.
    vector<vector<string>> filasUsuarios;
    for (size_t i = 0; i < usuariosSimulados.size(); ++i) {
        vector<string> fila;
        fila.push_back(to_string(usuariosSimulados[i]->getId()));
        fila.push_back(usuariosSimulados[i]->getNombre());
        fila.push_back(rolATexto(usuariosSimulados[i]->getRol()));
        fila.push_back(usuariosSimulados[i]->getContrasena());
        filasUsuarios.push_back(fila);
    }
    persistencia->guardarUsuarios("usuarios.csv", filasUsuarios);

    // Tareas: columnas (id, prioridad, idUsuarioResponsable, descripcion,
    // estado) segun guardarTareas.
    vector<vector<string>> filasTareas;
    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        vector<string> fila;
        fila.push_back(to_string(idsTareasSimuladas[i]));
        fila.push_back(prioridadesSimuladas[i]);
        fila.push_back(to_string(responsablesSimulados[i]));
        fila.push_back(descripcionesSimuladas[i]);
        fila.push_back(estadosSimuladas[i]);
        filasTareas.push_back(fila);
    }
    persistencia->guardarTareas("tareas.csv", filasTareas);

    // Subtareas: columnas (id, idTareaPadre, descripcion, estado) segun
    // guardarSubtareas, generadas aplanando el arbol real de tareas.
    vector<vector<string>> filasSubtareas = aplanarArbolParaGuardar();
    persistencia->guardarSubtareas("subtareas.csv", filasSubtareas);

    cout << "Cambios guardados correctamente." << endl;
}

// =====================================================================
//              REPORTE ORDENADO (Ordenador real del proyecto)
// =====================================================================

// Comparador para int: orden ascendente (lo usan los tres algoritmos).
static bool menorInt(const int& a, const int& b) {
    return a < b;
}

// ---------------------------------------------------------------------
// mostrarReporteOrdenado: muestra un reporte de prueba ordenado con el
// Ordenador REAL del proyecto (Bubble Sort, Merge Sort y Quick Sort,
// todos reales del repo, ninguno renombrado). Como aun no hay una lista
// real de tareas conectada, se ordena un vector<int> de ejemplo.
// ---------------------------------------------------------------------
void GestorSistema::mostrarReporteOrdenado() {
    cout << "  [Reporte usando el Ordenador real del proyecto]" << endl;
    cout << "  (Datos de prueba: no hay aun una lista real de tareas conectada.)" << endl;

    vector<int> datos;
    datos.push_back(42);
    datos.push_back(17);
    datos.push_back(8);
    datos.push_back(35);
    datos.push_back(23);
    int n = static_cast<int>(datos.size());

    // El Ordenador ordena arreglos in-place; se preparan tres copias
    // independientes (una por algoritmo) para mostrar cada resultado.
    int* arrBurbuja = new int[n];
    int* arrMerge   = new int[n];
    int* arrQuick   = new int[n];
    for (int i = 0; i < n; ++i) {
        arrBurbuja[i] = datos[i];
        arrMerge[i]   = datos[i];
        arrQuick[i]   = datos[i];
    }

    Ordenador<int> ordenador;
    ordenador.burbuja(arrBurbuja, n, menorInt);
    ordenador.mergeSort(arrMerge, n, menorInt);
    ordenador.quickSort(arrQuick, n, menorInt);

    cout << "\n--- Reporte ordenado (Bubble Sort real) ---" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  " << arrBurbuja[i];
    }
    cout << endl;

    cout << "\n--- Reporte ordenado (Merge Sort real) ---" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  " << arrMerge[i];
    }
    cout << endl;

    cout << "\n--- Reporte ordenado (Quick Sort real) ---" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  " << arrQuick[i];
    }
    cout << endl << endl;

    // Liberar la memoria manual (regla del proyecto).
    delete[] arrBurbuja;
    delete[] arrMerge;
    delete[] arrQuick;
}

// =====================================================================
//              HISTORIAL (deshacer / rehacer)
// =====================================================================

void GestorSistema::deshacer() {
    cout << "  [Pendiente de conectar con PilaHistorial]" << endl;

    string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAud, "DESHACER", "0");

    cout << "No hay acciones registradas que deshacer (simulacion)." << endl;
}

void GestorSistema::rehacer() {
    cout << "  [Pendiente de conectar con PilaHistorial]" << endl;

    string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAud, "REHACER", "0");

    cout << "No hay acciones deshechas que rehacer (simulacion)." << endl;
}