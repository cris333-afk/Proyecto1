#ifndef GESTOR_ARCHIVOS_CSV_H
#define GESTOR_ARCHIVOS_CSV_H

#include <string>
#include <vector>

// GestorArchivosCSV: unica clase del sistema responsable de leer y escribir
// los archivos .csv (usuarios, tareas, subtareas) y la bitacora de auditoria.
// Ningun otro modulo del proyecto debe tocar archivos directamente.
class GestorArchivosCSV {
public:
    GestorArchivosCSV();
    ~GestorArchivosCSV();

    // Carga: devuelven cada registro valido leido (fila = campos en el
    // orden del formato). Crean el archivo vacio si no existe. Las lineas
    // mal formadas o con numero de campos incorrecto se omiten con un
    // aviso, sin detener la carga del resto.
    std::vector<std::vector<std::string>> cargarUsuarios(const std::string& ruta);
    std::vector<std::vector<std::string>> cargarTareas(const std::string& ruta);
    std::vector<std::vector<std::string>> cargarSubtareas(const std::string& ruta);

    // Guardado: sobrescribe el archivo completo con los datos recibidos.
    // Si una fila viene incompleta, se omite con un aviso.
    void guardarUsuarios(const std::string& ruta, const std::vector<std::vector<std::string>>& datos);
    void guardarTareas(const std::string& ruta, const std::vector<std::vector<std::string>>& datos);
    void guardarSubtareas(const std::string& ruta, const std::vector<std::vector<std::string>>& datos);

    // Auditoria: agrega una linea a auditoria_log.csv, siempre en modo
    // append (bitacora inmutable). Formato:
    //   [Fecha y Hora] | [idUsuario] | [accion] | [idTarea]
    void registrarAuditoria(const std::string& idUsuario, const std::string& accion, const std::string& idTarea);

private:
    // Parsea una linea CSV respetando campos entre comillas dobles.
    // Devuelve un vector vacio si la linea esta mal formada.
    std::vector<std::string> parsearLinea(const std::string& linea) const;

    // Envuelve un campo entre comillas dobles, duplicando comillas internas.
    std::string escaparCampo(const std::string& campo) const;

    // true si el archivo puede abrirse para lectura.
    bool archivoExiste(const std::string& ruta) const;

    // Fecha/hora actual formateada como "YYYY-MM-DD HH:MM:SS".
    std::string obtenerFechaHoraActual() const;
};

#endif  // GESTOR_ARCHIVOS_CSV_H