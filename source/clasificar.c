/*************************************************
FUNCIONES PARA CLASIFICAR.
*************************************************/

#include <stdio.h>
#include "grafos_y_matrices.h"





// FUNCIONES CLASIFICADORAS DE VÉRTICES.

void Obtener_grados_vertices () {
	/*
	Para el caso de los grafos no dirigidos, solamente
	se maneja grado_externo para el grado de los vértices.
	*/

	unsigned int i;

	// Inicializando grados.
	for (i = 0; i < grafo.numero_de_vertices; i++) {
		grafo.vertices[i].grado_interno = 0; // Estableciendo grado interno.
		grafo.vertices[i].grado_externo = 0; // Estableciendo grado externo.
	}

	// Obteniendo el grado de los vértices.
	for (i = 0; i < grafo.numero_de_lineas; i++) {
		(*grafo.lineas[i].origen).grado_externo++;
		if (grafo.clasificacion & DIGRAFO)
			(*grafo.lineas[i].destino).grado_interno++;
		else
			(*grafo.lineas[i].destino).grado_externo++;
	}

}

void Clasificando_vertices_aislados () {
	unsigned int i;

	puts ("\nVértices aislados:");
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno == 0 && grafo.vertices[i].grado_externo == 0) {
			grafo.vertices[i].clasificacion += VERTICE_AISLADO;
			printf ("- %s.\n", grafo.vertices[i].nombre);
		}
}

void Clasificando_vertices_iniciales () {
	// Asegurandome de que se ejecute el resto del código de esta función si se trata de un digrafo.
	if (!(grafo.clasificacion & DIGRAFO)) return;

	unsigned int i;

	puts ("\nVértices iniciales:");
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno == 0 && grafo.vertices[i].grado_externo != 0) {
			grafo.vertices[i].clasificacion += VERTICE_INICIAL;
			printf ("- %s.\n", grafo.vertices[i].nombre);
		}
}

void Clasificando_vertices_finales () {
	// Asegurandome de que se ejecute el resto del código de esta función si se trata de un digrafo.
	if (!(grafo.clasificacion & DIGRAFO)) return;

	unsigned int i;

	puts ("\nVértices finales:");
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno != 0 && grafo.vertices[i].grado_externo == 0) {
			grafo.vertices[i].clasificacion += VERTICE_FINAL;
			printf ("- %s.\n", grafo.vertices[i].nombre);
		}
}

void Clasificando_vertices_colgantes () {
	// Asegurandome de que se ejecute el resto del código de esta función si se trata de un grafo.
	if (grafo.clasificacion & DIGRAFO) return;

	unsigned int i;

	puts ("\nVértices colgantes:");
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_externo == 1) {
			grafo.vertices[i].clasificacion += VERTICE_COLGANTE;
			printf ("- %s.\n", grafo.vertices[i].nombre);
		}
}

void Clasificar_vertices () {
	unsigned int i;

	// Inicializando en cero las clasificaciones de los vertices.
	for (i = 0; i < grafo.numero_de_vertices; i++)
		grafo.vertices[i].clasificacion = 0;

	Clasificando_vertices_aislados ();
	Clasificando_vertices_iniciales ();
	Clasificando_vertices_finales ();
	Clasificando_vertices_colgantes ();
}



// FUNCIONES CLASIFICADORAS DE LÍNEAS.

void Clasificando_lineas_paralelas () {
	unsigned int i, j;

	// Inicializando valores.
	grafo.numero_de_grupos_de_paralelas = 0;
	for (i = 0; i < grafo.numero_de_lineas; i++)
		grafo.lineas[i].grupo_de_paralelas = 0;

	for (i = 0; i < grafo.numero_de_lineas; i++) { // Selecciona línea a comparar.
		// Si la línea i ya es paralela con otra.
		if (grafo.lineas[i].grupo_de_paralelas != 0) continue;

		for (j = i + 1; j < grafo.numero_de_lineas; j++) { // Compara la línea seleccionada con el resto.
			// Si la línea j ya es paralela con otra.
			if (grafo.lineas[j].grupo_de_paralelas != 0) continue;

			if ((grafo.lineas[j].origen == grafo.lineas[i].origen && // Si se trata de un grafo dirigido.
				grafo.lineas[j].destino == grafo.lineas[i].destino) ||
				((!(grafo.clasificacion & DIGRAFO)) && // Si se trata de un grafo no dirigido.
				grafo.lineas[j].origen == grafo.lineas[i].destino &&
				grafo.lineas[j].destino == grafo.lineas[i].origen)
			) {
				grafo.lineas[i].grupo_de_paralelas = grafo.numero_de_grupos_de_paralelas + 1;
				grafo.lineas[j].grupo_de_paralelas = grafo.numero_de_grupos_de_paralelas + 1;
				grafo.lineas[i].clasificacion += LINEA_PARALELA;
				grafo.lineas[j].clasificacion += LINEA_PARALELA;
			}
		}

		if (grafo.lineas[i].grupo_de_paralelas != 0)
			grafo.numero_de_grupos_de_paralelas++;
	}

	puts ("\nGrupos de líneas paralelas:");
	for (i = 0; i < grafo.numero_de_grupos_de_paralelas; i++) {
		printf ("- Grupo %d de líneas paralelas (del vértice '%s' al vértice '%s'):\n",
			i+1,
			(*grafo.lineas[i].origen).nombre,
			(*grafo.lineas[i].destino).nombre
		);
		for (j = 0; j < grafo.numero_de_lineas; j++)
			if (grafo.lineas[j].grupo_de_paralelas == i+1)
				printf ("    - Línea '%s'\n", grafo.lineas[j].nombre);
	}
}

void Clasificando_bucles () {
	unsigned int i;

	puts ("\nBucles en el grafo:");
	for (i = 0; i < grafo.numero_de_lineas; i++)
		if (grafo.lineas[i].origen == grafo.lineas[i].destino) {
			grafo.lineas[i].clasificacion += LINEA_BUCLE;
			printf ("- '%s' en el vértice '%s'.\n",
				grafo.lineas[i].nombre,
				(*grafo.lineas[i].origen).nombre
			);
		}
}

void Clasificando_lineas_en_serie () {
	// Asegurandome que el código solo se ejecute para los grafos no dirigidos.
	if (grafo.clasificacion & DIGRAFO) return;

	unsigned int i, j, n;
	Linea *lineas[2];

	puts ("\nLíneas en serie:");
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_externo == 2) {
			n = 0;
			for (j = 0; j < grafo.numero_de_lineas; j++)
				if (grafo.lineas[j].origen == &grafo.vertices[i] || grafo.lineas[j].destino == &grafo.vertices[i]) {
					grafo.lineas[j].clasificacion += LINEA_SERIE;
					lineas[n++] = &grafo.lineas[j];
				}
			printf ("- '%s' y '%s' están en serie (tienen en común el vértice '%s').\n",
				(*lineas[0]).nombre,
				(*lineas[1]).nombre,
				grafo.vertices[i].nombre
			);
		}
}

void Clasificar_lineas () {
	unsigned int i;

	// Inicializando clasificaciones de las líneas.
	for (i = 0; i < grafo.numero_de_lineas; i++)
		grafo.lineas[i].clasificacion = 0;
	
	Clasificando_lineas_paralelas ();
	Clasificando_bucles ();
	Clasificando_lineas_en_serie ();
}



// FUNCIONES CLASIFICADORAS DEL GRAFO.

void Es_grafo_general () {
	// Si existen líneas paralelas, entonces el grafo es general.
	if (grafo.numero_de_grupos_de_paralelas != 0) {
		grafo.clasificacion += GRAFO_GENERAL;
		puts ("- Grafo general.");
		return;
	}

	// Buscando existencia de bucles para confirmar que sea un grafo general.
	unsigned int i;
	for (i = 0; i < grafo.numero_de_lineas; i++)
		if (grafo.lineas[i].clasificacion & LINEA_BUCLE) {
			grafo.clasificacion += GRAFO_GENERAL;
			puts ("- Grafo general.");
			return;
		}

	puts ("- Grafo simple.");
}

void Es_grafo_nulo () {
	if (grafo.numero_de_vertices == 0) {
		grafo.clasificacion += GRAFO_NULO;
		puts ("- Grafo nulo.");
	}
}

void Es_grafo_conectado () {
	unsigned int i;

	// Verificando que sea conectado el grafo.
	for (i = 0; i < matriz_de_accesibilidad.filas * matriz_de_accesibilidad.columnas; i++)
		if (matriz_de_accesibilidad.entrada[i] == 0) {
			puts ("- Grafo desconectado.");
			return;
		}
	
	grafo.clasificacion += GRAFO_CONECTADO;
	puts ("- Grafo conectado.");
}

void Es_grafo_regular () {
	unsigned int i;

	// Buscando un vértice o dos vértices que demuestren que el grafo no es regular.
	for (i = 0; i < grafo.numero_de_vertices - 1; i++)
		if (grafo.vertices[i].grado_externo != grafo.vertices[i+1].grado_externo ||
			(grafo.clasificacion & DIGRAFO &&
			grafo.vertices[i].grado_interno != grafo.vertices[i].grado_externo)
		) return;
	
	grafo.clasificacion += GRAFO_REGULAR;
	puts ("- Grafo regular.");
}

void Es_grafo_completo () {
	// Verificando que primero sea un grafo simple.
	if (grafo.clasificacion & GRAFO_GENERAL) return;

	// Si el grado de algún vértice es distinto del número de vertices menos uno, entonces no es un grafo completo.
	unsigned int i;
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_externo != grafo.numero_de_vertices - 1 ||
			(grafo.clasificacion & DIGRAFO &&
			grafo.vertices[i].grado_interno != grafo.numero_de_vertices - 1)
		) return;
	
	grafo.clasificacion += GRAFO_COMPLETO;
	puts ("- Grafo completo.");
}

void Es_arbol () {
	if (grafo.clasificacion & GRAFO_CONECTADO && grafo.numero_de_lineas == grafo.numero_de_vertices - 1) {
		grafo.clasificacion += GRAFO_ARBOL;
		puts ("- Árbol.");
	}
}

void Es_digrafo_simetrico () {
	// Verificando que primero sea un digrafo.
	if (!(grafo.clasificacion & DIGRAFO)) return;

	// Verificando simetría de la matriz de adyacencia.
	unsigned int i, j;
	for (i = 0; i < matriz_de_adyacencia.filas; i++)
		for (j = 0; j < matriz_de_adyacencia.columnas; j++)
			if (MATRIZ_ENTRADA(matriz_de_adyacencia, i, j) != MATRIZ_ENTRADA(matriz_de_adyacencia, j, i))
				return;
	
	grafo.clasificacion += DIGRAFO_SIMETRICO;
	puts ("- Digráfo simétrico.");
}

void Es_digrafo_balanceado () {
	// Verificando que primero sea un digrafo.
	if (!(grafo.clasificacion & DIGRAFO)) return;

	// Si es regular, entonces es balanceado.
	if (grafo.clasificacion & GRAFO_REGULAR) {
		grafo.clasificacion += DIGRAFO_BALANCEADO;
		return;
	}

	// Verificando que, para cada vértice, su grado interno sea igual a su grado externo.
	unsigned int i;
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno != grafo.vertices[i].grado_externo)
			return;
	
	grafo.clasificacion += DIGRAFO_BALANCEADO;
	puts ("- Digráfo balanceado.");
}

void Es_grafo_euleriano () {
	// Verificando que primero sea conectado.
	if (!(grafo.clasificacion & GRAFO_CONECTADO)) return;

	// Si es una digráfica balanceada, entonces es euleriana.
	if (grafo.clasificacion & DIGRAFO && grafo.clasificacion & DIGRAFO_BALANCEADO) {
		grafo.clasificacion += GRAFO_EULERIANO;
		return;
	}

	// Si es un grafo no dirigido, se verifica que todos los vértices tengan grado par.
	unsigned int i;
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_externo % 2 != 0)
			return;
	
	grafo.clasificacion += GRAFO_EULERIANO;
	puts ("- Grafo euleriano.");
}

void Es_grafo_unicursal () {
	// Verificando que primero sea conectado.
	if (!(grafo.clasificacion & GRAFO_CONECTADO)) return;

	// Contando los vertices tengan grado impar o estén desbalanceados si se trata de una digráfica.
	unsigned int i, impares_o_desbalanceados = 0;
	for(i = 0; i < grafo.numero_de_vertices; i++)
		if (
			// Si es un grafo no dirigido
			((!(grafo.clasificacion & DIGRAFO)) &&
			grafo.vertices[i].grado_externo % 2 != 0) ||
			// Si es un digrafo.
			(grafo.clasificacion & DIGRAFO &&
			grafo.vertices[i].grado_interno != grafo.vertices[i].grado_externo)
		) impares_o_desbalanceados++;
	
	if (impares_o_desbalanceados == 2) {
		grafo.clasificacion += GRAFO_UNICURSAL;
		puts ("- Grafo unicursal.");
	}
}

void Clasificar_grafo () {
	puts ("\nClasificación del grafo:");
	if (grafo.clasificacion & DIGRAFO)
		puts ("- Digrafo.");
	else
		puts ("- Grafo no dirigido.");
	Es_grafo_nulo ();
	if (!(grafo.clasificacion & GRAFO_NULO)) {
		Es_grafo_general ();
		Es_grafo_conectado ();
		Es_grafo_regular ();
		Es_grafo_completo ();
		Es_arbol ();
		Es_digrafo_simetrico ();
		Es_digrafo_balanceado ();
		Es_grafo_euleriano ();
		Es_grafo_unicursal ();
	}
}





// FUNCION GENERAL PARA CLASIFICAR

void Clasificar () {
	Obtener_grados_vertices ();
	puts ("CLASIFIACIONES DE VÉRTICES, LÍNEAS Y DEL GRAFO");
	Clasificar_vertices ();
	Clasificar_lineas ();
	Clasificar_grafo ();
}
