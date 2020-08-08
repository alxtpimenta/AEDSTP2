#define STR_MAX 11

typedef struct Corner
{
    int x,y;
} Corner;

typedef struct City
{
    int x,y;
    int BlockedCorners;
    int Destinations;
} City;

typedef struct Graph
{
    int *AdjacencyMatrix;
    int *Visited;
    int *MinDistance;
    int *ShortestPaths;
    int vertices;
    int columns;
    int rows;
} Graph;

typedef struct Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;

int matrixOffset(int Columns, int i, int j);
void insertEdge(int source, int dest, Graph *G);
void initializeGraph(Graph *G, City C);
int translateToVertex(int X, int Y, City C);
void removeVertex(int vertex, Graph *G);
int borderCheck(int Vertex, Graph G);
void buildGrid(Graph *G);
void BFS(Graph *G, int presentVertex);
void shortestPaths(Graph *G);
void Enqueue(Queue *Q,int element);
int Front(Queue *Q);
void Dequeue(Queue *Q);
Queue *CreateQueue(int maxElements);
