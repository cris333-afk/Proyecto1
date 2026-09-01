#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <sstream>

#include "GestorSistema.h"
#include "Usuario.h"
#include "ListaUsuarios.h"
#include "PilaHistorial.h"
#include "ColaTareas.h"
#include "ColaPrioridad.h"
#include "ArbolSubtareas.h"
#include "Ordenador.h"
#include "GestorArchivosCSV.h"

using namespace std;

// Comparadores para Ordenador<Tarea*>
static bool menorPorId(Tarea* const& a, Tarea* const& b) {
    return a->getId() < b->getId();
}

static bool menorPorPrioridad(Tarea* const& a, Tarea* const& b) {
    auto val = [](const string& p) {
        if (p == "ALTA") return 0;
        if (p == "MEDIA") return 1;
        return 2;
    };
    return val(a->getPrioridad()) < val(b->getPrioridad());
}

static bool menorPorResponsable(Tarea* const& a, Tarea* const& b) {
    return a->getIdUsuarioResponsable() < b->getIdUsuarioResponsable();
}

// ---------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------
GestorSistema::GestorSistema()
    : listaUsuarios(new ListaUsuarios()),
      historial(new PilaHistorial()),
      colaFIFO(new ColaTareas()),
      colaPrioridad(new ColaPrioridad()),
      arbolTareas(new ArbolSubtareas()),
      persistencia(new GestorArchivosCSV()),
      usuarioActual(nullptr) {
    cargarTodo();
}

GestorSistema::~GestorSistema() {
    delete colaFIFO;
    delete colaPrioridad;
    delete arbolTareas;
    delete listaUsuarios;
    delete historial;
    delete persistencia;
    usuarioActual = nullptr;
}

// ---------------------------------------------------------------------
// Carga y Persistencia
// ---------------------------------------------------------------------
void GestorSistema::cargarTodo() {
    // 1. Cargar Usuarios
    vector<vector<string>> filasUsuarios = persistencia->cargarUsuarios("usuarios.csv");
    if (!filasUsuarios.empty()) {
        for (const auto& fila : filasUsuarios) {
            try {
                int id = stoi(fila[0]);
                string nombre = fila[1];
                Rol rol = textoARol(fila[2]);
                string contrasena = fila[3];
                listaUsuarios->insertar(id, nombre, contrasena, rol);
            } catch (const exception&) {
                cout << "Aviso: fila de usuarios.csv invalida omitida." << endl;
            }
        }
    } else {
        // Usuarios por defecto si el archivo esta vacio
        listaUsuarios->insertar(1, "Administrador", "admin123", Rol::ADMINISTRADOR);
        listaUsuarios->insertar(2, "Usuario Normal", "user123", Rol::USUARIO_NORMAL);
    }

    // 2. Cargar Tareas Principales
    vector<vector<string>> filasTareas = persistencia->cargarTareas("tareas.csv");
    for (const auto& fila : filasTareas) {
        try {
            int id = stoi(fila[0]);
            string prioridad = fila[1];
            int idResp = stoi(fila[2]);
            string desc = fila[3];
            string estado = fila[4];

            Tarea* nueva = new Tarea(id, prioridad, estado, desc, idResp);
            arbolTareas->agregarTarea(nueva);

            if (estado != "Completada") {
                colaFIFO->encolar(nueva);
                colaPrioridad->encolar(nueva);
            }
        } catch (const exception&) {
            cout << "Aviso: fila de tareas.csv invalida omitida." << endl;
        }
    }

    // 3. Cargar Subtareas
    vector<vector<string>> filasSubtareas = persistencia->cargarSubtareas("subtareas.csv");
    for (const auto& fila : filasSubtareas) {
        try {
            int id = stoi(fila[0]);
            int idPadre = stoi(fila[1]);
            string desc = fila[2];
            string estado = fila[3];

            if (idPadre != 0) {
                Tarea* nueva = new Tarea(id, "MEDIA", estado, desc, -1);
                if (arbolTareas->agregarSubtarea(idPadre, nueva)) {
                    if (estado != "Completada") {
                        colaFIFO->encolar(nueva);
                        colaPrioridad->encolar(nueva);
                    }
                } else {
                    delete nueva;
                }
            }
        } catch (const exception&) {
            cout << "Aviso: fila de subtareas.csv invalida omitida." << endl;
        }
    }

    // Si no habia tareas, sembrar ejemplos
    if (arbolTareas->contarNodos() == 0) {
        Tarea* t1 = new Tarea(10, "ALTA", "Pendiente", "Respaldar Base de Datos", 1);
        Tarea* t2 = new Tarea(11, "MEDIA", "Pendiente", "Actualizar Documentacion", 2);
        arbolTareas->agregarTarea(t1);
        arbolTareas->agregarTarea(t2);

        colaFIFO->encolar(t1);
        colaFIFO->encolar(t2);
        colaPrioridad->encolar(t1);
        colaPrioridad->encolar(t2);

        Tarea* sub1 = new Tarea(101, "BAJA", "Pendiente", "Comprimir archivos de exportacion", 1);
        arbolTareas->agregarSubtarea(10, sub1);
        colaFIFO->encolar(sub1);
        colaPrioridad->encolar(sub1);
    }
}

vector<vector<string>> GestorSistema::aplanarArbolParaGuardar() {
    vector<vector<string>> filas;
    Tarea* raizVirtual = arbolTareas->getRaiz();
    if (raizVirtual == nullptr) return filas;

    function<void(Tarea*, Tarea*)> visitar = [&](Tarea* padre, Tarea* nodo) {
        if (nodo == nullptr) return;
        vector<string> fila;
        fila.push_back(to_string(nodo->getId()));
        fila.push_back(to_string(padre->getId()));
        fila.push_back(nodo->getDescripcion());
        fila.push_back(nodo->getEstado());
        filas.push_back(fila);

        for (Tarea* sub : nodo->getSubtareas()) {
            visitar(nodo, sub);
        }
    };

    for (Tarea* tarea : raizVirtual->getSubtareas()) {
        visitar(raizVirtual, tarea);
    }
    return filas;
}

vector<Tarea*> GestorSistema::obtenerTodasLasTareas() {
    vector<Tarea*> lista;
    arbolTareas->recorrerConFiltro(
        [](Tarea*) { return true; },
        [&](Tarea* t) { lista.push_back(t); }
    );
    return lista;
}

void GestorSistema::guardarTodo() {
    cout << "Guardando cambios en los archivos CSV..." << endl;

    // 1. Guardar usuarios desde ListaUsuarios
    vector<vector<string>> filasUsuarios;
    int cantUsuarios = listaUsuarios->contar();
    for (int i = 1; i <= 9999; ++i) {
        Usuario* u = listaUsuarios->buscar(i);
        if (u != nullptr) {
            vector<string> fila;
            fila.push_back(to_string(u->getId()));
            fila.push_back(u->getNombre());
            fila.push_back(rolATexto(u->getRol()));
            fila.push_back(u->getContrasena());
            filasUsuarios.push_back(fila);
            if (static_cast<int>(filasUsuarios.size()) == cantUsuarios) break;
        }
    }
    persistencia->guardarUsuarios("usuarios.csv", filasUsuarios);

    // 2. Guardar tareas de nivel superior
    vector<vector<string>> filasTareas;
    Tarea* raizVirtual = arbolTareas->getRaiz();
    if (raizVirtual != nullptr) {
        for (Tarea* t : raizVirtual->getSubtareas()) {
            vector<string> fila;
            fila.push_back(to_string(t->getId()));
            fila.push_back(t->getPrioridad());
            fila.push_back(to_string(t->getIdUsuarioResponsable()));
            fila.push_back(t->getDescripcion());
            fila.push_back(t->getEstado());
            filasTareas.push_back(fila);
        }
    }
    persistencia->guardarTareas("tareas.csv", filasTareas);

    // 3. Guardar jerarquia de subtareas
    vector<vector<string>> filasSubtareas = aplanarArbolParaGuardar();
    persistencia->guardarSubtareas("subtareas.csv", filasSubtareas);

    cout << "Cambios guardados correctamente." << endl;
}

// ---------------------------------------------------------------------
// Autenticacion y Sesion
// ---------------------------------------------------------------------
Usuario* GestorSistema::iniciarSesion(int id, const string& contrasena) {
    usuarioActual = nullptr;
    Usuario* u = listaUsuarios->buscar(id);
    if (u != nullptr && u->validarContrasena(contrasena)) {
        usuarioActual = u;
        return usuarioActual;
    }
    return nullptr;
}

Usuario* GestorSistema::getUsuarioActual() {
    return usuarioActual;
}

// ---------------------------------------------------------------------
// Módulo de Gestión de Usuarios
// ---------------------------------------------------------------------
bool GestorSistema::agregarUsuario(int id, const string& nombre, Rol rol, const string& contrasena) {
    if (listaUsuarios->existeId(id)) {
        cout << "Error: ya existe un usuario con el ID " << id << "." << endl;
        return false;
    }
    if (nombre.empty() || contrasena.empty()) {
        cout << "Error: nombre y contrasena no pueden estar vacios." << endl;
        return false;
    }

    if (!listaUsuarios->insertar(id, nombre, contrasena, rol)) {
        return false;
    }

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string pos = nombre + "|" + rolATexto(rol) + "|" + contrasena;
    historial->registrarAccion(Accion(TipoAccion::AGREGAR, TipoEntidad::USUARIO, id, idAct, "", pos));

    persistencia->registrarAuditoria(to_string(idAct), "AGREGAR_USUARIO", to_string(id));
    guardarTodo();
    return true;
}

bool GestorSistema::actualizarUsuario(int id, const string& nombre, Rol rol) {
    Usuario* u = listaUsuarios->buscar(id);
    if (u == nullptr) {
        cout << "Error: no se encontro un usuario con el ID " << id << "." << endl;
        return false;
    }

    string prev = u->getNombre() + "|" + rolATexto(u->getRol()) + "|" + u->getContrasena();
    if (!listaUsuarios->modificar(id, nombre, rol)) {
        return false;
    }

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string pos = nombre + "|" + rolATexto(rol) + "|" + u->getContrasena();
    historial->registrarAccion(Accion(TipoAccion::ACTUALIZAR, TipoEntidad::USUARIO, id, idAct, prev, pos));

    persistencia->registrarAuditoria(to_string(idAct), "ACTUALIZAR_USUARIO", to_string(id));
    guardarTodo();
    return true;
}

bool GestorSistema::eliminarUsuario(int id) {
    Usuario* u = listaUsuarios->buscar(id);
    if (u == nullptr) {
        cout << "Error: no se encontro un usuario con el ID " << id << "." << endl;
        return false;
    }

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string prev = u->getNombre() + "|" + rolATexto(u->getRol()) + "|" + u->getContrasena();

    if (u == usuarioActual) {
        usuarioActual = nullptr;
        cout << "Aviso: la sesion activa se ha cerrado." << endl;
    }

    if (!listaUsuarios->eliminar(id)) {
        return false;
    }

    historial->registrarAccion(Accion(TipoAccion::ELIMINAR, TipoEntidad::USUARIO, id, idAct, prev, ""));
    persistencia->registrarAuditoria(to_string(idAct), "ELIMINAR_USUARIO", to_string(id));
    guardarTodo();
    return true;
}

void GestorSistema::listarUsuarios() {
    if (listaUsuarios->estaVacia()) {
        cout << "No hay usuarios registrados." << endl;
        return;
    }
    listaUsuarios->mostrarAdelante();
}

Usuario* GestorSistema::buscarUsuario(int id) {
    return listaUsuarios->buscar(id);
}

// ---------------------------------------------------------------------
// Módulo de Gestión de Tareas
// ---------------------------------------------------------------------
bool GestorSistema::agregarTarea(int id, const string& prioridad, int idResponsable, const string& descripcion) {
    if (arbolTareas->buscar(id) != nullptr) {
        cout << "Error: ya existe una tarea con el ID " << id << "." << endl;
        return false;
    }
    if (descripcion.empty()) {
        cout << "Error: la descripcion no puede estar vacia." << endl;
        return false;
    }

    Tarea* nueva = new Tarea(id, prioridad, "Pendiente", descripcion, idResponsable);
    arbolTareas->agregarTarea(nueva);
    colaFIFO->encolar(nueva);
    colaPrioridad->encolar(nueva);

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string pos = prioridad + "|" + to_string(idResponsable) + "|Pendiente|" + descripcion;
    historial->registrarAccion(Accion(TipoAccion::AGREGAR, TipoEntidad::TAREA, id, idAct, "", pos));

    persistencia->registrarAuditoria(to_string(idAct), "CREAR_TAREA", to_string(id));
    guardarTodo();
    return true;
}

bool GestorSistema::agregarSubtarea(int idPadre, int idSubtarea, const string& prioridad, int idResponsable, const string& descripcion) {
    if (arbolTareas->buscar(idPadre) == nullptr) {
        cout << "Error: no existe la tarea padre con ID " << idPadre << "." << endl;
        return false;
    }
    if (arbolTareas->buscar(idSubtarea) != nullptr) {
        cout << "Error: ya existe una tarea/subtarea con ID " << idSubtarea << "." << endl;
        return false;
    }

    Tarea* nueva = new Tarea(idSubtarea, prioridad, "Pendiente", descripcion, idResponsable);
    if (!arbolTareas->agregarSubtarea(idPadre, nueva)) {
        delete nueva;
        return false;
    }

    colaFIFO->encolar(nueva);
    colaPrioridad->encolar(nueva);

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string prev = "PADRE:" + to_string(idPadre);
    string pos = prioridad + "|" + to_string(idResponsable) + "|Pendiente|" + descripcion;
    historial->registrarAccion(Accion(TipoAccion::AGREGAR, TipoEntidad::TAREA, idSubtarea, idAct, prev, pos));

    persistencia->registrarAuditoria(to_string(idAct), "AGREGAR_SUBTAREA", to_string(idSubtarea));
    guardarTodo();
    return true;
}

bool GestorSistema::actualizarTarea(int id, const string& descripcion) {
    Tarea* t = arbolTareas->buscar(id);
    if (t == nullptr) {
        cout << "Error: no se encontro la tarea ID " << id << "." << endl;
        return false;
    }

    string prev = t->getPrioridad() + "|" + to_string(t->getIdUsuarioResponsable()) + "|" + t->getEstado() + "|" + t->getDescripcion();
    t->setDescripcion(descripcion);

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string pos = t->getPrioridad() + "|" + to_string(t->getIdUsuarioResponsable()) + "|" + t->getEstado() + "|" + descripcion;
    historial->registrarAccion(Accion(TipoAccion::ACTUALIZAR, TipoEntidad::TAREA, id, idAct, prev, pos));

    persistencia->registrarAuditoria(to_string(idAct), "ACTUALIZAR_TAREA", to_string(id));
    guardarTodo();
    return true;
}

bool GestorSistema::eliminarTarea(int id) {
    Tarea* t = arbolTareas->buscar(id);
    if (t == nullptr) {
        cout << "Error: no se encontro la tarea ID " << id << "." << endl;
        return false;
    }

    int idAct = usuarioActual ? usuarioActual->getId() : -1;
    string prev = t->getPrioridad() + "|" + to_string(t->getIdUsuarioResponsable()) + "|" + t->getEstado() + "|" + t->getDescripcion();

    if (!arbolTareas->eliminar(id)) {
        return false;
    }

    historial->registrarAccion(Accion(TipoAccion::ELIMINAR, TipoEntidad::TAREA, id, idAct, prev, ""));
    persistencia->registrarAuditoria(to_string(idAct), "ELIMINAR_TAREA", to_string(id));
    guardarTodo();
    return true;
}

void GestorSistema::listarTareasPendientes() {
    cout << "\n=== ESTRUCTURA DE TAREAS Y SUBTAREAS ===" << endl;

    function<void(Tarea*, int)> mostrarSubtree = [&](Tarea* nodo, int nivel) {
        if (nodo == nullptr) return;
        string indent(nivel * 3, ' ');
        cout << indent << "|-- [ID " << nodo->getId() << "] (" << nodo->getPrioridad() << ") "
             << "Resp: " << nodo->getIdUsuarioResponsable() << " | Estado: " << nodo->getEstado()
             << " | SLA Espera: " << nodo->getCiclosEspera() << " c. | "
             << nodo->getDescripcion() << endl;

        for (Tarea* sub : nodo->getSubtareas()) {
            mostrarSubtree(sub, nivel + 1);
        }
    };

    Tarea* raizVirtual = arbolTareas->getRaiz();
    if (raizVirtual == nullptr || raizVirtual->getSubtareas().empty()) {
        cout << "(No hay tareas registradas)" << endl;
        return;
    }

    for (Tarea* top : raizVirtual->getSubtareas()) {
        mostrarSubtree(top, 0);
    }
}

void GestorSistema::buscarTarea(int id) {
    Tarea* t = arbolTareas->buscar(id);
    if (t == nullptr) {
        cout << "No se encontro ninguna tarea con ID " << id << "." << endl;
        return;
    }

    cout << "\n--- Tarea Encontrada ---" << endl;
    cout << "ID: " << t->getId() << endl;
    cout << "Prioridad: " << t->getPrioridad() << endl;
    cout << "Estado: " << t->getEstado() << endl;
    cout << "Responsable ID: " << t->getIdUsuarioResponsable() << endl;
    cout << "Ciclos de espera SLA: " << t->getCiclosEspera() << endl;
    cout << "Descripcion: " << t->getDescripcion() << endl;
    cout << "Cantidad de subtareas directas: " << t->getSubtareas().size() << endl;
}

bool GestorSistema::cambiarEstadoTarea(int idTarea, int idUsuario, const string& nuevoEstado) {
    if (usuarioActual == nullptr) {
        cout << "Error: no hay sesion activa." << endl;
        return false;
    }

    Tarea* t = arbolTareas->buscar(idTarea);
    if (t == nullptr) {
        cout << "Error: no se encontro la tarea ID " << idTarea << "." << endl;
        return false;
    }

    if (usuarioActual->getRol() == Rol::USUARIO_NORMAL) {
        if (t->getIdUsuarioResponsable() != idUsuario) {
            cout << "Error de permisos: solo puede modificar tareas asignadas a su ID." << endl;
            return false;
        }
    }

    string estadoAnterior = t->getEstado();
    string prev = t->getPrioridad() + "|" + to_string(t->getIdUsuarioResponsable()) + "|" + estadoAnterior + "|" + t->getDescripcion();
    t->setEstado(nuevoEstado);

    string pos = t->getPrioridad() + "|" + to_string(t->getIdUsuarioResponsable()) + "|" + nuevoEstado + "|" + t->getDescripcion();
    historial->registrarAccion(Accion(TipoAccion::ACTUALIZAR, TipoEntidad::TAREA, idTarea, usuarioActual->getId(), prev, pos));

    persistencia->registrarAuditoria(to_string(usuarioActual->getId()), "CAMBIO_ESTADO", to_string(idTarea));
    guardarTodo();
    cout << "Estado de tarea " << idTarea << " actualizado a '" << nuevoEstado << "'." << endl;
    return true;
}

// ---------------------------------------------------------------------
// Motor de Ejecucion & Escalamiento SLA (Prevencion de Inanicion)
// ---------------------------------------------------------------------
void GestorSistema::verificarEscalamientoSLA() {
    vector<Tarea*> tareas = obtenerTodasLasTareas();
    for (Tarea* t : tareas) {
        if (t->getEstado() == "Pendiente" && t->getPrioridad() != "ALTA") {
            t->incrementarCiclosEspera();
            if (t->getCiclosEspera() >= MAX_CICLOS_ESPERA_SLA) {
                cout << "\n>>> [ALERTA SLA] Tarea ID " << t->getId() << " (" << t->getDescripcion()
                     << ") acumulo " << t->getCiclosEspera() << " ciclos. ESCALANDO a Prioridad ALTA!" << endl;

                t->setPrioridad("ALTA");
                colaPrioridad->encolar(t);

                string idAct = usuarioActual ? to_string(usuarioActual->getId()) : "-1";
                persistencia->registrarAuditoria(idAct, "ESCALAMIENTO_SLA_INANICION", to_string(t->getId()));
            }
        }
    }
}

bool GestorSistema::atenderSiguienteTarea() {
    Tarea* aAtender = nullptr;

    if (!colaPrioridad->estaVacia()) {
        aAtender = colaPrioridad->frente();
        colaPrioridad->desencolar();
    } else if (!colaFIFO->estaVacia()) {
        aAtender = colaFIFO->frente();
        colaFIFO->desencolar();
    }

    if (aAtender == nullptr) {
        cout << "No hay tareas pendientes en la cola de ejecucion." << endl;
        return false;
    }

    aAtender->setEstado("En Proceso");
    aAtender->resetCiclosEspera();

    cout << "\n>>> ATENDIENDO TAREA ID " << aAtender->getId()
         << " | Prioridad: " << aAtender->getPrioridad()
         << " | Responsable: " << aAtender->getIdUsuarioResponsable()
         << " | " << aAtender->getDescripcion() << endl;

    // Incrementa ciclos de espera y aplica regla SLA de inanicion a las demas tareas
    verificarEscalamientoSLA();

    string idAct = usuarioActual ? to_string(usuarioActual->getId()) : "-1";
    persistencia->registrarAuditoria(idAct, "ATENDER_TAREA", to_string(aAtender->getId()));
    guardarTodo();
    return true;
}

// ---------------------------------------------------------------------
// Historial (Deshacer / Rehacer)
// ---------------------------------------------------------------------
void GestorSistema::deshacer() {
    Accion acc;
    if (!historial->deshacer(acc)) {
        cout << "No hay acciones registradas para deshacer." << endl;
        return;
    }

    cout << "Deshaciendo accion: " << acc.toString() << endl;
    int idAct = usuarioActual ? usuarioActual->getId() : -1;

    if (acc.getTipoEntidad() == TipoEntidad::USUARIO) {
        if (acc.getTipoAccion() == TipoAccion::AGREGAR) {
            listaUsuarios->eliminar(acc.getIdEntidad());
        } else if (acc.getTipoAccion() == TipoAccion::ELIMINAR) {
            stringstream ss(acc.getEstadoAnterior());
            string nom, rolStr, pass;
            getline(ss, nom, '|');
            getline(ss, rolStr, '|');
            getline(ss, pass, '|');
            listaUsuarios->insertar(acc.getIdEntidad(), nom, pass, textoARol(rolStr));
        } else if (acc.getTipoAccion() == TipoAccion::ACTUALIZAR) {
            stringstream ss(acc.getEstadoAnterior());
            string nom, rolStr;
            getline(ss, nom, '|');
            getline(ss, rolStr, '|');
            listaUsuarios->modificar(acc.getIdEntidad(), nom, textoARol(rolStr));
        }
    } else if (acc.getTipoEntidad() == TipoEntidad::TAREA) {
        if (acc.getTipoAccion() == TipoAccion::AGREGAR) {
            arbolTareas->eliminar(acc.getIdEntidad());
        } else if (acc.getTipoAccion() == TipoAccion::ELIMINAR) {
            stringstream ss(acc.getEstadoAnterior());
            string prio, respStr, est, desc;
            getline(ss, prio, '|');
            getline(ss, respStr, '|');
            getline(ss, est, '|');
            getline(ss, desc, '|');
            Tarea* nueva = new Tarea(acc.getIdEntidad(), prio, est, desc, stoi(respStr));
            arbolTareas->agregarTarea(nueva);
            colaFIFO->encolar(nueva);
            colaPrioridad->encolar(nueva);
        } else if (acc.getTipoAccion() == TipoAccion::ACTUALIZAR) {
            Tarea* t = arbolTareas->buscar(acc.getIdEntidad());
            if (t != nullptr) {
                stringstream ss(acc.getEstadoAnterior());
                string prio, respStr, est, desc;
                getline(ss, prio, '|');
                getline(ss, respStr, '|');
                getline(ss, est, '|');
                getline(ss, desc, '|');
                t->setPrioridad(prio);
                t->setIdUsuarioResponsable(stoi(respStr));
                t->setEstado(est);
                t->setDescripcion(desc);
            }
        }
    }

    persistencia->registrarAuditoria(to_string(idAct), "DESHACER", to_string(acc.getIdEntidad()));
    guardarTodo();
}

void GestorSistema::rehacer() {
    Accion acc;
    if (!historial->rehacer(acc)) {
        cout << "No hay acciones para rehacer." << endl;
        return;
    }

    cout << "Rehaciendo accion: " << acc.toString() << endl;
    int idAct = usuarioActual ? usuarioActual->getId() : -1;

    if (acc.getTipoEntidad() == TipoEntidad::USUARIO) {
        if (acc.getTipoAccion() == TipoAccion::AGREGAR) {
            stringstream ss(acc.getEstadoPosterior());
            string nom, rolStr, pass;
            getline(ss, nom, '|');
            getline(ss, rolStr, '|');
            getline(ss, pass, '|');
            listaUsuarios->insertar(acc.getIdEntidad(), nom, pass, textoARol(rolStr));
        } else if (acc.getTipoAccion() == TipoAccion::ELIMINAR) {
            listaUsuarios->eliminar(acc.getIdEntidad());
        } else if (acc.getTipoAccion() == TipoAccion::ACTUALIZAR) {
            stringstream ss(acc.getEstadoPosterior());
            string nom, rolStr;
            getline(ss, nom, '|');
            getline(ss, rolStr, '|');
            listaUsuarios->modificar(acc.getIdEntidad(), nom, textoARol(rolStr));
        }
    } else if (acc.getTipoEntidad() == TipoEntidad::TAREA) {
        if (acc.getTipoAccion() == TipoAccion::AGREGAR) {
            stringstream ss(acc.getEstadoPosterior());
            string prio, respStr, est, desc;
            getline(ss, prio, '|');
            getline(ss, respStr, '|');
            getline(ss, est, '|');
            getline(ss, desc, '|');
            Tarea* nueva = new Tarea(acc.getIdEntidad(), prio, est, desc, stoi(respStr));
            arbolTareas->agregarTarea(nueva);
            colaFIFO->encolar(nueva);
            colaPrioridad->encolar(nueva);
        } else if (acc.getTipoAccion() == TipoAccion::ELIMINAR) {
            arbolTareas->eliminar(acc.getIdEntidad());
        } else if (acc.getTipoAccion() == TipoAccion::ACTUALIZAR) {
            Tarea* t = arbolTareas->buscar(acc.getIdEntidad());
            if (t != nullptr) {
                stringstream ss(acc.getEstadoPosterior());
                string prio, respStr, est, desc;
                getline(ss, prio, '|');
                getline(ss, respStr, '|');
                getline(ss, est, '|');
                getline(ss, desc, '|');
                t->setPrioridad(prio);
                t->setIdUsuarioResponsable(stoi(respStr));
                t->setEstado(est);
                t->setDescripcion(desc);
            }
        }
    }

    persistencia->registrarAuditoria(to_string(idAct), "REHACER", to_string(acc.getIdEntidad()));
    guardarTodo();
}

// ---------------------------------------------------------------------
// Módulo de Reportes & Ordenamiento (Análisis Asintótico Big-O)
// ---------------------------------------------------------------------
void GestorSistema::mostrarReporteOrdenado() {
    vector<Tarea*> tareas = obtenerTodasLasTareas();
    if (tareas.empty()) {
        cout << "No hay tareas para ordenar." << endl;
        return;
    }

    int n = static_cast<int>(tareas.size());
    Tarea** arrBurbuja = new Tarea*[n];
    Tarea** arrMerge = new Tarea*[n];
    Tarea** arrQuick = new Tarea*[n];

    for (int i = 0; i < n; ++i) {
        arrBurbuja[i] = tareas[i];
        arrMerge[i] = tareas[i];
        arrQuick[i] = tareas[i];
    }

    Ordenador<Tarea*> ordenador;
    ordenador.burbuja(arrBurbuja, n, menorPorId);
    ordenador.mergeSort(arrMerge, n, menorPorPrioridad);
    ordenador.quickSort(arrQuick, n, menorPorResponsable);

    cout << "\n=======================================================" << endl;
    cout << "        REPORTE DE TAREAS Y ALGORITMOS DE ORDENAMIENTO  " << endl;
    cout << "=======================================================" << endl;
    cout << "\n--- 1. BUBBLE SORT (Ordenado por ID asc) ---" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  ID " << arrBurbuja[i]->getId() << " | Prioridad: " << arrBurbuja[i]->getPrioridad()
             << " | Resp: " << arrBurbuja[i]->getIdUsuarioResponsable() << " | " << arrBurbuja[i]->getDescripcion() << endl;
    }

    cout << "\n--- 2. MERGE SORT (Ordenado por Prioridad: ALTA < MEDIA < BAJA) ---" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  ID " << arrMerge[i]->getId() << " | Prioridad: " << arrMerge[i]->getPrioridad()
             << " | Resp: " << arrMerge[i]->getIdUsuarioResponsable() << " | " << arrMerge[i]->getDescripcion() << endl;
    }

    cout << "\n--- 3. QUICK SORT (Ordenado por ID Usuario Responsable asc) ---" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  ID " << arrQuick[i]->getId() << " | Prioridad: " << arrQuick[i]->getPrioridad()
             << " | Resp: " << arrQuick[i]->getIdUsuarioResponsable() << " | " << arrQuick[i]->getDescripcion() << endl;
    }

    delete[] arrBurbuja;
    delete[] arrMerge;
    delete[] arrQuick;

    mostrarAnalisisAsintotico();
}

void GestorSistema::mostrarAnalisisAsintotico() {
    cout << "\n==========================================================================" << endl;
    cout << "             ANALISIS DE COMPLEJIDAD ASINTOTICA (BIG-O)                   " << endl;
    cout << "==========================================================================" << endl;
    cout << " Algoritmo    | Mejor Caso    | Caso Promedio | Peor Caso     | Memoria  " << endl;
    cout << "--------------+---------------+---------------+---------------+-----------" << endl;
    cout << " Bubble Sort  | O(n)          | O(n^2)        | O(n^2)        | O(1)     " << endl;
    cout << " Merge Sort   | O(n log n)    | O(n log n)    | O(n log n)    | O(n)     " << endl;
    cout << " Quick Sort   | O(n log n)    | O(n log n)    | O(n^2)        | O(log n) " << endl;
    cout << "==========================================================================" << endl;
    cout << " Explicacion Tecnica:" << endl;
    cout << " 1. Bubble Sort: Incluye bandera de optimizacion 'intercambio'. Si el arreglo" << endl;
    cout << "    esta ordenado, se detiene en 1 pasada -> O(n)." << endl;
    cout << " 2. Merge Sort: Estrategia 'Divide y Vence'. Divide recursivamente en mitades" << endl;
    cout << "    y las mezcla. Siempre ejecuta log(n) niveles y mezcla en O(n) -> O(n log n)." << endl;
    cout << " 3. Quick Sort: Particion por pivote Lomuto central. En promedio divide a la" << endl;
    cout << "    mitad -> O(n log n). Peor caso si el pivote es extremo repetitivo -> O(n^2)." << endl;
    cout << "==========================================================================" << endl;
}