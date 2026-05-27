#include <stdio.h>
#include <math.h>
#include <SDL3/SDL.h>
#include "funciones_basicas.h"
#include "grafos_y_matrices.h"

// Variables para la ventana de dibujo
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event evento;
int abortar;
int ejecutandose = 1;
int arrastrar = 0;
unsigned int i, j, vertice_seleccionado = -1;
double radianes;
float posx = 0.0f, posy = 0.0f;

void Salida_panico () {
	abortar = 1;
	SDL_Quit ();
	if (window != NULL) free (window);
	if (renderer != NULL) free (renderer);
	puts ("Abortando SDL.");
}

void Inicializar_ventana () {
	abortar = 0;
	if (!SDL_Init (SDL_INIT_VIDEO)) {
		puts ("SDL ERROR: No se pudo inicializar SDL.");
		Salida_panico ();
	}
	window = SDL_CreateWindow ("Grafo", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (window == NULL) {
		puts ("SDL ERROR: No se pudo crear la ventana donde se dibujaría el grafo.");
		Salida_panico ();
	}
	renderer = SDL_CreateRenderer (window, "");
	if (renderer == NULL) {
		puts ("SDL ERROR: No se pudo crear el renderizador en la ventana que dibuja el grafo.");
		Salida_panico ();
	}
}

void Terminar_ventana () {
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();
}

float Obtener_distancia (float x1, float y1, float x2, float y2) {
	return (float) sqrt (pow ((double) (x2 - x1), 2.0) + pow ((double) (y2 - y1), 2.0));
}

void Posicionar_vertices () {
	unsigned int i;
	float radio = (float) (WINDOW_WIDTH < WINDOW_HEIGHT ? WINDOW_WIDTH : WINDOW_HEIGHT) / 2.0f - RADIO_VERTICES * 2.0f - 10.0f;
	double radianes = 2.0 * PI / (double) grafo.numero_de_vertices;

	for (i = 0; i < grafo.numero_de_vertices; i++) {
		grafo.vertices[i].x = radio * (float) cos ((double) i * radianes) + (float) WINDOW_WIDTH / 2.0f;
		grafo.vertices[i].y = radio * (float) sin ((double) i * radianes) + (float) WINDOW_HEIGHT / 2.0f;
		grafo.vertices[i].x_original = grafo.vertices[i].x;
		grafo.vertices[i].y_original = grafo.vertices[i].y;
	}
}

void Establecer_color (int r, int g, int b, int a) {
	if (!SDL_SetRenderDrawColor (renderer, r, g, b, a)) {
		puts ("SDL ERROR: No se pudo establecer el color de dibujo del renderizador correctamente.");
		Salida_panico ();
	}
}

void Dibujar_rectangulo (SDL_FRect *rect) {
	if (!SDL_RenderFillRect (renderer, rect)) {
		puts ("SDL ERROR: No se pudo dibujar correctamente el círculo en el renderizador.");
		Salida_panico ();
	}
}

void Dibujar_circulo (float x, float y, float radio) {
	/* `x' y `y' son las coordenadas del centro del
	círculo. */

	int i;
	SDL_FRect rect;
	
	for (i = 0; i < RECTANGULOS_POR_CIRCULO && abortar == 0; i++) {
		rect.w = (float) cos ((double) (i+1) * PI / (2.0 * (double) (RECTANGULOS_POR_CIRCULO + 1) ) ) * radio * 2.0f;
		rect.h = (float) sin ((double) (i+1) * PI / (2.0 * (double) (RECTANGULOS_POR_CIRCULO + 1) ) ) * radio * 2.0f;
		rect.x = x - rect.w / 2.0f;
		rect.y = y - rect.h / 2.0f;
		if (!SDL_RenderFillRect (renderer, &rect)) {
			puts ("SDL ERROR: No se pudo dibujar correctamente un rectángulo.");
			Salida_panico ();
		}
	}
}

void Dibujar_vertice (Vertice vertice) {
	Establecer_color (50, 200, 200, 255);
	Dibujar_circulo (vertice.x, vertice.y, RADIO_VERTICES);
	Establecer_color (100, 255, 255, 255);
	Dibujar_circulo (vertice.x, vertice.y, RADIO_VERTICES - 10.0f);
}

void Dibujar_vertices () {
	unsigned int i;

	for (i = 0; i < grafo.numero_de_vertices && abortar == 0; i++)
		Dibujar_vertice (grafo.vertices[i]);
}

void Dibujar_linea (float x1, float y1, float x2, float y2, float grosor) {
	int i;
	int circulos;
	float distancia;
	float dx, dy;

	dx = x2 - x1;
	dy = y2 - y1;
	distancia = (float) sqrt (pow ((double) dx, 2.0) + pow ((double) dy, 2.0));
	circulos = (int) (distancia * 4.0f / grosor);
	dx /= circulos;
	dy /= circulos;
	for (i = 0; i < circulos && abortar == 0; i++) {
		x1 += dx;
		y1 += dy;
		Dibujar_circulo (x1, y1, grosor / 2.0f);
	}
}

void Dibujar_triangulo (float x, float y, float h, float b, double radianes, float grosor) {
	float x1, y1, x2, y2;

	x1 = h * (float) cos (radianes) - b * (float) cos (radianes + PI / 2.0) / 2.0f + x;
	y1 = h * (float) sin (radianes) - b * (float) sin (radianes + PI / 2.0) / 2.0f + y;
	x2 = h * (float) cos (radianes) + b * (float) cos (radianes + PI / 2.0) / 2.0f + x;
	y2 = h * (float) sin (radianes) + b * (float) sin (radianes + PI / 2.0) / 2.0f + y;

	Dibujar_linea (x, y, x1, y1, grosor);
	Dibujar_linea (x, y, x2, y2, grosor);
}

float Altura_eliptica (float x, float distancia, float semieje) {
	distancia /= 2.0f;
	return semieje * (float) sqrt (2.0 * (double) (distancia * x) - pow ((double) x, 2.0)) / distancia;
}

double Obtener_angulo (float x, float y) {
	if (y == 0.0f) {
		if (x >= 0.0f)
			return 0.0;
		else
			return PI;
	} else if (x == 0.0f) {
		if (y >= 0.0f)
			return PI / 2.0;
		else
			return 1.5 * PI;
	} else
		if (x >= 0)
			return atan ((double) y / (double) x);
		else
			return PI + atan ((double) y / (double) x);
}

void Dibujar_relacion (float x1, float y1, float x2, float y2, float grosor, float semieje) {
	int i;
	float dx = x2 - x1, dy = y2 - y1;
	float h, distancia = (float) sqrt( pow ((double) dx, 2.0) + pow ((double) dy, 2.0));
	double radianes = Obtener_angulo (dx, dy);

	x2 = x1;
	y2 = y1;

	x1 += dx / 2.0f + semieje * (float) cos (radianes + PI / 2.0);
	y1 += dy / 2.0f + semieje * (float) sin (radianes + PI / 2.0);
	if (grafo.clasificacion & DIGRAFO)
		Dibujar_triangulo (x1, y1, 20.0f, 45.0f, PI + radianes, grosor);

	x1 = x2;
	y1 = y2;

	for (i = 0; i < 24 && abortar == 0; i++) {
		h = Altura_eliptica (distancia * (float) (i + 1) / 24.0f, distancia, semieje) - Altura_eliptica (distancia * (float) i / 24.0f, distancia, semieje);
		x2 += h * (float) cos (radianes + PI / 2.0) + dx / 24.0f;
		y2 += h * (float) sin (radianes + PI / 2.0) + dy / 24.0f;
		Dibujar_linea (x1, y1, x2, y2, grosor);
		x1 = x2;
		y1 = y2;
	}
}

void Dibujar_relaciones () {
	unsigned int i, j;
	int k;
	unsigned int lineas;
	float semieje;
	float n;
	double radianes;

	Establecer_color (50, 200, 200, 255);
	for (i = 0; i < grafo.numero_de_vertices && abortar == 0; i++)
		for (j = i; j < grafo.numero_de_vertices && abortar == 0; j++) {
			lineas = MATRIZ_ENTRADA (matriz_de_adyacencia, i, j) + ((grafo.clasificacion & DIGRAFO) ? MATRIZ_ENTRADA (matriz_de_adyacencia, j, i) && i != j: 0);
			if (lineas == 0)
				continue;
			else if (i == j) {
				n = 1.5f;
				for (k = 0; k < (int) lineas && abortar == 0; k++) {
					radianes = Obtener_angulo (grafo.vertices[i].x - (float) WINDOW_WIDTH / 2.0f, grafo.vertices[i].y - (float) WINDOW_HEIGHT / 2.0f);
					Dibujar_relacion (
						grafo.vertices[i].x,
						grafo.vertices[i].y,
						grafo.vertices[i].x + RADIO_VERTICES * n * 2.0f * (float) cos (radianes),
						grafo.vertices[i].y + RADIO_VERTICES * n * 2.0f * (float) sin (radianes),
						GROSOR_LINEAS,
						RADIO_VERTICES * n
					);
					Dibujar_relacion (
						grafo.vertices[i].x + RADIO_VERTICES * n * 2.0f * (float) cos (radianes),
						grafo.vertices[i].y + RADIO_VERTICES * n * 2.0f * (float) sin (radianes),
						grafo.vertices[i].x,
						grafo.vertices[i].y,
						GROSOR_LINEAS,
						RADIO_VERTICES * n
					);
					Dibujar_circulo (
						grafo.vertices[i].x + RADIO_VERTICES * n * 2.0f * (float) cos (radianes),
						grafo.vertices[i].y + RADIO_VERTICES * n * 2.0f * (float) sin (radianes),
						GROSOR_LINEAS
					);
					n++;
				}
			} else {
				n = 0.0f;
				semieje = ANCHO_SEMIEJE * ((float) lineas - 1.0f) / 2.0f;
				for (k = 0; k < MATRIZ_ENTRADA (matriz_de_adyacencia, i, j) && abortar == 0; k++)
					Dibujar_relacion (
						grafo.vertices[i].x,
						grafo.vertices[i].y,
						grafo.vertices[j].x,
						grafo.vertices[j].y,
						GROSOR_LINEAS,
						semieje - ANCHO_SEMIEJE * (n++)
					);
				if (i != j) {
					n = 0.0f;
					for (k = 0; k < MATRIZ_ENTRADA (matriz_de_adyacencia, j, i) && abortar == 0; k++)
						Dibujar_relacion (
							grafo.vertices[j].x,
							grafo.vertices[j].y,
							grafo.vertices[i].x,
							grafo.vertices[i].y,
							GROSOR_LINEAS,
							semieje - ANCHO_SEMIEJE * (n++)
						);
				}
			}
		}
}

void Evento_cerrar_ventana () {
	if (evento.type == SDL_EVENT_QUIT)
		ejecutandose = 0;
	else if (evento.type == SDL_EVENT_KEY_DOWN)
		if (evento.key.key == SDLK_Q || evento.key.key == SDLK_ESCAPE)
			ejecutandose = 0;
}

void Evento_reiniciar_posicion_del_visualizador () {
	unsigned int i;
	if (evento.type == SDL_EVENT_KEY_DOWN && evento.key.key == SDLK_R) {
		for (i = 0; i < grafo.numero_de_vertices; i++) {
			grafo.vertices[i].x -= posx;
			grafo.vertices[i].y -= posy;
		}
		posx = 0.0f;
		posy = 0.0f;
	}
}

void Evento_reiniciar_posiciones_de_los_vertices () {
	unsigned int i;
	if (
		evento.type == SDL_EVENT_KEY_DOWN &&
		evento.key.key == SDLK_R &&
		(evento.key.mod == SDL_KMOD_LALT ||
		evento.key.mod == SDL_KMOD_RALT)
	)
		for (i = 0; i < grafo.numero_de_vertices; i++) {
			grafo.vertices[i].x = grafo.vertices[i].x_original;
			grafo.vertices[i].y = grafo.vertices[i].y_original;
		}
}

void Evento_activar_arrastre () {
	if (evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		arrastrar = 1;

		// Seleccionando vértice a arrastrar.
		for (i = 0; i < grafo.numero_de_vertices; i++)
			if (
				Obtener_distancia (
					grafo.vertices[i].x,
					grafo.vertices[i].y,
					evento.motion.x,
					evento.motion.y
				) <= RADIO_VERTICES + 10.0f
			) {
				vertice_seleccionado = i;
				break;
			}
	}
}

void Evento_desactivar_arrastre () {
	if (evento.type == SDL_EVENT_MOUSE_BUTTON_UP) {
		arrastrar = 0;
		vertice_seleccionado = -1;
	}
}

void Evento_mover_area_de_visualizacion () {
	unsigned int i;
	if (
		evento.type == SDL_EVENT_MOUSE_MOTION &&
		arrastrar == 1 &&
		vertice_seleccionado == (unsigned int) (-1)
	) {
		for (i = 0; i < grafo.numero_de_vertices; i++) {
			grafo.vertices[i].x += evento.motion.xrel;
			grafo.vertices[i].y += evento.motion.yrel;
		}
		posx += evento.motion.xrel;
		posy += evento.motion.yrel;
	}
}

void Evento_arrastrar_vertice () {
	if (
		evento.type == SDL_EVENT_MOUSE_MOTION &&
		arrastrar == 1 &&
		vertice_seleccionado != (unsigned int) (-1)
	) {
		grafo.vertices[vertice_seleccionado].x += evento.motion.xrel;
		grafo.vertices[vertice_seleccionado].y += evento.motion.yrel;
	}
}

void Evento_colisionar_vertices () {
	unsigned int i, j;
	if (
		arrastrar == 1 &&
		vertice_seleccionado != (unsigned int) (-1)
	)
		for (i = 0; i < grafo.numero_de_vertices; i++) {
			for (j = 0; j < grafo.numero_de_vertices; j++) {
				if (i == j)
					continue;
				else if (j == vertice_seleccionado)
					continue;
				else if (
					Obtener_distancia (
						grafo.vertices[i].x,
						grafo.vertices[i].y,
						grafo.vertices[j].x,
						grafo.vertices[j].y
					) <= 2.0f * RADIO_VERTICES + 10.0f
				) {
					radianes = Obtener_angulo (
						grafo.vertices[j].x - grafo.vertices[i].x,
						grafo.vertices[j].y - grafo.vertices[i].y
					);
					grafo.vertices[j].x += evento.motion.xrel + 2.0f * (float) cos (radianes);
					grafo.vertices[j].y += evento.motion.yrel + 2.0f * (float) sin (radianes);
				}
			}
		}
}

void Evento_imprimir_informacion_de_vertice () {
	unsigned int i;
	if (
		evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
		evento.button.button == SDL_BUTTON_RIGHT
	) {
		puts ("");
		puts ("==========");
		printf ("Nombre del vértice: %s.\n", grafo.vertices[vertice_seleccionado].nombre);
		if (grafo.clasificacion & DIGRAFO) {
			printf ("Grado interno: %d.\n", grafo.vertices[vertice_seleccionado].grado_interno);
			printf ("Grado externo: %d.\n", grafo.vertices[vertice_seleccionado].grado_externo);
			puts ("Es adyacente a los vértices:");
			for (i = 0; i < grafo.numero_de_vertices; i++)
				if (MATRIZ_ENTRADA (matriz_de_adyacencia, vertice_seleccionado, i) != 0)
					printf ("- '%s'.\n", grafo.vertices[i].nombre);
			printf ("Los vértices adyacentes a '%s' son:\n", grafo.vertices[vertice_seleccionado].nombre);
			for (i = 0; i < grafo.numero_de_vertices; i++)
				if (MATRIZ_ENTRADA (matriz_de_adyacencia, i, vertice_seleccionado) != 0)
					printf ("- '%s'.\n", grafo.vertices[i].nombre);
			puts ("Líneas que salen del vértice:");
			for (i = 0; i < grafo.numero_de_lineas; i++)
				if (MATRIZ_ENTRADA (matriz_de_incidencia, vertice_seleccionado, i) == 1)
					printf ("- '%s'.\n", grafo.lineas[i].nombre);
			puts ("Líneas que entran al vértice:");
			for (i = 0; i < grafo.numero_de_lineas; i++)
				if (MATRIZ_ENTRADA (matriz_de_incidencia, vertice_seleccionado, i) == 2)
					printf ("- '%s'.\n", grafo.lineas[i].nombre);
		} else {
			printf ("Grado: %d.\n", grafo.vertices[vertice_seleccionado].grado_externo);
			puts ("Vértices adyacentes:");
			for (i = 0; i < grafo.numero_de_vertices; i++)
				if (MATRIZ_ENTRADA (matriz_de_adyacencia, vertice_seleccionado, i) != 0)
					printf ("- '%s'.\n", grafo.vertices[i].nombre);
			puts ("Líneas que inciden en el vértice:");
			for (i = 0; i < grafo.numero_de_lineas; i++)
				if (MATRIZ_ENTRADA (matriz_de_incidencia, vertice_seleccionado, i) != 0)
					printf ("- '%s'.\n", grafo.lineas[i].nombre);
		}
		if (grafo.vertices[vertice_seleccionado].clasificacion != 0) {
			printf ("Clasificación: ");
				if (grafo.vertices[vertice_seleccionado].clasificacion & VERTICE_AISLADO)
					puts ("aislado.");
				else if (
					(grafo.vertices[vertice_seleccionado].clasificacion & VERTICE_INICIAL) &&
					(grafo.clasificacion & DIGRAFO)
				)
					puts ("vértice inicial.");
				else if (
					(grafo.clasificacion & DIGRAFO) &&
					(grafo.vertices[vertice_seleccionado].clasificacion & VERTICE_FINAL)
				)
						puts ("vértie final.");
				else if (
					!(grafo.clasificacion & DIGRAFO) &&
					(grafo.vertices[vertice_seleccionado].clasificacion & VERTICE_COLGANTE)
				)
						puts ("vértie colgante.");
		}
		puts ("==========");
	}
}

void Dibujar_grafo () {

	Posicionar_vertices ();
	Inicializar_ventana ();

	puts ("En la ventana del grafo usted puede:");
	puts ("- Arrastrar la vista manteniendo presionado click izquierdo y arrastrando el ratón.");
	puts ("- Arrastrar un vértice manteniendo presionado click izquierdo sobre él y arrastrando el ratón.");
	puts ("- Presionar click derecho sobre un vértice para imprimir información sobre este en la terminal.");
	puts ("- Presionar 'Q' o 'ESCAPE' para cerrar la ventana.");
	puts ("- Presionar 'R' para reiniciar la posición del visualizador.");
	puts ("- Presionar 'Alt + R' para reiniciar las posiciones de los vértices.");

	ejecutandose = 1;
	while (ejecutandose == 1 && abortar == 0) {
		Establecer_color (255, 255, 255, 255);
		if (!SDL_RenderClear (renderer)) {
			puts ("SDL ERROR: No se pudo actualizar el renderizador.");
			Salida_panico ();
			break;
		}
		while (SDL_PollEvent (&evento) && abortar == 0) {
			Evento_cerrar_ventana ();
			Evento_reiniciar_posicion_del_visualizador ();
			Evento_reiniciar_posiciones_de_los_vertices ();
			Evento_activar_arrastre ();
			Evento_desactivar_arrastre ();
			Evento_mover_area_de_visualizacion ();
			Evento_arrastrar_vertice ();
			Evento_colisionar_vertices ();
			Evento_imprimir_informacion_de_vertice ();
		}
		Dibujar_relaciones ();
		Dibujar_vertices ();

		if(!SDL_RenderPresent (renderer)) {
			puts ("SDL ERROR: No se pudo actualizar el renderizador.");
			Salida_panico ();
		}
	}

	if (abortar == 0) Terminar_ventana ();
}
