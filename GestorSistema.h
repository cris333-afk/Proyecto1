#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include <string>
#include <vector>

#include "GestorArchivosCSV.h"
#include "Usuario.h"
#include "ListaUsuarios.h"
#include "PilaHistorial.h"
#include "ColaTareas.h"
#include "ColaPrioridad.h"
#include "ArbolSubtareas.h"

// GestorSistema: clase fachada del modulo "Sistema e Integracion". Es el
// unico punto de contacto entre el CLI y el resto de los modulos del proyecto.
class GestorSistema {
public:
    GestorSistema();
    ~GestorSistema();

    // Login y sesion.
    Usuario* iniciarSesion(int id, const std::string& contrasena);
    Usuario* getUsuarioActual();

    // Gestion de usuarios (Administrador).
    bool agregarUsuario(int id, const std::string& nombre, Rol rol, const std::string& contrasena);
    bool actualizarUsuario(int id, const std::string& nombre, Rol rol);
    bool eliminarUsuario(int id);
    void listarUsuarios();
    Usuario* buscarUsuario(int id);

    // Gestion de tareas (Administrador) y cambio de estado (ambos roles).
    bool agregarTarea(int id, const std::string& prioridad, int idResponsable, const std::string& descripcion);
    bool agregarSubtarea(int idPadre, int idSubtarea, const std::string& prioridad, int idResponsable, const std::string& descripcion);
    bool actualizarTarea(int id, const std::string& descripcion);
    bool eliminarTarea(int id);
    void listarTareasPendientes();
    void buscarTarea(int id);
    bool cambiarEstadoTarea(int idTarea, int idUsuario, const std::string& nuevoEstado);

    // Motor de Ejecucion y Escalamiento SLA
    bool atenderSiguienteTarea();

    // Persistencia y reportes.
    void guardarTodo();
    void mostrarReporteOrdenado();
    void mostrarAnalisisAsintotico();

    // Historial (deshacer/rehacer).
    void deshacer();
    void rehacer();

private:
    // Estructuras reales del proyecto (punteros; memoria dinamica manual).
    ListaUsuarios* listaUsuarios;
    PilaHistorial* historial;
    ColaTareas* colaFIFO;
    ColaPrioridad* colaPrioridad;
    ArbolSubtareas* arbolTareas;
    GestorArchivosCSV* persistencia;

    // Sesion activa. NO es dueno de su memoria: apunta a un nodo dentro de listaUsuarios.
    Usuario* usuarioActual;

    static constexpr int MAX_CICLOS_ESPERA_SLA = 3; // Limite de espera para auto-escalamiento SLA

    void cargarTodo();
    std::vector<std::vector<std::string>> aplanarArbolParaGuardar();
    std::vector<Tarea*> obtenerTodasLasTareas();
    void verificarEscalamientoSLA();
};

#endif  // GESTOR_SISTEMA_H