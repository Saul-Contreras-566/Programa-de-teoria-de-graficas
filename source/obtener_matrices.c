/*************************************************
FUNCIONES PARA OBTENER MATRICES.
*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "grafos_y_matrices.h"





void Mostrar_matriz (Matriz matriz) {
	unsigned int i, j;

	for (i = 0; i < matriz.filas; i++) {
		for (j = 0; j < matriz.columnas; j++)
			printf ("|%2d", MATRIZ_ENTRADA (matriz, i, j));
		puts ("|");
	}
}

int *Inicializar_entradas (int numero_de_entradas) {
	int i;
	int *entrada;

	entrada = (int *) malloc (sizeof(int) * numero_de_entradas);
	for (i = 0; i < numero_de_entradas; i++)
		entrada[i] = 0;

	return entrada;
}

void Generar_matriz_de_incidencia () {	
	/*
	Esta funcion genera la matriz de incidencia. En
	las entradas de esta matriz, 1 significa que el
	vertice i es el vértice de origen de la línea j, 2
	significa que el vertice i es el vértice de
	destino de la línea j, 3 significa que el vertice
	i es el vértice tanto de origen como destino de la
	línea j.

	Para el caso de los grafos no dirigidos, basta que
	la entradas sean distinta de cero para indicar que
	la línea j incide determinados vértices.

	Para el caso de los grafos dirigidos, podemos
	distinguir el vértice de origen del de destino si
	las entradas toman el valor de 1 o 2
	respectivamente. Si toma el valor de 3, es porque
	la línea es un bucle.
	*/

	unsigned int i, j;

	matriz_de_incidencia.filas = grafo.numero_de_vertices;
	matriz_de_incidencia.columnas = grafo.numero_de_lineas;
	matriz_de_incidencia.entrada = Inicializar_entradas (grafo.numero_de_vertices * grafo.numero_de_lineas);

	for (j = 0; j < matriz_de_incidencia.columnas; j++)
		for (i = 0; i < matriz_de_incidencia.filas; i++) {
			if (grafo.lineas[j].origen == &grafo.vertices[i])
				MATRIZ_ENTRADA(matriz_de_incidencia, i, j) = 1;
			if (grafo.lineas[j].destino == &grafo.vertices[i])
				MATRIZ_ENTRADA(matriz_de_incidencia, i, j) += 2;
		}
}

void Generar_matriz_de_adyacencia () {
	/*
	La matriz manejará números a parte de 0 y 1 con
	tal de poder averiguar luego si un digrafo es
	simétrico o no. Así, las entradas de la matriz
	cuentan el número de líneas que van de un vértice
	a otro.
	*/

	unsigned int i, j, k;

	matriz_de_adyacencia.filas = grafo.numero_de_vertices;
	matriz_de_adyacencia.columnas = grafo.numero_de_vertices;
	matriz_de_adyacencia.entrada = Inicializar_entradas (grafo.numero_de_vertices * grafo.numero_de_vertices);

	for (k = 0; k < grafo.numero_de_lineas; k++)
		for (i = 0; i < grafo.numero_de_vertices; i++)
			for (j = 0; j < grafo.numero_de_vertices; j++)
				if (grafo.lineas[k].origen == &grafo.vertices[i] && grafo.lineas[k].destino == &grafo.vertices[j]) {
					MATRIZ_ENTRADA(matriz_de_adyacencia, i, j)++;
					if (!(grafo.clasificacion & DIGRAFO) && i != j)
						MATRIZ_ENTRADA(matriz_de_adyacencia, j, i)++;
				}
}

void Copiar_matriz (Matriz origen, Matriz *destino) {
	if (origen.entrada == (*destino).entrada)
		return;


	unsigned int i;

	(*destino).filas = origen.filas;
	(*destino).columnas = origen.columnas;
	if ((*destino).entrada != NULL)
		free ((*destino).entrada);
	(*destino).entrada = (int *) malloc (sizeof (int) * (*destino).filas * (*destino).columnas);

	for (i = 0; i < origen.filas * origen.columnas; i++)
		(*destino).entrada[i] = origen.entrada[i];
}

void Sumar_matrices (Matriz matriz_1, Matriz matriz_2) {
	unsigned int i;

	for (i = 0; i < matriz_1.filas * matriz_1.columnas; i++)
		matriz_1.entrada[i] += matriz_2.entrada[i];
}

void Multiplicar_matrices (Matriz matriz_1, Matriz matriz_2, Matriz *resultado) {
	unsigned int i, j, k;

	(*resultado).filas = matriz_1.filas;
	(*resultado).columnas = matriz_1.columnas;
	if ((*resultado).entrada != NULL)
		free ((*resultado).entrada);
	(*resultado).entrada = Inicializar_entradas ((*resultado).filas * (*resultado).columnas);

	for (i = 0; i < matriz_1.filas; i++) // i selecciona la fila.
		for (j = 0; j < matriz_1.columnas; j++) // j selecciona la columna.
			for (k = 0; k < matriz_1.columnas; k++) // k se usa para realizar sumas.
				MATRIZ_ENTRADA (*resultado, i, j) += MATRIZ_ENTRADA (matriz_1, i, k) * MATRIZ_ENTRADA (matriz_2, k, j);
}

void Generar_matriz_de_accesibilidad () {
	/*
	Copia la matriz de adyacencia y la eleva a
	potencias sucesivas.
	*/

	int i;
	Matriz copia_original, copia_potencias_1, copia_potencias_2;

	matriz_de_accesibilidad.entrada = NULL;
	copia_original.entrada = NULL;
	copia_potencias_1.entrada = NULL;
	copia_potencias_2.entrada = NULL;

	Copiar_matriz(matriz_de_adyacencia, &matriz_de_accesibilidad);
	Copiar_matriz(matriz_de_adyacencia, &copia_original);
	Copiar_matriz(matriz_de_adyacencia, &copia_potencias_1);

	// Elevando a potencias sucesivas la matriz de adyacencia (su copia en esta matriz).
	for (i = 0; i < (int) matriz_de_adyacencia.filas - 2; i++) {
		Multiplicar_matrices (copia_potencias_1, copia_original, &copia_potencias_2);
		Copiar_matriz (copia_potencias_2, &copia_potencias_1);
		Sumar_matrices (matriz_de_accesibilidad, copia_potencias_2);
	}

	free (copia_original.entrada);
	free (copia_potencias_1.entrada);
	free (copia_potencias_2.entrada);
}

void Imprimir_matriz (Matriz matriz, char *caso_0, char *caso_1, char *caso_2, char *caso_3, char *caso_4) {
	/* En esta función las variables de caso tienen
	un significado distinto dependiendo de la matriz
	que se vaya a imprimir. */

	unsigned int i, j;
	char *caso_seleccionado;

	for (i = 0; i < matriz.filas; i++) {
		for (j = 0; j < matriz.columnas; j++) {
			switch (MATRIZ_ENTRADA (matriz, i, j)) {
				case 0: caso_seleccionado = caso_0; break;
				case 1: caso_seleccionado = caso_1; break;
				case 2: caso_seleccionado = caso_2; break;
				case 3: caso_seleccionado = caso_3; break;
				default: caso_seleccionado = caso_4; break;
			}
			printf ("| %2s ", caso_seleccionado);
		}
		puts ("|");
	}
}





// FUNCIÓN GENERAL PARA OBTENER LAS MATRICES.

void Obtener_matrices () {
	Generar_matriz_de_incidencia ();
	Generar_matriz_de_adyacencia ();
	Generar_matriz_de_accesibilidad ();

	char cero[] = "0";
	char uno[] = "1";
	char menos_uno[] = "-1";
	char mas_menos_uno[] = "±1";
	char mas[] = "+";

	puts ("Matriz de incidencia");
	if (grafo.clasificacion & DIGRAFO)
		Imprimir_matriz (matriz_de_incidencia, cero, uno, menos_uno, mas_menos_uno, cero);
	else
		Imprimir_matriz (matriz_de_incidencia, cero, uno, uno, uno, cero);
	puts ("");

	puts ("Matriz de adyacencia");
	Imprimir_matriz (matriz_de_adyacencia, cero, uno, uno, uno, cero);
	puts ("");

	puts ("Matriz de accesibilidad");
	Imprimir_matriz (matriz_de_accesibilidad, cero, mas, mas, mas, mas);
	puts ("");
}
