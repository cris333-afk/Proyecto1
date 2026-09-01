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

public:
    // Constructor
    Tarea(int id, const string& prioridad, const string& estado,
          const string& descripcion = "", int idUsuarioResponsable = -1);

    // Destructor
    ~Tarea();

    // Getters
    int getId() const;
    string getPrioridad() const;
    string getEstado() const;
    vector<Tarea*> getSubtareas() const;
    std::string getDescripcion() const;
    int getIdUsuarioResponsable() const;

    // Setters
    void setId(int id);
    void setPrioridad(const string& prioridad);
    void setEstado(const string& estado);
    void setSubtareas(const vector<Tarea*>& subtareas);
    void setDescripcion(const std::string& descripcion);
    void setIdUsuarioResponsable(int idUsuarioResponsable);
};

#endif // TAREA_H