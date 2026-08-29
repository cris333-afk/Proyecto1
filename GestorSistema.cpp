#include <iostream>   // cout/endl para mensajes al usuario (nunca crashea).
#include <string>
#include <vector>

#include "GestorSistema.h"
#include "Usuario.h"     // Real: Usuario y el enum Rol.
#include "Ordenador.h"   // Real: Bubble Sort, Merge Sort y Quick Sort del repo.

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
// Constructor: instancia el unico modulo ya integrado (persistencia) y
// deja en nullptr los punteros a las cinco estructuras de los companeros:
// aqui solo existen sus declaraciones adelantadas (tipos incompletos) y
// C++ no permite hacer new de un tipo cuyo tamano no se conoce, por lo que
// cada uno se instanciara con new al momento de integrar su modulo. Al
// final llama a cargarTodo() para reconstruir el estado desde los CSV
// (o sembrar datos de ejemplo si es la primera ejecucion).
// ---------------------------------------------------------------------
GestorSistema::GestorSistema()
    : listaUsuarios(nullptr),   // Pendiente: ListaUsuarios (Cesar).
      historial(nullptr),       // Pendiente: PilaHistorial (modulo historial).
      colaFIFO(nullptr),        // Pendiente: ColaTareas (Cris).
      colaPrioridad(nullptr),   // Pendiente: ColaPrioridad (Cris).
      arbolTareas(nullptr),     // Pendiente: ArbolSubtareas (Cris).
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

    // TODO: cargar subtareas.csv aqui cuando ArbolSubtareas este integrado.
}

// ---------------------------------------------------------------------
// Destructor: libera con delete cada puntero del cual la clase es duena.
// persistencia es el unico recurso real por ahora. Los punteros a los
// modulos pendientes de integracion valen nullptr y NO se liberan aqui:
// hacer delete de un tipo incompleto (solo declarado adelante) es un
// comportamiento indefinido. AL INTEGRAR cada modulo se debe: incluir su
// .h, hacer new en el constructor y agregar su delete en este destructor.
//
// usuarioActual NO se libera: apunta a un nodo dentro de usuariosSimulados
// (o, cuando se integre, dentro de listaUsuarios), y ese vector ya se
// encarga de liberar sus elementos abajo. Hacerle delete aqui provocaria
// una doble liberacion de memoria (undefined behavior).
// ---------------------------------------------------------------------
GestorSistema::~GestorSistema() {
    delete persistencia;      // Unico modulo integrado por ahora.

    // Pendientes de integracion (nullptr; liberar al integrar cada modulo):
    // delete arbolTareas;
    // delete colaPrioridad;
    // delete colaFIFO;
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
// (Pendiente de conectar con ColaTareas/ColaPrioridad; hoy usan la
//  simulacion local, que ahora SI persiste correctamente)
// =====================================================================

void GestorSistema::agregarTarea(int id, const std::string& prioridad, int idResponsable, const std::string& descripcion) {
    cout << "  [Pendiente de conectar con ColaTareas] usando simulacion local." << endl;

    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        if (idsTareasSimuladas[i] == id) {
            cout << "Error: ya existe una tarea con el ID " << id << "." << endl;
            return;                          // Sin duplicados.
        }
    }
    if (descripcion.empty()) {
        cout << "Error: la descripcion no puede estar vacia." << endl;
        return;
    }

    idsTareasSimuladas.push_back(id);
    prioridadesSimuladas.push_back(prioridad);
    responsablesSimulados.push_back(idResponsable);
    estadosSimuladas.push_back("Pendiente");
    descripcionesSimuladas.push_back(descripcion);

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
    cout << "  [Pendiente de conectar con ColaTareas] usando simulacion local." << endl;

    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        if (idsTareasSimuladas[i] == id) {
            idsTareasSimuladas.erase(idsTareasSimuladas.begin() + i);
            prioridadesSimuladas.erase(prioridadesSimuladas.begin() + i);
            responsablesSimulados.erase(responsablesSimulados.begin() + i);
            estadosSimuladas.erase(estadosSimuladas.begin() + i);
            descripcionesSimuladas.erase(descripcionesSimuladas.begin() + i);

            cout << "Tarea ID " << id << " eliminada correctamente." << endl;

            guardarTodo();
            string idAud = (usuarioActual != nullptr) ? to_string(usuarioActual->getId()) : "-1";
            persistencia->registrarAuditoria(idAud, "eliminarTarea", to_string(id));
            return;
        }
    }

    cout << "No se encontro una tarea con el ID " << id << "." << endl;
}

void GestorSistema::listarTareasPendientes() {
    cout << "  [Pendiente de conectar con ColaTareas] usando simulacion local." << endl;

    bool hayPendientes = false;
    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        if (estadosSimuladas[i] != "Completada") {
            cout << "  ID " << idsTareasSimuladas[i]
                 << " | Prioridad: " << prioridadesSimuladas[i]
                 << " | Responsable: " << responsablesSimulados[i]
                 << " | Estado: " << estadosSimuladas[i]
                 << " | " << descripcionesSimuladas[i] << endl;
            hayPendientes = true;
        }
    }

    if (!hayPendientes) {
        cout << "No hay tareas pendientes." << endl;
    }
}

void GestorSistema::buscarTarea(int id) {
    cout << "  [Pendiente de conectar con ColaTareas] usando simulacion local." << endl;

    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        if (idsTareasSimuladas[i] == id) {
            cout << "Tarea ID " << id
                 << " | Prioridad: " << prioridadesSimuladas[i]
                 << " | Responsable: " << responsablesSimulados[i]
                 << " | Estado: " << estadosSimuladas[i]
                 << " | " << descripcionesSimuladas[i] << endl;
            return;
        }
    }

    cout << "No se encontro una tarea con el ID " << id << "." << endl;
}

// ---------------------------------------------------------------------
// cambiarEstadoTarea: cambia el estado de una tarea.
// Regla de rol: si usuarioActual es USUARIO_NORMAL, el cambio solo se
// permite cuando idUsuario coincide con el responsable asignado a la
// tarea. Un ADMINISTRADOR puede cambiar cualquier estado.
// ---------------------------------------------------------------------
bool GestorSistema::cambiarEstadoTarea(int idTarea, int idUsuario, const std::string& nuevoEstado) {
    cout << "  [Pendiente de conectar con ColaTareas] usando simulacion local." << endl;

    if (usuarioActual == nullptr) {
        cout << "Error: no hay una sesion activa para cambiar estados." << endl;
        return false;
    }

    long indice = -1;
    for (size_t i = 0; i < idsTareasSimuladas.size(); ++i) {
        if (idsTareasSimuladas[i] == idTarea) {
            indice = static_cast<long>(i);
            break;
        }
    }

    if (indice == -1) {
        cout << "No se encontro una tarea con el ID " << idTarea << "." << endl;
        return false;
    }

    // Validacion de rol para usuarios normales.
    if (usuarioActual->getRol() == Rol::USUARIO_NORMAL) {
        if (responsablesSimulados[indice] != idUsuario) {
            cout << "Error: un usuario normal solo puede cambiar el estado de las "
                    "tareas que le fueron asignadas." << endl;
            return false;
        }
        cout << "Permiso validado: la tarea esta asignada al usuario " << idUsuario << "." << endl;
    }

    estadosSimuladas[indice] = nuevoEstado;
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
// guardarTodo: guarda el estado actual en los CSV (lo llama CLI como
// gestor.guardarTodo()).
//
// CORREGIDO: ya NO se llama a guardarSubtareas con un vector vacio en
// cada guardado. Eso borraba subtareas.csv cada vez que se agregaba o
// modificaba cualquier usuario o tarea. Se reactivara cuando
// ArbolSubtareas este integrado y haya un vector real de subtareas que
// guardar.
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

    // TODO: activar cuando ArbolSubtareas este integrado y se tenga el
    // vector real de subtareas a guardar. Mientras tanto, NO se llama a
    // guardarSubtareas() para no sobrescribir subtareas.csv con datos
    // vacios en cada guardado.
    // persistencia->guardarSubtareas("subtareas.csv", filasSubtareas);

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