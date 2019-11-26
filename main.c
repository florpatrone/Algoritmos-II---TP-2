#define _POSIX_C_SOURCE 200809L
#include "funciones.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]){
    hash_t* hash = hash_crear(NULL);
    abb_t* abb = abb_crear(strcmp,NULL);

    char* linea = NULL;
	size_t n = 0;

    while ((getline(&linea, &n, stdin)) > 0){
        char** comando = split(linea,' ');
        ejecutar_comando(comando,hash,abb);        
    }
    return 0;
}
