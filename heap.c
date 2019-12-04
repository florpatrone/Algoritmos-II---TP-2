#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define _POSIX_C_SOURCE 200809L 
#define CAPACIDAD_INICIAL 20
#define FACTOR_REDIMENSION 2
#define PROPORCION_CANT_CAP 4

struct heap{
    void** datos;
    size_t cant;
    size_t tam; // capacidad
    cmp_func_t cmp;
};

/***************************
* Funciones auxiliares
****************************/

/* Devuelve la nueva capacidad del heap, disminuida */
size_t disminuir_capacidad_heap(heap_t* heap){
    return heap->tam / FACTOR_REDIMENSION;
}

/* Devuelve la nueva capacidad del heap, aumentada */
size_t aumentar_capacidad_heap(heap_t* heap){
    return heap->tam * FACTOR_REDIMENSION;
}

/* Redimensiona la capacidad del heap según la operación recibida
(aumentar_capacidad_heap o disminuir_capacidad_heap) */
bool heap_redimensionar(heap_t* heap, size_t (*operacion) (heap_t*)){
    size_t nuevo_tam = operacion(heap);
    void** n_datos = realloc(heap->datos, sizeof(void*) * nuevo_tam);
    if (!n_datos) return false;
    heap->datos = n_datos;
    heap->tam = nuevo_tam;
    return true;
}

/* Intercambia los datos de la posición a y b del arreglo "vector".
Post: El arreglo se encuentra modificado */
void swap(void** vector, size_t a, size_t b){
    void* aux = vector[a];
    vector[a] = vector[b];
    vector[b] = aux;
}

void upheap(void** vector, size_t pos_elem, cmp_func_t cmp){
    if ((pos_elem) == 0) return;
    size_t pos_padre = (pos_elem-1)/2;
    void* elem = vector[pos_elem];
    void* padre = vector[pos_padre];
    if (cmp(elem,padre) < 0){
        swap(vector,pos_padre,pos_elem);
        upheap(vector,pos_padre,cmp);
    }
}

void downheap(void** vector, size_t tam, size_t pos, cmp_func_t cmp){
	if (pos >= tam) return;
	size_t max = pos;
	size_t izq = 2 * pos + 1;
	size_t der = 2 * pos + 2;
    
	if (izq < tam && cmp(vector[izq], vector[max]) < 0)     max = izq;
	if (der < tam && cmp(vector[der], vector[max]) < 0)     max = der;
    
	if (max != pos){
		swap(vector,pos,max);
		downheap(vector,   tam,   max,   cmp);
    }
}

/* Recibe un arreglo de tamaño n y una funcion de comparación,
le da al arreglo propiedad de heap
Post: el arreglo se encuentra modificado */
void heapify(void** vector, size_t n, cmp_func_t cmp){
    for (long i = n-1; i >= 0; i--){
        downheap(vector,n,i,cmp);
    }
}

/* Recibe un arreglo de tamaño n y devuelve otro del mismo tamaño, en memoria dinámica,
con los mismos elementos del arreglo original */
void** copiar_arreglo(void** original,size_t n){
    void** copia = malloc(sizeof(void*)*n);
    if (!copia) return NULL;
    for (size_t i = 0; i < n; i++){
        copia[i] = original[i];
    }
    return copia;
}

/***************************
* Primitivas del heap
****************************/

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
    if (destruir_elemento){
        for (int i = 0; i < heap->cant; i++){
            destruir_elemento((heap->datos)[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

bool heap_encolar(heap_t *heap, void *elem){
    if (!elem) return false;
    if (heap->cant == heap->tam){
        if (!heap_redimensionar(heap,aumentar_capacidad_heap)) return false;
    }
    void** datos = heap->datos;
    datos[heap->cant] = elem;
    upheap(datos,heap->cant,heap->cmp);
    heap->cant++;
    return true;
}

void *heap_ver_tope(const heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    
    void** datos = copiar_arreglo(arreglo,n);
    if (!datos){
        free(heap);
        return NULL;
    }
    heapify(datos,n,cmp);
    heap->datos = datos;
    heap->cant = n;
    heap->tam = n;
    heap->cmp = cmp;
    return heap;
}

/* Ordena de mayor a menor */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos,cant,cmp);
    size_t pos_final = cant - 1;

    for (size_t i = 0; i < cant; i++){
        swap(elementos, 0, pos_final);
        downheap(elementos,pos_final,0,cmp);
        pos_final--;
    }
}

void *heap_desencolar(heap_t *heap){
    if (heap_esta_vacio(heap))  return NULL;
    
    void** arr = heap->datos;
    size_t pos_final = heap->cant -1;
    swap(arr,0,pos_final);
    
    void* desencolado = arr[pos_final];
    arr[pos_final] = NULL;

    heap->cant--;
    downheap(arr, heap->cant, 0, heap->cmp);
    if (heap->tam > CAPACIDAD_INICIAL && PROPORCION_CANT_CAP * heap->cant <= heap->tam){
        if (!heap_redimensionar(heap,disminuir_capacidad_heap)) return NULL;
    }
    return desencolado;
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    
    void** datos = malloc(sizeof(void*) * CAPACIDAD_INICIAL);
    if (!datos){
        free(heap);
        return NULL;
    }

    for (size_t i = 0; i < CAPACIDAD_INICIAL; i++){
        datos[i] = NULL;
    }

    heap->datos = datos;
    heap->cant = 0;
    heap->tam = CAPACIDAD_INICIAL;
    heap->cmp = cmp;
    return heap;
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    size_t elems = heap->cant;
    return  elems == 0;
}