#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "funciones.h"
#include "lista.c"
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

bool solicitar_linea(char* linea){
    /* Solicita al usuario que ingrese una linea, le quita el caracter de fin de línea
    y guarda la misma en el char* pasado por parámetro*/
    printf("Ingrese un comando: \n");
    gets(linea);
    linea = rstrip(linea);
    return linea != NULL;
}

bool quedan_vuelos(abb_t* abb){
    /* Devuelve true si no quedan vuelos sin borrar o false de manera contraria*/
    return abb_cantidad(abb) != 0;
}

bool igual_comando(const char* a, const char* b){
    /* Compara si los dos comandos pasados por parámetro son iguales y si
    es así, devuelve true. De lo contrario, devuelve false.*/
    return strcmp(a,b) == 0
}

bool comando_valido(int cant_elem, char* linea[]){
    /* Verifica que al comando ingresado por stdin le corresponda
    la cantidad de parámetros que necesita según cuál sea. Devuelve true si
    la cantidad de parámetros ingresada es válida y false de lo contrario*/
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

void mensaje_error(char* comando){
    /* Imprime un mensaje de error conteniendo el comando indicado*/
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
    fprintf(stdout,("%s ",vuelo->numero_vuelo);
    fprintf(stdout,("%s ",vuelo->aerolinea);
    fprintf(stdout,("%s ",vuelo->aeropuerto_origen);
    fprintf(stdout,("%s ",vuelo->aeropuerto_destino);
    fprintf(stdout,("%s ",vuelo->matricula);
    fprintf(stdout,("%s ",vuelo->prioridad);
    fprintf(stdout,("%s ",vuelo->fecha);
    fprintf(stdout,("%s ",vuelo->retraso_salida);
    fprintf(stdout,("%s ",vuelo->tiempo_vuelo);
    fprintf(stdout,("%s\n",vuelo->cancelado);
}

void imprimir_prioridad(vuelo_t* vuelo){
    /* Imprime la prioridad junto con el número de vuelo */
    fprintf(stdout,"%s - %s\n",vuelo->prioridad,vuelo->numero_vuelo);
}

bool lista_insertar_ordenado(lista_t* lista, vuelo_t* vuelo){
    /* Inserta en una lista de manera ordenada comparando los números de vuelo*/    
    nodo_t* nodo = nodo_crear(vuelo);
    if (nodo == NULL) return false;

    lista_iter_t* iter = lista_iter_crear(lista);
    if (!iter)     return false;

    if (lista_esta_vacia(lista)){
        if (lista_iter_insertar(lista,nodo)){
            lista->longitud++;
            return true;
        }
        return false;
    }

    vuelo_t* actual;
    while (!lista_iter_al_final(iter)){
        actual = lista_iter_ver_actual(iter);
        if (vuelo->numero_vuelo > actual->numero_vuelo){
            if(!lista_iter_insertar(lista,nodo))    return false;
            break;
        }
        if (!lista_iter_avanzar(iter))  return false;
    }
    if (!lista_iter_insertar(lista,nodo))   return false;
    lista->longitud++;
    lista_iter_destruir(iter);
    return true;
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
    
	        case INFO_VUELO:
	        		int num_vuelo = atoi(c_input[1]);
                    info_vuelo(hash,num_vuelo,comando);

	        case PRIORIDAD_VUELOS:
                int cant_vuelos = atoi(c_input[1]);
                prioridad_vuelos(comando, cant_vuelos, abb);

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
        fprintf(stdout,("OK\n");
        return;
    }
    if (!hash_pertenece(num_vuelo)){
        mensaje_error(comando);
        return;
    }
    vuelo_t* vuelo = hash_obtener(hash,num_vuelo);
    imprimir_datos_vuelo(vuelo);
    fprintf(stdout,("OK\n");
}

void prioridad_vuelos(char* comando, int k, abb_t* abb){
    if (abb_cantidad(abb) == 0){
        fprintf(stdout,("OK\n");
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
    int restantes = k - 1;

    while (restantes >= 0){
        
        // Si queda un sólo elemento por analizar, se encola en la lista en caso de que
        // haya más elementos de igual prioridad con los que haya que ordenarlo.
        // Si no hay ninguno, no cambia nada y simplemente lo borro e imprimo al final.
        if (restantes == 0){
            lista_insertar_ordenado(lista,actual);
            break;
        }
            
        abb_iter_in_avanzar(iterador);
        siguiente = abb_obtener(abb, abb_iter_in_ver_actual(iterador));

        if (actual->prioridad == siguiente->prioridad){
            lista_insertar_ordenado(lista,actual);
        
        // distintas prioridades
        } else {
            // La lista no está vacía implica que "actual" tiene la misma prioridad que los vuelos de la lista
            // y tengo que guardarla para que se ordene con ellas
            if (!lista_esta_vacia(lista)){
                lista_insertar_ordenado(lista,actual);

            // Si la lista está vacía, simplemente se imprime la prioridad actual y se considera "aislada"
            } else {
                imprimir_prioridad(actual);
            }

            // Se vacía la lista ya que el nuevo actual tendrá un número de prioridad diferente al guardado
            while (!lista_esta_vacia(lista)){
                imprimir_prioridad(lista_borrar_primero(lista))
            }

        }
        // se actualiza el actual
        actual = siguiente;
        restantes--;
    }

    // Se vacían la lista en caso de aún contener elementos
    while (!lista_esta_vacia(lista)){
        imprimir_prioridad(lista_borrar_primero(lista))
    }

    lista_destruir(lista);
    abb_iter_in_destruir(iterador);
    fprintf(stdout,("OK\n");
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
