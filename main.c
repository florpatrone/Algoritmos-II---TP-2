#define _POSIX_C_SOURCE 200809L

#include "funciones.h"
#include "funciones_auxiliares.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    hash_t* hash = hash_crear(vuelo_destruir);
    abb_t* abb = abb_crear(strcmp,wrapper_abb_destruir);
    if (!hash || !abb ) return -1;
    char* linea = NULL;
	size_t n = 0;
    while ((getline(&linea, &n, stdin)) > 0){
        char** comando = split(linea,' ');
        remover_salto_linea(comando);
        bool operacion_valida = ejecutar_comando(comando,hash,abb);
        if (operacion_valida){
            mensaje_exito();
        }else{
            mensaje_error(comando[0]);
        }

        free_strv(comando);
    }
    free(linea);
    hash_destruir(hash);
    abb_destruir(abb);
    return 0;
}