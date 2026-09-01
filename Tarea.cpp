#include "Tarea.h"

// Constructor
Tarea::Tarea(int id, const string& prioridad, const string& estado,
             const string& descripcion, int idUsuarioResponsable, int ciclosEspera) {
    this->id = id;
    this->prioridad = prioridad;
    this->estado = estado;
    this->descripcion = descripcion;
    this->idUsuarioResponsable = idUsuarioResponsable;
    this->ciclosEspera = ciclosEspera;
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

int Tarea::getCiclosEspera() const {
    return ciclosEspera;
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

void Tarea::setCiclosEspera(int ciclos) {
    this->ciclosEspera = ciclos;
}

void Tarea::incrementarCiclosEspera() {
    this->ciclosEspera++;
}

void Tarea::resetCiclosEspera() {
    this->ciclosEspera = 0;
}

void Tarea::setSubtareas(const vector<Tarea*>& subtareas) {
    this->subtareas = subtareas;
}