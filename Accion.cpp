#include "Accion.h"

// Constructor por defecto.
Accion::Accion()
    : tipoAccion(TipoAccion::AGREGAR),
      tipoEntidad(TipoEntidad::USUARIO),
      idEntidad(0),
      estadoAnterior(""),
      estadoPosterior("") {}

// Constructor que inicializa todos los datos de la acción.
Accion::Accion(TipoAccion tipoAccion, TipoEntidad tipoEntidad, int idEntidad,
               std::string estadoAnterior, std::string estadoPosterior)
    : tipoAccion(tipoAccion),
      tipoEntidad(tipoEntidad),
      idEntidad(idEntidad),
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

std::string Accion::getEstadoAnterior() const {
    return estadoAnterior;
}

std::string Accion::getEstadoPosterior() const {
    return estadoPosterior;
}