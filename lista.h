#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

//Crea una lista
//Post: devuelve una nueva lista vacia.
lista_t *lista_crear(void);

// Devuelve true si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al comienzo de la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento al comienzo de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista.
// Si la lista tiene elementos, se quita el primero y se devuelve su valor, si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el valor del primer elemento de la lista.
// La lista contiene un elemento menos, si no estaba vacia.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista.
// Si la lista tiene elementos, se devuelve el valor del primero, si esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista, cuando no esta vacia.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista.
// Si la lista tiene elementos, se devuelve el valor del ultimo, si esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el ultimo elemento de la lista, cuando no esta vacia.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad de elementos que tiene la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista.
// Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador para una lista recibida por parametro.
// Pre: la lista fue creada.
// Post: devuelve un iterador posicionado en el primer elemento de la lista,
// o en NULL si la lista esta vacia.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza, de ser posible, desde el elemento actual del iterador al proximo elemento en la lista.
// Devuelve true si fue posible, false en caso contrario.
// Pre: el iterador fue creado.
// Post: avanza al iterador en un elemento y actualiza su elemento actual, si no se encontraba ya en el final.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del elemento actual del iterador, o NULL si este esta al final.
// Pre: el iterador fue creado.
// Post: se devolvio el elemento actual del iterador, si no esta en el final.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador se encuentra al final, false en caso contrario.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador, sin destruir la lista asociada.
// Pre: el iterador fue creado.
// Post: se elimino el iterador; no se modifico la lista asociada.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la lista, en la posicion actual del iterador. Devuelve false en caso de error.
// Pre: el iterador fue creado.
// Post: se agregó un nuevo elemento a la lista, en la posicion actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Saca el elemento de la lista en la posicion actual del iterador. Si la lista esta vacia, devuelve NULL.
// Pre: el iterador fue creado.
// Post: se devolvio el valor del elemento en la posicion actual del iterador.
// La lista contiene un elemento menos, si no estaba vacia.
void *lista_iter_borrar(lista_iter_t *iter);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera una lista recibida por parametro, le aplica a sus elementos la funcion visitar
// hasta que esta devueva false.
// Pre: la lista fue creada.
// Visitar es una funcion que devuelve un booleano.
// Post: se aplico la funcion visitar a cada elemento de la lista, mientras que visitar devolviera true.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

#endif