#include <iostream>
#include <fstream>
#include <string>

#include "GestorArchivosCSV.h"

using namespace std;

int main() {
    GestorArchivosCSV gestor;

    // Simulacion: cinco eventos de auditoria con datos distintos.
    // 1) Cambio de estado de una tarea.
    gestor.registrarAuditoria("1", "cambioEstado", "10");
    // 2) Deshacer de la pila de historial.
    gestor.registrarAuditoria("1", "deshacer", "10");
    // 3) Rehacer de la pila de historial.
    gestor.registrarAuditoria("1", "rehacer", "10");
    // 4) Escalado por SLA (primer evento).
    gestor.registrarAuditoria("2", "escaladoSLA", "11");
    // 5) Escalado por SLA (segundo evento).
    gestor.registrarAuditoria("3", "escaladoSLA", "12");

    cout << "=== Contenido completo de auditoria_log.csv ===\n";

    ifstream bitacora("auditoria_log.csv");
    if (!bitacora.is_open()) {
        cout << "Error: no se pudo abrir 'auditoria_log.csv' para lectura.\n";
        return 1;
    }

    string linea;
    int numeroLinea = 0;
    while (getline(bitacora, linea)) {
        numeroLinea++;
        cout << numeroLinea << ": " << linea << "\n";
    }
    bitacora.close();

    return 0;
}