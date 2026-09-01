#ifndef TAREA_H
#define TAREA_H

#include <string>
#include <vector>

using namespace std;

class Tarea {
private:
    int id;                    // Identificador de la tarea
    string prioridad;          // Prioridad: ALTA, MEDIA, BAJA
    string estado;             // Estado de la tarea
    vector<Tarea*> subtareas;  // Lista de subtareas
    std::string descripcion;   // Descripción de la tarea
    int idUsuarioResponsable;  // ID del usuario responsable
    int ciclosEspera;          // Contador de ciclos de espera para control de SLA

public:
    // Constructor
    Tarea(int id, const string& prioridad, const string& estado,
          const string& descripcion = "", int idUsuarioResponsable = -1, int ciclosEspera = 0);

    // Destructor
    ~Tarea();

    // Getters
    int getId() const;
    string getPrioridad() const;
    string getEstado() const;
    vector<Tarea*> getSubtareas() const;
    std::string getDescripcion() const;
    int getIdUsuarioResponsable() const;
    int getCiclosEspera() const;

    // Setters
    void setId(int id);
    void setPrioridad(const string& prioridad);
    void setEstado(const string& estado);
    void setSubtareas(const vector<Tarea*>& subtareas);
    void setDescripcion(const std::string& descripcion);
    void setIdUsuarioResponsable(int idUsuarioResponsable);
    void setCiclosEspera(int ciclos);
    void incrementarCiclosEspera();
    void resetCiclosEspera();
};

#endif // TAREA_H