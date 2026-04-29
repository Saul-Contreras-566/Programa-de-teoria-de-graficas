#include <stdio.h>

// funcion entrada para el tipo de grafica
// 1 si es grafica (no dirigida), 2 si es digraficaaa (dirigida)
int seleccionar_tipo_grafica() {
    int opcion = 0;

    while (opcion != 1 && opcion != 2) {
        printf("\n=== TIPO DE GRAFICA ===\n");
        printf("1. Grafica (No dirigida)\n");
        printf("2. Digrafica (Dirigida)\n");
        printf("Elige el tipo de grafica ingresando 1 o 2: ");
        
        // lee el dato
        if (scanf("%d", &opcion) != 1) {
            // si el usuario mete una letra por error, limpiamos buffer
            while (getchar() != '\n');
            printf("Error: Entrada no valida.\n");
        } else if (opcion != 1 && opcion != 2) {
            // si mete un numero distinto a 1 o 2
            printf("Error: Solo puedes ingresar 1 o 2.\n");
        }
    }

    if (opcion == 1) {
        printf("-> Has seleccionado: Grafica (No dirigida)\n");
    } else {
        printf("-> Has seleccionado: Digrafica (Dirigida)\n");
    }

    return opcion;
}
