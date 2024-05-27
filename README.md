# Codigo para resolver 8-puzzle segun dfs o bfs

<br>
<br>

Para ejecutar el codigo hay que usar estos comandos:
````console
gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
````

Y luego ejecutar:
````console
./tarea3
````

<br>

<hr>

--> El codigo te abre un menu donde tu eliges que opcion te interesa.

````bash
1) Hacer busqueda DFS
2) Hacer busqueda BFS
3) Salir del programa

````

--> Structs

````c
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
````
<br>

### --> PROTOTIPOS DE FUNCIONES 

`int isValidMove(int x, int y);`

`void imprimirEstado(const State *estado);`

`void copiarEstado(State original, State* nuevo);`

`int isFinal(const State* estado);`

`void move(State* actual, int opcion);`

`State* transition(State* estadoOriginal, int movimiento);`

`List *getAdjNodes(Node* parentNode);`

`void dfs(State estadoInicial, int cont);`

`void bfs(State estadoInicial, int cont);`
<br>

<hr>

<br>
echo por javiera muñoz
