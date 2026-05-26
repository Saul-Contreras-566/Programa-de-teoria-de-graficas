#include <stdio.h>
#include <math.h>
#include <SDL/SDL3.h>
#include "funciones_basicas.h"

#define RECTANGULOS_POR_CIRCULO 64
#define RADIO_VERTICES 50.0f
#define GROSOR_LINEA 15

// Variables para la ventana de dibujo
SDL_Window *window;
SDL_Renderer *renderer;

void Establecer_color (int r, int g, int b, int a) {
	if (!SDL_SetRenderDrawColor (renderer, r, g, b, a)) {
		puts ("SDL ERROR: No se pudo establecer el color de dibujo del renderizador correctamente.");
		Salida_panico ();
	}
}

void Dibujar_rectangulo (*rect) {
	if (!SDL_RenderFillRect (renderer, &rect)) {
		puts ("SDL ERROR: No se pudo dibujar correctamente el círculo en el renderizador.");
		Salida_panico ();
	}
}

void Dibujar_circulo (float x, float y, float radio) {
	int i;
	SDL_FRect rect;

	for (i = 0; i < RECTANGULOS_POR_CIRCULO; i++) {
		rect.w = (float) cos ((double) (i+1) / (float) (RECTANGULOS_POR_CIRCULO + 1) * PI / 2.0) * 2.0f * radio;
		rect.h = (float) sin ((double) (i+1) / (float) (RECTANGULOS_POR_CIRCULO + 1) * PI / 2.0) * 2.0f * radio;
		rect.x = x + rect.w / 2.0f;
		rect.y = y + rect.h / 2.0f;
		Dibujar_rectangulo (renderer, &rect);

	}
}

void Dibujar_vertice (Vertice vertice) {
	Establecer_color (50, 200, 200, 255);
	Dibujar_circulo (vertice.x, vertice.y, RADIO_CIRCULOS);
	Establecer_color (100, 255, 255, 255);
	Dibujar_circulo (vertice.x, vertice.y, RADIO_CIRCULOS - 10.0f);
}

void Dibujar_vertices (Grafo grafo) {
	int i;

	for (i = 0; i < grafo.numero_vertices; i++)
		Dibujar_vertice (grafo.vertices[i]);
}

void Dibujar_linea (float x1, float y1, float x2, float y2) {
	int i;
	float xi, yi, xf, yf;
	double radianes;

	if (x1 == x2 && y1 == y2) {
		Dibujar_circulo (x1, y1, (float) GROSOR_LINEA);
	} else {
		radianes = atan ((double) ((y2 - y1)/(x2 - x1)));
		radianes += PI / 2.0;
		xi = x1 - (float) (cos (radianes) * (double) GROSOR_LINEA / 2.0);
		yi = y1 - (float) (sin (radianes) * (double) GROSOR_LINEA / 2.0);
		xf = x2 - (float) (cos (radianes) * (double) GROSOR_LINEA / 2.0);
		yf = y2 - (float) (sin (radianes) * (double) GROSOR_LINEA / 2.0);
		for (i = 0; i < 32; i++) {
			xi += (float) cos (radianes) * (float) GROSOR_LINEA * (float) i / 32.0f;
			yi += (float) sin (radianes) * (float) GROSOR_LINEA * (float) i / 32.0f;
			xf += (float) cos (radianes) * (float) GROSOR_LINEA * (float) i / 32.0f;
			yf += (float) sin (radianes) * (float) GROSOR_LINEA * (float) i / 32.0f;
			if(!SDL_RenderLine (renderer, xi, yi, xf, yf)) {
				puts ("SDL ERROR: No se pudo dibujar correctamente la línea en el renderizador.");
				Salida_panico ();
			}
		}
	}
}

void Dibujar_relacion (Linea linea) {
	Dibujar_linea ((*linea.origen).x, (*linea.origen).y, (*linea.destino).x, (*linea.destino).y);
}

void Dibujar_relaciones (Grafo grafo) {
	int i;

	for (i = 0; grafo.numero_de_lineas; i++)
		Dibujar_relacion (grafo.lineas[i]);
}

void Inicializar_ventana () {
	if (!SDL_Init ()) {
		puts ("SDL ERROR: No se pudo inicializar SDL.");
		Salida_panico ();
	}
	window = SDL_CreateWindow ("Grafo", 1280, 720, 0);
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

void Dibujar_grafo (Grafo grafo) {
	int ejecutandose = 1;
	SDL_Event evento;

	Inicializar_ventana ();

	while (ejecutandose == 1) {
		while (SDL_PollEvent (&evento) {
			if (evento.type == SDL_EVENT_QUIT) {
				ejecutandose = 0;
			} else if (evento.type == SDL_EVENT_KEY_DOWN) {
				if (evento.key.key == SDLK_Q || evento.key.key == SDLK_ESCAPE) {
					ejecutandose = 0;
				}
			}
		}
		Dibujar_lineas (grafo);
		Dibujar_vertices (grafo);

		if(!SDL_RenderPresent (renderer)) {
			puts ("SDL ERROR: No se pudo actualizar el renderizador.");
			Salida_panico ();
		}
	}

	Terminar_ventana ();
}
