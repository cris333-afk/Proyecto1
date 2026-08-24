#ifndef GESTOR_ARCHIVOS_CSV_H
#define GESTOR_ARCHIVOS_CSV_H

#include <string>
#include <vector>

/**
 * Clase GestorArchivosCSV: unica clase del sistema responsable de leer y
 * escribir los archivos .csv de usuarios, tareas y subtareas.
 *
 * Segun el UML del proyecto, este modulo es UNA sola clase: las funciones de
 * apoyo del parseo viven aqui como metodos privados, no en archivos aparte.
 * Los metodos publicos de lectura/escritura por entidad se agregaran luego
 * dentro de esta misma clase, sin crear clases auxiliares.
 *
 * Regla de robustez: ningun metodo lanza excepciones a proposito; si una linea
 * viene mal formada se devuelve un vector vacio para que el llamador muestre
 * un mensaje amigable y vuelva a intentarlo.
 */
class GestorArchivosCSV {
public:
    // Constructor por defecto (sin recursos que inicializar por ahora).
    GestorArchivosCSV();

    // Destructor por defecto (la clase no administra memoria dinamica aun).
    ~GestorArchivosCSV();

    // Carga los registros de usuarios.csv y los imprime en consola (placeholder).
    void cargarUsuarios(const std::string& ruta);

    // Carga los registros de tareas.csv y los imprime en consola (placeholder).
    void cargarTareas(const std::string& ruta);

    // Carga los registros de subtareas.csv y los imprime en consola (placeholder).
    void cargarSubtareas(const std::string& ruta);

    // Guarda los registros de usuarios en usuarios.csv, sobrescribiendo el archivo.
    void guardarUsuarios(const std::string& ruta,
                         const std::vector<std::vector<std::string>>& datos);

    // Guarda los registros de tareas en tareas.csv, sobrescribiendo el archivo.
    void guardarTareas(const std::string& ruta,
                       const std::vector<std::vector<std::string>>& datos);

    // Guarda los registros de subtareas en subtareas.csv, sobrescribiendo el archivo.
    void guardarSubtareas(const std::string& ruta,
                          const std::vector<std::vector<std::string>>& datos);

    // Registra una accion de auditoria agregando una linea (siempre en modo
    // append) a auditoria_log.csv con el formato:
    // [Fecha y Hora] | [idUsuario] | [accion] | [idTarea].
    void registrarAuditoria(const std::string& idUsuario,
                            const std::string& accion,
                            const std::string& idTarea);

private:
    // Separa una linea CSV por comas respetando los campos entre comillas dobles
    // (los de texto libre pueden contener comas internas que no deben partirse).
    // Devuelve un vector vacio si la linea esta mal formada: comillas sin cerrar
    // o una comilla a mitad de un campo que no esta entrecomillado.
    std::vector<std::string> parsearLinea(const std::string& linea) const;

    // Envuelve un campo entre comillas dobles para escribirlo de vuelta al CSV.
    // Las comillas internas se duplican (regla CSV "") para que parsearLinea
    // recupere el texto original sin ambiguedad.
    std::string escaparCampo(const std::string& campo) const;

    // Indica si existe un archivo en la ruta indicada. No lanza excepciones:
    // si el archivo no se puede abrir, simplemente devuelve false.
    bool archivoExiste(const std::string& ruta) const;

    // Devuelve la fecha y hora actual con formato "YYYY-MM-DD HH:MM:SS".
    std::string obtenerFechaHoraActual() const;
};

#endif // GESTOR_ARCHIVOS_CSV_H