#include "hash.h"
#include "abb.h"
#include "strutil.h"

bool agregar_archivo(abb_t* abb, hash_t* hash, char* nombre_archivo);

bool ver_tablero(abb_t* abb, int cant_vuelos, char* modo, char* desde, char* hasta);

bool info_vuelo(hash_t* hash, char* num_vuelo);

bool prioridad_vuelos(hash_t* hash, int k);

bool borrar(abb_t* abb, hash_t* hash, char* desde, char* hasta);

bool ejecutar_comando(char** comando, hash_t* hash, abb_t* abb);

void remover_salto_linea(char** vector);

void mensaje_exito(void);

void mensaje_error(char* comando);