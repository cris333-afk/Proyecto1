// ==========================================================
// Modulo Usuarios - Sistema Gestor de Flujos de Trabajo
// Curso: EIF207 - Estructuras de Datos
// Autor: Cesar
//
// Clase Usuario: representa un nodo dentro de la lista doblemente
// enlazada ListaUsuarios. Incluye ademas los campos "rol" y
// "contrasena" requeridos por la Aclaratoria Oficial #1 (roles y
// permisos) y la Aclaratoria Oficial #2 (autenticacion / login).
// ==========================================================

#ifndef USUARIO_H
#define USUARIO_H

#include <string>

// Roles de acceso definidos en la Aclaratoria Oficial #1, seccion 1.
enum class Rol {
    ADMINISTRADOR,
    USUARIO_NORMAL
};

// Utilidades de conversion para mostrar el rol en consola o guardarlo en CSV.
std::string rolATexto(Rol rol);
Rol textoARol(const std::string& texto);

class Usuario {
private:
    int id;
    std::string nombre;
    std::string contrasena;
    Rol rol;

    // Punteros de enlace, administrados por ListaUsuarios.
    Usuario* anterior;
    Usuario* siguiente;

public:
    Usuario(int id, const std::string& nombre,
            const std::string& contrasena = "1234",
            Rol rol = Rol::USUARIO_NORMAL);

    // --- Getters ---
    int getId() const;
    std::string getNombre() const;
    Rol getRol() const;
    // Nota: expuesto solo para que ListaUsuarios pueda persistir el usuario
    // en CSV; en un sistema real nunca se expondria la contrasena asi.
    std::string getContrasena() const;
    Usuario* getAnterior() const;
    Usuario* getSiguiente() const;

    // --- Setters (soportan la operacion "modificar usuario") ---
    void setNombre(const std::string& nuevoNombre);
    void setRol(Rol nuevoRol);
    void setContrasena(const std::string& nuevaContrasena);
    void setAnterior(Usuario* nodo);
    void setSiguiente(Usuario* nodo);

    // Compara la contrasena ingresada contra la almacenada (para el login).
    bool validarContrasena(const std::string& intento) const;

    // Representacion en texto de un usuario, para listados en consola.
    std::string toString() const;
};

#endif // USUARIO_H
