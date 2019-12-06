#define _POSIX_C_SOURCE 200809L
#define AGREGAR_ARCHIVO "agregar_archivo"
#define VER_TABLERO "ver_tablero"
#define INFO_VUELOS "info_vuelos"
#define PRIORIDAD_VUELOS "prioridad_vuelos"
#define BORRAR_VUELOS "borrar"
#define ASC "asc"
#define DESC "desc"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "heap.h"
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
    /* Imprime un mensaje de error conteniendo el comando indicado*/
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

char* rstrip(char* s){
    /* Borra el /n de la linea s*/
    size_t tam;
    char *fin;

    if (!s)     return NULL;

    tam = strlen(s);

    if (!tam)  return s;

    fin = s + tam - 1;
    if (*(fin) == '\n'){
        *(fin) = '\0';
    }
    return s;
}

bool quedan_vuelos(abb_t* abb){
    /* Devuelve true si no quedan vuelos sin borrar o false de manera contraria*/
    return abb_cantidad(abb) != 0;
}

bool igual_comando(const char* a, const char* b){
    /* Compara si los dos comandos pasados por parámetro son iguales y si
    es así, devuelve true. De lo contrario, devuelve false.*/
    return strcmp(a,b) == 0;
}

bool comando_valido(int cant_elem, char* linea[], int operacion){
    /* Verifica que al comando ingresado por stdin le corresponda
    la cantidad de parámetros que necesita según cuál sea. Devuelve true si
    la cantidad de parámetros ingresada es válida y false de lo contrario*/
    if (!(cant_elem > 1))  return false;

    // condiciones que debe cumplir el comando según cuál sea
    if ( (operacion == AGREGAR || operacion == INFO || operacion == PRIORIDAD) && cant_elem != 2){
        return false;
    }
    if (operacion == VER){
        if (cant_elem != 5 || !es_natural(linea[1])) return false;
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
    /* Crea un vuelo a partir de los datos de un puntero a char* pasado por parámetro*/
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
    /* Devuelve el tamaño del vector*/
    int i = 0;
    while (vector[i] != NULL){
        i++;
    }
    return i;
}

void imprimir_datos_vuelo(vuelo_t* vuelo){
    /* Imprime todos los datos del vuelo en una sola línea*/
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
    /* Imprime la prioridad junto con el número de vuelo */
    fprintf(stdout,"%s - %s\n",vuelo->prioridad,vuelo->numero_vuelo);
}

int cmp_prioridad_vuelo(const void* void_a, const void* void_b){
    vuelo_t* vuelo_a = (vuelo_t*) void_a;
    vuelo_t* vuelo_b = (vuelo_t*) void_b;

    if (vuelo_a->prioridad == vuelo_b->prioridad){
        return vuelo_a->numero_vuelo > vuelo_b->numero_vuelo ? 1 : -1;
    }
    return vuelo_a->prioridad > vuelo_b->prioridad ? 1 : -1;
}


void buscar_guardar(lista_t* lista, bool modo_asc, nodo_abb_t* raiz, char* desde, char* hasta){ 
   if (!raiz) return;
    
    nodo_abb_t* proximo;
    vuelo_t* vuelo = (vuelo_t*) abb_ver_dato_nodo(raiz);

    if (cmp_fechas(desde, vuelo->fecha) < 0){
        proximo = (nodo_abb_t*) abb_nodo_ver_izq(raiz);
        buscar_guardar(lista, modo_asc, proximo, desde, hasta); 
    }

    if (cmp_fechas(desde,vuelo->fecha)  <= 0 && cmp_fechas(hasta, vuelo->fecha) >= 0){
        if (modo_asc){
            lista_insertar_ultimo(lista,raiz);
        } else {
            lista_insertar_primero(lista, raiz);
        }
    }

    if ( cmp_fechas(hasta, vuelo->fecha) > 0){
        proximo = (nodo_abb_t*) abb_nodo_ver_der(raiz);
        buscar_guardar(lista, modo_asc, proximo, desde, hasta); 
    }
}

int cmp_fechas(const void* void_a, const void* void_b){
    char* fecha_a = (char*) void_a;
    char* fecha_b = (char*) void_b;
    return strcmp(fecha_a,fecha_b);
}

void imprimir_en_tablero(vuelo_t* vuelo){
    fprintf(stdout, "%s - %s\n",vuelo->fecha, vuelo->numero_vuelo);
}


bool ejecutar_comando(char** comando, hash_t* hash, abb_t* abb){
    int cant_elem = len_vector(comando);
    char* operacion = comando[0];

    if (igual_comando(AGREGAR_ARCHIVO,operacion)){
        if (comando_valido(cant_elem,comando,AGREGAR)){
            return agregar_archivo(abb,hash,comando[1]);
        }
    }
    else if (igual_comando(VER_TABLERO,operacion)){
        if (comando_valido(cant_elem,comando,VER)){
            return ver_tablero(abb,hash,atoi(comando[1]),comando[2],comando[3],comando[4]);
        }
    }
    else if (igual_comando(INFO_VUELOS,operacion)){
        if (comando_valido(cant_elem,comando,INFO)){
            return info_vuelo(hash,comando[1]);
        }
    }
    else if (igual_comando(PRIORIDAD_VUELOS,operacion)){
        if (comando_valido(cant_elem,comando,PRIORIDAD)){
            return prioridad_vuelos(hash,atoi(comando[1]));
        }
    }
    else if (igual_comando(BORRAR_VUELOS,operacion)){
        if (comando_valido(cant_elem,comando,AGREGAR)){
            return borrar(abb,hash,comando[1],comando[2]);
        }
    }          
    return false;   
}

/*************************
 * FUNCIONES PRINCIPALES
 * **********************/

bool agregar_archivo(abb_t* abb, hash_t* hash, char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo,"r");
    if (!archivo){
        return false;
    }
    
    char* linea = NULL;
	size_t n = 0;
    while ((getline(&linea, &n, archivo)) > 0){
        char** datos = split(linea,',');
        if (!datos) return false;
        remover_salto_linea(datos);
        vuelo_t* vuelo = vuelo_crear(datos);
        if (!vuelo){
            free_strv(datos);
            return false;
        }
        free_strv(datos);
        hash_guardar(hash,vuelo->numero_vuelo,vuelo);
        abb_guardar(abb,vuelo->fecha,vuelo->numero_vuelo);
    }
    fclose(archivo);
    return true;
}

bool ver_tablero(abb_t* abb,  hash_t* hash, int cant_vuelos, char* param_modo, char* desde, char* hasta){
    if (abb_cantidad(abb) == 0){
        mensaje_exito();
        return true;
    }

    lista_t* lista = lista_crear();
    if (!lista) return false;
    bool modo = strcmp(param_modo,"asc") == 0;   // true: asc or false: desc
    
    nodo_abb_t* raiz = abb_ver_raiz(abb);
    buscar_guardar(lista, modo, raiz, desde, hasta);
    if (lista_esta_vacia(lista))    return false;

    while (!lista_esta_vacia(lista)){
        imprimir_en_tablero(lista_borrar_primero(lista));
    }
    lista_destruir(lista,vuelo_destruir);

    mensaje_exito();
    return true;

}

bool info_vuelo(hash_t* hash, char* num_vuelo){
    
    if (hash_cantidad(hash) == 0)   return false;
    if (!hash_pertenece(hash, num_vuelo)) return false;

    vuelo_t* vuelo = hash_obtener(hash,num_vuelo);
    imprimir_datos_vuelo(vuelo);
    mensaje_exito();
    return true;
}

bool prioridad_vuelos(hash_t* hash, int k){
    heap_t* heap = heap_crear(cmp_prioridad_vuelo);
    if (!heap)  return false;

    hash_iter_t* hash_iter = hash_iter_crear(hash);
    if (!hash_iter){
        heap_destruir(heap,vuelo_destruir);
        return false;
    }

    vuelo_t* vuelo;
    const char* clave_vuelo;

    for (size_t i = 0; i < k; i++){
        clave_vuelo = hash_iter_ver_actual(hash_iter);
        vuelo = hash_obtener(hash, clave_vuelo);
        heap_encolar(heap, vuelo);

        if (!hash_iter_avanzar(hash_iter)){
            heap_destruir(heap,vuelo_destruir);
            hash_iter_destruir(hash_iter);
            return false;
            }
    }

    for (size_t i = k; !hash_iter_al_final(hash_iter); i++){
        clave_vuelo = hash_iter_ver_actual(hash_iter);
        vuelo = hash_obtener(hash, clave_vuelo);
        if (cmp_prioridad_vuelo(vuelo,heap_ver_tope(heap))){
            heap_desencolar(heap);
            heap_encolar(heap, vuelo);
        }
        if (!hash_iter_avanzar(hash_iter)){
            heap_destruir(heap,vuelo_destruir);
            hash_iter_destruir(hash_iter);
            return false;
            }
    }

    lista_t* lista = lista_crear();
    if (!lista){
        heap_destruir(heap,vuelo_destruir);
        hash_iter_destruir(hash_iter);
        return false;
    }

    while (!heap_esta_vacio(heap)){
        vuelo = heap_ver_tope(heap);
        lista_insertar_ultimo(lista, vuelo);
    }

    heap_destruir(heap,vuelo_destruir);
    hash_iter_destruir(hash_iter);

    while (!lista_esta_vacia(lista)){
        imprimir_prioridad(lista_borrar_primero(lista));
    }

    lista_destruir(lista,vuelo_destruir);
    mensaje_exito();
    return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* desde, char* hasta){
    abb_iter_t* iter = abb_iter_in_crear(abb,desde,hasta);
    if (!iter) return false;

    const char* actual = abb_iter_in_ver_actual(iter);

    while (!abb_iter_in_al_final(iter) && strcmp(actual,desde) < 0){
        abb_iter_in_avanzar(iter);
        actual = abb_iter_in_ver_actual(iter);
    }
    while (!abb_iter_in_al_final(iter) && !(strcmp(actual,hasta) > 0)){
        char* num_vuelo = abb_borrar(abb,actual);
        vuelo_t* vuelo = hash_borrar(hash,num_vuelo);
        imprimir_datos_vuelo(vuelo);
        abb_iter_in_avanzar(iter);
        actual = abb_iter_in_ver_actual(iter);
        vuelo_destruir(vuelo);
    }
    abb_iter_in_destruir(iter);
    return true;
}
