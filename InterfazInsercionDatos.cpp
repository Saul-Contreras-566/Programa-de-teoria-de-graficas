/*Aqui DEBEMOS CONTEMPLAR TODO LO ANTERIOR ELABORADO (FUNCIONES: Validaciones y Campo de entrada de Interfaz) YA QUE NECESITAMOS de ello
Validar enteros, validar vertices y seleccionar tiipo de graficas. (Recordemos que ya se encuentran en las funciones validaciones y campos de entrada Interfaz).*/

#include <stdio.h>
#include <stdlib.h>

// funcion principal de inserci�n de datos
void insercion_de_datos() {
    char buffer[100];
    int num_vertices = -1;
    int num_lineas = -1;
    int tipo_grafica = 0;

    printf("\n=== 2.1 INSERCION DE DATOS ===\n");

    // lee el numero de vertices
    while (1) {
        printf("1. Ingresa el numero de vertices (maximo 20): ");
        scanf("%s", buffer);
        
        if (validar_entero(buffer)) {
            num_vertices = atoi(buffer);
            if (validar_vertice(num_vertices)) {
                break; //dato es correcto, salimos del ciclo
            } else {
                printf("  Error: El numero de vertices debe estar entre 1 y 20.\n");
            }
        } else {
            printf("  Error: Debes ingresar un numero entero valido.\n");
        }
    }

    // lee el numero de lineas
    while (1) {
        printf("\n2. Ingresa el numero de lineas (aristas/relaciones): ");
        scanf("%s", buffer);
        
        if (validar_entero(buffer)) {
            num_lineas = atoi(buffer);
            if (num_lineas >= 0) {
                break;
            } else {
                printf("  Error: El numero de lineas no puede ser negativo.\n");
            }
        } else {
            printf("  Error: Debes ingresar un numero entero valido.\n");
        }
    }

    //lee el tipo de grafico
    printf("\n3. Seleccion del tipo de grafico:\n");
    tipo_grafica = seleccionar_tipo_grafica();

    //lee una lista con las relaciones entre los vertices
    printf("\n4. Ingresa las relaciones entre los vertices (origen y destino):\n");
    //Nota: si los v�rtices van del 0 al (num_vertices - 1)
    for (int i = 0; i < num_lineas; i++) {
        int origen = -1, destino = -1;
        
        printf("- Linea %d:\n", i + 1);
        
        // validar vertice de salida
        while (1) {
            printf("    Vertice de salida: ");
            scanf("%s", buffer);
            if (validar_entero(buffer)) {
                origen = atoi(buffer);
                if (validar_vertice(origen) && origen < num_vertices) {
                    break;
                } else {
                    printf("    Error: Vertice invalido. Ingresa un numero entre 0 y %d.\n", num_vertices - 1);
                }
            } else {
                printf("    Error: Ingresa un numero entero valido.\n");
            }
        }

        // validar vertice de llegada
        while (1) {
            printf("    Vertice de llegada: ");
            scanf("%s", buffer);
            if (validar_entero(buffer)) {
                destino = atoi(buffer);
                if (validar_vertice(destino) && destino < num_vertices) {
                    break;
                } else {
                    printf("    Error: Vertice invalido. Ingresa un numero entre 0 y %d.\n", num_vertices - 1);
                }
            } else {
                printf("    Error: Ingresa un numero entero valido.\n");
            }
        }
        
        printf("    -> Relacion guardada: (%d, %d)\n", origen, destino);
    }

    //boton para generar la grafica
    printf("\n5. [Boton 'Generar Grafica' presionado]\n");
    printf("-> Datos recolectados exitosamente. Pasando al procesamiento de datos...\n");
    printf("==============================\n");
}
