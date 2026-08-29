#include "GestorArchivosCSV.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

// Constructor por defecto: no hay recursos que inicializar por ahora.
GestorArchivosCSV::GestorArchivosCSV() {
}

// Destructor por defecto: no hay recursos que liberar por ahora.
GestorArchivosCSV::~GestorArchivosCSV() {
}

/**
 * Separa una linea CSV por comas respetando los campos entre comillas dobles.
 * Recorre la linea caracter por caracter con una bandera "dentroComillas":
 *  - las comas solo separan campos cuando estan FUERA de comillas;
 *  - dentro de comillas, la secuencia "" es una comilla literal;
 *  - si la linea termina con comillas sin cerrar, o aparece una comilla a
 *    mitad de un campo sin comillas, la linea esta mal formada y se devuelve
 *    un vector vacio (no se lanza ninguna excepcion).
 */
vector<string> GestorArchivosCSV::parsearLinea(const string& linea) const {
    vector<string> campos;
    string actual;
    bool dentroComillas = false;

    // Los archivos estilo Windows terminan cada linea con \r\n; el \r sobra.
    string contenido = linea;
    if (!contenido.empty() && contenido[contenido.size() - 1] == '\r') {
        contenido.erase(contenido.size() - 1);
    }

    size_t i = 0;
    while (i < contenido.size()) {
        char c = contenido[i];

        if (c == '"') {
            // Doble comilla dentro de un campo entrecomillado: comilla literal.
            if (dentroComillas && i + 1 < contenido.size() && contenido[i + 1] == '"') {
                actual += '"';
                i += 2;
                continue;
            }
            if (dentroComillas) {
                dentroComillas = false;          // Cierre del campo de texto libre.
            } else if (actual.empty()) {
                dentroComillas = true;           // Apertura de un campo de texto libre.
            } else {
                return vector<string>();         // Comilla a mitad de campo: mal formada.
            }
            i++;
            continue;
        }

        // La coma separa campos solo cuando esta fuera de comillas.
        if (c == ',' && !dentroComillas) {
            campos.push_back(actual);
            actual.clear();
            i++;
            continue;
        }

        actual += c;
        i++;
    }

    // Si la linea termina con comillas abiertas sin cerrar, esta mal formada.
    if (dentroComillas) {
        return vector<string>();
    }

    // Se agrega el ultimo campo (si la linea termina en coma, este queda vacio).
    campos.push_back(actual);

    return campos;
}

/**
 * Envuelve un campo entre comillas dobles para escribirlo de vuelta al CSV,
 * duplicando las comillas internas (regla CSV "") cuando el texto las tenga.
 * Asi, cualquier campo con comas internas o comillas se lee identico al
 * guardarlo con parsearLinea.
 */
string GestorArchivosCSV::escaparCampo(const string& campo) const {
    string resultado = "\"";
    for (size_t i = 0; i < campo.size(); i++) {
        if (campo[i] == '"') {
            resultado += "\"\"";                 // Comilla interna duplicada.
        } else {
            resultado += campo[i];
        }
    }
    resultado += '"';
    return resultado;
}

/**
 * Indica si existe un archivo en la ruta indicada. Se usa std::ifstream sin
 * activar la mascara de excepciones, por lo que nunca lanza: si el archivo
 * no existe (o no puede abrirse) devuelve false.
 */
bool GestorArchivosCSV::archivoExiste(const string& ruta) const {
    ifstream archivo(ruta);
    return archivo.is_open();
}

/**
 * Carga los registros de usuarios.csv y los devuelve como vector de filas,
 * ademas de imprimirlos en consola (comprobacion).
 * Formato esperado: id,nombre,rol,password (rol "Administrador" o "Usuario
 * Normal", la conversion canonica de rolATexto/textoARol). Si el archivo
 * no existe se crea vacio; las lineas malformadas o con numero de campos distinto
 * del esperado se omiten mostrando el numero de linea en consola.
 */
vector<vector<string>> GestorArchivosCSV::cargarUsuarios(const string& ruta) {
    vector<vector<string>> resultado;   // Acumula cada registro valido leido.

    // Si el archivo no existe, se crea vacio para continuar sin error.
    if (!archivoExiste(ruta)) {
        ofstream nuevo(ruta);
        nuevo.close();
    }

    ifstream archivo(ruta);
    string linea;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;
        vector<string> campos = parsearLinea(linea);

        // La primera linea puede traer los nombres de columna; se omite.
        if (numeroLinea == 1 && campos.size() == 4 &&
            campos[0] == "id" && campos[1] == "nombre" &&
            campos[2] == "rol" && campos[3] == "password") {
            continue;
        }

        if (campos.empty()) {
            cout << "Aviso: usuario.csv, linea " << numeroLinea
                 << " malformada; se omite." << endl;
            continue;
        }
        if (campos.size() != 4) {
            cout << "Aviso: usuario.csv, linea " << numeroLinea << " tiene "
                 << campos.size() << " campos (esperados 4); se omite." << endl;
            continue;
        }

        // Placeholder: aqui luego se creara un objeto Usuario del companero.
        cout << "Usuario: id=" << campos[0] << ", nombre=" << campos[1]
             << ", rol=" << campos[2] << ", password=" << campos[3] << endl;

        resultado.push_back(campos);   // Registro valido: se acumula para devolverlo.
    }

    return resultado;
}

/**
 * Carga los registros de tareas.csv (id,prioridad,idUsuarioResponsable,
 * descripcion,estado) y los devuelve como vector de filas, ademas de
 * imprimirlos en consola (comprobacion). Si el archivo no existe se crea
 * vacio; las lineas malformas o de conteo incorrecto se omiten.
 */
vector<vector<string>> GestorArchivosCSV::cargarTareas(const string& ruta) {
    vector<vector<string>> resultado;   // Acumula cada registro valido leido.

    // Si el archivo no existe, se crea vacio para continuar sin error.
    if (!archivoExiste(ruta)) {
        ofstream nuevo(ruta);
        nuevo.close();
    }

    ifstream archivo(ruta);
    string linea;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;
        vector<string> campos = parsearLinea(linea);

        // La primera fila suele ser el encabezado; se omite.
        if (numeroLinea == 1 && campos.size() == 5 &&
            campos[0] == "id" && campos[1] == "prioridad" &&
            campos[2] == "idUsuarioResponsable" && campos[3] == "descripcion" &&
            campos[4] == "estado") {
            continue;
        }

        if (campos.empty()) {
            cout << "Aviso: tarea.csv, linea " << numeroLinea
                 << " malformada; se omite." << endl;
            continue;
        }
        if (campos.size() != 5) {
            cout << "Aviso: tarea.csv, linea " << numeroLinea << " tiene "
                 << campos.size() << " campos (esperados 5); se omite." << endl;
            continue;
        }

        // Placeholder: aqui se construira una Tarea con los datos reales.
        cout << "Tarea: id=" << campos[0] << ", prioridad=" << campos[1]
             << ", responsable=" << campos[2] << ", descripcion=" << campos[3]
             << ", estado=" << campos[4] << endl;

        resultado.push_back(campos);   // Registro valido: se acumula para devolverlo.
    }

    return resultado;
}

/**
 * Carga los registros de subtareas.csv (id,idTareaPadre,descripcion,estado) y
 * los devuelve como vector de filas, ademas de imprimirlos en consola
 * (comprobacion). Si el archivo no existe se crea vacio; las lineas malformas
 * o de conteo incorrecto se omiten con su numero.
 */
vector<vector<string>> GestorArchivosCSV::cargarSubtareas(const string& ruta) {
    vector<vector<string>> resultado;   // Acumula cada registro valido leido.

    // Si el archivo no existe, se crea vacio para continuar sin error.
    if (!archivoExiste(ruta)) {
        ofstream nuevo(ruta);
        nuevo.close();
    }

    ifstream archivo(ruta);
    string linea;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;
        vector<string> campos = parsearLinea(linea);

        // La primera fila puede ser el encabezado; se omite.
        if (numeroLinea == 1 && campos.size() == 4 &&
            campos[0] == "id" && campos[1] == "idTareaPadre" &&
            campos[2] == "descripcion" && campos[3] == "estado") {
            continue;
        }

        if (campos.empty()) {
            cout << "Aviso: subtarea.csv, linea " << numeroLinea
                 << " malformada; se omite." << endl;
            continue;
        }
        if (campos.size() != 4) {
            cout << "Aviso: subtarea.csv, linea " << numeroLinea << " tiene "
                 << campos.size() << " campos (esperados 4); se omite." << endl;
            continue;
        }

        // Placeholder: aqui se insertara la subtarea en ArbolSubtareas.
        cout << "Subtarea: id=" << campos[0] << ", tareaPadre=" << campos[1]
             << ", descripcion=" << campos[2] << ", estado=" << campos[3] << endl;

        resultado.push_back(campos);   // Registro valido: se acumula para devolverlo.
    }

    return resultado;
}

/**
 * Guarda los registros de usuarios en usuarios.csv, sobrescribiendo el archivo.
 * Cada fila de "datos" es un registro (id,nombre,rol,password). Todos los campos
 * se envuelven con escaparCampo para que el round-trip se recupere identico con
 * cargarUsuarios/parsearLinea. Si no se puede abrir el archivo, muestra error.
 */
void GestorArchivosCSV::guardarUsuarios(const string& ruta,
                                        const vector<vector<string>>& datos) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir '" << ruta
             << "' para escribir los usuarios." << endl;
        return;
    }

    for (const vector<string>& fila : datos) {
        // Una fila incompleta no debe dejar de escribir lo demas.
        if (fila.size() < 4) {
            cout << "Aviso: registro de usuario incompleto; se omite." << endl;
            continue;
        }
        archivo << escaparCampo(fila[0]) << ',' << escaparCampo(fila[1]) << ','
                << escaparCampo(fila[2]) << ',' << escaparCampo(fila[3]) << '\n';
    }
}

/**
 * Guarda los registros de tareas en tareas.csv, sobrescribiendo el archivo.
 * Cada fila es id,prioridad,idUsuarioResponsable,descripcion,estado. Todos los
 * campos se escapan con escaparCampo para que cargarTareas los lea identicos.
 * Si no se puede abrir el archivo, muestra un mensaje de error y no crashea.
 */
void GestorArchivosCSV::guardarTareas(const string& ruta,
                                      const vector<vector<string>>& datos) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir '" << ruta
             << "' para escribir las tareas." << endl;
        return;
    }

    for (const vector<string>& fila : datos) {
        if (fila.size() < 5) {
            cout << "Aviso: registro de tarea incompleto; se omite." << endl;
            continue;
        }
        archivo << escaparCampo(fila[0]) << ',' << escaparCampo(fila[1]) << ','
                << escaparCampo(fila[2]) << ',' << escaparCampo(fila[3]) << ','
                << escaparCampo(fila[4]) << '\n';
    }
}

/**
 * Guarda los registros de subtareas en subtareas.csv, sobrescribiendo el archivo.
 * Cada fila es id,idTareaPadre,descripcion,estado. Los campos se escapan con
 * escaparCampo para que cargarSubtareas recupere el texto original. Si el archivo
 * no se puede abrir, se muestra un error claro sin que el programa crashee.
 */
void GestorArchivosCSV::guardarSubtareas(const string& ruta,
                                         const vector<vector<string>>& datos) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir '" << ruta
             << "' para escribir las subtareas." << endl;
        return;
    }

    for (const vector<string>& fila : datos) {
        if (fila.size() < 4) {
            cout << "Aviso: registro de subtarea incompleto; se omite." << endl;
            continue;
        }
        archivo << escaparCampo(fila[0]) << ',' << escaparCampo(fila[1]) << ','
                << escaparCampo(fila[2]) << ',' << escaparCampo(fila[3]) << '\n';
    }
}

/**
 * Obtiene la fecha y hora actual formateada como "YYYY-MM-DD HH:MM:SS".
 * Usa el reloj del sistema (chrono) y strftime (ctime), ambas de la biblioteca
 * estandar; no requiere librerias externas.
 */
string GestorArchivosCSV::obtenerFechaHoraActual() const {
    time_t ahora = chrono::system_clock::to_time_t(chrono::system_clock::now());
    char buffer[20];
    // localtime convierte el tiempo a la hora local del sistema.
    if (strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&ahora)) == 0) {
        return "0000-00-00 00:00:00";   // Fallback ante cualquier error de formato.
    }
    return string(buffer);
}

/**
 * Registra una accion de auditoria agregando una linea al final de
 * auditoria_log.csv con el formato exacto:
 *   [Fecha y Hora] | [idUsuario] | [accion] | [idTarea]
 * El archivo se abre SIEMPRE en modo append (la bitacora es inmutable: solo se
 * agregan lineas, nunca se truncan). Si no existe, se crea en la primera llamada.
 * Si el archivo no puede abrirse, muestra un error claro por consola y continua
 * la ejecucion sin crashear.
 */
void GestorArchivosCSV::registrarAuditoria(const string& idUsuario,
                                           const string& accion,
                                           const string& idTarea) {
    // Modo append: crea el archivo si no existe y nunca sobrescribe lineas previas.
    ofstream bitacora("auditoria_log.csv", ios::app);
    if (!bitacora.is_open()) {
        cout << "Error: no se pudo abrir 'auditoria_log.csv' para registrar la auditoria."
             << endl;
        return;
    }

    bitacora << obtenerFechaHoraActual() << " | " << idUsuario << " | " << accion
             << " | " << idTarea << '\n';
    bitacora.close();
}