#define _POSIX_C_SOURCE 200809L
#define AGREGAR_ARCHIVO "agregar_archivo"

#include "funciones.h"
#include "funciones_auxiliares.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    printf("**Bienvenido/a**\n");
    hash_t* hash = hash_crear(vuelo_destruir);
    abb_t* abb = abb_crear(strcmp,NULL);

    char* linea = NULL;
	size_t n = 0;
    printf("Inserte un comando:\n");
    while ((getline(&linea, &n, stdin)) > 0){
        char** comando = split(linea,' ');
        remover_salto_linea(comando);
        bool operacion_valida = ejecutar_comando(comando,hash,abb);
        
        if (!operacion_valida){
            mensaje_error(comando[0]);
        } else if (operacion_valida && (strcmp(comando[0],AGREGAR_ARCHIVO) != 0)){
            mensaje_exito();
        }

        free_strv(comando);
        printf("Inserte un comando:\n");
    }
    free(linea);
    hash_destruir(hash);
    abb_destruir(abb);
    return 0;
}