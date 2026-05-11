#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utilidades.h"

typedef struct{
int origen;
int destino;
}   Linea;

int** GenerarMatrizAdyacencia(int numVertices,int numLineas, Linea* lineas, bool esDirigida){
    
int** AG;
int i;

    AG = (int **)calloc(numVertices, sizeof(int *));
    if (AG == NULL) return NULL;

for(i=0; i<numVertices; i++){
    
    AG[i] = (int *)calloc(numVertices, sizeof(int));
    if (AG[i] == NULL) return NULL;
}

for (int j = 0; j < numLineas; ++j) {
int u = lineas[j].origen;   
int v = lineas[j].destino;


AG[u][v] = 1;   
if (!esDirigida) {
            AG[v][u] = 1;
        }
        
// En la matriz, habria un uno en la interseccion entre U y v
/*
  U V  matriz[u][v] = 1; 
U 0 1
V 1 0  matriz[v][u] = 1;
*/
}


return AG;
}
