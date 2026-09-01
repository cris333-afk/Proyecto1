#include "Tarea.h"

// Constructor
Tarea::Tarea(int id, const string& prioridad, const string& estado,
             const string& descripcion, int idUsuarioResponsable) {
    this->id = id;
    this->prioridad = prioridad;
    this->estado = estado;
    this->descripcion = descripcion;
    this->idUsuarioResponsable = idUsuarioResponsable;
}

// Destructor
Tarea::~Tarea() {
    // Libera la memoria de cada subtarea
    for (Tarea* subtarea : subtareas) {
        delete subtarea;
    }
}

// Getters
int Tarea::getId() const {
    return id;
}

string Tarea::getPrioridad() const {
    return prioridad;
}

string Tarea::getEstado() const {
    return estado;
}

std::string Tarea::getDescripcion() const {
    return descripcion;
}

int Tarea::getIdUsuarioResponsable() const {
    return idUsuarioResponsable;
}

vector<Tarea*> Tarea::getSubtareas() const {
    return subtareas;
}

// Setters
void Tarea::setId(int id) {
    this->id = id;
}

void Tarea::setPrioridad(const string& prioridad) {
    this->prioridad = prioridad;
}

void Tarea::setEstado(const string& estado) {
    this->estado = estado;
}

void Tarea::setDescripcion(const std::string& descripcion) {
    this->descripcion = descripcion;
}

void Tarea::setIdUsuarioResponsable(int idUsuarioResponsable) {
    this->idUsuarioResponsable = idUsuarioResponsable;
}

void Tarea::setSubtareas(const vector<Tarea*>& subtareas) {
    this->subtareas = subtareas;
}