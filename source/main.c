#include <stdio.h>
#include "funciones_basicas.h"
#include "grafos_y_matrices.h"

void Leer_tipo_de_grafo (Grafo *grafo) {
	puts ("¿Qué tipo de grafo es?");
	puts ("1. Grafo no dirigido.");
	puts ("2. Grafo dirigido.");
	puts ("Seleccionar:");
	
	if (Leer_entero_entre (1, 2) == 2)
		(*grafo).clasificacion += DIGRAFO;
}

void Borrar_todo (Grafo grafo, Matriz matriz_de_incidencia, Matriz matriz_de_adyacencia, Matriz matriz_de_accesibilidad) {
	unsigned int i;

	// Liberando memoria del grafo.
	for (i = 0; i < grafo.numero_de_vertices; i++)
		free (grafo.vertices[i].nombre);
	free (grafo.vertices);
	free (grafo.lineas);

	// Liberando memoria de las matrices.
	free (matriz_de_incidencia.entrada);
	free (matriz_de_adyacencia.entrada);
	free (matriz_de_accesibilidad.entrada);
}

int main () {
	int respuesta;
	Grafo grafo;
	Matriz matriz_de_accesibilidad, matriz_de_adyacencia, matriz_de_incidencia;

	#ifdef WIN32
	system ("chcp 65001");
	#endif

	LIMPIAR_PANTALLA ();

	// Portada.
	puts ("Integrantes del equipo:");
	puts ("- Carmona Maldonado Gibrán Isaí.");
	puts ("- Contreras Rojas Emanuel Saúl.");
	puts ("- Guzmán Ramos Carlos Emilio.");
	puts ("- Rodríguez Medina José Alfredo.");
	ESPERAR_TECLA ();

	do {
		LIMPIAR_PANTALLA ();
		Leer_tipo_de_grafo (&grafo);
		Leer_relaciones (&grafo);
		Obtener_matrices (grafo, &matriz_de_incidencia, &matriz_de_adyacencia, &matriz_de_accesibilidad);
		Clasificar (&grafo, matriz_de_adyacencia, matriz_de_accesibilidad);
		// Dibujar_grafo (grafo);
		Borrar_todo (grafo, matriz_de_incidencia, matriz_de_adyacencia, matriz_de_accesibilidad);

		respuesta = Pregunta_cerrada ("¿Quiére insertar otro grafo?");
	} while (respuesta == 1);

	return 0;
}
