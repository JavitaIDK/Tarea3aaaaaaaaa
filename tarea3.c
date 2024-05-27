#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include "tdas/queue.h"
#include <string.h>

typedef struct {

		int square[3][3]; // Matriz 3x3 que representa el tablero
		int x;    // Posición x del espacio vacío
		int y;    // Posición y del espacio vacío
		int totalMovimientos; 
} State;
typedef struct Node {
		State state;
		struct Node* parent;
} Node;

int movimientoValido(int x, int y) { //verificar si el movimiento es valido o no
  return (x >= 0 && x < 3 && y >= 0 && y < 3); 
}
void imprimirEstado(const State *estado) { // Imprime el estado actual del puzzle
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
void copiarEstado(State original, State* nuevo) { //copiar el estado original en el nuevo estado
   for (int i = 0; i < 3; i++)
       for (int j = 0; j < 3; j++)
           nuevo->square[i][j] = original.square[i][j]; 
   nuevo->x = original.x;
   nuevo->y = original.y;
   nuevo->totalMovimientos = original.totalMovimientos;
 }
int isFinal(const State* estado) { //comprobar si se llego al estado final
  State estadoFinal = { //definiendo estado final 
      {{0, 1, 2},
       {3, 4, 5},
      {6, 7, 8}},
      0,0
  };
  for (int i = 0; i < 3; i++) { //comparar cada casilla con el estado final
      for (int j = 0; j < 3; j++) {
          if (estado->square[i][j] != estadoFinal.square[i][j])
              return 0; //si alguna casilla no coincide, el estado no es final
      }
  }
  return 1; //si todas coinciden, el estado es final
}
void move(State* actual, int opcion) { //funcion para mover
  int x = actual->x; //cordenadas eje x del espacio vacio
  int y = actual->y; //cordenadas eje y del espacio vacio
  int temp;
  
  if(opcion==1 && movimientoValido(x , y - 1)){ // Mover espacio vacío hacia arriba
    temp = actual->square[x][y - 1];
    actual->square[x][y - 1] = actual->square[x][y];
    actual->square[x][y] = temp;
    actual->y = y - 1;	//actualizar posicion del espacio vacioo 
  }else if(opcion == 2 && movimientoValido(x , y + 1)){// Mover espacio vacío hacia abajo
    temp = actual->square[x][y + 1];
    actual->square[x][y + 1] = actual->square[x][y];
    actual->square[x][y] = temp;
    actual->y = y + 1; //actualizar posicion del espacio vacioo 
  }else if(opcion == 3 && movimientoValido(x - 1, y)){ // Mover espacio vacío hacia la izquierda
    temp = actual->square[x - 1][y];
    actual->square[x - 1][y] = actual->square[x][y];
    actual->square[x][y] = temp;
    actual->x = x - 1; //actualizar posicion del espacio vacioo 
  }else if(opcion == 4 && movimientoValido(x + 1, y)){ // Mover espacio vacío hacia la derecha
    temp = actual->square[x + 1][y];
    actual->square[x + 1][y] = actual->square[x][y];
    actual->square[x][y] = temp;
    actual->x = x + 1; //actualizar posicion del espacio vacioo 
  }
}

State* transition(State* estadoOriginal, int movimiento) {
  State *nuevoEstado = (State*) malloc(sizeof(State)); //Crea un nuevo estado 
  copiarEstado(*estadoOriginal, nuevoEstado); //copia el nuevo estado con el original
  move(nuevoEstado, movimiento); //aplicar movimiento
  nuevoEstado->totalMovimientos++; //aumenta el contador del nuevo estado
  return nuevoEstado;
}
List *getAdjNodes(Node* parentNode) {
  
  List *adjList = list_create(); // crear una lista para almacenar los nodos adyacentes
  //obtener las coordenadas del espacio vacio del nodo padre
  int x = parentNode->state.x;
  int y = parentNode->state.y;
  for (int i = 1; i <= 4; i++) {
    State *newState = transition(&parentNode->state, i); // generar un nuevo estado aplicando el movimiento i al estado del nodo padre
    if (newState->x != x || newState->y != y) { // verificar si el nuevo estado es diferente al estado del nodo padre
      // si el nuevo estado es diferente, crear un nuevo nodo y agregarlo a la lista de adyacentes
      Node *newNode = (Node*) malloc(sizeof(Node)); 
      newNode->state = *newState;
      newNode->parent = parentNode;
      list_pushBack(adjList, newNode);
    } else { // sino se liberara la memoria del nuevo estado
      free(newState);
    }
  }
  return adjList;
}

void dfs(State estadoInicial, int cont) { //funcion para encontrar la solucion con la busqueda por profundidad
	Stack* stack = stack_create(stack); //crea una pila para la busqueda por profundidad

	printf("\nEstado del puzzle:\n\n");
	imprimirEstado(&estadoInicial);
	printf("\n");

	Node *root = (Node *) malloc(sizeof(Node )); //crear nodo raiz
	root->parent = NULL;
	root->state = estadoInicial;
	stack_push(stack, root);

	while (list_size(stack) > 0) { //bucle para la busqueda por profundidad 
		Node *nodoActual = (Node *) stack_top(stack); //obtener nodo actual de la pila 
		stack_pop(stack);
		if (nodoActual->state.totalMovimientos >= 15)
				continue;
		if (isFinal(&nodoActual->state)) { //verificar si se encontro el estado final 
      printf("8-Puzzle a sido resuelto! :D\n");
      printf("Total de acciones: %d\n\n", cont);
      Node *nodo = nodoActual;
      int totalInteracciones = nodoActual->state.totalMovimientos;
      
      while (nodo != NULL) { //inprimir los estados hasta llegar al final
        if (totalInteracciones == 0) 
          printf("Estado inicial:\n\n");  
        else if(totalInteracciones == nodoActual->state.totalMovimientos)
          printf("Estado final del 8-puzzle:\n\n");
        else
          printf("Iteración %d:\n", totalInteracciones);                  
        imprimirEstado(&nodo->state);
        printf("\n");
        nodo = nodo->parent;
          totalInteracciones--;
      }
      //limpiar pila y liberar memoria
      stack_clean(stack);
      free(nodo);
      return;
		}
    //obtener nodos adyacentes para agregarlos a la pila 
		List *adjNodes = getAdjNodes(nodoActual);
		void *aux = list_first(adjNodes);
		while (aux != NULL) {
      stack_push(stack, aux);
      aux = list_next(adjNodes);
		}
		list_clean(adjNodes);
		cont++; 
	}
  //por si se agotaron las opciones sin encontrar la solucion
	printf("La solucion no se pudo encontrar :(\n");
	stack_clean(stack);
}
void bfs(State estadoInicial, int cont) { //funcion para encontrar la solucion con la busqueda por anchura
  
	Queue* queue = queue_create(queue); //crear cola para la busqueda por anchura
	printf("\nEstado inicial del puzzle:\n\n");
	imprimirEstado(&estadoInicial);
	printf("\n");

	Node *raiz = (Node *) malloc(sizeof(Node )); //crear nodo raiz
	raiz->parent = NULL;
	raiz->state = estadoInicial;
	queue_insert(queue, raiz);

	while (list_size(queue) > 0) { //bucle para la busqueda por anchura
    Node *NodoActual = (Node *) queue_front(queue); //obtener el nodo actual de la cola
    queue_remove(queue);
    
    if (cont >= 10000000) { // verificar si se ha alcanzado el límite de movimientos
      printf("Se a alcanzado un limite de movimientos\n");
      printf("Proceso finalizado.");
      return;
    }
  
    if (isFinal(&NodoActual->state)) { //verificar si se encontro el estado final

      //imprimir los estados hasta llegar al final
      printf("8-Puzzle a sido resuelto! :D\n");
      printf("Total de acciones: %d\n\n", cont);
      Node *nodo = NodoActual;
      int totalInteracciones = NodoActual->state.totalMovimientos;

      while (nodo != NULL) { 
        if (totalInteracciones == 0) 
          printf("Estado inicial:\n\n");  
        else if(totalInteracciones == NodoActual->state.totalMovimientos)
          printf("Estado final del 8-puzzle:\n\n");
        else
          printf("Iteración %d:\n", totalInteracciones);                  
        imprimirEstado(&nodo->state);
        printf("\n");
        nodo = nodo->parent;
          totalInteracciones--;
      }
      //limpiar cola y liberar memoria
      queue_clean(queue);
      free(nodo);
      return;
    }
    // Obtener los nodos adyacentes y agregarlos a la cola
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
		estado_inicial.totalMovimientos = 0;

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
