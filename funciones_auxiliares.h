#ifndef AUX_H
#define AUX_H

#include "abb.h"
#include "lista.h"

typedef struct vuelo vuelo_t;

void mensaje_exito(void);

/* Imprime un mensaje de error conteniendo el comando indicado*/
void mensaje_error(char* comando);

bool rango_valido(const char* inicio, const char* fin);

bool es_natural(const char* str);

char* rstrip(char* s);

bool quedan_vuelos(abb_t* abb);

/* Compara si los dos comandos pasados por parámetro son iguales y si
es así, devuelve true. De lo contrario, devuelve false.*/
bool igual_comando(const char* a, const char* b);

/* Verifica que al comando ingresado por stdin le corresponda
la cantidad de parámetros que necesita según cuál sea. Devuelve true si
la cantidad de parámetros ingresada es válida y false de lo contrario*/
bool comando_valido(int cant_elem, char* linea[], int operacion);

/* Crea un vuelo a partir de los datos de un puntero a char* pasado por parámetro*/
vuelo_t* vuelo_crear(char** datos);

void vuelo_destruir(void* mem_vuelo);

void remover_salto_linea(char** vector);

/* Devuelve el tamaño del vector*/
int len_vector(char** vector);

/* Imprime todos los datos del vuelo en una sola línea*/
void imprimir_datos_vuelo(vuelo_t* vuelo);

/* Imprime la prioridad junto con el número de vuelo */
void imprimir_prioridad(vuelo_t* vuelo);

int cmp_prioridad_vuelo(const void* void_a, const void* void_b);

/*Recibe por parámetro una lista, un modo de guardado, un ABB y dos fechas de cota.
Busca en el ABB aquellos vuelos que se encuentren dentro de la cota pedida y los agrega a la lista según 
si se pide imprimir de manera ascendente (lista_insertar_ultimo) o descendente (lista_insertar_primero)*/
void buscar_guardar(lista_t* lista, bool modo_asc, nodo_abb_t* raiz, char* desde, char* hasta);

int cmp_fechas(const void* void_a, const void* void_b);

void imprimir_en_tablero(vuelo_t* vuelo);

bool ejecutar_comando(char** comando, hash_t* hash, abb_t* abb);

bool imprimir_vuelo_fecha(const char* clave, void * dato, void * nada);

int cmp_fechas_abb(const char* fecha1, const char* fecha2);

#endif // AUX_H