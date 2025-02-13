#define _POSIX_C_SOURCE 200809L
#define AGREGAR_ARCHIVO "agregar_archivo"
#define VER_TABLERO "ver_tablero"
#define INFO_VUELOS "info_vuelo"
#define PRIORIDAD_VUELOS "prioridad_vuelos"
#define BORRAR_VUELOS "borrar"
#define ASC "asc"
#define DESC "desc"
#define DATO_RELLENO "esto_es_un_relleno"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "heap.h"
#include "abb.h"
#include "lista.h"
#include "funciones.h"
#include "funciones_auxiliares.h"

enum operacion{AGREGAR,VER,INFO,PRIORIDAD,BORRAR};

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

/*************************
 * FUNCIONES AUXILIARES
 * **********************/

void mensaje_exito(void){
    fprintf(stdout,"OK\n");
}

void mensaje_error(char* comando){
    fprintf(stderr,"%s %s\n","Error en comando",comando);
}

bool rango_valido(const char* inicio, const char* fin){
    return strcmp(inicio,fin) <= 0;
}

bool es_natural(const char* str){
    int i = 0;
    while (str[i] != '\0'){
        if (!isdigit(str[i])) return false;
        i++;
    }
    if ((i == 1) && (str[0] == '0')) return false;
    return i > 0;
}

bool igual_comando(const char* a, const char* b){
    return strcmp(a,b) == 0;
}

bool comando_valido(int cant_elem, char* linea[], int operacion){
    if (!(cant_elem > 1))  return false;

    // condiciones que debe cumplir el comando según cuál sea
    if ( (operacion == AGREGAR || operacion == INFO || operacion == PRIORIDAD) && cant_elem != 2){
        return false;
    }
    if (operacion == VER){
        if (cant_elem != 5 || !es_natural(linea[1]) || !rango_valido(linea[3],linea[4])) return false;
        if (!(igual_comando(ASC,linea[2]) || igual_comando(DESC,linea[2]))) return false;
    }
    else if (operacion == PRIORIDAD && !es_natural(linea[1])){
        return false;
    }
    else if (operacion == BORRAR){
        if (cant_elem != 3 || !rango_valido(linea[1],linea[2])){
            return false;
        }
    }
    return true;
}

vuelo_t* vuelo_crear(char** datos){
    vuelo_t* vuelo = malloc(sizeof(vuelo_t));
    if (!vuelo) return NULL;

    vuelo->numero_vuelo = strdup(datos[0]);
    vuelo->aerolinea = strdup(datos[1]);
    vuelo->aeropuerto_origen = strdup(datos[2]);
    vuelo->aeropuerto_destino = strdup(datos[3]);
    vuelo->matricula = strdup(datos[4]);
    vuelo->prioridad = strdup(datos[5]);
    vuelo->fecha = strdup(datos[6]);
    vuelo->retraso_salida = strdup(datos[7]);
    vuelo->tiempo_vuelo = strdup(datos[8]);
    vuelo->cancelado = strdup(datos[9]);

    return vuelo;
}

void vuelo_destruir(void* mem_vuelo){
    vuelo_t* vuelo = (vuelo_t*) mem_vuelo;
    free(vuelo->numero_vuelo);
    free(vuelo->aerolinea);
    free(vuelo->aeropuerto_origen);
    free(vuelo->aeropuerto_destino);
    free(vuelo->matricula);
    free(vuelo->prioridad);
    free(vuelo->fecha);
    free(vuelo->retraso_salida);
    free(vuelo->tiempo_vuelo);
    free(vuelo->cancelado);
    free(vuelo);
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

int len_vector(char** vector){
    int i = 0;
    while (vector[i] != NULL){
        i++;
    }
    return i;
}

void imprimir_datos_vuelo(vuelo_t* vuelo){
    fprintf(stdout,"%s ",vuelo->numero_vuelo);
    fprintf(stdout,"%s ",vuelo->aerolinea);
    fprintf(stdout,"%s ",vuelo->aeropuerto_origen);
    fprintf(stdout,"%s ",vuelo->aeropuerto_destino);
    fprintf(stdout,"%s ",vuelo->matricula);
    fprintf(stdout,"%s ",vuelo->prioridad);
    fprintf(stdout,"%s ",vuelo->fecha);
    fprintf(stdout,"%s ",vuelo->retraso_salida);
    fprintf(stdout,"%s ",vuelo->tiempo_vuelo);
    fprintf(stdout,"%s\n",vuelo->cancelado);
}

void imprimir_prioridad(vuelo_t* vuelo){
    fprintf(stdout,"%d - %s\n",atoi(vuelo->prioridad),vuelo->numero_vuelo);
}

int cmp_prioridad_vuelo(const void* void_a, const void* void_b){
    vuelo_t* vuelo_a = (vuelo_t*) void_a;
    vuelo_t* vuelo_b = (vuelo_t*) void_b;

    if (atoi( (const char*)vuelo_a->prioridad) == atoi( (const char*)vuelo_b->prioridad)){
        return strcmp(vuelo_b->numero_vuelo,vuelo_a->numero_vuelo);
    }
    return atoi( (const char*)vuelo_a->prioridad) > atoi( (const char*)vuelo_b->prioridad) ? 1 : -1;
}

void imprimir_en_tablero(vuelo_t* vuelo){ 
    char* fecha = vuelo->fecha;
    char* num_vuelo = vuelo->numero_vuelo;
    fprintf(stdout, "%s - %s\n", fecha, num_vuelo);
}

bool ejecutar_comando(char** comando, hash_t* hash, abb_t* abb){
    int cant_elem = len_vector(comando);
    char* operacion = comando[0];
    if (igual_comando(AGREGAR_ARCHIVO,operacion)){
        if (comando_valido(cant_elem,comando,AGREGAR)){
            return agregar_archivo(abb,hash,comando[1]);
        }
    }else if (igual_comando(VER_TABLERO,operacion)){
        if (comando_valido(cant_elem,comando,VER)){
            return ver_tablero(abb,hash,atoi( (const char*)comando[1]),comando[2],comando[3],comando[4]);
        }
    }else if (igual_comando(INFO_VUELOS,operacion)){
        if (comando_valido(cant_elem,comando,INFO)){
            return info_vuelo(hash,comando[1]);
        }
    }else if (igual_comando(PRIORIDAD_VUELOS,operacion)){
        if (comando_valido(cant_elem,comando,PRIORIDAD)){
            return prioridad_vuelos(hash,atoi( (const char*)comando[1]));
        }
    }else if (igual_comando(BORRAR_VUELOS,operacion)){
        if (comando_valido(cant_elem,comando,BORRAR)){
            return borrar(abb,hash,comando[1],comando[2]);
        }
    }return false; 
}

bool abb_guardar_fecha(abb_t *arbol, const char *clave, void *dato){
    bool pertenece = abb_pertenece(arbol, clave);
    abb_t* sub_arbol;

    if (pertenece){
        sub_arbol = abb_obtener(arbol,clave);
    } else {
        sub_arbol = abb_crear(abb_obtener_cmp(arbol), NULL);
        if (!sub_arbol) return false;
        if (!abb_guardar(arbol,clave,sub_arbol)){
            abb_destruir(sub_arbol);
            return false;
        }
    }

    if (!abb_guardar(sub_arbol,dato,DATO_RELLENO)){
        return false;
    }
    
	return true;
}

void wrapper_abb_destruir(void* dato){
	abb_t* abb = dato;
    abb_destruir(abb);
}

lista_t* obtener_claves_abb(abb_t* abb, char* desde, char* hasta){   
    abb_iter_rango_t* iter = abb_iter_rango_in_crear(abb,desde,hasta);
    if (!iter) return NULL;

    lista_t* lista = lista_crear();
    if (!lista){
        abb_iter_rango_in_destruir(iter);
        return NULL;
    }

    while (!abb_iter_rango_in_al_final(iter)){
        char* fecha_copia = strdup(abb_iter_rango_in_ver_actual(iter));
        if (!lista_insertar_ultimo(lista,fecha_copia) || !abb_iter_rango_in_avanzar(iter)){
            lista_destruir(lista,free);
            abb_iter_rango_in_destruir(iter);
            return NULL;
        }
    }
    abb_iter_rango_in_destruir(iter);
    return lista;
}

lista_t* borrar_fechas_abb(abb_t* abb, lista_t* lista_fechas){
    lista_t* lista_vuelos = lista_crear();
    if (!lista_vuelos)  return NULL;

    while (!lista_esta_vacia(lista_fechas)){
        char* fecha = lista_borrar_primero(lista_fechas);
        abb_t* sub_arbol = abb_borrar(abb,fecha);
        abb_iter_t* iter = abb_iter_in_crear(sub_arbol);
        if (!iter){
            lista_destruir(lista_vuelos,NULL);
            abb_destruir(sub_arbol);
            return NULL;
        }

        while (!abb_iter_in_al_final(iter)){
            char* num_vuelo = strdup(abb_iter_in_ver_actual(iter));
            if (!lista_insertar_ultimo(lista_vuelos,num_vuelo) || !abb_iter_in_avanzar(iter)){ ///???????????
                lista_destruir(lista_vuelos,NULL);
                abb_destruir(sub_arbol);
                abb_iter_in_destruir(iter);
                return NULL;
            }
        }
        free(fecha);
        abb_iter_in_destruir(iter);
        abb_destruir(sub_arbol);
    }
    lista_destruir(lista_fechas,NULL);
    return lista_vuelos;
}

void borrar_vuelos_hash(hash_t* hash, lista_t* lista){
    while (!lista_esta_vacia(lista)){
        char* num_vuelo = lista_borrar_primero(lista);
        vuelo_t* vuelo = hash_borrar(hash, num_vuelo);
        imprimir_datos_vuelo(vuelo);
        vuelo_destruir(vuelo);
        free(num_vuelo);
    }
    lista_destruir(lista,NULL);
}

/*************************
 * FUNCIONES PRINCIPALES
 * **********************/

bool agregar_archivo(abb_t* abb, hash_t* hash, char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo,"r");
    if (!archivo) return false;

    char* linea = NULL;
	size_t n = 0;
    bool errores = false;
    while (!errores && (getline(&linea, &n, archivo)) > 0){
        char** datos = split(linea,',');
        if (!datos) return false;
        remover_salto_linea(datos);
        
        vuelo_t* vuelo = vuelo_crear(datos);
        if (!vuelo){
            errores = true;
            free_strv(datos);
            break;
        }

        char* num_vuelo = vuelo->numero_vuelo;
        char* fecha = vuelo->fecha;
        if (hash_pertenece(hash,num_vuelo)){
            vuelo_t* vuelo_viejo = hash_obtener(hash,num_vuelo);
            char* fecha_vieja = vuelo_viejo->fecha;
            if (strcmp(fecha_vieja,fecha) != 0){
                abb_t* sub_arbol = abb_obtener(abb,fecha_vieja);
                abb_borrar(sub_arbol,num_vuelo);
            }
        }

        if (!hash_guardar(hash,num_vuelo,vuelo) || !abb_guardar_fecha(abb,fecha,num_vuelo)){
            errores = true;
        }
        free_strv(datos);
    }
    free(linea);
    fclose(archivo);
    return true;
}

bool ver_tablero(abb_t* abb,  hash_t* hash, int cant_vuelos, char* param_modo, char* desde, char* hasta){
    if (abb_cantidad(abb) == 0){
        return true;
    }
    lista_t* lista = lista_crear();
    if (!lista) return false;
    bool modo_asc = strcmp(param_modo,ASC) == 0;   // true: asc or false: desc
  
    abb_iter_rango_t* iter_global = abb_iter_rango_in_crear(abb,desde,hasta);
    if (!iter_global){
        lista_destruir(lista,NULL);
        return false;
    }

    bool errores = false;
    while(!errores && !abb_iter_rango_in_al_final(iter_global)){

        // Entro en el sub-árbol de la clave y lo recorro
        abb_t* sub_arbol = abb_iter_rango_in_ver_actual_dato(iter_global);
        abb_iter_t* iter_local = abb_iter_in_crear(sub_arbol);

        while (!abb_iter_in_al_final(iter_local)){
            if (modo_asc && lista_largo(lista) == cant_vuelos)   break;

            const char* num_vuelo = abb_iter_in_ver_actual(iter_local);
            vuelo_t* vuelo = hash_obtener(hash,num_vuelo);
            if (modo_asc){
                if (!lista_insertar_ultimo(lista,vuelo)) errores = true;
            } else {
                if (!lista_insertar_primero(lista,vuelo)) errores = true;
            }

            if (!abb_iter_in_avanzar(iter_local)){
                lista_destruir(lista,vuelo_destruir);
                abb_iter_in_destruir(iter_local);
                abb_iter_rango_in_destruir(iter_global);
                return false;
            }
        }
        abb_iter_in_destruir(iter_local);
        if (!abb_iter_rango_in_avanzar(iter_global)){
            lista_destruir(lista,vuelo_destruir);
            abb_iter_rango_in_destruir(iter_global);
            return false;
        }
    }
    abb_iter_rango_in_destruir(iter_global);

    for (size_t i = 0; i < cant_vuelos && !lista_esta_vacia(lista); i++){
        imprimir_en_tablero(lista_borrar_primero(lista));
    }
    lista_destruir(lista,NULL);
    return !errores;
}

bool info_vuelo(hash_t* hash, char* num_vuelo){
    if (hash_cantidad(hash) == 0)   return false;
    if (!hash_pertenece(hash, num_vuelo)) return false;

    vuelo_t* vuelo = hash_obtener(hash,num_vuelo);
    imprimir_datos_vuelo(vuelo);
    return true;
}

bool prioridad_vuelos(hash_t* hash, int k){
    if (hash_cantidad(hash) == 0)   return true;

    heap_t* heap = heap_crear(cmp_prioridad_vuelo); // de mínimos
    if (!heap)  return false;

    hash_iter_t* hash_iter = hash_iter_crear(hash);
    if (!hash_iter){
        heap_destruir(heap,NULL);
        return false;
    }
    vuelo_t* vuelo;
    const char* clave_vuelo;

    for (size_t i = 0; i < k && !hash_iter_al_final(hash_iter); i++){
        clave_vuelo = hash_iter_ver_actual(hash_iter);
        vuelo = hash_obtener(hash, clave_vuelo);
        if (!heap_encolar(heap, vuelo) || !hash_iter_avanzar(hash_iter)){
            heap_destruir(heap,NULL);
            hash_iter_destruir(hash_iter);
            return false;
        }
    }
    vuelo_t* min;
    while (!hash_iter_al_final(hash_iter)){
        clave_vuelo = hash_iter_ver_actual(hash_iter);
        vuelo = hash_obtener(hash, clave_vuelo);
        min = heap_ver_tope(heap);

        if (cmp_prioridad_vuelo(vuelo,min) > 0){
            heap_desencolar(heap);
            if (!heap_encolar(heap, vuelo)){
                heap_destruir(heap,NULL);
                hash_iter_destruir(hash_iter);
                return false;
            }
        }

        if (!hash_iter_avanzar(hash_iter)){
            heap_destruir(heap,NULL);
            hash_iter_destruir(hash_iter);
            return false;
        }
    }

    hash_iter_destruir(hash_iter);

    lista_t* lista = lista_crear();
    if (!lista){
        heap_destruir(heap,NULL);
        return false;
    }

    while (!heap_esta_vacio(heap)){
        vuelo = heap_desencolar(heap);
        if (!lista_insertar_primero(lista, vuelo)){
            heap_destruir(heap,NULL);
            lista_destruir(lista,NULL);
            return false;
        };
    }

    heap_destruir(heap,NULL);

    while (!lista_esta_vacia(lista)){
        imprimir_prioridad(lista_borrar_primero(lista));
    }

    lista_destruir(lista,NULL);
    return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* desde, char* hasta){
    if (hash_cantidad(hash) == 0)   return true;

    lista_t* lista_fechas = obtener_claves_abb(abb,desde,hasta);
    if (!lista_fechas)  return false;

    lista_t* lista_vuelos = borrar_fechas_abb(abb,lista_fechas);
    if (!lista_vuelos){
        lista_destruir(lista_fechas,free);
        return false;
    }
    borrar_vuelos_hash(hash,lista_vuelos);
    return true;
}
