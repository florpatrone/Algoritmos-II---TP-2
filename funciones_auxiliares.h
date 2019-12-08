#ifndef AUX_H
#define AUX_H

#include "abb.h"
#include "lista.h"

typedef struct vuelo vuelo_t;

void mensaje_exito(void);

void mensaje_error(char* comando);

bool rango_valido(const char* inicio, const char* fin);

bool es_natural(const char* str);

char* rstrip(char* s);

bool quedan_vuelos(abb_t* abb);

bool igual_comando(const char* a, const char* b);

bool comando_valido(int cant_elem, char* linea[], int operacion);

vuelo_t* vuelo_crear(char** datos);

void vuelo_destruir(void* mem_vuelo);

void remover_salto_linea(char** vector);

int len_vector(char** vector);

void imprimir_datos_vuelo(vuelo_t* vuelo);

void imprimir_prioridad(vuelo_t* vuelo);

int cmp_prioridad_vuelo(const void* void_a, const void* void_b);

void buscar_guardar(lista_t* lista, bool modo_asc, nodo_abb_t* raiz, char* desde, char* hasta);
/*Recibe por parámetro una lista, un modo de guardado, un ABB y dos fechas de cota.
Busca en el ABB aquellos vuelos que se encuentren dentro de la cota pedida y los agrega a la lista según 
si se pide imprimir de manera ascendente (lista_insertar_ultimo) o descendente (lista_insertar_primero)*/

int cmp_fechas(const void* void_a, const void* void_b);

void imprimir_en_tablero(nodo_abb_t* nodo_abb);

bool ejecutar_comando(char** comando, hash_t* hash, abb_t* abb);

bool imprimir_vuelo_fecha(const char* clave, void * dato, void * nada);

int cmp_fechas_abb(const char* fecha1, const char* fecha2);

#endif // AUX_H