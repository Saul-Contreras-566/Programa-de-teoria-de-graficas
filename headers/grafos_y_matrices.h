#ifndef GRAFOS

/*************************************************
UTILIDADES.

Aquí se encuentran las definiciones de las
estructuras de grafos, vértices y líneas que
usaremos en todo el programa.
*************************************************/



// VALORES PARA CLASIFICACIONES

#define VERTICE_AISLADO   1 
#define VERTICE_INICIAL   2
#define VERTICE_FINAL     4
#define VERTICE_COLGANTE  4

#define LINEA_PARALELA  1
#define LINEA_BUCLE     2
#define LINEA_SERIE     4

#define DIGRAFO                1 // 2^0
#define GRAFO_GENERAL          2 // 2^1
#define GRAFO_NULO             4 // 2^2
#define GRAFO_CONECTADO        8 // 2^3
#define GRAFO_REGULAR         16 // 2^4
#define GRAFO_COMPLETO        32 // 2^5
#define GRAFO_ARBOL           64 // 2^6
#define DIGRAFO_SIMETRICO    128 // 2^7
#define DIGRAFO_BALANCEADO   256 // 2^8
#define GRAFO_EULERIANO      512 // 2^9
#define GRAFO_UNICURSAL     1024 // 2^10



// ESTRUCTURAS

typedef struct {
	char *vertice_inicial;
	char *vertice_final;
} Relacion;

typedef struct {
	char                *nombre;
	unsigned int         grado_interno;
	unsigned int         grado_externo;
	unsigned short int   clasificacion;
	float                posicion_x;
	float                posicion_y;
} Vertice;

typedef struct {
	char                *nombre;
	Vertice             *origen;
	Vertice             *destino;
	unsigned int         grupo_de_paralelas;
	unsigned short int   clasificacion;
} Linea;

typedef struct {
	unsigned int         numero_de_vertices;
	unsigned int         numero_de_lineas;
	Vertice             *vertices;
	Linea               *lineas;
	unsigned int         numero_de_grupos_de_paralelas;
	unsigned short int   clasificacion;
} Grafo;

typedef struct {
	unsigned int   filas;
	unsigned int   columnas;
	int           *entrada;
} Matriz;



// FUNCIONES Y MACROS

#define MATRIZ_ENTRADA(matriz, fila, columna) (matriz).entrada[fila*(matriz).columnas + columna]

void Leer_relaciones (Grafo *grafo);
void Obtener_matrices (Grafo grafo, Matriz *matriz_de_incidencia, Matriz *matriz_de_accesibilidad, Matriz *matriz_de_adyacencia);
void Clasificar (Grafo *grafo, Matriz matriz_de_adyacencia, Matriz matriz_de_accesibilidad);

#endif
