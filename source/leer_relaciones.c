/*************************************************
FUNCIONES PARA LA LECTURA DE LAS RELACIONES.
*************************************************/

#include <string.h>
#include "funciones_basicas.h"
#include "grafos_y_matrices.h"





Vertice *Buscar_vertice (int numero_de_vertices, char *nombre) {
	/* Esta función devuelve la dirección de memoria
	del vértice encontrado. Si no se encuentra, se
	devuelve NULL.*/

	int i;

	for (i = 0; i < numero_de_vertices; i++)
		if (strcmp (nombre, grafo.vertices[i].nombre) == 0)
			return &grafo.vertices[i];
	
	return NULL;
}

Linea *Buscar_linea (int numero_de_lineas, char *nombre) {
	/* Esta función devuelve la dirección de memoria
	del la linea encontrado. Si no se encuentra, se
	devuelve NULL.*/

	int i;

	for (i = 0; i < numero_de_lineas; i++)
		if (strcmp (nombre, grafo.lineas[i].nombre) == 0)
			return &grafo.lineas[i];
	
	return NULL;
}

char *Nombre_por_defecto (int numero) {
	numero++;
	char *nombre = (char *) malloc (sizeof (char) * (((int) log10 ((double) numero)) + 2));
	sprintf (nombre, "%d", numero);
	printf ("Nombre usado: '%s'.\n", nombre);
	return nombre;
}

char *Leer_nombre_de_vertice (int leidos) {
	int existe;
	char *nombre;

	do {
		existe = 0;
		Leer_cadena (&nombre);
		if (strcmp (nombre, "\0") == 0) {
			nombre = Nombre_por_defecto (leidos);
			break;
		} else if (Buscar_vertice (leidos, nombre) != NULL) {
			existe = 1;
			free (nombre);
			break;
		}
	} while (existe == 1);

	return nombre;
}

char *Leer_nombre_de_linea (int leidos) {
	int existe;
	char *nombre;

	do {
		existe = 0;
		Leer_cadena (&nombre);
		if (strcmp (nombre, "\0") == 0) {
			nombre = Nombre_por_defecto (leidos);
			break;
		} else if (Buscar_linea (leidos, nombre) != NULL) {
			existe = 1;
			free (nombre);
			break;
		}
	} while (existe == 1);

	return nombre;
}

Vertice *Seleccionar_vertice () {
	char *nombre;
	Vertice *seleccionado;
	
	do {
		Leer_cadena (&nombre);
		seleccionado = Buscar_vertice (grafo.numero_de_vertices, nombre);
		free (nombre);
		if (seleccionado == NULL)
			puts ("Error: No existe un vértice con este nombre.");
	} while (seleccionado == NULL);

	return seleccionado;
}

void Leer_relaciones () {
	unsigned int i;
	char mayor_o_igual_que[] = ">=";
	
	// Leyendo el número de vértices en el grafo.
	puts ("\nInserte el número de vértices del grafo (un número mayor o igual que cero):");
	grafo.numero_de_vertices = Leer_entero_que_sea (mayor_o_igual_que, 0);

	// Leyendo el nombre de los vértices.
	grafo.vertices = (Vertice *) malloc (sizeof(Vertice) * grafo.numero_de_vertices);
	if (grafo.numero_de_vertices != 0) {
		puts ("\nInserte el nombre de los vértices:");
		for (i = 0; i < grafo.numero_de_vertices; i++) {
			printf ("Inserte el nombre del vértice %d (nombre por defecto si no inserta nada: %d):\n", i+1, i+1);
			grafo.vertices[i].nombre = Leer_nombre_de_vertice (i);
		}

		// Leyendo el número de líneas en el grafo.
		puts ("\nInserte el número de líneas del grafo (un número mayor o igual que cero):");
		grafo.numero_de_lineas = Leer_entero_que_sea (mayor_o_igual_que, 0);
	} else
		grafo.numero_de_lineas = 0;

	// Leyendo relaciones.
	grafo.lineas = (Linea *) malloc (sizeof(Linea) * grafo.numero_de_lineas);
	for (i = 0; i < grafo.numero_de_lineas; i++) {
		grafo.lineas[i].dibujado = 0;
		printf ("\n>>>Línea %d:\n", i+1);

		printf ("Inserte el nombre o etiqueta de la línea (nombre por defecto si no inserta nada: %d):\n", i+1);
		grafo.lineas[i].nombre = Leer_nombre_de_linea (i);

		puts ("Inserte el nombre del vértice inicial:");
		grafo.lineas[i].origen = Seleccionar_vertice ();

		puts ("Inserte el nombre del vértice final:");
		grafo.lineas[i].destino = Seleccionar_vertice ();
	}
}
