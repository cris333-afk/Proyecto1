// ==========================================================
// Modulo Usuarios - Sistema Gestor de Flujos de Trabajo
// Curso: EIF207 - Estructuras de Datos
// Autor: Cesar
//
// ListaUsuarios: lista doblemente enlazada de Usuario, con cabeza,
// cola y contador de elementos. Cubre las responsabilidades de la
// tarjeta del proyecto: insertar, eliminar, buscar, modificar,
// recorrer en ambos sentidos, validar IDs duplicados, contar y
// verificar lista vacia. Se agregan ademas autenticar() (Aclaratoria
// #2) y guardarCSV()/cargarCSV() (persistencia exigida en el
// enunciado y en la Aclaratoria #1, seccion 4).
// ==========================================================

#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include "Usuario.h"
#include <string>

class ListaUsuarios {
private:
    Usuario* cabeza;
    Usuario* cola;
    int cantidad;

public:
    ListaUsuarios();
    ~ListaUsuarios();

    // Se prohibe copiar la lista para evitar doble liberacion
    // de memoria de los nodos.
    ListaUsuarios(const ListaUsuarios&) = delete;
    ListaUsuarios& operator=(const ListaUsuarios&) = delete;

    // --- Operaciones principales ---

    // Inserta un usuario al final de la lista.
    // Retorna false si el ID ya existe.
    bool insertar(int id,
                  const std::string& nombre,
                  const std::string& contrasena = "1234",
                  Rol rol = Rol::USUARIO_NORMAL);

    // Elimina un usuario por su ID.
    // Retorna false si no existe.
    bool eliminar(int id);

    // Busca un usuario por su ID.
    // Retorna nullptr si no existe.
    Usuario* buscar(int id) const;

    // Modifica el nombre de un usuario.
    // Retorna false si no existe.
    bool modificar(int id, const std::string& nuevoNombre);

    // Modifica el nombre y el rol de un usuario.
    // Retorna false si no existe.
    bool modificar(int id,
                   const std::string& nuevoNombre,
                   Rol nuevoRol);

    // --- Recorridos ---

    // Recorre la lista desde cabeza hasta cola.
    void mostrarAdelante() const;

    // Recorre la lista desde cola hasta cabeza.
    void mostrarAtras() const;

    // --- Utilidades ---

    // Verifica si existe un usuario con el ID indicado.
    bool existeId(int id) const;

    // Retorna la cantidad de usuarios.
    int contar() const;

    // Indica si la lista esta vacia.
    bool estaVacia() const;
};

#endif // LISTAUSUARIOS_H