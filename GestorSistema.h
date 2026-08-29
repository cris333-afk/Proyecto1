#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include <string>
#include <vector>

#include "GestorArchivosCSV.h"  // Real: ya integrada.
#include "Usuario.h"            // Real: define Usuario y el enum Rol.

// Declaraciones adelantadas: modulos de companeros aun no integrados en
// esta rama. Se usan solo como punteros aqui, asi que no hace falta ver su
// contenido completo todavia.
class ListaUsuarios;
class PilaHistorial;
class ColaTareas;
class ColaPrioridad;
class ArbolSubtareas;

// GestorSistema: clase fachada del modulo "Sistema e Integracion". Es el
// unico punto de contacto entre el CLI y el resto de los modulos del
// proyecto. Mientras los modulos de los companeros no esten integrados,
// se usan vectores de simulacion como respaldo (usuariosSimulados,
// idsTareasSimuladas, etc.), que se reemplazaran por las estructuras
// reales (listaUsuarios, colaFIFO, ...) cuando esten disponibles.
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
    void agregarTarea(int id, const std::string& prioridad, int idResponsable, const std::string& descripcion);
    void actualizarTarea(int id, const std::string& descripcion);
    void eliminarTarea(int id);
    void listarTareasPendientes();
    void buscarTarea(int id);
    bool cambiarEstadoTarea(int idTarea, int idUsuario, const std::string& nuevoEstado);

    // Persistencia y reportes.
    void guardarTodo();
    void mostrarReporteOrdenado();

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

    // Sesion activa. NO es dueno de su memoria: apunta a un nodo dentro de
    // listaUsuarios (o, mientras se usa la simulacion, dentro de
    // usuariosSimulados), asi que el destructor no debe liberarlo aqui.
    Usuario* usuarioActual;

    // --- SIMULACION TEMPORAL ---
    // Respaldo mientras ListaUsuarios/ColaTareas/ColaPrioridad/ArbolSubtareas
    // no esten integrados. Se elimina cuando esos modulos reales lleguen.
    std::vector<Usuario*> usuariosSimulados;
    std::vector<int> idsTareasSimuladas;
    std::vector<std::string> prioridadesSimuladas;
    std::vector<int> responsablesSimulados;
    std::vector<std::string> estadosSimuladas;
    std::vector<std::string> descripcionesSimuladas;

    // Carga inicial: reconstruye usuariosSimulados e idsTareasSimuladas (y
    // vectores relacionados) a partir de usuarios.csv y tareas.csv. Si un
    // archivo viene vacio (primera ejecucion), siembra los datos de
    // ejemplo como respaldo. Se llama una sola vez, al final del
    // constructor.
    void cargarTodo();
};

#endif  // GESTOR_SISTEMA_H