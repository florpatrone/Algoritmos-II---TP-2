#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>

/* Definición del struct nodo */
struct nodo{
    void* dato; 
    struct nodo* proximo;
};

typedef struct nodo nodo_t;

/* Definición del struct lista*/
struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t longitud;
};

/* Definición del struct lista_iter */
struct lista_iter{
    lista_t* lista;
    nodo_t* actual; 
    nodo_t* anterior;
};

/* *****************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/

nodo_t* nodo_crear(void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    
    if (nodo == NULL) return NULL;

    nodo->proximo = NULL;
    nodo->dato = dato;
    return nodo;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));

    if (lista == NULL) return NULL;

    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->longitud = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return lista->longitud == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo = nodo_crear(dato);
    
    if (nodo == NULL) return false;
    
    if (lista->primero != NULL){
        nodo->proximo = lista->primero;
    }
    else{
        lista->ultimo = nodo;
    }

    lista->primero = nodo;
    lista->longitud++;

    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = nodo_crear(dato);

    if (nodo == NULL) return false;

    if (lista->ultimo == NULL){
        lista->primero = nodo;
    }

    else{
    lista->ultimo->proximo = nodo;
    }
    
    lista->ultimo = nodo;
    lista->longitud++;

    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;

    nodo_t* nodo = lista->primero;
    void* dato = nodo->dato;
    
    lista->primero = lista->primero->proximo;
    lista->longitud--;
    free(nodo);

    if (lista->primero == NULL){
        lista->ultimo = NULL;
    }

    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;

    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;

    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->longitud;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
    while (!lista_esta_vacia(lista)){
        void* dato = lista_borrar_primero(lista);

        if (destruir_dato != NULL){
            destruir_dato(dato);
        }
    }
    free(lista);
}


/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));

    if (iterador == NULL) return NULL;

    iterador->lista = lista;
    iterador->actual = lista->primero;
    iterador->anterior = NULL;

    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->proximo;

    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    return iter->actual == NULL ? NULL : iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return  iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo = nodo_crear(dato);

    if (nodo == NULL) return false;

    lista_t* lista = iter->lista;

    if (iter->anterior == NULL){  //El iterador esta al comienzo de la lista
        lista->primero = nodo;
    }

    else{
        iter->anterior->proximo = nodo;
    }

    if (iter->actual == NULL){  //El iterador esta al final
        lista->ultimo = nodo;
    }

    nodo->proximo = iter->actual;
    lista->longitud++;
    iter->actual = nodo; 

    return true; 
}

void *lista_iter_borrar(lista_iter_t *iter){
    lista_t* lista = iter->lista;

    if (iter->actual == NULL) return NULL;

    nodo_t* nodo = iter->actual;
    void* dato = nodo->dato; 

    if (iter->anterior == NULL){  //El iterador esta al comienzo de la lista

        lista->primero = iter->actual->proximo;

        if (lista->primero == NULL){ //La lista queda vacia
            lista->ultimo = NULL;
        }
    }

    else{
        iter->anterior->proximo = iter->actual->proximo;

        if (iter->anterior->proximo == NULL){ //El iterador esta al final de la lista
            lista->ultimo = iter->anterior;
        }
    }
    
    iter->actual = iter->actual->proximo;
    lista->longitud--;
    free(nodo);
    return dato;
    
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t* actual = lista->primero;

    while (actual != NULL){
        if (!visitar(actual->dato,extra)) return;
        actual = actual->proximo;
    }
}
