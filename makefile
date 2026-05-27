NAME=Programa-Teoría_de_gráficas
COMPILER=gcc
FLAGS=-Wall -Werror -Wextra
LINKERS= -lSDL3 -lm
HEADERS_DIR=headers
SOURCE_DIR=source
SOURCE=$(SOURCE_DIR)/main.c $(SOURCE_DIR)/funciones_basicas.c $(SOURCE_DIR)/leer_relaciones.c $(SOURCE_DIR)/obtener_matrices.c $(SOURCE_DIR)/clasificar.c $(SOURCE_DIR)/dibujar_grafo.c

all:
	$(COMPILER) -o $(NAME) -I$(HEADERS_DIR) $(FLAGS) $(SOURCE) $(LINKERS)

windows:
	x86_64-w64-mingw32-g++ -o $(NAME) -I$(HEADERS_DIR) $(FLAGS) $(SOURCE) $(LINKERS)
