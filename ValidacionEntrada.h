// ==========================================================
// Modulo ValidacionEntrada - Sistema Gestor de Flujos de Trabajo
// Curso: EIF207 - Estructuras de Datos
//
// Lectura validada de la consola: funciones que piden datos al
// usuario de forma robusta, sin dejar que un ingreso invalido
// (letras, simbolos, numeros fuera de rango, falta de entrada)
// crashee el programa ni lo deje en un bucle infinito.
// ==========================================================

#ifndef VALIDACIONENTRADA_H
#define VALIDACIONENTRADA_H

#include <string>

/**
 * Pide al usuario un numero entero dentro del rango [min, max].
 *
 * - Si ingresa letras, simbolos o cualquier cosa que no sea un
 *   numero, limpia el estado de cin (cin.clear() + cin.ignore()),
 *   muestra un mensaje de error amigable y vuelve a pedir.
 * - Si el numero esta fuera de [min, max], muestra un mensaje de
 *   error y vuelve a pedir.
 * - Nunca crashea ni queda en bucle infinito: trata el EOF (entrada
 *   finalizada) y si min > max los intercambia para mantener un
 *   rango valido.
 *
 * @param min Limite inferior (inclusive) de la opcion permitida.
 * @param max Limite superior (inclusive) de la opcion permitida.
 * @return El numero entero valido ingresado por el usuario.
 */
int leerOpcionMenu(int min, int max);

/**
 * Pide al usuario un numero entero mostrando primero el mensaje.
 * Solo valida que el numero sea mayor o igual a min (sin limite
 * superior).
 *
 * - Si ingresa letras, simbolos o cualquier cosa que no sea un
 *   numero, limpia el estado de cin (cin.clear() + cin.ignore()),
 *   muestra un mensaje de error amigable y vuelve a pedir.
 * - Si el numero es menor a min, muestra un mensaje de error y
 *   vuelve a pedir.
 * - Nunca crashea ni queda en bucle infinito: trata el EOF (entrada
 *   finalizada) retornando min como valor por defecto.
 *
 * @param mensaje Texto que se muestra antes de leer (el prompt).
 * @param min     Limite inferior (inclusive): se exige numero >= min.
 * @return El numero entero valido ingresado por el usuario.
 */
int leerEnteroValidado(const std::string& mensaje, int min);

/**
 * Pide al usuario un numero entero mostrando primero el mensaje.
 * Valida que el numero este entre min y max (ambos inclusive).
 *
 * - Si ingresa letras, simbolos o cualquier cosa que no sea un
 *   numero, limpia el estado de cin (cin.clear() + cin.ignore()),
 *   muestra un mensaje de error amigable y vuelve a pedir.
 * - Si el numero esta fuera de [min, max], muestra un mensaje de
 *   error y vuelve a pedir.
 * - Nunca crashea ni queda en bucle infinito: trata el EOF (entrada
 *   finalizada) y si min > max los intercambia para mantener un
 *   rango valido.
 *
 * @param mensaje Texto que se muestra antes de leer (el prompt).
 * @param min     Limite inferior (inclusive) del numero permitido.
 * @param max     Limite superior (inclusive) del numero permitido.
 * @return El numero entero valido ingresado por el usuario.
 */
int leerEnteroValidado(const std::string& mensaje, int min, int max);

/**
 * Pide al usuario una linea completa de texto mostrando primero el
 * mensaje, y la devuelve tal cual (conserva espacios y mayusculas).
 * Solo se valida que la linea NO este vacia ni sea solo espacios en
 * blanco.
 *
 * - Usa std::getline, por lo que acepta texto con espacios (a
 *   diferencia de cin >>, que cortaria en el primer espacio).
 * - Si la entrada esta vacia o contiene unicamente espacios, muestra
 *   un mensaje de error amigable y vuelve a pedir hasta recibir
 *   texto valido.
 * - Nunca crashea ni queda en bucle infinito: trata el EOF (entrada
 *   finalizada) retornando una cadena vacia.
 *
 * @param mensaje Texto que se muestra antes de leer (el prompt).
 * @return La linea de texto no vacia ingresada por el usuario.
 */
std::string leerTextoNoVacio(const std::string& mensaje);

#endif // VALIDACIONENTRADA_H