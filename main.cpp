// ==========================================================
// main.cpp - Sistema Gestor de Flujos de Trabajo
//
// Punto de entrada unico del programa. Su unica responsabilidad
// es arrancar la CLI: toda la logica real (login, menus por rol,
// gestion de usuarios/tareas, persistencia, historial y reportes)
// vive en CLI y en GestorSistema.
// ==========================================================

#include "CLI.h"

int main() {
    CLI cli;
    cli.ejecutar();
    return 0;
}