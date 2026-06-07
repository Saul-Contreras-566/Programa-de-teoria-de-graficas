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

	unsigned int i;

	matriz_de_incidencia.filas = grafo.numero_de_vertices;
	matriz_de_incidencia.columnas = grafo.numero_de_lineas;
	matriz_de_incidencia.entrada = Inicializar_entradas (grafo.numero_de_vertices * grafo.numero_de_lineas);

	for (i = 0; i < grafo.numero_de_lineas; i++) {
		MATRIZ_ENTRADA (
			matriz_de_incidencia,
			(unsigned int) (grafo.lineas[i].origen - &grafo.vertices[0]),
			i
		) = 1;
		MATRIZ_ENTRADA (
			matriz_de_incidencia,
			(unsigned int) (grafo.lineas[i].destino - &grafo.vertices[0]),
			i
		) += 2;
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

	unsigned int i;

	matriz_de_adyacencia.filas = grafo.numero_de_vertices;
	matriz_de_adyacencia.columnas = grafo.numero_de_vertices;
	matriz_de_adyacencia.entrada = Inicializar_entradas (grafo.numero_de_vertices * grafo.numero_de_vertices);

	for (i = 0; i < grafo.numero_de_lineas; i++) {
		MATRIZ_ENTRADA (
			matriz_de_adyacencia,
			(unsigned int) (grafo.lineas[i].origen - &grafo.vertices[0]),
			(unsigned int) (grafo.lineas[i].destino - &grafo.vertices[0])
		)++;

		if (!(grafo.clasificacion & DIGRAFO))
			MATRIZ_ENTRADA (
				matriz_de_adyacencia,
				(unsigned int) (grafo.lineas[i].destino - &grafo.vertices[0]),
				(unsigned int) (grafo.lineas[i].origen - &grafo.vertices[0])
			)++;
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

	unsigned int potencias; // Contador de las potencias sucesivas.
	Matriz copia, resultado;

	matriz_de_accesibilidad.entrada = NULL;
	copia.entrada = NULL;
	resultado.entrada = NULL;

	Copiar_matriz(matriz_de_adyacencia, &matriz_de_accesibilidad);
	Copiar_matriz(matriz_de_adyacencia, &copia);

	// Elevando a potencias sucesivas la matriz de adyacencia.
	for (potencias = 2; potencias <= grafo.numero_de_vertices; potencias++) {
		Multiplicar_matrices (copia, matriz_de_adyacencia, &resultado);
		Copiar_matriz (resultado, &copia);
		Sumar_matrices (matriz_de_accesibilidad, resultado);
	}

	free (copia.entrada);
	free (resultado.entrada);
}

void Imprimir_matriz_de_incidencia () {
	unsigned int i, j;
	char llega[] = "1";
	char no_incide[] = "0";
	char sale[] = "-1";
	char bucle[] = "±1";
	char *caso;

	// Encabezado de la tabla/matriz.
	printf ("|    ");
	for (i = 0; i < grafo.numero_de_lineas; i++)	
		printf ("| %2s ", grafo.lineas[i].nombre);
	puts ("|");

	// Cuerpo de la tabla/matriz.
	for (i = 0; i < grafo.numero_de_vertices; i++) {
		printf ("| %2s ", grafo.vertices[i].nombre);
		for (j = 0; j < grafo.numero_de_lineas; j++) {
			switch (MATRIZ_ENTRADA (matriz_de_incidencia, i, j)) {
				case 0: caso = no_incide; break;
				case 1: caso = llega; break;
				case 2: caso = (grafo.clasificacion & DIGRAFO) ? sale : llega; break;
				case 3: caso = (grafo.clasificacion & DIGRAFO) ? bucle : llega; break;
				default: caso = no_incide; break;
			}
			printf ("| %2s ", caso);
		}
		puts ("|");
	}
}

void Imprimir_matriz_cuadrada (Matriz matriz, char *cero, char *positivo) {
	/* Esta función se utiliza para imprimir la matriz
	de adyacencia y de accesibilidad */

	unsigned int i, j;
	char *caso;

	// Encabezado de la tabla/matriz.
	printf ("|    ");
	for (i = 0; i < grafo.numero_de_vertices; i++)
		printf ("| %2s ", grafo.vertices[i].nombre);
	puts ("|");

	// Cuerpo de la tabla/matriz.
	for (i = 0; i < grafo.numero_de_vertices; i++) {
		printf ("| %2s ", grafo.vertices[i].nombre);
		for (j = 0; j < grafo.numero_de_vertices; j++) {
			if (MATRIZ_ENTRADA (matriz, i, j) == 0)
				caso = cero;
			else
				caso = positivo;
			printf ("| %2s ", caso);
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
	char mas[] = "+";

	puts ("Matriz de incidencia");
	Imprimir_matriz_de_incidencia ();
	puts ("");

	puts ("Matriz de adyacencia");
	Imprimir_matriz_cuadrada (matriz_de_adyacencia, cero, uno);
	puts ("");

	puts ("Matriz de accesibilidad");
	Imprimir_matriz_cuadrada (matriz_de_accesibilidad, cero, mas);
	puts ("");
}
