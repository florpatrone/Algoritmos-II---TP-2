#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "hash.h"
#include "abb.h"
#include "strutil.h"

/* Guarda en las estructuras recibidas los datos contenidos en el archivo recibido*/
bool agregar_archivo(abb_t* abb, hash_t* hash, char* nombre_archivo);

/* Imprime por salida estandar los datos del numero de vuelos recibido por parametro
que se encuentren en el intervalo recibido */ 
bool ver_tablero(abb_t* abb,  hash_t* hash, int cant_vuelos, char* modo, char* desde, char* hasta);

/* Dado un numero de vuelo existente, imprime por salida estandar los datos asociados a dicho vuelo */
bool info_vuelo(hash_t* hash, char* num_vuelo);

/* Imprime por salida estandar los datos de los k vuelos con mayor prioridad*/
bool prioridad_vuelos(hash_t* hash, int k);

/* Borra de las estructuras recibidas los datos de los vuelos pertenecientes al intervalo recibido */
bool borrar(abb_t* abb, hash_t* hash, char* desde, char* hasta);


#endif
