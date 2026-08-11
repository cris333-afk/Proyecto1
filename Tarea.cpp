#include "Tarea.h"

// Constructor
Tarea::Tarea(int id, const string& prioridad, const string& estado) {
    this->id = id;
    this->prioridad = prioridad;
    this->estado = estado;
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

void Tarea::setSubtareas(const vector<Tarea*>& subtareas) {
    this->subtareas = subtareas;
}