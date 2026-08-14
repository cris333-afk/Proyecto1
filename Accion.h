#pragma once
#include <string>

// Operación que se realizó sobre la entidad.
// Se combina con TipoEntidad para identificar qué ocurrió.
enum class TipoAccion {
    AGREGAR,
    ACTUALIZAR,
    ELIMINAR
};

// Entidad afectada por la acción.
enum class TipoEntidad {
    USUARIO,
    TAREA
};

// Registra una operación que ya modificó el sistema.
// Solo guarda la información; no aplica los cambios por sí misma.
class Accion {
private:
    // Operación realizada (agregar, actualizar o eliminar).
    TipoAccion tipoAccion;

    // Entidad afectada (usuario o tarea).
    TipoEntidad tipoEntidad;

    // Identifica qué entidad se modificó.
    int idEntidad;

    // Se guarda para restaurar la información al deshacer.
    std::string estadoAnterior;

    // Se guarda para recuperar el cambio al rehacer.
    // Se usa string para no depender de Usuario ni Tarea.
    std::string estadoPosterior;

public:
    // Constructor por defecto.
    Accion();

    // Inicializa todos los datos de la acción.
    Accion(TipoAccion tipoAccion, TipoEntidad tipoEntidad, int idEntidad,
           std::string estadoAnterior, std::string estadoPosterior);

    // Getters: permiten consultar la información de la acción.
    TipoAccion getTipoAccion() const;
    TipoEntidad getTipoEntidad() const;
    int getIdEntidad() const;
    std::string getEstadoAnterior() const;
    std::string getEstadoPosterior() const;
};
