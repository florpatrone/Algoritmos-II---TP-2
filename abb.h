#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;
typedef struct nodo_abb nodo_abb_t;
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/**************************
*   Primitivas del abb
**************************/

/* Crea un arbol binario de búsqueda con una raiz nula, cantidad 0,
una función de comparación y una función de destrucción.
Las funciones son pasadas por parámetro al momento de crear la misma.*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Si la clave ya existe, se reemplaza su dato. De lo contrario,
se guarda en una nueva hoja del arbol la clave y el dato.
Para ambos casos, devuelve true.
En caso de error, devuelve false.
Pre: el arbol debe haber sido creado. */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra el nodo con la clave indicada y lo devuelve.
Si el nodo no está, devuelve NULL.
Pre: el arbol debe haber sido creado.
Post: el arbol tiene un elemento menos. */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Devuelve el dato del nodo con la clave pasada por parámetro.
De no estar en el arbol, devuelve NULL.
Pre: el arbol debe haber sido creado.*/
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Devuelve true si la clave buscada se encuentra en el arbol.
De lo contrario, devuelve false.
Pre: el arbol debe haber sido creado. */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de nodos que tiene el arbol.*/
size_t abb_cantidad(abb_t *arbol);

/* Destruye el arbol y todos sus nodos.*/
void abb_destruir(abb_t *arbol);

/******************************
 * Iterador externo del abb
 * **************************/

typedef struct abb_iter_rango abb_iter_rango_t;
typedef struct abb_iter abb_iter_t;

/*****************************************************
 * Primitivas del iterador externo del abb con rango
* ****************************************************/

/* Crea un iterador del arbol que señala al primer
elemento inorder del mismo.*/
abb_iter_rango_t *abb_iter_rango_in_crear(const abb_t *arbol, char* inicio, char* fin);

/* Avanza una posición del iterador de manera INORDER y
devuelve true. Si no puede avanzar o el árbol está vacío
devuelve false. */
bool abb_iter_rango_in_avanzar(abb_iter_rango_t *iter);

/* Muestra la clave del nodo señalado por el iterador*/
const char *abb_iter_rango_in_ver_actual(const abb_iter_rango_t *iter);

/* Devuelve true si el iterador llegó al final del arbol
o, de lo contrario, false. */
bool abb_iter_rango_in_al_final(const abb_iter_rango_t *iter);

/* Destruye el iterador */
void abb_iter_rango_in_destruir(abb_iter_rango_t* iter);

/* Primitivas del iterador interno del abb */

/* Aplica la función "visitar" nodo a nodo de modo INORDER */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/*Devuelve el dato del nodo señalado por el iterador */
void *abb_iter_rango_in_ver_actual_dato(const abb_iter_rango_t *iter);
/*****************************************************
 * Primitivas del iterador externo del abb sin rango
* ****************************************************/

/* Crea un iterador del arbol que señala al primer
elemento inorder del mismo.*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza una posición del iterador de manera INORDER y
devuelve true. Si no puede avanzar o el árbol está vacío
devuelve false. */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Muestra la clave del nodo señalado por el iterador*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Devuelve true si el iterador llegó al final del arbol
o, de lo contrario, false. */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador */
void abb_iter_in_destruir(abb_iter_t* iter);

/******************
Primitivas Extra 
******************/
nodo_abb_t* abb_raiz(abb_t* abb);

nodo_abb_t* abb_nodo_ver_izq(nodo_abb_t *raiz);

nodo_abb_t* abb_nodo_ver_der(nodo_abb_t *raiz);

char* abb_ver_clave_nodo(nodo_abb_t* nodo);

void* abb_ver_dato_nodo(nodo_abb_t* nodo);

abb_comparar_clave_t abb_obtener_cmp(abb_t* abb);

#endif