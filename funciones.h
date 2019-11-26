#include "hash.h"
#include "abb.h"
#include "strutil.h"

typedef struct vuelo vuelo_t;

void agregar_archivo(char** comando, hash_t* hash, abb_t* abb);

void ver_tablero(char** comando, abb_t* abb);

void info_vuelo(char** comando, hash_t* hash);

void prioridad_vuelos(char** comando, abb_t* abb);

void borrar(char** comando,hash_t* hash, abb_t* abb);

void ejecutar_comando(char** comando, hash_t* hash, abb_t* abb);