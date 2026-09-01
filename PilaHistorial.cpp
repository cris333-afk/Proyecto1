#include "PilaHistorial.h"

// Constructor por defecto.
PilaHistorial::PilaHistorial() {}


// Registra una nueva acción en la pila de deshacer.
// Al registrar, la pila de rehacer se vacía: descarta rehacer previos
// si el usuario ejecuta una nueva acción.
void PilaHistorial::registrarAccion(const Accion& accion) {

    // La nueva acción queda disponible para deshacer.
    pilaDeshacer.push(accion);

    // Al registrar una acción nueva, las acciones rehechas pierden validez(se eliminan de rehacer).
    while (!pilaRehacer.empty()) {

         // Se elimina cada acción que ya no pertenece a la secuencia actual.
        pilaRehacer.pop();
    }
}

    //Extrae la última acción de la pila de deshacer, la mueve a la pila de rehacer
    // (para que pueda recuperarse después con rehacer()) y la entrega para ser
    // procesada.
bool PilaHistorial::deshacer(Accion& accion) {

    // Retorna false si no hay acciones para deshacer (pila vacía).
    if (pilaDeshacer.empty()) {
        return false;
    }

    accion = pilaDeshacer.top();  // Se obtiene la acción más reciente registrada

    pilaDeshacer.pop();   // Se retira porque ya fue deshecha.

    pilaRehacer.push(accion);    // Se conserva para permitir rehacerla posteriormente.

    return true;
}
    // Extrae la última acción de la pila de rehacer, la mueve a la pila de deshacer
    // (para que quede disponible ante un nuevo deshacer). 
bool PilaHistorial::rehacer(Accion& accion) {


  // Retorna false si no hay acciones para rehacer (pila vacía).
    if (pilaRehacer.empty()) {
        
        return false;  // No hay nada que rehacer.
    }

    accion = pilaRehacer.top();   // Se obtiene la última acción que fue deshecha.

    pilaRehacer.pop();    // Se retira porque ya fue rehecha.

    pilaDeshacer.push(accion);    // Vuelve a estar disponible para deshacerla.

    return true;
}

// Verifica si la pila de deshacer está vacía.
bool PilaHistorial::estaVaciaDeshacer() const {

    return pilaDeshacer.empty();
}

// Verifica si la pila de rehacer está vacía.
bool PilaHistorial::estaVaciaRehacer() const {
    
    return pilaRehacer.empty();
}

// Retorna la cantidad de acciones disponibles para deshacer.
size_t PilaHistorial::tamanoDeshacer() const {
    
    return pilaDeshacer.size();
}

// Retorna la cantidad de acciones disponibles para rehacer.
size_t PilaHistorial::tamanoRehacer() const {
   
    return pilaRehacer.size();
}

// Copia la acción de la cima de deshacer sin eliminarla.
bool PilaHistorial::cimaDeshacer(Accion& accion) const {
    if (pilaDeshacer.empty()) {
        
        return false;
    }

    accion = pilaDeshacer.top();
    return true;
}

// Copia la acción de la cima de rehacer sin eliminarla.
bool PilaHistorial::cimaRehacer(Accion& accion) const {
    
    if (pilaRehacer.empty()) {
        return false;
    }

    accion = pilaRehacer.top();
    return true;
}

// Vacía ambas pilas por completo.
void PilaHistorial::limpiar() {
   
    while (!pilaDeshacer.empty()) {
        pilaDeshacer.pop();
    }

    while (!pilaRehacer.empty()) {
        pilaRehacer.pop();
    }
}


// Genera un texto con todas las acciones de una pila para mostrarlas al usuario.
// Recibe una copia de la pila (no la original) para que consultarla no afecte
// el historial real.
std::string PilaHistorial::mostrarPila(std::stack<Accion> pila, const std::string& titulo) const {
    
    // Título inicial: Crea el texto formateado con el encabezado recibido
    std::string texto = "--- " + titulo + " ---\n";

    // Validación de pila vacía: Si no hay elementos, añade "Vacia" y retorna
    if (pila.empty()) {
        texto += "Vacia\n";
        return texto;
    }

    while (!pila.empty()) {

        Accion accion = pila.top();  // Obtiene el elemento superior

        pila.pop();  // Desapila(pop) el elemento para avanzar

        texto += "Accion:\n";
        texto += accion.toString();  // Añade la representación en texto de la acción
        texto += "\n";
    }

    return texto; // Devuelve la cadena formateada con todo el historial
}



// Consulta ambas pilas y construye el texto completo del historial,
// sin modificar su contenido.
std::string PilaHistorial::mostrarHistorial() const {
    std::string texto = "================ HISTORIAL ================\n";

    texto += mostrarPila(pilaDeshacer, "ACCIONES PARA DESHACER");
    texto += mostrarPila(pilaRehacer, "ACCIONES PARA REHACER");

    texto += "============================================\n";

    return texto;
}
