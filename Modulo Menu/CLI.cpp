#include "CLI.h"
#include "ValidacionEntrada.h"
#include "EntradaSegura.h"
#include <iostream>

using namespace std;

CLI::CLI() {}

void CLI::mostrarBienvenida() const {
    cout << "\n============================================" << endl;
    cout << "  Sistema Gestor de Flujos de Trabajo" << endl;
    cout << "============================================" << endl;
}

bool CLI::iniciarSesion() {
    cout << "\n--- Inicio de sesion ---" << endl;
    int id = leerEnteroValidado("ID de usuario: ", 0);

    cout << "Contrasena: ";
    string contrasena = leerContrasenaOculta();

    Usuario* usuario = gestor.iniciarSesion(id, contrasena);
    if (usuario == nullptr) {
        cout << "ID o contrasena incorrectos." << endl;
        return false;
    }

    cout << "Bienvenido/a, " << usuario->getNombre()
         << " (" << rolATexto(usuario->getRol()) << ")" << endl;
    return true;
}

void CLI::ejecutar() {
    mostrarBienvenida();

    bool sesionActiva = false;
    while (!sesionActiva) {
        sesionActiva = iniciarSesion();
        if (!sesionActiva) {
            int opcion = leerEnteroValidado("Presione 1 para reintentar, 0 para salir: ", 0, 1);
            if (opcion == 0) return;
        }
    }

    Usuario* usuario = gestor.getUsuarioActual();
    if (usuario->getRol() == Rol::ADMINISTRADOR) {
        menuAdministrador();
    } else {
        menuUsuarioNormal();
    }

    cout << "\nSesion finalizada. Hasta pronto." << endl;
}

void CLI::menuAdministrador() {
    bool salir = false;
    while (!salir) {
        cout << "\n--- Menu Administrador ---" << endl;
        cout << "1. Gestion de Usuarios" << endl;
        cout << "2. Gestion de Tareas" << endl;
        cout << "3. Ver reportes ordenados" << endl;
        cout << "4. Deshacer ultima accion" << endl;
        cout << "5. Rehacer" << endl;
        cout << "6. Guardar cambios" << endl;
        cout << "0. Cerrar sesion y salir" << endl;
        int opcion = leerEnteroValidado("Seleccione una opcion: ", 0, 6);

        switch (opcion) {
            case 1: submenuGestionUsuarios(); break;
            case 2: submenuGestionTareas(); break;
            case 3: gestor.mostrarReporteOrdenado(); break;
            case 4: gestor.deshacer(); break;
            case 5: gestor.rehacer(); break;
            case 6: gestor.guardarTodo(); break;
            case 0: salir = true; break;
        }
    }
}

void CLI::menuUsuarioNormal() {
    Usuario* usuario = gestor.getUsuarioActual();
    bool salir = false;
    while (!salir) {
        cout << "\n--- Menu Usuario Normal ---" << endl;
        cout << "1. Ver lista general de tareas" << endl;
        cout << "2. Cambiar estado de una tarea asignada" << endl;
        cout << "0. Cerrar sesion y salir" << endl;
        int opcion = leerEnteroValidado("Seleccione una opcion: ", 0, 2);

        switch (opcion) {
            case 1:
                gestor.listarTareasPendientes();
                break;
            case 2: {
                int idTarea = leerEnteroValidado("ID de la tarea: ", 0);
                string nuevoEstado = leerTextoNoVacio("Nuevo estado (ej. Completada): ");
                bool exito = gestor.cambiarEstadoTarea(idTarea, usuario->getId(), nuevoEstado);
                if (!exito) cout << "No se pudo actualizar el estado de la tarea." << endl;
                break;
            }
            case 0:
                salir = true;
                break;
        }
    }
}

void CLI::submenuGestionUsuarios() {
    bool volver = false;
    while (!volver) {
        cout << "\n-- Gestion de Usuarios --" << endl;
        cout << "1. Agregar usuario" << endl;
        cout << "2. Actualizar usuario" << endl;
        cout << "3. Eliminar usuario" << endl;
        cout << "4. Listar todos los usuarios" << endl;
        cout << "5. Buscar usuario por ID" << endl;
        cout << "0. Volver" << endl;
        int opcion = leerEnteroValidado("Seleccione una opcion: ", 0, 5);

        switch (opcion) {
            case 1: {
                int id = leerEnteroValidado("ID del nuevo usuario: ", 0);
                string nombre = leerTextoNoVacio("Nombre: ");
                int tipoRol = leerEnteroValidado("Rol (1=Administrador, 2=Usuario Normal): ", 1, 2);
                Rol rol = (tipoRol == 1) ? Rol::ADMINISTRADOR : Rol::USUARIO_NORMAL;
                cout << "Contrasena para el nuevo usuario: ";
                string contrasena = leerContrasenaOculta();

                bool exito = gestor.agregarUsuario(id, nombre, rol, contrasena);
                cout << (exito ? "Usuario agregado correctamente." : "Ya existe un usuario con ese ID.") << endl;
                break;
            }
            case 2: {
                int id = leerEnteroValidado("ID del usuario a actualizar: ", 0);
                string nombre = leerTextoNoVacio("Nuevo nombre: ");
                int tipoRol = leerEnteroValidado("Nuevo rol (1=Administrador, 2=Usuario Normal): ", 1, 2);
                Rol rol = (tipoRol == 1) ? Rol::ADMINISTRADOR : Rol::USUARIO_NORMAL;

                bool exito = gestor.actualizarUsuario(id, nombre, rol);
                cout << (exito ? "Usuario actualizado correctamente." : "No se encontro un usuario con ese ID.") << endl;
                break;
            }
            case 3: {
                int id = leerEnteroValidado("ID del usuario a eliminar: ", 0);
                bool exito = gestor.eliminarUsuario(id);
                cout << (exito ? "Usuario eliminado correctamente." : "No se encontro un usuario con ese ID.") << endl;
                break;
            }
            case 4:
                gestor.listarUsuarios();
                break;
            case 5: {
                int id = leerEnteroValidado("ID a buscar: ", 0);
                Usuario* usuario = gestor.buscarUsuario(id);
                if (usuario != nullptr) cout << usuario->toString() << endl;
                else cout << "No se encontro un usuario con ese ID." << endl;
                break;
            }
            case 0:
                volver = true;
                break;
        }
    }
}

void CLI::submenuGestionTareas() {
    bool volver = false;
    while (!volver) {
        cout << "\n-- Gestion de Tareas --" << endl;
        cout << "1. Agregar tarea" << endl;
        cout << "2. Actualizar tarea" << endl;
        cout << "3. Eliminar/cancelar tarea" << endl;
        cout << "4. Listar tareas pendientes" << endl;
        cout << "5. Buscar tarea por ID" << endl;
        cout << "0. Volver" << endl;
        int opcion = leerEnteroValidado("Seleccione una opcion: ", 0, 5);

        switch (opcion) {
            case 1: {
                int id = leerEnteroValidado("ID de la nueva tarea: ", 0);
                int tipoPrioridad = leerEnteroValidado("Prioridad (1=ALTA, 2=MEDIA, 3=BAJA): ", 1, 3);
                string prioridad = (tipoPrioridad == 1) ? "ALTA" : (tipoPrioridad == 2) ? "MEDIA" : "BAJA";
                int idResponsable = leerEnteroValidado("ID del usuario responsable: ", 0);
                string descripcion = leerTextoNoVacio("Descripcion: ");
                gestor.agregarTarea(id, prioridad, idResponsable, descripcion);
                break;
            }
            case 2: {
                int id = leerEnteroValidado("ID de la tarea a actualizar: ", 0);
                string descripcion = leerTextoNoVacio("Nueva descripcion: ");
                gestor.actualizarTarea(id, descripcion);
                break;
            }
            case 3: {
                int id = leerEnteroValidado("ID de la tarea a eliminar: ", 0);
                gestor.eliminarTarea(id);
                break;
            }
            case 4:
                gestor.listarTareasPendientes();
                break;
            case 5: {
                int id = leerEnteroValidado("ID a buscar: ", 0);
                gestor.buscarTarea(id);
                break;
            }
            case 0:
                volver = true;
                break;
        }
    }
}
