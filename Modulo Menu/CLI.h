// ==========================================================
// Modulo Menu - Sistema Gestor de Flujos de Trabajo
// Responsable: Angel (Sistema e Integracion)
// Colaborador: Cesarr
//
// CLI: la clase de menu propiamente. Responsable de la interfaz de
// consola, el login y disparar la validacion estricta de entradas
// (Aclaratorias #1 y #2). No contiene logica de negocio: todo se
// delega a GestorSistema.
// ==========================================================

#ifndef CLI_H
#define CLI_H

#include "GestorSistema.h"

class CLI {
private:
    GestorSistema gestor;

    void mostrarBienvenida() const;
    bool iniciarSesion(); // true si el login fue exitoso

    void menuAdministrador();
    void menuUsuarioNormal();

    void submenuGestionUsuarios();
    void submenuGestionTareas();

public:
    CLI();
    void ejecutar(); // bucle principal del programa
};

#endif // CLI_H
