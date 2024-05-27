#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include "tdas/queue.h"
#include <string.h>
//Estructuras principales
typedef struct {

		int square[3][3]; // Matriz 3x3 que representa el tablero
		int x;    // Posición x del espacio vacío
		int y;    // Posición y del espacio vacío
		int nActions;
} State;
typedef struct Node {
		State state;
		struct Node* parent;
} Node;


int isValidMove(int x, int y) { //verificar si el movimiento es valido o no
  return (x >= 0 && x < 3 && y >= 0 && y < 3); 
}
void imprimirEstado(const State *estado) {
		for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
						if (estado->square[i][j] == 0)
								printf("x ");
						else
								printf("%d ", estado->square[i][j]);
				}
				printf("\n");
		}
}
void copiarEstado(State original, State* new) { //copiar el estado original en el nuevo estado
     for (int i = 0; i < 3; i++)
         for (int j = 0; j < 3; j++)
             new->square[i][j] = original.square[i][j]; 
     new->x = original.x;
     new->y = original.y;
     new->nActions = original.nActions;
 }
int isFinal(const State* estado) {
		State estadoFinal = {
				{{0, 1, 2},
				 {3, 4, 5},
				{6, 7, 8}},
				0,0
		};
		for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
						if (estado->square[i][j] != estadoFinal.square[i][j])
								return 0;
				}
		}
		return 1;
}

void move(State* actual, int opcion) {
		int x = actual->x;
		int y = actual->y;
		int temp;
		switch (opcion) {
				case 1: // Mover espacio vacío hacia arriba
						if (isValidMove(x , y - 1)) {

								temp = actual->square[x][y - 1];
                actual->square[x][y - 1] = actual->square[x][y];
                actual->square[x][y] = temp;
                actual->y = y - 1;
						}
						break;
				case 2: // Mover espacio vacío hacia abajo
						if (isValidMove(x , y + 1)) {
								temp = actual->square[x][y + 1];
                actual->square[x][y + 1] = actual->square[x][y];
                actual->square[x][y] = temp;
                actual->y = y + 1;
						}
						break;
				case 3: // Mover espacio vacío hacia la izquierda
						if (isValidMove(x - 1, y)) {
								temp = actual->square[x - 1][y];
                actual->square[x - 1][y] = actual->square[x][y];
                actual->square[x][y] = temp;
                actual->x = x - 1;
						}
						break;
      
				case 4: // Mover espacio vacío hacia la derecha
						if (isValidMove(x + 1, y)) {
                temp = actual->square[x + 1][y];
                actual->square[x + 1][y] = actual->square[x][y];
                actual->square[x][y] = temp;
                actual->x = x + 1;
						}
						break;
		}
}
State* transition(State* oldState, int movimiento) {
		State *nuevoEstado = (State*) malloc(sizeof(State));
		copiarEstado(*oldState, nuevoEstado);
		move(nuevoEstado, movimiento);
    nuevoEstado->nActions++;
		return nuevoEstado;
}
List *getAdjNodes(Node* parentNode) {
		List *adjList = list_create();
		int x = parentNode->state.x;
		int y = parentNode->state.y;
		for (int i = 1; i <= 4; i++) {
				State *newState = transition(&parentNode->state, i);
				if (newState->x != x || newState->y != y) {
						Node *newNode = (Node*) malloc(sizeof(Node));
						newNode->state = *newState;
						newNode->parent = parentNode;
						list_pushBack(adjList, newNode);
				} else {
						free(newState);
				}
		}
		return adjList;
}

void dfs(State estadoInicial, int cont) {
	Stack* stack = stack_create(stack);

	printf("\nEstado del puzzle:\n\n");
	imprimirEstado(&estadoInicial);
	printf("\n");

	Node *root = (Node *) malloc(sizeof(Node ));

	root->parent = NULL;
	root->state = estadoInicial;

	stack_push(stack, root);

	while (list_size(stack) > 0) {

		Node *nodoActual = (Node *) stack_top(stack);
		stack_pop(stack);

		if (nodoActual->state.nActions >= 15)
				continue;

		if (isFinal(&nodoActual->state)) {

        printf("8-Puzzle a sido resuelto! :D\n");
        printf("Total de movimientos: %d\n\n", cont);

				Node *nodo = nodoActual;
				int totalPasos = nodoActual->state.nActions;

				while (nodo != NULL) {

						if (totalPasos == 0) 
								printf("Estado inicial:\n\n");

						else
								printf("Paso %d:\n\n", totalPasos);                  
						imprimirEstado(&nodo->state);
						printf("\n");
            nodo = nodo->parent;
            totalPasos--;
				}

				stack_clean(stack);
				free(nodo);

				return;
		}

		List *adjNodes = getAdjNodes(nodoActual);
		void *aux = list_first(adjNodes);

		while (aux != NULL) {

				stack_push(stack, aux);
				aux = list_next(adjNodes);

		}

		list_clean(adjNodes);
		cont++;

	}

	printf("The solution was not found within the limited range.\n");
	stack_clean(stack);
}
void bfs(State estadoInicial, int cont) {

	Queue* queue = queue_create(queue);

	printf("Estado del puzzle:\n\n\n");
	imprimirEstado(&estadoInicial);
	printf("\n");

	Node *raiz = (Node *) malloc(sizeof(Node ));
	raiz->parent = NULL;
	raiz->state = estadoInicial;
	queue_insert(queue, raiz);

	while (list_size(queue) > 0) {

			Node *NodoActual = (Node *) queue_front(queue);
			queue_remove(queue);
			if (cont >= 10000000) {
					printf("Se a alcanzado un limite de movimientos\n");
          printf("Proceso finalizado.");
					return;
			}
    
			if (isFinal(&NodoActual->state)) {
					printf("8-Puzzle a sido resuelto! :D\n");
					printf("Total de movimientos: %d\n\n", cont);
					Node *nodo = NodoActual;
					int totalPasos = NodoActual->state.nActions;

					while (nodo != NULL) {

							if (totalPasos == 0) 
									printf("Estado inicial:\n\n");  

							else
									printf("Paso %d:\n\n", totalPasos);                  
							imprimirEstado(&nodo->state);
							printf("\n");
							nodo = nodo->parent;
              totalPasos--;

					}
					queue_clean(queue);
					free(nodo);
					return;
			}
			List *adjNodes = getAdjNodes(NodoActual);
			void *aux = list_first(adjNodes);
			while (aux != NULL) {
					queue_insert(queue, aux);
					aux = list_next(adjNodes);
			}
			list_clean(adjNodes);
			cont++;
	}
	printf("La solucion no se pudo encontrar :(\n");
	queue_clean(queue);
}

int main() {
		State estado_inicial = {
				{{0, 2, 8}, // Primera fila (0 representa espacio vacío)
				 {1, 3, 4}, // Segunda fila
				 {6, 5, 7}, // Tercera fila
				 },  
				0, 0  
		};
		estado_inicial.nActions = 0;

		int cont = 0;
    int opcion;
		do {
        limpiarPantalla();
        puts("========================================\n");
        puts("        Bienvenido al 8-Puzzle!\n");
        puts("========================================\n");
        puts("Porfavor escoja una opcion:\n");
    
        puts("1) Búsqueda en Profundidad (DFS)");
        puts("2) Buscar en Anchura (BFS)");
        puts("3) Salir\n");
				printf("Ingrese su opción: ");
				scanf(" %d", &opcion);

				switch (opcion) {
				case 1:

					dfs(estado_inicial, cont);
					break;
				case 2:

					bfs(estado_inicial, cont);
					break;
				case 3:
						printf("Saliendo del programa...\n");
						break;
				}
				presioneTeclaParaContinuar();
				limpiarPantalla();

	} while (opcion != 3);
	return 0;
}
