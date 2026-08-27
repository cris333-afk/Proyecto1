#include "Accion.h"

// Convierte TipoAccion a texto legible.
static std::string tipoAccionATexto(TipoAccion tipo) {
    switch (tipo) {
        case TipoAccion::AGREGAR: return "AGREGAR";
        case TipoAccion::ACTUALIZAR: return "ACTUALIZAR";
        case TipoAccion::ELIMINAR: return "ELIMINAR";
    }
    return "DESCONOCIDO";
}

// Convierte TipoEntidad a texto legible.
static std::string tipoEntidadATexto(TipoEntidad tipo) {
    switch (tipo) {
        case TipoEntidad::USUARIO: return "USUARIO";
        case TipoEntidad::TAREA: return "TAREA";
    }
    return "DESCONOCIDO";
}

// Constructor por defecto.
Accion::Accion()
    : tipoAccion(TipoAccion::AGREGAR),
      tipoEntidad(TipoEntidad::USUARIO),
      idEntidad(0),
      idUsuario(0),
      estadoAnterior(""),
      estadoPosterior("") {}

// Constructor que inicializa todos los datos de la acción.
Accion::Accion(TipoAccion tipoAccion, TipoEntidad tipoEntidad, int idEntidad,
               int idUsuario, std::string estadoAnterior, std::string estadoPosterior)
    : tipoAccion(tipoAccion),
      tipoEntidad(tipoEntidad),
      idEntidad(idEntidad),
      idUsuario(idUsuario),
      estadoAnterior(estadoAnterior),
      estadoPosterior(estadoPosterior) {}

TipoAccion Accion::getTipoAccion() const {
    return tipoAccion;
}

TipoEntidad Accion::getTipoEntidad() const {
    return tipoEntidad;
}

int Accion::getIdEntidad() const {
    return idEntidad;
}

int Accion::getIdUsuario() const {
    return idUsuario;
}

std::string Accion::getEstadoAnterior() const {
    return estadoAnterior;
}

std::string Accion::getEstadoPosterior() const {
    return estadoPosterior;
}

// Une en un único string todos los campos de la acción, apoyándose en los
// conversores de los enums. El módulo solo genera la información; mostrarla
// en pantalla queda a cargo de quien reciba este texto (pruebas o menú).
std::string Accion::toString() const {
    std::string texto;
    texto += "Tipo de accion: " + tipoAccionATexto(tipoAccion) + "\n";
    texto += "Tipo de entidad: " + tipoEntidadATexto(tipoEntidad) + "\n";
    texto += "ID de entidad: " + std::to_string(idEntidad) + "\n";
    texto += "ID de usuario: " + std::to_string(idUsuario) + "\n";
    texto += "Estado anterior: " + estadoAnterior + "\n";
    texto += "Estado posterior: " + estadoPosterior + "\n";
    return texto;
}