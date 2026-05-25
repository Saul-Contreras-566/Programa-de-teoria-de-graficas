/*************************************************
FUNCIONES PARA LA LECTURA DE LAS RELACIONES.
*************************************************/

#include "funciones_basicas.h"
#include "grafos_y_matrices.h"
#include "string.h"





Vertice *Buscar_vertice (Grafo *grafo, int numero_de_vertices, char *nombre) {
	/* Esta función devuelve la dirección de memoria
	del vértice encontrado. Si no se encuentra, se
	devuelve NULL.*/

	int i;

	for (i = 0; i < numero_de_vertices; i++)
		if (strcmp (nombre, (*grafo).vertices[i].nombre) == 0)
			return &(*grafo).vertices[i];
	
	return NULL;
}

char *Leer_nombre_de_vertice (Grafo *grafo, int leidos) {
	int existe;
	char *nombre;

	do {
		existe = 0;
		Leer_cadena (&nombre);
		if (Buscar_vertice (grafo, leidos, nombre) != NULL) {
			existe = 1;
			free (nombre);
			break;
		}
	} while (existe == 1);

	return nombre;
}

Vertice *Seleccionar_vertice (Grafo *grafo) {
	char *nombre;
	Vertice *seleccionado;
	
	do {
		Leer_cadena (&nombre);
		seleccionado = Buscar_vertice (grafo, (*grafo).numero_de_vertices, nombre);
		free (nombre);
		if (seleccionado == NULL)
			puts ("Error: No existe un vértice con este nombre.");
	} while (seleccionado == NULL);

	return seleccionado;
}

void Leer_relaciones (Grafo *grafo) {
	unsigned int i;
	
	// Leyendo el número de vértices en el grafo.
	puts ("\nInserte el número de vértices del grafo (un número mayor o igual que cero):");
	(*grafo).numero_de_vertices = Leer_entero_que_sea (">=", 0);

	// Leyendo el nombre de los vértices.
	(*grafo).vertices = (Vertice *) malloc (sizeof(Vertice) * (*grafo).numero_de_vertices);
	puts ("\nInserte el nombre de los vértices:");
	for (i = 0; i < (*grafo).numero_de_vertices; i++) {
		printf ("Inserte el nombre del vértice %d:\n", i+1);
		(*grafo).vertices[i].nombre = Leer_nombre_de_vertice (grafo, i);
	}

	// Leyendo el número de líneas en el grafo.
	puts ("\nInserte el número de líneas del grafo (un número mayor o igual que cero):");
	(*grafo).numero_de_lineas = Leer_entero_que_sea (">=", 0);

	// Leyendo relaciones.
	(*grafo).lineas = (Linea *) malloc (sizeof(Linea) * (*grafo).numero_de_lineas);
	for (i = 0; i < (*grafo).numero_de_lineas; i++) {
		printf ("\n>>>Línea %d:\n", i+1);

		puts ("Inserte el nombre del vértice inicial:");
		(*grafo).lineas[i].origen = Seleccionar_vertice (grafo);

		puts ("Inserte el nombre del vértice final:");
		(*grafo).lineas[i].destino = Seleccionar_vertice (grafo);
	}
}
