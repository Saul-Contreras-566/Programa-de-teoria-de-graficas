#include <stdio.h>

// validación enteros
int validar_entero(const char *cadena) {
    if (cadena == NULL || cadena[0] == '\0') return 0;
    int i = 0;
    if (cadena[0] == '-' || cadena[0] == '+') {
        i = 1;
        if (cadena[1] == '\0') return 0;
    }
    while (cadena[i] != '\0') {
        if (cadena[i] < '0' || cadena[i] > '9') return 0; 
        i++;
    }
    return 1;
}

// validación reales
int validar_real(const char *cadena) {
    if (cadena == NULL || cadena[0] == '\0') return 0;
    int i = 0;
    int puntos_decimales = 0;
    if (cadena[0] == '-' || cadena[0] == '+') {
        i = 1;
        if (cadena[1] == '\0') return 0;
    }
    while (cadena[i] != '\0') {
        if (cadena[i] == '.') {
            puntos_decimales++;
            if (puntos_decimales > 1) return 0;
        } else if (cadena[i] < '0' || cadena[i] > '9') {
            return 0; 
        }
        i++;
    }
    return 1;
}

// validación vertices 
int validar_vertice(int vertice) {
    int max_vertices = 20;
    if (vertice >= 0 && vertice < max_vertices) {
        return 1;
    }
    return 0;
}

