#include <iostream>
#include <string>
using namespace std;

#include "../Accion.h"
#include "../PilaHistorial.h"

// Convierte opcion numerica a TipoAccion.
TipoAccion opcionATipoAccion(int opcion) {
    switch (opcion) {
        case 1: return TipoAccion::AGREGAR;
        case 2: return TipoAccion::ACTUALIZAR;
        case 3: return TipoAccion::ELIMINAR;
    }
    return TipoAccion::AGREGAR;
}

// Convierte opcion numerica a TipoEntidad.
TipoEntidad opcionATipoEntidad(int opcion) {
    switch (opcion) {
        case 1: return TipoEntidad::USUARIO;
        case 2: return TipoEntidad::TAREA;
    }
    return TipoEntidad::USUARIO;
}

// Convierte TipoAccion a texto legible.
string tipoAccionTexto(TipoAccion tipo) {
    switch (tipo) {
        case TipoAccion::AGREGAR: return "AGREGAR";
        case TipoAccion::ACTUALIZAR: return "ACTUALIZAR";
        case TipoAccion::ELIMINAR: return "ELIMINAR";
    }
    return "DESCONOCIDO";
}

// Convierte TipoEntidad a texto legible.
string tipoEntidadTexto(TipoEntidad tipo) {
    switch (tipo) {
        case TipoEntidad::USUARIO: return "USUARIO";
        case TipoEntidad::TAREA: return "TAREA";
    }
    return "DESCONOCIDO";
}

// Solicita los datos de una Accion al usuario.
Accion solicitarAccion() {
    int opcionAccion, opcionEntidad, id, idUsuario;
    string estadoAnterior, estadoPosterior;

    cout << "Tipo de accion (1=AGREGAR, 2=ACTUALIZAR, 3=ELIMINAR): ";
    cin >> opcionAccion;
    cout << "Tipo de entidad (1=USUARIO, 2=TAREA): ";
    cin >> opcionEntidad;
    cout << "ID de entidad: ";
    cin >> id;
    cout << "ID del usuario que realizo la operacion: ";
    cin >> idUsuario;
    cout << "Estado anterior: ";
    cin.ignore();
    getline(cin, estadoAnterior);
    cout << "Estado posterior: ";
    getline(cin, estadoPosterior);

    return Accion(opcionATipoAccion(opcionAccion),
                  opcionATipoEntidad(opcionEntidad),
                  id,
                  idUsuario,
                  estadoAnterior,
                  estadoPosterior);
}

// Muestra los datos de una Accion.
void mostrarAccion(const Accion& accion) {
    cout << "Tipo: " << tipoAccionTexto(accion.getTipoAccion()) << endl;
    cout << "Entidad: " << tipoEntidadTexto(accion.getTipoEntidad()) << endl;
    cout << "ID: " << accion.getIdEntidad() << endl;
    cout << "ID usuario: " << accion.getIdUsuario() << endl;
    cout << "Estado anterior: " << accion.getEstadoAnterior() << endl;
    cout << "Estado posterior: " << accion.getEstadoPosterior() << endl;
}

// Muestra el estado actual de las pilas.
void mostrarEstado(const PilaHistorial& historial) {
    cout << "========================================" << endl;
    cout << "ESTADO ACTUAL DEL HISTORIAL" << endl;
    cout << "========================================" << endl;
    cout << "Acciones para deshacer: " << historial.tamanoDeshacer() << endl;
    cout << "Acciones para rehacer: " << historial.tamanoRehacer() << endl;
    cout << "Pila de deshacer vacia: " << (historial.estaVaciaDeshacer() ? "SI" : "NO") << endl;
    cout << "Pila de rehacer vacia: " << (historial.estaVaciaRehacer() ? "SI" : "NO") << endl;

    Accion cima;
    if (historial.cimaDeshacer(cima)) {
        cout << endl << "Cima de deshacer:" << endl;
        mostrarAccion(cima);
    }
    if (historial.cimaRehacer(cima)) {
        cout << endl << "Cima de rehacer:" << endl;
        mostrarAccion(cima);
    }
}

// Prueba 1: crear y consultar una Accion.
void probarAccion() {
    cout << "=== PRUEBA DE ACCION ===" << endl;
    Accion accion = solicitarAccion();
    cout << endl << "Datos almacenados en la accion:" << endl;
    mostrarAccion(accion);
}

// Prueba 2: registrar una accion.
void probarRegistrarAccion() {
    cout << "=== PRUEBA DE registrarAccion() ===" << endl;
    PilaHistorial historial;
    Accion accion = solicitarAccion();
    historial.registrarAccion(accion);

    cout << endl << "Despues de registrar:" << endl;
    cout << "Tamano deshacer: " << historial.tamanoDeshacer() << endl;
    cout << "Tamano rehacer: " << historial.tamanoRehacer() << endl;
    cout << "Deshacer vacia: " << (historial.estaVaciaDeshacer() ? "SI" : "NO") << endl;
    cout << "Rehacer vacia: " << (historial.estaVaciaRehacer() ? "SI" : "NO") << endl;

    Accion cima;
    if (historial.cimaDeshacer(cima)) {
        cout << "Cima de deshacer:" << endl;
        mostrarAccion(cima);
    }
}

// Prueba 3: deshacer una accion.
void probarDeshacer() {
    cout << "=== PRUEBA DE deshacer() ===" << endl;
    PilaHistorial historial;

    cout << "Registre acciones. Ingrese 0 para dejar de registrar." << endl;
    while (true) {
        cout << endl << "Datos de la accion:" << endl;
        Accion accion = solicitarAccion();
        historial.registrarAccion(accion);

        cout << "Registrar otra? (1=SI, 0=NO): ";
        int continuar;
        cin >> continuar;
        if (continuar == 0) break;
    }

    Accion salida;
    if (historial.deshacer(salida)) {
        cout << endl << "Deshacer exitoso. Accion devuelta:" << endl;
        mostrarAccion(salida);
    } else {
        cout << endl << "No se pudo deshacer: la pila de deshacer esta vacia." << endl;
    }

    cout << "Tamano deshacer: " << historial.tamanoDeshacer() << endl;
    cout << "Tamano rehacer: " << historial.tamanoRehacer() << endl;
}

// Prueba 4: rehacer una accion.
void probarRehacer() {
    cout << "=== PRUEBA DE rehacer() ===" << endl;
    PilaHistorial historial;

    cout << "Registre acciones. Ingrese 0 para dejar de registrar." << endl;
    while (true) {
        cout << endl << "Datos de la accion:" << endl;
        Accion accion = solicitarAccion();
        historial.registrarAccion(accion);

        cout << "Registrar otra? (1=SI, 0=NO): ";
        int continuar;
        cin >> continuar;
        if (continuar == 0) break;
    }

    // Deshacer una accion para que la pila de rehacer tenga contenido.
    Accion auxiliar;
    if (historial.deshacer(auxiliar)) {
        cout << endl << "Se deshizo una accion para poder probar rehacer()." << endl;
    }

    Accion salida;
    if (historial.rehacer(salida)) {
        cout << "Rehacer exitoso. Accion devuelta:" << endl;
        mostrarAccion(salida);
    } else {
        cout << "No se pudo rehacer: la pila de rehacer esta vacia." << endl;
    }

    cout << "Tamano deshacer: " << historial.tamanoDeshacer() << endl;
    cout << "Tamano rehacer: " << historial.tamanoRehacer() << endl;
}

// Prueba 5: consultar cima de deshacer sin eliminar.
void probarCimaDeshacer() {
    cout << "=== PRUEBA DE cimaDeshacer() ===" << endl;
    PilaHistorial historial;
    Accion accion = solicitarAccion();
    historial.registrarAccion(accion);

    size_t tamanoAntes = historial.tamanoDeshacer();
    Accion cima;
    if (historial.cimaDeshacer(cima)) {
        cout << endl << "Cima de deshacer:" << endl;
        mostrarAccion(cima);
    } else {
        cout << "La pila de deshacer esta vacia." << endl;
    }

    cout << "Tamano antes de consultar: " << tamanoAntes << endl;
    cout << "Tamano despues de consultar: " << historial.tamanoDeshacer() << endl;
    if (historial.tamanoDeshacer() == tamanoAntes) {
        cout << "La consulta NO elimino la accion." << endl;
    } else {
        cout << "ERROR: la consulta modifico la pila." << endl;
    }
}

// Prueba 6: consultar cima de rehacer sin eliminar.
void probarCimaRehacer() {
    cout << "=== PRUEBA DE cimaRehacer() ===" << endl;
    PilaHistorial historial;
    Accion accion = solicitarAccion();
    historial.registrarAccion(accion);

    // Deshacer para que la pila de rehacer tenga contenido.
    Accion auxiliar;
    historial.deshacer(auxiliar);

    size_t tamanoAntes = historial.tamanoRehacer();
    Accion cima;
    if (historial.cimaRehacer(cima)) {
        cout << endl << "Cima de rehacer:" << endl;
        mostrarAccion(cima);
    } else {
        cout << "La pila de rehacer esta vacia." << endl;
    }

    cout << "Tamano antes de consultar: " << tamanoAntes << endl;
    cout << "Tamano despues de consultar: " << historial.tamanoRehacer() << endl;
    if (historial.tamanoRehacer() == tamanoAntes) {
        cout << "La consulta NO elimino la accion." << endl;
    } else {
        cout << "ERROR: la consulta modifico la pila." << endl;
    }
}

// Prueba 7: limpiar ambas pilas.
void probarLimpiar() {
    cout << "=== PRUEBA DE limpiar() ===" << endl;
    PilaHistorial historial;

    // Registrar dos acciones.
    Accion accion1(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 1, 1, "ant1", "post1");
    Accion accion2(TipoAccion::ACTUALIZAR, TipoEntidad::TAREA, 2, 1, "ant2", "post2");
    historial.registrarAccion(accion1);
    historial.registrarAccion(accion2);

    // Deshacer una para que rehacer tambien tenga contenido.
    Accion auxiliar;
    historial.deshacer(auxiliar);

    cout << "Antes de limpiar:" << endl;
    cout << "Deshacer = " << historial.tamanoDeshacer() << endl;
    cout << "Rehacer = " << historial.tamanoRehacer() << endl;

    historial.limpiar();

    cout << endl << "Despues de limpiar:" << endl;
    cout << "Deshacer = " << historial.tamanoDeshacer() << endl;
    cout << "Rehacer = " << historial.tamanoRehacer() << endl;
    cout << "Deshacer vacia: " << (historial.estaVaciaDeshacer() ? "SI" : "NO") << endl;
    cout << "Rehacer vacia: " << (historial.estaVaciaRehacer() ? "SI" : "NO") << endl;
}

// Prueba 8: nueva accion despues de deshacer vacia rehacer.
void probarReglaNuevaAccion() {
    cout << "=== PRUEBA: NUEVA ACCION ELIMINA REHACER ===" << endl;

    PilaHistorial historial;

    Accion accionA(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 1, 1, "antA", "postA");
    Accion accionB(TipoAccion::ACTUALIZAR, TipoEntidad::TAREA, 2, 1, "antB", "postB");
    Accion accionC(TipoAccion::ELIMINAR, TipoEntidad::USUARIO, 3, 1, "antC", "postC");

    historial.registrarAccion(accionA);
    historial.registrarAccion(accionB);

    Accion salida;
    historial.deshacer(salida);  // Deshace B

    cout << "Despues de registrar A, B y deshacer B:" << endl;
    cout << "Deshacer = " << historial.tamanoDeshacer() << endl;
    cout << "Rehacer = " << historial.tamanoRehacer() << endl;

    historial.registrarAccion(accionC);  // Nueva accion C

    cout << endl << "Despues de registrar C:" << endl;
    cout << "Deshacer = " << historial.tamanoDeshacer() << endl;
    cout << "Rehacer = " << historial.tamanoRehacer() << endl;

    if (historial.tamanoDeshacer() == 2 && historial.tamanoRehacer() == 0) {
        cout << "REGLA CUMPLIDA: la nueva accion vacio la pila de rehacer." << endl;
    } else {
        cout << "ERROR: la regla no se cumplio." << endl;
    }
}

// Prueba 9: mostrar historial.
void probarMostrarHistorial() {
    cout << "=== PRUEBA DE mostrarHistorial() ===" << endl;

    PilaHistorial historial;

    Accion accion1(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 10, 1, "ID=10|Nombre=Carlos", "ID=10|Nombre=Carlos");
    Accion accion2(TipoAccion::ACTUALIZAR, TipoEntidad::USUARIO, 10, 1, "ID=10|Nombre=Carlos", "ID=10|Nombre=Pedro");
    Accion accion3(TipoAccion::ELIMINAR, TipoEntidad::TAREA, 25, 1, "ID=25|Titulo=Tarea", "ID=25|Titulo=Tarea");

    historial.registrarAccion(accion1);
    historial.registrarAccion(accion2);
    historial.registrarAccion(accion3);

    // Deshacer una para que rehacer tenga contenido.
    Accion auxiliar;
    historial.deshacer(auxiliar);

    size_t deshacerAntes = historial.tamanoDeshacer();
    size_t rehacerAntes = historial.tamanoRehacer();

    cout << historial.mostrarHistorial();

    cout << "Tamano deshacer antes: " << deshacerAntes << endl;
    cout << "Tamano deshacer despues: " << historial.tamanoDeshacer() << endl;
    cout << "Tamano rehacer antes: " << rehacerAntes << endl;
    cout << "Tamano rehacer despues: " << historial.tamanoRehacer() << endl;

    if (historial.tamanoDeshacer() == deshacerAntes && historial.tamanoRehacer() == rehacerAntes) {
        cout << "mostrarHistorial() NO modifico las pilas." << endl;
    } else {
        cout << "ERROR: mostrarHistorial() modifico las pilas." << endl;
    }
}

// Prueba 10: ejecutar todas las pruebas automaticas.
void ejecutarTodasLasPruebas() {
    cout << "=== EJECUTANDO TODAS LAS PRUEBAS ===" << endl << endl;

    // 1. Estado inicial.
    PilaHistorial historial;
    bool ok = historial.estaVaciaDeshacer() && historial.estaVaciaRehacer()
              && historial.tamanoDeshacer() == 0 && historial.tamanoRehacer() == 0;
    cout << (ok ? "[OK] Estado inicial" : "[ERROR] Estado inicial") << endl;

    // 2. Registrar accion.
    Accion accionA(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 1, 1, "antA", "postA");
    historial.registrarAccion(accionA);
    ok = historial.tamanoDeshacer() == 1 && historial.tamanoRehacer() == 0;
    cout << (ok ? "[OK] Registrar accion" : "[ERROR] Registrar accion") << endl;

    // 3. Comportamiento LIFO.
    PilaHistorial historialLifo;
    Accion lifoA(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 1, 1, "a", "a");
    Accion lifoB(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 2, 1, "b", "b");
    Accion lifoC(TipoAccion::AGREGAR, TipoEntidad::USUARIO, 3, 1, "c", "c");
    historialLifo.registrarAccion(lifoA);
    historialLifo.registrarAccion(lifoB);
    historialLifo.registrarAccion(lifoC);

    Accion salida;
    bool lifoOk = true;
    int idsEsperados[3] = {3, 2, 1};
    for (int i = 0; i < 3; i++) {
        if (!historialLifo.deshacer(salida) || salida.getIdEntidad() != idsEsperados[i]) {
            lifoOk = false;
        }
    }
    cout << (lifoOk ? "[OK] Comportamiento LIFO" : "[ERROR] Comportamiento LIFO") << endl;

    // 4. Deshacer.
    PilaHistorial historialDeshacer;
    historialDeshacer.registrarAccion(accionA);
    ok = historialDeshacer.deshacer(salida);
    ok = ok && historialDeshacer.tamanoDeshacer() == 0 && historialDeshacer.tamanoRehacer() == 1;
    cout << (ok ? "[OK] Deshacer" : "[ERROR] Deshacer") << endl;

    // 5. Rehacer.
    ok = historialDeshacer.rehacer(salida);
    ok = ok && historialDeshacer.tamanoDeshacer() == 1 && historialDeshacer.tamanoRehacer() == 0;
    cout << (ok ? "[OK] Rehacer" : "[ERROR] Rehacer") << endl;

    // 6. Cima de deshacer.
    PilaHistorial historialCima;
    historialCima.registrarAccion(accionA);
    size_t tamanoAntes = historialCima.tamanoDeshacer();
    ok = historialCima.cimaDeshacer(salida);
    ok = ok && historialCima.tamanoDeshacer() == tamanoAntes;
    cout << (ok ? "[OK] Cima de deshacer" : "[ERROR] Cima de deshacer") << endl;

    // 7. Cima de rehacer.
    PilaHistorial historialCimaR;
    historialCimaR.registrarAccion(accionA);
    historialCimaR.deshacer(salida);
    tamanoAntes = historialCimaR.tamanoRehacer();
    ok = historialCimaR.cimaRehacer(salida);
    ok = ok && historialCimaR.tamanoRehacer() == tamanoAntes;
    cout << (ok ? "[OK] Cima de rehacer" : "[ERROR] Cima de rehacer") << endl;

    // 8. Nueva accion elimina rehacer.
    PilaHistorial historialRegla;
    historialRegla.registrarAccion(accionA);
    Accion accionB(TipoAccion::ACTUALIZAR, TipoEntidad::TAREA, 2, 1, "antB", "postB");
    historialRegla.registrarAccion(accionB);
    historialRegla.deshacer(salida);  // Deshace B
    Accion accionC(TipoAccion::ELIMINAR, TipoEntidad::USUARIO, 3, 1, "antC", "postC");
    historialRegla.registrarAccion(accionC);  // Nueva accion C
    ok = historialRegla.tamanoDeshacer() == 2 && historialRegla.tamanoRehacer() == 0;
    cout << (ok ? "[OK] Nueva accion elimina rehacer" : "[ERROR] Nueva accion elimina rehacer") << endl;

    // 9. Limpiar.
    historialRegla.limpiar();
    ok = historialRegla.tamanoDeshacer() == 0 && historialRegla.tamanoRehacer() == 0
         && historialRegla.estaVaciaDeshacer() && historialRegla.estaVaciaRehacer();
    cout << (ok ? "[OK] Limpiar" : "[ERROR] Limpiar") << endl;

    // 10. Deshacer con pila vacia.
    PilaHistorial historialVacia;
    ok = !historialVacia.deshacer(salida);
    cout << (ok ? "[OK] Deshacer con pila vacia" : "[ERROR] Deshacer con pila vacia") << endl;

    // 11. Rehacer con pila vacia.
    ok = !historialVacia.rehacer(salida);
    cout << (ok ? "[OK] Rehacer con pila vacia" : "[ERROR] Rehacer con pila vacia") << endl;

    // 12. Conservacion de estados.
    PilaHistorial historialEstados;
    Accion accionEstados(TipoAccion::ACTUALIZAR, TipoEntidad::USUARIO, 10, 1,
                         "estado_anterior", "estado_posterior");
    historialEstados.registrarAccion(accionEstados);
    historialEstados.deshacer(salida);
    ok = salida.getEstadoAnterior() == "estado_anterior"
         && salida.getEstadoPosterior() == "estado_posterior";
    historialEstados.rehacer(salida);
    ok = ok && salida.getEstadoAnterior() == "estado_anterior"
         && salida.getEstadoPosterior() == "estado_posterior"
         && salida.getIdUsuario() == 1;
    cout << (ok ? "[OK] Conservacion de estados" : "[ERROR] Conservacion de estados") << endl;

    // 13. Mostrar historial.
    PilaHistorial historialMostrar;
    historialMostrar.registrarAccion(accionA);
    historialMostrar.registrarAccion(accionB);
    historialMostrar.deshacer(salida);
    size_t deshacerAntes = historialMostrar.tamanoDeshacer();
    size_t rehacerAntes = historialMostrar.tamanoRehacer();
    cout << historialMostrar.mostrarHistorial();
    ok = historialMostrar.tamanoDeshacer() == deshacerAntes
         && historialMostrar.tamanoRehacer() == rehacerAntes;
    cout << (ok ? "[OK] Mostrar historial" : "[ERROR] Mostrar historial") << endl;
}

// Demostracion interactiva del historial.
void demostracionInteractiva() {
    PilaHistorial historial;
    int opcion;

    do {
        cout << "========================================" << endl;
        cout << "       DEMOSTRACION DEL HISTORIAL" << endl;
        cout << "========================================" << endl;
        cout << endl;
        cout << "1. Registrar una accion" << endl;
        cout << "2. Deshacer una accion" << endl;
        cout << "3. Rehacer una accion" << endl;
        cout << "4. Mostrar historial" << endl;
        cout << "5. Mostrar estado del historial" << endl;
        cout << "6. Limpiar historial" << endl;
        cout << "0. Salir de la demostracion" << endl;
        cout << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << endl << "Datos de la nueva accion:" << endl;
                Accion accion = solicitarAccion();
                historial.registrarAccion(accion);
                cout << endl << "Accion registrada correctamente." << endl;
                cout << "Deshacer: " << historial.tamanoDeshacer() << endl;
                cout << "Rehacer: " << historial.tamanoRehacer() << endl;
                break;
            }
            case 2: {
                if (historial.estaVaciaDeshacer()) {
                    cout << endl << "No hay acciones disponibles para deshacer." << endl;
                    break;
                }
                cout << endl << "Desea deshacer la ultima accion?" << endl;
                cout << "1. Si" << endl;
                cout << "0. No" << endl;
                cout << "Seleccione: ";
                int confirmar;
                cin >> confirmar;
                if (confirmar == 1) {
                    Accion accion;
                    if (historial.deshacer(accion)) {
                        cout << endl << "Accion deshecha:" << endl;
                        mostrarAccion(accion);
                        cout << "Deshacer: " << historial.tamanoDeshacer() << endl;
                        cout << "Rehacer: " << historial.tamanoRehacer() << endl;
                    }
                }
                break;
            }
            case 3: {
                if (historial.estaVaciaRehacer()) {
                    cout << endl << "No hay acciones disponibles para rehacer." << endl;
                    break;
                }
                cout << endl << "Desea rehacer la ultima accion?" << endl;
                cout << "1. Si" << endl;
                cout << "0. No" << endl;
                cout << "Seleccione: ";
                int confirmar;
                cin >> confirmar;
                if (confirmar == 1) {
                    Accion accion;
                    if (historial.rehacer(accion)) {
                        cout << endl << "Accion rehecha:" << endl;
                        mostrarAccion(accion);
                        cout << "Deshacer: " << historial.tamanoDeshacer() << endl;
                        cout << "Rehacer: " << historial.tamanoRehacer() << endl;
                    }
                }
                break;
            }
            case 4: {
                cout << endl << "Desea mostrar el historial?" << endl;
                cout << "1. Si" << endl;
                cout << "0. No" << endl;
                cout << "Seleccione: ";
                int confirmar;
                cin >> confirmar;
                if (confirmar == 1) {
                    size_t deshacerAntes = historial.tamanoDeshacer();
                    size_t rehacerAntes = historial.tamanoRehacer();
                    cout << historial.mostrarHistorial();
                    cout << "Deshacer antes: " << deshacerAntes
                         << " | Deshacer despues: " << historial.tamanoDeshacer() << endl;
                    cout << "Rehacer antes: " << rehacerAntes
                         << " | Rehacer despues: " << historial.tamanoRehacer() << endl;
                    if (historial.tamanoDeshacer() == deshacerAntes
                        && historial.tamanoRehacer() == rehacerAntes) {
                        cout << "mostrarHistorial() no modifico las pilas." << endl;
                    }
                }
                break;
            }
            case 5:
                cout << endl;
                mostrarEstado(historial);
                break;
            case 6: {
                cout << endl << "Desea limpiar todo el historial?" << endl;
                cout << "1. Si" << endl;
                cout << "0. No" << endl;
                cout << "Seleccione: ";
                int confirmar;
                cin >> confirmar;
                if (confirmar == 1) {
                    historial.limpiar();
                    cout << endl << "Deshacer = " << historial.tamanoDeshacer() << endl;
                    cout << "Rehacer = " << historial.tamanoRehacer() << endl;
                    cout << "Deshacer vacia: " << (historial.estaVaciaDeshacer() ? "SI" : "NO") << endl;
                    cout << "Rehacer vacia: " << (historial.estaVaciaRehacer() ? "SI" : "NO") << endl;
                }
                break;
            }
            case 0:
                cout << "Saliendo de la demostracion..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
        cout << endl;
    } while (opcion != 0);
}

// Menu principal.
int main() {
    int opcion;

    do {
        cout << "========================================" << endl;
        cout << "       PRUEBAS UNITARIAS - HISTORIAL" << endl;
        cout << "========================================" << endl;
        cout << endl;
        cout << "1. Probar Accion" << endl;
        cout << "2. Probar registrarAccion()" << endl;
        cout << "3. Probar deshacer()" << endl;
        cout << "4. Probar rehacer()" << endl;
        cout << "5. Probar cimaDeshacer()" << endl;
        cout << "6. Probar cimaRehacer()" << endl;
        cout << "7. Probar limpiar()" << endl;
        cout << "8. Probar nueva accion despues de deshacer" << endl;
        cout << "9. Probar mostrarHistorial()" << endl;
        cout << "10. Ejecutar todas las pruebas automaticas" << endl;
        cout << "11. Demostracion interactiva del historial" << endl;
        cout << "0. Salir" << endl;
        cout << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: probarAccion(); break;
            case 2: probarRegistrarAccion(); break;
            case 3: probarDeshacer(); break;
            case 4: probarRehacer(); break;
            case 5: probarCimaDeshacer(); break;
            case 6: probarCimaRehacer(); break;
            case 7: probarLimpiar(); break;
            case 8: probarReglaNuevaAccion(); break;
            case 9: probarMostrarHistorial(); break;
            case 10: ejecutarTodasLasPruebas(); break;
            case 11: demostracionInteractiva(); break;
            case 0: cout << "Saliendo..." << endl; break;
            default: cout << "Opcion no valida." << endl; break;
        }
        cout << endl;
    } while (opcion != 0);

    return 0;
}