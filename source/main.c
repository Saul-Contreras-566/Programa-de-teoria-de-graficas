#include <stdio.h>
#include "funciones_basicas.h"
#include "grafos_y_matrices.h"

Grafo grafo;
Matriz matriz_de_accesibilidad, matriz_de_adyacencia, matriz_de_incidencia;

void Leer_tipo_de_grafo () {
	grafo.clasificacion = 0;

	puts ("¿Qué tipo de grafo es?");
	puts ("1. Grafo no dirigido.");
	puts ("2. Grafo dirigido.");
	puts ("Seleccionar:");
	
	if (Leer_entero_entre (1, 2) == 2)
		grafo.clasificacion += DIGRAFO;
}

void Borrar_todo () {
	unsigned int i;

	// Liberando memoria del grafo.
	for (i = 0; i < grafo.numero_de_vertices; i++)
		free (grafo.vertices[i].nombre);
	free (grafo.vertices);
	for (i = 0; i < grafo.numero_de_lineas; i++)
		free (grafo.lineas[i].nombre);
	free (grafo.lineas);

	// Liberando memoria de las matrices.
	free (matriz_de_incidencia.entrada);
	free (matriz_de_adyacencia.entrada);
	free (matriz_de_accesibilidad.entrada);
}

int main () {
	int respuesta;
	char insertar_grafo[] = "¿Quiére insertar otro grafo?";

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
		Leer_tipo_de_grafo ();
		Leer_relaciones ();
		Obtener_matrices ();
		Clasificar ();
		Dibujar_grafo ();
		Borrar_todo ();

		respuesta = Pregunta_cerrada (insertar_grafo);
	} while (respuesta == 1);

	return 0;
}
