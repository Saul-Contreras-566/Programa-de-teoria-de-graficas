#include <stdio.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "funciones_basicas.h"
#include "grafos_y_matrices.h"

// Variables para la ventana de dibujo
TTF_Font *fuente = NULL;
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
	unsigned int i;
	abortar = 1;
	puts ("Abortando SDL.");
	TTF_CloseFont (fuente);
	if (window != NULL) SDL_DestroyWindow (window);
	if (renderer != NULL) SDL_DestroyRenderer (renderer);
	for (i = 0; i < grafo.numero_de_lineas; i++)
		if (grafo.lineas[i].etiqueta.textura != NULL)
			SDL_DestroyTexture (grafo.lineas[i].etiqueta.textura);
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].etiqueta.textura != NULL)
			SDL_DestroyTexture (grafo.vertices[i].etiqueta.textura);
	TTF_Quit ();
	SDL_Quit ();
}

void Inicializar_ventana () {
	abortar = 0;
	if (!SDL_Init (SDL_INIT_VIDEO)) {
		puts ("SDL ERROR: No se pudo inicializar SDL.");
		printf ("Mensaje de SDL:\n%s\n", SDL_GetError ());
		Salida_panico ();
	}
	if (!TTF_Init ()) {
		puts ("SDL ERROR: No se pudo inicializar la biblioteca SDL3-TTF.");
		printf ("Mensaje de SDL:\n%s\n", SDL_GetError ());
		Salida_panico ();
	}
	fuente = TTF_OpenFont ("LiberationMono-Regular.ttf", 32);
	if (fuente == NULL) {
		puts ("SDL ERROR: No se pudo cargar la fuente de letra 'Liberation Mono'.");
		printf ("Mensaje de SDL:\n%s\n", SDL_GetError ());
		Salida_panico ();
	}
	window = SDL_CreateWindow ("Grafo", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (window == NULL) {
		puts ("SDL ERROR: No se pudo crear la ventana donde se dibujaría el grafo.");
		printf ("Mensaje de SDL:\n%s\n", SDL_GetError ());
		Salida_panico ();
	}
	renderer = SDL_CreateRenderer (window, "");
	if (renderer == NULL) {
		puts ("SDL ERROR: No se pudo crear el renderizador en la ventana que dibuja el grafo.");
		printf ("Mensaje de SDL:\n%s\n", SDL_GetError ());
		Salida_panico ();
	}
}

void Terminar_ventana () {
	unsigned int i;
	for (i = 0; i < grafo.numero_de_lineas; i++)
		if (grafo.lineas[i].etiqueta.textura != NULL)
			SDL_DestroyTexture (grafo.lineas[i].etiqueta.textura);
	for (i = 0; i < grafo.numero_de_vertices; i++)
		if (grafo.vertices[i].etiqueta.textura != NULL)
			SDL_DestroyTexture (grafo.vertices[i].etiqueta.textura);
	TTF_CloseFont (fuente);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	TTF_Quit ();
	SDL_Quit ();
}

float Obtener_distancia (float x1, float y1, float x2, float y2) {
	return (float) sqrt (pow ((double) (x2 - x1), 2.0) + pow ((double) (y2 - y1), 2.0));
}

void Posicionar_vertices () {
	unsigned int i;
	double radianes = 2.0 * PI / (double) grafo.numero_de_vertices;
	double radio;

	switch (grafo.numero_de_vertices) {
		case 0: radio = 0; break;
		case 1: radio = 0; break;
		default: radio = 2.25f * RADIO_VERTICES * grafo.numero_de_vertices / PI; break;
	}

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

void Crear_etiquetas () {
	unsigned int i;
	SDL_Color color = {0, 0, 0, 255};
	SDL_Surface *texto = NULL;

	for (i = 0; i < grafo.numero_de_vertices; i++) {
		texto = TTF_RenderText_Blended (fuente, grafo.vertices[i].nombre, 0, color);
		grafo.vertices[i].etiqueta.textura = SDL_CreateTextureFromSurface (renderer, texto);
		grafo.vertices[i].etiqueta.w = (float) (*texto).w;
		grafo.vertices[i].etiqueta.h = (float) (*texto).h;
		SDL_DestroySurface (texto);
	}

	for (i = 0; i < grafo.numero_de_lineas; i++) {
		texto = TTF_RenderText_Blended (fuente, grafo.lineas[i].nombre, 0, color);
		grafo.lineas[i].etiqueta.textura = SDL_CreateTextureFromSurface (renderer, texto);
		grafo.lineas[i].etiqueta.w = (float) (*texto).w;
		grafo.lineas[i].etiqueta.h = (float) (*texto).h;
		SDL_DestroySurface (texto);
	}
}

void Dibujar_vertice (Vertice vertice) {
	SDL_FRect cuadro_de_texto;

	Establecer_color (50, 200, 200, 255);
	Dibujar_circulo (vertice.x, vertice.y, RADIO_VERTICES);
	Establecer_color (100, 255, 255, 255);
	Dibujar_circulo (vertice.x, vertice.y, RADIO_VERTICES - 10.0f);

	cuadro_de_texto.x = vertice.x - vertice.etiqueta.w / 2.0f;
	cuadro_de_texto.y = vertice.y - vertice.etiqueta.h / 2.0f;
	cuadro_de_texto.w = vertice.etiqueta.w;
	cuadro_de_texto.h = vertice.etiqueta.h;
	SDL_RenderTexture (renderer, vertice.etiqueta.textura, NULL, &cuadro_de_texto);
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

void Dibujar_relacion (Linea linea, float grosor, float semieje) {
	float radio = Obtener_distancia (
		(*linea.origen).x,
		(*linea.origen).y,
		(*linea.destino).x,
		(*linea.destino).y
	) / 2.0f;
	float xc, yc, x1, y1, x2, y2; // Variables para dibujar lineas.
	xc = ((*linea.destino).x - (*linea.origen).x) / 2.0f;
	yc = ((*linea.destino).y - (*linea.origen).y) / 2.0f;
	double radianes = Obtener_angulo (xc, yc);
	double radianes_elipse;
	double x_elipse, y_elipse;
	int i;
	SDL_FRect cuadro_de_texto;

	if (semieje == 0.0f)
		Dibujar_linea (
			(*linea.origen).x,
			(*linea.origen).y,
			(*linea.destino).x,
			(*linea.destino).y,
			grosor
		);
	else 
		for (i = 0; i < 32; i++) {
			radianes_elipse = PI * (float) i / 32.0f;
			x_elipse = radio * (1 - cos (radianes_elipse));
			y_elipse = semieje * sin (radianes_elipse);
			x1 = (*linea.origen).x + x_elipse * cos (radianes) + y_elipse * cos (radianes + PI / 2.0);
			y1 = (*linea.origen).y + x_elipse * sin (radianes) + y_elipse * sin (radianes + PI / 2.0);
			radianes_elipse = PI * (float) (i + 1) / 32.0f;
			x_elipse = radio * (1 - cos (radianes_elipse));
			y_elipse = semieje * sin (radianes_elipse);
			x2 = (*linea.origen).x + x_elipse * cos (radianes) + y_elipse * cos (radianes + PI / 2.0);
			y2 = (*linea.origen).y + x_elipse * sin (radianes) + y_elipse * sin (radianes + PI / 2.0);
			Dibujar_linea (x1, y1, x2, y2, grosor);
		}

	radianes = Obtener_angulo (
		(*linea.destino).x - (*linea.origen).x,
		(*linea.destino).y - (*linea.origen).y
	) + PI / 2.0;
	xc += (*linea.origen).x + semieje * (float) cos (radianes);
	yc += (*linea.origen).y + semieje * (float) sin (radianes);

	if (grafo.clasificacion & DIGRAFO) {
		Dibujar_triangulo (xc, yc, 20.0f, 45.0f, radianes + PI / 2.0, grosor);
	}

	cuadro_de_texto.x = xc + 30.0f - linea.etiqueta.w / 2.0f;
	cuadro_de_texto.y = yc + 30.0f - linea.etiqueta.h / 2.0f;
	cuadro_de_texto.w = linea.etiqueta.w;
	cuadro_de_texto.h = linea.etiqueta.h;
	SDL_RenderTexture (renderer, linea.etiqueta.textura, NULL, &cuadro_de_texto);
}

void Dibujar_circunferencia (float x, float y, float grosor, float radio) {
	int i;
	float x1, y1, x2, y2; // Estas variables son para graficar lineas.

	for (i = 0; i < 32; i++) {
		x1 = x + radio * (float) cos (2.0 * PI * (double) i / 32.0);
		y1 = y + radio * (float) sin (2.0 * PI * (double) i / 32.0);
		x2 = x + radio * (float) cos (2.0 * PI * (double) (i+1) / 32.0);
		y2 = y + radio * (float) sin (2.0 * PI * (double) (i+1) / 32.0);
		Dibujar_linea (x1, y1, x2, y2, grosor);
	}
}

void Dibujar_bucle (Linea linea, float grosor, float radio) {
	SDL_FRect cuadro_de_texto;
	double radianes = Obtener_angulo (
		(*linea.origen).x - posx - WINDOW_WIDTH / 2.0f,
		(*linea.origen).y - posy - WINDOW_HEIGHT / 2.0f
	);
	float coseno = (float) cos (radianes);
	float seno = (float) sin (radianes);
	float x = radio * coseno;
	float y = radio * seno;
	Dibujar_circunferencia (
		(*linea.origen).x + x,
		(*linea.origen).y + y,
		grosor,
		radio
	);
	if (grafo.clasificacion & DIGRAFO)
		Dibujar_triangulo (
			(*linea.origen).x + 2.0f * x,
			(*linea.origen).y + 2.0f * y,
			20.0f,
			45.0f,
			PI / 2.0 + radianes,
			grosor
		);
	
	cuadro_de_texto.x = (*linea.origen).x + 2.0f * x - linea.etiqueta.w / 2.0f + 30.0f * coseno;
	cuadro_de_texto.y = (*linea.origen).y + 2.0f * y - linea.etiqueta.h / 2.0f + 30.0f * seno;
	cuadro_de_texto.w = linea.etiqueta.w;
	cuadro_de_texto.h = linea.etiqueta.h;
	SDL_RenderTexture (renderer, linea.etiqueta.textura, NULL, &cuadro_de_texto);
}

void Dibujar_relaciones () {
	unsigned int i, j;
	unsigned int lineas;
	float ancho;
	float n;
	float sentido;

	Establecer_color (50, 200, 200, 255);

	// Marcando todas las líneas como no dibujadas.
	for (i = 0; i < grafo.numero_de_lineas; i++)
		grafo.lineas[i].dibujado = 0;
	
	// Dibujando líneas.
	for (i = 0; i < grafo.numero_de_lineas; i++) {
		if (grafo.lineas[i].dibujado == 1)
			continue;
		lineas = MATRIZ_ENTRADA (
			matriz_de_adyacencia,
			(unsigned int) (grafo.lineas[i].origen - &grafo.vertices[0]),
			(unsigned int) (grafo.lineas[i].destino - &grafo.vertices[0])
		) + ((grafo.clasificacion & DIGRAFO) && grafo.lineas[i].origen != grafo.lineas[i].destino ?
			MATRIZ_ENTRADA (
				matriz_de_adyacencia,
				(unsigned int) (grafo.lineas[i].destino - &grafo.vertices[0]),
				(unsigned int) (grafo.lineas[i].origen - &grafo.vertices[0])
			) : 0
		); // Obtiene el número de líneas que conectan dos vértices, toma en cuenta si el grafo es dirigido o no.
		ancho = ANCHO_SEMIEJE * (float) (lineas - 1) / 2.0f;
		n = 0.0f;
		if (grafo.lineas[i].origen == grafo.lineas[i].destino)
			n++;
		// Dibujando todas las líneas que inciden en los mismos vértices que la línea i-ésima.
		for (j = 0; j < grafo.numero_de_lineas; j++) {
			sentido = 0.0f;
			if (grafo.lineas[j].origen == grafo.lineas[i].origen && grafo.lineas[j].destino == grafo.lineas[i].destino)
				sentido = 1.0f;
			else if (grafo.lineas[j].origen == grafo.lineas[i].destino && grafo.lineas[j].destino == grafo.lineas[i].origen)
				sentido = -1.0f;
			else
				sentido = 0.0f;
			if (sentido != 0.0f) {
				if (grafo.lineas[j].origen == grafo.lineas[j].destino)
					Dibujar_bucle (grafo.lineas[j], GROSOR_LINEAS, ANCHO_SEMIEJE * (n++) / 2.0f);
				else
					Dibujar_relacion (grafo.lineas[j], GROSOR_LINEAS, sentido * (ancho - ANCHO_SEMIEJE * (n++)));
				grafo.lineas[j].dibujado = 1;
			}
		}
		grafo.lineas[i].dibujado = 1;
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
	Crear_etiquetas ();

	puts ("");
	puts ("En la ventana del grafo usted puede:");
	puts ("- Arrastrar la vista manteniendo presionado click izquierdo y arrastrando el ratón.");
	puts ("- Arrastrar un vértice manteniendo presionado click izquierdo sobre él y arrastrando el ratón.");
	puts ("- Presionar click derecho sobre un vértice para imprimir información sobre este en la terminal.");
	puts ("- Presionar 'Q' o 'ESCAPE' para cerrar la ventana.");
	puts ("- Presionar 'R' para reiniciar la posición del visualizador.");
	puts ("- Presionar 'Alt + R' para reiniciar las posiciones de los vértices.");
	puts ("");

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
