#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *substr(const char *str, size_t n){
    char* sub_str = malloc(sizeof(char)*(n+1));
 
    if (sub_str == NULL) return NULL;
 
    strncpy(sub_str,str,n);
    sub_str[n] = '\0';
    return sub_str;
}

char **split(const char *str, char sep){
    int i = 0;
    int cont_str = 0;
    int cont_char = 0;

    char* vector_est[strlen(str)+1];

    while (true){
        if ((str[i] != sep) && (str[i] != '\0')){
            cont_char++;
            
        }else{
            cont_str++;
            char* n_str = malloc(sizeof(char)*(cont_char+1));
            strncpy(n_str,str+(i-cont_char),cont_char);
            n_str[cont_char] = '\0';
            vector_est[cont_str-1] = n_str;

            if (str[i] == '\0') break;
            cont_char = 0;
        }        
        i++;
    }

    char** vector_din = malloc(sizeof(char*)*(cont_str+1));

    for (int j = 0; j < cont_str; j++){
        vector_din[j] = vector_est[j];
    }

    vector_din[cont_str] = NULL;
    return vector_din;
}

char *join(char **strv, char sep){
    int x = 0;
    size_t tam = 0;
    
    while (strv[x]){
        tam += strlen(strv[x]);
        if (sep != '\0') tam++;
        x++;
    }

    if ( (sep == '\0') || (tam == 0) ) tam++;

    char* jstr = malloc(sizeof(char)*tam);

    int pos_arreglo = 0;
    int pos_jstr = 0;

    while(strv[pos_arreglo]){
        int pos_str = 0;
        while(strv[pos_arreglo][pos_str] != '\0'){
            jstr[pos_jstr] = strv[pos_arreglo][pos_str];
            pos_str++;
            pos_jstr++;
        }
        if (sep != '\0'){
            jstr[pos_jstr] = sep;
            pos_jstr++;
        }
        pos_arreglo++;
    }
    jstr[tam-1] = '\0';

    return jstr;
}
 
void free_strv(char *strv[]){
    int i = 0;
    while (strv[i]){
        free(strv[i]);
        i++;
    }
    free(strv);
}