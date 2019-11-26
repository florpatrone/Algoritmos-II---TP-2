#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 50
#define CTE_INCREMENTO 2
#define CTE_REDUCCION 2
#define CRITERIO_REDUCCION 4


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){
    pila_t* pila = malloc(sizeof(pila_t));

    if (pila == NULL){
        return NULL;
    }
    
    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->datos = malloc(sizeof(void*)*CAPACIDAD_INICIAL);

    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad == 0);
}

void* pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }

    return pila->datos[pila->cantidad-1];
}


bool pila_redimensionar(pila_t *pila, size_t tamanio){
    void* datos = realloc(pila->datos, sizeof(void*)*tamanio);

    if (datos == NULL){
        return false;
    }

    pila->datos = datos;
    pila->capacidad = tamanio;

    return true;
}

bool pila_apilar(pila_t *pila, void* valor){
    if (pila->capacidad == pila->cantidad){
        if (!(pila_redimensionar(pila,pila->capacidad*CTE_INCREMENTO))){
            return false;
        }
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;

    return true;
}

void* pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    
    void* dato = pila->datos[pila->cantidad-1];

    pila->cantidad--;

    if ( (pila->capacidad > CAPACIDAD_INICIAL) && (pila->cantidad <= (pila->capacidad/(size_t)CRITERIO_REDUCCION)) ){
        pila_redimensionar(pila,pila->capacidad/(size_t)CTE_REDUCCION);
    }
    
    return dato;
}