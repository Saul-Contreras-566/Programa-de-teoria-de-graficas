/*************************************************
FUNCIONES PARA CLASIFICAR.
*************************************************/

#include "grafos_y_matrices.h"





// FUNCIONES CLASIFICADORAS DE VÉRTICES.

void Obtener_grados_vertices (Grafo grafo) {
	/*
	Para el caso de los grafos no dirigidos, solamente
	se maneja grado_interno para el grado de los vértices.
	*/

	unsigned int i;

	// Inicializando grados.
	for (i = 0; i < grafo.numero_de_vertices; i++) {
		grafo.vertices[i].grado_interno = 0; // Estableciendo grado interno.
		grafo.vertices[i].grado_externo = 0; // Estableciendo grado externo.
	}

	// Obteniendo el grado de los vértices.
	for (i = 0; i < grafo.numero_de_lineas; i++) {
		(*grafo.lineas[i].origen).grado_interno++;
		if (grafo.clasificacion & DIGRAFO)
			(*grafo.lineas[i].destino).grado_externo++;
		else
			(*grafo.lineas[i].destino).grado_interno++;
	}

}

void Clasificando_vertices_aislados (Grafo grafo) {
	unsigned int i;

	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno == 0 && grafo.vertices[i].grado_externo == 0)
			grafo.vertices[i].clasificacion += VERTICE_AISLADO;
}

void Clasificando_vertices_iniciales (Grafo grafo) {
	// Asegurandome de que se ejecute el resto del código de esta función si se trata de un digrafo.
	if (!(grafo.clasificacion & DIGRAFO)) return;

	unsigned int i;

	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno != 0 && grafo.vertices[i].grado_externo == 0)
			grafo.vertices[i].clasificacion += VERTICE_INICIAL;
}

void Clasificando_vertices_finales (Grafo grafo) {
	// Asegurandome de que se ejecute el resto del código de esta función si se trata de un digrafo.
	if (!(grafo.clasificacion & DIGRAFO)) return;

	unsigned int i;

	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno == 0 && grafo.vertices[i].grado_externo != 0)
			grafo.vertices[i].clasificacion += VERTICE_FINAL;
}

void Clasificando_vertices_colgantes (Grafo grafo) {
	// Asegurandome de que se ejecute el resto del código de esta función si se trata de un grafo.
	if (grafo.clasificacion & DIGRAFO) return;

	unsigned int i;

	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].grado_interno == 0)
			grafo.vertices[i].clasificacion += VERTICE_COLGANTE;
}

void Clasificar_vertices (Grafo grafo) {
	unsigned int i;

	// Inicializando en cero las clasificaciones de los vertices.
	for (i = 0; i < grafo.numero_de_vertices; i++)
		grafo.vertices[i].clasificacion = 0;

	Clasificando_vertices_aislados (grafo);
	Clasificando_vertices_iniciales (grafo);
	Clasificando_vertices_finales (grafo);
	Clasificando_vertices_colgantes (grafo);
}



// FUNCIONES CLASIFICADORAS DE LÍNEAS.

void Clasificando_lineas_paralelas (Grafo *grafo) {
	unsigned int i, j;

	// Inicializando valores.
	(*grafo).numero_de_grupos_de_paralelas = 0;
	for (i = 0; i < (*grafo).numero_de_lineas; i++)
		(*grafo).lineas[i].grupo_de_paralelas = 0;

	for (i = 0; i < (*grafo).numero_de_lineas; i++) { // Selecciona línea a comparar.
		// Si la línea i ya es paralela con otra.
		if ((*grafo).lineas[i].grupo_de_paralelas != 0) continue;

		for (j = i + 1; j < (*grafo).numero_de_lineas; j++) { // Compara la línea seleccionada con el resto.
			// Si la línea j ya es paralela con otra.
			if ((*grafo).lineas[j].grupo_de_paralelas != 0) break;

			if (((*grafo).lineas[j].origen == (*grafo).lineas[i].origen &&
				(*grafo).lineas[j].destino == (*grafo).lineas[i].destino) ||
				(!((*grafo).clasificacion & DIGRAFO) && // Si se trata de un grafo no dirigido.
				(*grafo).lineas[j].origen == (*grafo).lineas[i].destino &&
				(*grafo).lineas[j].destino == (*grafo).lineas[i].origen)
			) {
				(*grafo).lineas[i].grupo_de_paralelas = (*grafo).numero_de_grupos_de_paralelas + 1;
				(*grafo).lineas[j].grupo_de_paralelas = (*grafo).numero_de_grupos_de_paralelas + 1;
				(*grafo).lineas[i].clasificacion += LINEA_PARALELA;
				(*grafo).lineas[j].clasificacion += LINEA_PARALELA;
			}
		}

		// Actualizando el contador de grupos de líneas paralaleas.
		if ((*grafo).lineas[i].grupo_de_paralelas != 0)
			(*grafo).numero_de_grupos_de_paralelas++;
	}
}

void Clasificando_bucles (Grafo *grafo) {
	unsigned int i;

	for (i = 0; i < (*grafo).numero_de_lineas; i++)
		if ((*grafo).lineas[i].origen == (*grafo).lineas[i].destino)
			(*grafo).lineas[i].clasificacion += LINEA_BUCLE;
}

void Clasificando_lineas_en_serie (Grafo *grafo) {
	// Asegurandome que el código solo se ejecute para los (*grafo)s no dirigidos.
	if ((*grafo).clasificacion & DIGRAFO) return;

	unsigned int i;

	for (i = 0; i < (*grafo).numero_de_lineas; i++)
		if ((*(*grafo).lineas[i].origen).grado_interno == 2 || (*(*grafo).lineas[i].destino).grado_interno == 2)
			(*grafo).lineas[i].clasificacion += LINEA_SERIE;
}

void Clasificar_lineas (Grafo *grafo) {
	unsigned int i;

	// Inicializando clasificaciones de las líneas.
	for (i = 0; i < (*grafo).numero_de_lineas; i++)
		(*grafo).lineas[i].clasificacion = 0;
	
	Clasificando_lineas_paralelas (grafo);
	Clasificando_bucles (grafo);
	Clasificando_lineas_en_serie (grafo);
}



// FUNCIONES CLASIFICADORAS DEL GRAFO.

void Es_grafo_general (Grafo *grafo) {
	// Si existen líneas paralelas, entonces el grafo es general.
	if ((*grafo).numero_de_grupos_de_paralelas != 0) {
		(*grafo).clasificacion += GRAFO_GENERAL;
		return;
	}

	// Buscando existencia de bucles para confirmar que sea un grafo general.
	unsigned int i;
	for (i = 0; i < (*grafo).numero_de_lineas; i++)
		if ((*grafo).lineas[i].clasificacion & LINEA_BUCLE) {
			(*grafo).clasificacion += GRAFO_GENERAL;
			return;
		}
}

void Es_grafo_nulo (Grafo *grafo) {
	if ((*grafo).numero_de_vertices == 0)
		(*grafo).clasificacion += GRAFO_NULO;
}

void Es_grafo_conectado (Grafo *grafo, Matriz matriz_de_accesibilidad) {
	unsigned int i;

	// Verificando que sea conectado el grafo.
	for (i = 0; i < matriz_de_accesibilidad.filas * matriz_de_accesibilidad.columnas; i++)
		if (matriz_de_accesibilidad.entrada[i] == 0) return;
	
	(*grafo).clasificacion += GRAFO_CONECTADO;
}

void Es_grafo_regular (Grafo *grafo) {
	unsigned int i;

	// Buscando un vértice o dos vértices que demuestren que el grafo no es regular.
	for (i = 0; i < (*grafo).numero_de_vertices - 1; i++)
		if ((*grafo).vertices[i].grado_interno != (*grafo).vertices[i+1].grado_interno ||
			((*grafo).clasificacion & DIGRAFO &&
			(*grafo).vertices[i].grado_interno != (*grafo).vertices[i].grado_externo)
		) return;
	
	(*grafo).clasificacion += GRAFO_REGULAR;
}

void Es_grafo_completo (Grafo *grafo) {
	// Verificando que primero sea un grafo simple.
	if ((*grafo).clasificacion & GRAFO_GENERAL) return;

	// Si el grado de algún vértice es distinto del número de vertices menos uno, entonces no es un grafo completo.
	unsigned int i;
	for (i = 0; i < (*grafo).numero_de_vertices; i++)
		if ((*grafo).vertices[i].grado_interno != (*grafo).numero_de_vertices - 1 ||
			((*grafo).clasificacion & DIGRAFO &&
			(*grafo).vertices[i].grado_externo != (*grafo).numero_de_vertices - 1)
		) return;
	
	(*grafo).clasificacion += GRAFO_COMPLETO;
}

void Es_arbol (Grafo *grafo) {
	if ((*grafo).clasificacion & GRAFO_CONECTADO && (*grafo).numero_de_lineas == (*grafo).numero_de_vertices - 1)
		(*grafo).clasificacion += GRAFO_ARBOL;
}

void Es_digrafo_simetrico (Grafo *grafo, Matriz matriz_de_adyacencia) {
	// Verificando que primero sea un digrafo.
	if (!((*grafo).clasificacion & DIGRAFO)) return;

	// Verificando simetría de la matriz de adyacencia.
	unsigned int i, j;
	for (i = 0; i < matriz_de_adyacencia.filas; i++)
		for (j = 0; j < matriz_de_adyacencia.columnas; j++)
			if (MATRIZ_ENTRADA(matriz_de_adyacencia, i, j) != MATRIZ_ENTRADA(matriz_de_adyacencia, j, i))
				return;
	
	(*grafo).clasificacion += DIGRAFO_SIMETRICO;
}

void Es_digrafo_balanceado (Grafo *grafo) {
	// Verificando que primero sea un digrafo.
	if (!((*grafo).clasificacion & DIGRAFO)) return;

	// Si es regular, entonces es balanceado.
	if ((*grafo).clasificacion & GRAFO_REGULAR) {
		(*grafo).clasificacion += DIGRAFO_BALANCEADO;
		return;
	}

	// Verificando que, para cada vértice, su grado interno sea igual a su grado externo.
	unsigned int i;
	for (i = 0; i < (*grafo).numero_de_vertices; i++)
		if ((*grafo).vertices[i].grado_interno != (*grafo).vertices[i].grado_externo)
			return;
	
	(*grafo).clasificacion += DIGRAFO_BALANCEADO;
}

void Es_grafo_euleriano (Grafo *grafo) {
	// Verificando que primero sea conectado.
	if (!((*grafo).clasificacion & GRAFO_CONECTADO)) return;

	// Si es una digráfica balanceada, entonces es euleriana.
	if ((*grafo).clasificacion & DIGRAFO && (*grafo).clasificacion & DIGRAFO_BALANCEADO) {
		(*grafo).clasificacion += GRAFO_EULERIANO;
		return;
	}

	// Si es un grafo no dirigido, se verifica que todos los vértices tengan grado par.
	unsigned int i;
	for (i = 0; i < (*grafo).numero_de_vertices; i++)
		if ((*grafo).vertices[i].grado_interno % 2 != 0)
			return;
	
	(*grafo).clasificacion += GRAFO_EULERIANO;
}

void Es_grafo_unicursal (Grafo *grafo) {
	// Verificando que primero sea conectado.
	if (!((*grafo).clasificacion & GRAFO_CONECTADO)) return;

	// Contando los vertices tengan grado impar o estén desbalanceados si se trata de una digráfica.
	unsigned int i, impares_o_desbalanceados = 0;
	for(i = 0; i < (*grafo).numero_de_vertices; i++)
		if (
			// Si es un grafo no dirigido
			((!((*grafo).clasificacion & DIGRAFO)) &&
			(*grafo).vertices[i].grado_interno % 2 != 0) ||
			// Si es un digrafo.
			((*grafo).clasificacion & DIGRAFO &&
			(*grafo).vertices[i].grado_interno != (*grafo).vertices[i].grado_externo)
		) impares_o_desbalanceados++;
	
	if (impares_o_desbalanceados == 2)
		(*grafo).clasificacion += GRAFO_UNICURSAL;
}

void Clasificar_grafo (Grafo *grafo, Matriz matriz_de_adyacencia, Matriz matriz_de_accesibilidad) {
	(*grafo).clasificacion = 0;

	Es_grafo_general (grafo);
	Es_grafo_nulo (grafo);
	Es_grafo_conectado (grafo, matriz_de_accesibilidad);
	Es_grafo_regular (grafo);
	Es_grafo_completo (grafo);
	Es_arbol (grafo);
	Es_digrafo_simetrico (grafo, matriz_de_adyacencia);
	Es_digrafo_balanceado (grafo);
	Es_grafo_euleriano (grafo);
	Es_grafo_unicursal (grafo);
}





// FUNCION GENERAL PARA CLASIFICAR

void Clasificar (Grafo *grafo, Matriz matriz_de_adyacencia, Matriz matriz_de_accesibilidad) {
	Clasificar_vertices (*grafo);
	Clasificar_lineas (grafo);
	Clasificar_grafo (grafo, matriz_de_adyacencia, matriz_de_accesibilidad);
}
