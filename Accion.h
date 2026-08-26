#pragma once
#include <string>

// Tipo de operación realizada sobre una entidad.
// Estos valores permiten saber qué operación debe interpretarse
// cuando se procesa una Accion del historial.
enum class TipoAccion {
    AGREGAR,
    ACTUALIZAR,
    ELIMINAR
};

// Identifica qué tipo de entidad fue afectada por la operación;
// actualmente el historial registra operaciones sobre usuarios y tareas.
enum class TipoEntidad {
    USUARIO,
    TAREA
};


// Representa una operación que ya fue realizada sobre una entidad del sistema.
// NO ejecuta, deshace ni rehace operaciones: únicamente conserva la información
// necesaria para que el Gestor pueda interpretar lo ocurrido y actuar en consecuencia.
// Está desacoplada de Usuario y Tarea gracias a los identificadores y a que los
// estados se almacenan como string.
class Accion {
    private:
    // Indica qué operación ocurrió (agregar, actualizar o eliminar).
    TipoAccion tipoAccion;

    // Indica qué tipo de entidad fue afectada.
    TipoEntidad tipoEntidad;

    // Identifica exactamente qué usuario o tarea fue modificada (la entidad afectada).
    int idEntidad;

    // Identifica al usuario que realizó la operación.
    // No es la entidad afectada: es el usuario responsable de ejecutarla.
    int idUsuario;

    // Estado de la entidad antes de la operación; durante un deshacer
    // Se guarda como string para mantener el módulo Historial desacoplado de Usuario y Tarea.
    std::string estadoAnterior;

    // Estado de la entidad después de la operación; durante un rehacer
    // Se guarda como string para mantener el módulo Historial desacoplado de Usuario y Tarea.
    std::string estadoPosterior;

public:

    // Constructor por defecto.
    Accion();

    //Inicializa todos los datos de la acción.

    Accion(TipoAccion tipoAccion, TipoEntidad tipoEntidad, int idEntidad,
           int idUsuario, std::string estadoAnterior, std::string estadoPosterior);

    // Retorna el tipo de operación realizada (AGREGAR, ACTUALIZAR, ELIMINAR).
    TipoAccion getTipoAccion() const;

    // Retorna el tipo de entidad afectada (USUARIO o TAREA).
    TipoEntidad getTipoEntidad() const;

    // Retorna el ID de la entidad modificada (usuario o tarea específica).
    int getIdEntidad() const;

     // Retorna el ID del usuario que ejecutó la operación.
    int getIdUsuario() const;

    // Retorna el estado de la entidad antes de la operación.
    // Se usa durante deshacer para restaurar este estado.
    std::string getEstadoAnterior() const;


    // Retorna el estado de la entidad después de la operación.
    // Se usa durante rehacer para volver a aplicarlo.
    std::string getEstadoPosterior() const;

    // Devuelve como texto los datos de la acción: tipo de operación, tipo e ID
    // de entidad, ID de usuario y estado anterior/posterior. Se genera aquí para
    // que Accion no dependa de cómo ni dónde se imprime esa información.
    std::string toString() const;
};
