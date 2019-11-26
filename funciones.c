#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "funciones.h"

struct vuelo{
    char* numero_vuelo;
    char* aerolinea;
    char* aeropuerto_origen;
    char* aeropuerto_destino;
    char* matricula;
    char* prioridad;
    char* fecha;
    char* retraso_salida;
    char* tiempo_vuelo;
    char* cancelado;
};

void ejecutar_comando(char** comando, hash_t* hash, abb_t* abb){
    if ((strcmp(comando[0],"agregar_archivo")) == 0) agregar_archivo(comando,hash,abb);
    else if ((strcmp(comando[0],"ver_tablero")) == 0) ver_tablero(comando,abb);
    else if ((strcmp(comando[0],"info_vuelo")) == 0) info_vuelo(comando,hash);
    else if ((strcmp(comando[0],"prioridad_vuelos")) == 0) prioridad_vuelos(comando,abb);
    else if ((strcmp(comando[0],"borrar") == 0)) borrar(comando,hash,abb);
    else {fprintf(stderr,"%s %s","Error en comando",comando[0]);}
}