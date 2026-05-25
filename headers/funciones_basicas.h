/*
Funciones básicas que ocuparemos principalmente para la lectura de datos introducidos por el
usuario.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef FUNCIONES_BASICAS_H
#define FUNCIONES_BASICAS_H
	// Constantes
	#define PI 3.141592653589793
	#define EULER 2.718281828459045

	// Funciones de lectura de texto
	void Leer_cadena (char **cadena);
	char Leer_caracter ();

	// Funciones de lectura de números enteros
	int Leer_entero ();
	int Leer_entero_entre (int minimo, int maximo);
	int Leer_entero_que_sea (char *comparador, int numero_criterio);

	// Funciones de lectura de números reales
	double Leer_real ();
	double Leer_real_entre (double minimo, double maximo);
	double Leer_real_que_sea (char *comparador, double numero_criterio);

	// Otras funciones
	int Pregunta_cerrada (char *mensaje);
	double Redondear(double numero, unsigned int digitos);
	void LIMPIAR_PANTALLA ();
	void ESPERAR_TECLA ();
#endif
