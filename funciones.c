#define _POSIX_C_SOURCE 200809L
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

vuelo_t* vuelo_crear(char** datos){
    vuelo_t* vuelo = malloc(sizeof(vuelo_t));
    if (!vuelo) return NULL;

    vuelo->numero_vuelo = datos[0];
    vuelo->aerolinea = datos[1];
    vuelo->aeropuerto_origen = datos[2];
    vuelo->aeropuerto_destino = datos[3];
    vuelo->matricula = datos[4];
    vuelo->prioridad = datos[5];
    vuelo->fecha = datos[6];
    vuelo->retraso_salida = datos[7];
    vuelo->tiempo_vuelo = datos[8];
    vuelo->cancelado = datos[9];

    return vuelo;
}

void mensaje_error(char* comando){
    fprintf(stderr,"%s %s\n","Error en comando",comando);
}

void agregar_archivo(char** comando, hash_t* hash, abb_t* abb){}

void ver_tablero(char** comando, abb_t* abb){}

void info_vuelo(char** comando, hash_t* hash){}

void prioridad_vuelos(char** comando, abb_t* abb){}

void borrar(char** comando,hash_t* hash, abb_t* abb){}

void ejecutar_comando(char** comando, hash_t* hash, abb_t* abb){
    if ((strcmp(comando[0],"agregar_archivo")) == 0) agregar_archivo(comando,hash,abb);
    else if ((strcmp(comando[0],"ver_tablero")) == 0) ver_tablero(comando,abb);
    else if ((strcmp(comando[0],"info_vuelo")) == 0) info_vuelo(comando,hash);
    else if ((strcmp(comando[0],"prioridad_vuelos")) == 0) prioridad_vuelos(comando,abb);
    else if ((strcmp(comando[0],"borrar") == 0)) borrar(comando,hash,abb);
    else{ mensaje_error(comando[0]);}
}

void remover_salto_linea(char** vector){
    int i = 0;
    
    while (vector[i] != NULL) i++;
    if (i == 0) return;
    
    char* str = vector[i-1];
    int j = 0;
    
    while(str[j] != '\n'){
        if (str[j] == '\0') return;
        j++;
    }
    str[j] = '\0';
}