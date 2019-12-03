#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "funciones.h"
#include "lista.h"
#define AGREGAR_ARCHIVO "agregar_archivo"
#define VER_TABLERO "ver_tablero"
#define INFO_VUELOS "info_vuelos"
#define PRIORIDAD_VUELOS "prioridad_vuelos"
#define BORRAR "borrar"
#define ASC "asc"
#define DESC "desc"

typedef struct vuelo{
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
} vuelo_t;


/*************************
 * FUNCIONES AUXILIARES
 * **********************/
char* rstrip(char* s){
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

bool solicitar_linea(char* linea){
    printf("Ingrese un comando: \n");
    gets(linea);
    linea = rstrip(linea);
    return linea != NULL;
}

bool quedan_vuelos(abb_t* abb){
    return abb_cantidad(abb) != 0;
}

bool igual_comando(const char* a, const char* b){
    return strcmp(a,b) == 0
}

bool comando_valido(int cant_elem, char* linea[]){   return -1;
    if (cant_elem == 0)  return false;

    if !(cant_elem == 1){
        mensaje_error(linea[0]);
        return false;
    }

    const char* comando = linea[0];
    // condiciones que debe cumplir el comando según cuál sea
    if (igual_comando(comando,AGREGAR_ARCHIVO) || igual_comando(comando,INFO_VUELOS) || igual_comando(comando,PRIORIDAD_VUELOS) && cant_elem>=2){
        return true;
    }
    if (igual_comando(comando, BORRAR) && cant_elem >= 3)     return true;
    if (igual_comando(comando,  VER_TABLERO) && cant_elem >= 5)  return true;

    mensaje_error(comando);
    return false;
}

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

int len_vector(char** c_input){
    int i = 0;
    while (c_input[i] != NULL){
        i++;
    }
    return i;
}

void imprimir_datos_vuelo(vuelo_t* vuelo){
    printf("%s ",vuelo->numero_vuelo);
    printf("%s ",vuelo->aerolinea);
    printf("%s ",vuelo->aeropuerto_origen);
    printf("%s ",vuelo->aeropuerto_destino);
    printf("%s ",vuelo->matricula);
    printf("%s ",vuelo->prioridad);
    printf("%s ",vuelo->fecha);
    printf("%s ",vuelo->retraso_salida);
    printf("%s ",vuelo->tiempo_vuelo);
    printf("%s\n",vuelo->cancelado);
}

void imprimir_prioridad(vuelo_t* vuelo){
    printf("%s - %s\n",vuelo->prioridad,vuelo->numero_vuelo);
}
/*************************
 * FUNCIONES PRINCIPALES
 * **********************/

int main(){
    
    printf("Bienvenido/a\n");

    char* linea;
    if (!solicitar_linea(linea))    return -1;
    char** c_input = split(linea," ");

    int cant_elem = len_vector(c_input);
    if (!(comando_valido(cant_elem, c_input)))  return -1;

    char* comando = c_input[0];

    hash_t* hash = hash_crear();
    abb_t* abb = abb_crear(strcmp);
    bool hay_vuelos = true;

    while (comando || hay_vuelos){
        switch(comando){

	        case AGREGAR_ARCHIVO:
	        		char* nombre_arch = c_input[1];
                    agregar_archivo(comando,nombre_arch,hash,abb);

	        case VER_TABLERO:
	        		int cant_vuelos = atoi(c_input[1]);
                    char* modo = c_input[2];
                    char* desde = c_input[3];
                    char* hasta = c_input[4];
    
	        case INFO_VUELOS:
	        		int num_vuelo = atoi(c_input[1]);
                    info_vuelo(hash,num_vuelo,comando);

	        case PRIORIDAD_VUELOS:
                int cant_vuelos = atoi(c_input[1]);
                void prioridad_vuelos(char* comando, int k, abb_t* abb)

            case BORRAR:
	        		char* desde = c_input[1];
                    char* hasta = c_input[2];
            default:
                    mensaje_error(comando);
            }

        solicitar_linea(linea);
        char** c_input = split(linea," ");
        char* comando = c_input[0];
        hay_vuelos = quedan_vuelos(abb);
    }  
    hash_destruir(hash);
    abb_destruir(abb);
}

void agregar_archivo(char* comando, char* nombre, hash_t* hash, abb_t* abb){
    //if (!comando[1] || !comando[2]){
    //    mensaje_error(comando[0]);
    //    return;
    //}
    
    FILE* archivo = fopen(nombre,"r");
    if (!archivo){
        mensaje_error(comando);
        return;
    }
    
    char* linea = NULL;
	size_t n = 0;

    while ((getline(&linea, &n, archivo)) > 0){
        char** datos = split(linea,',');
        if (!datos){
            mensaje_error(comando);
            break;
        }
        vuelo_t* vuelo = vuelo_crear(datos);
        if (!vuelo){
            mensaje_error(comando);
            free_strv(datos);
            break;
        }
        free(datos);
        abb_guardar(abb,vuelo->fecha,vuelo);
        hash_guardar(hash,vuelo->numero_vuelo,vuelo);
    }
    fclose(archivo);
}

void ver_tablero(char** comando, abb_t* abb){}

void info_vuelo(hash_t* hash, char* num_vuelo, char* comando){
    
    if (hash_cantidad(hash) == 0){
        printf("OK\n");
        return;
    }
    if (!hash_pertenece(num_vuelo)){
        mensaje_error(comando);
        return;
    }
    vuelo_t* vuelo = hash_obtener(hash,num_vuelo);
    imprimir_datos_vuelo(vuelo);
    printf("OK\n");
}

void prioridad_vuelos(char* comando, int k, abb_t* abb){
    if (abb_cantidad(abb) == 0){
        printf("OK\n");
        return;
    }   
    lista_t* lista = lista_crear();
    if (!lista){
        mensaje_error(comando);
        return;
    }
    abb_iter_t* iterador = abb_iter_in_crear(abb);
    vuelo_t* actual;
    vuelo_t* siguiente;

    actual = abb_obtener(abb, abb_iter_in_ver_actual(iterador));
    int restantes = k ;

    while (restantes > 0){
        if (restantes == 1){
            lista_insertar_ordenado(lista,actual);
            break;
        }
            
        abb_iter_in_avanzar(iterador);
        siguiente = abb_obtener(abb, abb_iter_in_ver_actual(iterador));

        if (actual->prioridad != siguiente->prioridad){
            
            if (!lista_esta_vacia(lista)){
                lista_insertar_ordenado(lista,actual);
            } else {
                imprimir_prioridad(actual);
            }

            while (!lista_esta_vacia(lista)){
                imprimir_prioridad(lista_borrar_primero(lista))
                }
            
        } else {
            lista_insertar_ordenado(lista,actual);
            
        }
        actual = siguiente;
        restantes--;
    }

    while (!lista_esta_vacia(lista)){
        imprimir_prioridad(lista_borrar_primero(lista))
    }

    lista_destruir(lista);
    abb_iter_in_destruir(iterador);
    printf("OK\n");
}

void borrar(char** comando,hash_t* hash, abb_t* abb){}

//void ejecutar_comando(char** comando, hash_t* hash, abb_t* abb){
//    if ((strcmp(comando[0],"agregar_archivo")) == 0) agregar_archivo(comando,hash,abb);
//    else if ((strcmp(comando[0],"ver_tablero")) == 0) ver_tablero(comando,abb);
//    else if ((strcmp(comando[0],"info_vuelo")) == 0) info_vuelo(comando,hash);
//    else if ((strcmp(comando[0],"prioridad_vuelos")) == 0) prioridad_vuelos(comando,abb);
//    else if ((strcmp(comando[0],"borrar") == 0)) borrar(comando,hash,abb);
//    else{ mensaje_error(comando[0]);}
//}
