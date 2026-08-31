#include "Usuario.h"
#include <sstream>

using namespace std;

string rolATexto(Rol rol) {
    return (rol == Rol::ADMINISTRADOR) ? "Administrador" : "Usuario Normal";
}

Rol textoARol(const string& texto) {
    if (texto == "Administrador") return Rol::ADMINISTRADOR;
    return Rol::USUARIO_NORMAL;
}

Usuario::Usuario(int id, const string& nombre, const string& contrasena, Rol rol)
    : id(id), nombre(nombre), contrasena(contrasena), rol(rol),
      anterior(nullptr), siguiente(nullptr) {}

int Usuario::getId() const { return id; }
string Usuario::getNombre() const { return nombre; }
Rol Usuario::getRol() const { return rol; }
string Usuario::getContrasena() const { return contrasena; }
Usuario* Usuario::getAnterior() const { return anterior; }
Usuario* Usuario::getSiguiente() const { return siguiente; }

void Usuario::setNombre(const string& nuevoNombre) { nombre = nuevoNombre; }
void Usuario::setRol(Rol nuevoRol) { rol = nuevoRol; }
void Usuario::setContrasena(const string& nuevaContrasena) { contrasena = nuevaContrasena; }
void Usuario::setAnterior(Usuario* nodo) { anterior = nodo; }
void Usuario::setSiguiente(Usuario* nodo) { siguiente = nodo; }

bool Usuario::validarContrasena(const string& intento) const {
    return contrasena == intento;
}

string Usuario::toString() const {
    ostringstream oss;
    oss << "ID: " << id << " | Nombre: " << nombre << " | Rol: " << rolATexto(rol);
    return oss.str();
}
