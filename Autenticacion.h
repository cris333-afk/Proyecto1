// ==========================================================
// Modulo Autenticacion - Sistema Gestor de Flujos de Trabajo
// Curso: EIF207 - Estructuras de Datos
//
// Autenticacion: clase que encapsula el inicio de sesion del
// sistema. Busca un Usuario por ID dentro de ListaUsuarios y valida
// su contrasena (leida sin mostrarse en pantalla) mediante
// Usuario::validarContrasena().
//
// Se usa compilacion condicional (#ifdef _WIN32) porque el equipo
// trabaja repartido entre Windows y Linux, y cada sistema requiere
// una primitiva distinta para ocultar la contrasena:
//   - Windows:  conio.h, funcion _getch().
//   - Linux/Mac: termios.h, cambiar el eco de la terminal.
// Forma parte de la Aclaratoria Oficial #2 (autenticacion/login).
// ==========================================================

#ifndef AUTENTICACION_H
#define AUTENTICACION_H

#include <string>

// Usuario y ListaUsuarios: se incluyen porque el metodo
// iniciarSesion() busca un Usuario por ID dentro de ListaUsuarios y
// valida su contrasena con Usuario::validarContrasena().
#include "Usuario.h"
#include "ListaUsuarios.h"

/**
 * Lee una contrasena desde la consola sin mostrarla en pantalla.
 * (Funcion libre ya existente en el modulo Autenticacion; la usan
 * CLI y el metodo Autenticacion::iniciarSesion().)
 *
 * - Windows: lee tecla por tecla con _getch() (conio.h) hasta que
 *   el usuario presiona Enter; no se imprime nada de lo escrito.
 * - Linux/Mac: desactiva el eco de la terminal (termios.h) antes de
 *   leer con std::getline y lo reactiva al terminar, incluso si
 *   ocurre una excepcion (guard RAII).
 *
 * @return La contrasena ingresada como std::string.
 */
std::string leerContrasenaOculta();

/**
 * Clase Autenticacion: encapsula el inicio de sesion.
 * Cubre la Aclaratoria Oficial #2 (autenticacion/login).
 */
class Autenticacion {
public:
    /**
     * Pide por consola el ID (int) del usuario y su contrasena (con
     * lectura oculta), busca el usuario por ID en la lista y valida
     * la contrasena con Usuario::validarContrasena().
     *
     * Por seguridad, si el usuario no existe o la contrasena es
     * incorrecta se muestra un mensaje de error generico y se
     * devuelve nullptr (no se revela cual de los dos fallo).
     *
     * @param usuarios Lista de usuarios donde buscar.
     * @return Puntero al Usuario autenticado, o nullptr si fallo.
     */
    Usuario* iniciarSesion(ListaUsuarios& usuarios);
};

/**
 * Verifica que el usuario autenticado tenga el rol necesario para
 * ejecutar una accion protegida (usa Usuario::getRol() y el
 * enum class Rol con ADMINISTRADOR y USUARIO_NORMAL).
 *
 * Como la funcion es void, si el rol no cumple NO puede detener la
 * accion con un simple return: muestra el mensaje "Acceso denegado."
 * y lanza std::runtime_error para interrumpir el flujo. El menu que
 * usa esta funcion debe envolver la llamada (y la accion protegida)
 * en un try/catch.
 *
 * @param usuario       Usuario autenticado (nullptr = sin sesion).
 * @param rolRequerido  Rol minimo necesario para la accion.
 */
void verificarPermiso(Usuario* usuario, Rol rolRequerido);

#endif // AUTENTICACION_H