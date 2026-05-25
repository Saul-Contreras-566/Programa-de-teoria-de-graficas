/*
Aquí están las funciones que estaríamos ocupando comúnmente para la lectura de datos, estas nos
ayudarán a evitar que el usuario introduzca datos inesperados que rompan la funcionalidad de
nuestro programa.
*/

#include <stdio.h>
#include <stdlib.h>
#include "funciones_basicas.h"

// ------------------------------------------------------------------------------------------------
// FUNCIONES DE LECTURA DE TEXTO

void Leer_cadena (char **cadena) {
	/*
	Para usar esta función, es necesario que se pase la dirección del apuntador de la cadena, no
	el apuntador como tal, pues esta función reserva el espacio de memoria suficiente para
	leer una cadena.
	Tomen en cuenta que luego de usar la cadena, será necesario liberar el espacio de memoria
	dinámica.
	Un ejemplo de cómo usar esta función es:
		Leer_cadena (&respuesta);
	Aquí, `respuesta` es una variable apuntador.
	Cuando ya no es neceario seguir usando la cadena, se declara:
		free (respuesta);
	*/

	int i, fin_de_la_cadena = 0;

	// Limpieza del buffer
	fflush (stdin);

	// Reservando espacio para el primer caracter
	*cadena = (char *) malloc (sizeof (char));

	// Lectura del resto de caracteres
	for (i = 1; fin_de_la_cadena == 0; i++) {

		// Leyendo un caracter más
		(*cadena)[i - 1] = getchar ();

		// Reservando espacio para un caracter más
		*cadena = (char *) realloc (*cadena, i + 1);

		/* Señal para detener el proceso de lectura en caso de encontrar el caracter de
		salto de línea */
		fin_de_la_cadena = ((*cadena)[i - 1] == '\n') ? 1 : 0;
	}

	/* Cambiando el último caracter (el del salto de línea) por el caracter nulo para
	finalizar la cadena */
	(*cadena)[i - 2] = 0;

	// Limpiando el buffer una vez más
	fflush (stdin);
}



char Leer_caracter () {
	char caracter, *lectura;

	/* Leyendo una cadena entera en vez de un solo caracter en caso de que el usuario
	introduzca más de un caracter */
	Leer_cadena (&lectura);
	
	// Tomando el primer caracter de la cadena leída
	caracter = lectura[0];

	// Liberando la memoria dinámica de la cadena leída
	free (lectura);

	// Regresando el caracter anteriormente obtenido
	return caracter;
}





// ------------------------------------------------------------------------------------------------
// FUNCIONES DE LECTURA DE NÚMEROS ENTEROS

int Leer_entero () {
	char *lectura, *fin_de_la_cadena, copia_fin_de_la_cadena;
	int numero;

	// Atrapando al usuario en un bucle hasta que introduzca un número entero
	do {
		// Leyendo una cadena para luego verificar que sea un número
		Leer_cadena (&lectura);

		// Verificando que sea un número y obteniéndolo a la par
		numero = (int) strtol (lectura, &fin_de_la_cadena, 10);
		copia_fin_de_la_cadena = *fin_de_la_cadena;

		// En caso de no ser un número lo obtenido
		if (copia_fin_de_la_cadena != '\0')
			printf ("No ha insertado un número entero, vuelva a intentarlo\n");

		// Liberando espacio de la cadena leída para el número
		free (lectura);
	}
	while (copia_fin_de_la_cadena != '\0');

	// Regresando el número leído
	return numero;
}



int Leer_entero_entre (int minimo, int maximo) {
	int numero;

	// Atrapando al usuario en un bucle hasta que introduzca un número dentro del intervalo
	do {
		numero = Leer_entero ();
		if (numero < minimo || numero > maximo)
			printf ("Ha insertado un número que no está entre %d y %d, vuelva a intentarlo.\n", minimo, maximo);
	}
	while (numero < minimo || numero > maximo);

	return numero;
}



int Leer_entero_que_sea (char *comparador, int numero_criterio) {
	/*
	Esta función lee un entero que satisfaga el comparador. Este último es uno de los símbolos
	siguientes:
	<
	<=
	>
	>=
	Esta función es útil para leer un número dentro de un intervalo donde se compara un solo
	número y no dos como en el caso de la función `Leer_entero_entre`.
	*/

	int numero_leido, tipo_comparador = 0, valido;

	// Detectando el tipo de comparador
	tipo_comparador += (comparador[0] == '<') ? 1 : 0;
	tipo_comparador += (comparador[0] == '>') ? 3 : 0;
	tipo_comparador += (comparador[1] == '=') ? 1 : 0;

	// Validando el comparador
	if (tipo_comparador == 0) {
		puts ("ERROR: INVALID COMPARATOR"); // Mensaje en inglés para que resalte más
		return 0;
	}
	
	// Atrapando al usuario hasta que introduzca un número que vuelva verdad la comparación
	do {
		// Reseteando la validez del número introducido
		valido = 1;

		// Leyendo el número
		numero_leido = Leer_entero ();

		// Realizando la comparación correspondiente
		switch (tipo_comparador) {
			case 1:
				if ( !(numero_leido < numero_criterio) ) {
					printf ("No ha introducido un número menor que %d, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
			case 2:
				if ( !(numero_leido <= numero_criterio) ) {
					printf ("No ha introducido un número menor o igual que %d, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
			case 3:
				if ( !(numero_leido > numero_criterio) ) {
					printf ("No ha introducido un número mayor que %d, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
			case 4:
				if ( !(numero_leido >= numero_criterio) ) {
					printf ("No ha introducido un número mayor o igual que %d, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
		}
	}
	while (valido == 0);

	// Regresando el número leido
	return numero_leido;
}





// ------------------------------------------------------------------------------------------------
// FUNCIONES DE LECTURA DE NÚMEROS REALES

double Leer_real () {
	char *lectura, *fin_de_la_cadena, copia_fin_de_la_cadena;
	double numero;

	// Atrapando al usuario en un bucle hasta que introduzca un número real
	do {
		// Leyendo una cadena para luego verificar que sea un número
		Leer_cadena (&lectura);

		// Verificando que sea un número y obteniéndolo a la par
		numero = strtod (lectura, &fin_de_la_cadena);
		copia_fin_de_la_cadena = *fin_de_la_cadena;

		// En caso de no ser un número lo obtenido
		if (copia_fin_de_la_cadena != '\0')
			printf ("No ha insertado un número, vuelva a intentarlo\n");

		// Liberando espacio de la cadena leída para el número
		free (lectura);
	}
	while (copia_fin_de_la_cadena != '\0');

	return numero;
}



double Leer_real_entre (double minimo, double maximo) {
	double numero;

	// Atrapando al usuario en un bucle hasta que introduzca un número dentro del intervalo
	do {
		numero = Leer_real ();
		if (numero < minimo || numero > maximo)
			printf ("Ha insertado un número que no está entre %lf y %lf, vuelva a intentarlo.\n", minimo, maximo);
	}
	while (numero < minimo || numero > maximo);

	return numero;
}



double Leer_real_que_sea (char *comparador, double numero_criterio) {
	/*
	Esta función lee un real que satisfaga el comparador. Este último es uno de los símbolos
	siguientes:
	<
	<=
	>
	>=
	Esta función es útil para leer un número dentro de un intervalo donde se compara un solo
	número y no dos como en el caso de la función `Leer_real_entre`.
	*/

	int tipo_comparador = 0, valido;
	double numero_leido;

	// Detectando el tipo de comparador
	tipo_comparador += (comparador[0] == '<') ? 1 : 0;
	tipo_comparador += (comparador[0] == '>') ? 3 : 0;
	tipo_comparador += (comparador[1] == '=') ? 1 : 0;

	// Validando el comparador
	if (tipo_comparador == 0) {
		puts ("ERROR: INVALID COMPARATOR"); // Mensaje en inglés para que resalte más
		return 0;
	}
	
	// Atrapando al usuario hasta que introduzca un número que vuelva verdad la comparación
	do {
		// Reseteando la validez del número introducido
		valido = 1;

		// Leyendo el número
		numero_leido = Leer_real ();

		// Realizando la comparación correspondiente
		switch (tipo_comparador) {
			case 1:
				if ( !(numero_leido < numero_criterio) ) {
					printf ("No ha introducido un número menor que %lf, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
			case 2:
				if ( !(numero_leido <= numero_criterio) ) {
					printf ("No ha introducido un número menor o igual que %lf, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
			case 3:
				if ( !(numero_leido > numero_criterio) ) {
					printf ("No ha introducido un número mayor que %lf, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
			case 4:
				if ( !(numero_leido >= numero_criterio) ) {
					printf ("No ha introducido un número mayor o igual que %lf, vuelva a intentarlo.\n", numero_criterio);
					valido = 0;
				}
				break;
		}
	}
	while (valido == 0);

	// Regresando el número leido
	return numero_leido;
}





// ------------------------------------------------------------------------------------------------
// OTRAS FUNCIONES

int Pregunta_cerrada (char *mensaje) {
	char contestacion; // Usado para leer la respuesta del usuario
	int respuesta; // Usado para envíar un valor de salida en la función y para validar la respuesta

	do {
		// Haciendo la pregunta y pidiendo respuesta
		printf ("%s (S / n): ", mensaje);
		contestacion = Leer_caracter ();
		contestacion -= (contestacion >= 'a' && contestacion <= 'z') ? 32 : 0;

		// Verificando la respuesta
		if (contestacion == 'S')
			respuesta = 1;
		else if (contestacion == 'N')
			respuesta = 0;
		// En caso de no ser válida la respuesta
		else {
			puts ("No ha dado una respuesta válida, vuelva a intentarlo.");
			respuesta = -1;
		}
	}
	while (respuesta == -1);

	return respuesta;
}



double Redondear (double numero, unsigned int digitos) {
	double fact = pow(10.0, (double) digitos);
	return round(numero * fact) / fact;
}



void LIMPIAR_PANTALLA () {
	/*
	Esta función permite limpiar la pantalla de la terminal dependiendo el sistema operativo
	si es Windows o Linux.
	*/

	#ifdef _WIN32
		system ("cls");
	#elif __linux__
		system ("clear");
	#endif
}



void ESPERAR_TECLA () {
	/*
	Esta función permite pausar el programa hasta que se presione una tecla dependiendo del
	sistema operativo si es Windows o Linux.
	*/

	fflush (stdin);
	#ifdef _WIN32
		system ("pause");
	#elif __linux__
		system ("bash -c 'read -r -s -N 1 -p \"Presione cualquier tecla para continuar...\"'");
	#endif
	fflush (stdin);
}
