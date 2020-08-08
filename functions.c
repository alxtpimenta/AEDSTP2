#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int matrixOffset(int Columns, int i, int j)
{
    //RETORNA A POSIÇÃO i,j DA MATRIZ "IMAGINÁRIA" NO VETOR
    return i * Columns + j;
}

void insertEdge(int source, int dest, Graph *G)
{
    //INSERE ARESTA DIRETAMENTE NA MATRIZ DE ADJACÊNCIA
    G->AdjacencyMatrix[matrixOffset(G->vertices,source,dest)] = 1;
}

void initializeGraph(Graph *G, City C)
{
    //INICIALIZA PROPRIEDADES E ALOCA MEMÓRIA PARA A MATRIZ DE ADJACÊNCIA
    int i,j;
    G->vertices = C.x * C.y;
    G->columns = C.y;
    G->rows = C.x;
    G->AdjacencyMatrix = (int*)malloc((G->vertices * G->vertices) * sizeof(int));

    //PERCORRE A MATRIZ, A PREENCHENDO COM 0
    for(i=0;i<G->vertices;i++)
    {
        for(j=0;j<G->vertices;j++)
        {
            G->AdjacencyMatrix[matrixOffset(G->vertices,i,j)] = 0;
        }
    }
}

int translateToVertex(int X, int Y, City C)
{
    //"TRADUZ" UM PAR DE COORDENADAS X,Y LIDOS DO ARQUIVO DE ENTRADA EM UM INTEIRO
    //QUE INDICA O VÉRTICE REPRESENTADO POR ESSAS COORDENADAS
    return ( (Y-1) + ( (X-1)*(C.y) ) );
}

void removeVertex(int vertex, Graph *G)
{
    int i;

    //PERCORRE A LINHA E COLUNA DO VÉRTICE NA MATRIZ, REMOVENDO AS ARESTAS
    for(i=0;i<G->vertices;i++)
        G->AdjacencyMatrix[matrixOffset(G->vertices,vertex,i)] = 0;

    for(i=0;i<G->vertices;i++)
        G->AdjacencyMatrix[matrixOffset(G->vertices,i,vertex)] = 0;

        return;
}

int borderCheck(int Vertex, Graph G)
{
    //VERIFICA SE O VÉRTICE ESTÁ EM UMA DAS QUATRO EXTREMIDADES DO GRAFO
    if(Vertex == 0)
        return 0;
    else if(Vertex == (G.columns - 1) )
        return 1;
    else if(Vertex == (G.columns * (G.rows-1) ) )
        return 2;
    else if(Vertex == G.vertices-1)
        return 3;
    //VERIFICA SE O VÉRTICE ESTÁ NA "BORDA" DO GRAFO
    else if( (Vertex > 0) && (Vertex < (G.columns - 1) ) )
        return 4;
    else if( ( (Vertex+1) % G.columns ) == 0)
        return 5;
    else if( (Vertex > (G.columns * (G.rows-1) ) ) && (Vertex < (G.vertices-1) ) )
        return 6;
    else if( ( Vertex % G.columns ) == 0)
        return 7;

    return 8;
}

void buildGrid(Graph *G)
{
    int vertex;
    for(vertex=0;vertex<G->vertices;vertex++)
    {
        switch(borderCheck(vertex,*G))
        {
        case 0:
            insertEdge(0,1,G);
            insertEdge(0,G->columns,G);
            break;
        case 1:
            insertEdge(vertex,vertex-1,G);
            insertEdge(vertex,vertex+G->columns,G);
            break;
        case 2:
            insertEdge(vertex,vertex+1,G);
            insertEdge(vertex,vertex-G->columns,G);
            break;
        case 3:
            insertEdge(vertex,vertex-1,G);
            insertEdge(vertex,vertex-G->columns,G);
            break;
        case 4:
            insertEdge(vertex,vertex-1,G);
            insertEdge(vertex,vertex+1,G);
            insertEdge(vertex,vertex+G->columns,G);
            break;
        case 5:
            insertEdge(vertex,vertex-1,G);
            insertEdge(vertex,vertex+G->columns,G);
            insertEdge(vertex,vertex-G->columns,G);
            break;
        case 6:
            insertEdge(vertex,vertex-1,G);
            insertEdge(vertex,vertex+1,G);
            insertEdge(vertex,vertex-G->columns,G);
            break;
        case 7:
            insertEdge(vertex,vertex+1,G);
            insertEdge(vertex,vertex-G->columns,G);
            insertEdge(vertex,vertex+G->columns,G);
            break;
        case 8:
            insertEdge(vertex,vertex+1,G);
            insertEdge(vertex,vertex-1,G);
            insertEdge(vertex,vertex-G->columns,G);
            insertEdge(vertex,vertex+G->columns,G);
            break;
        }
    }
}

Queue *CreateQueue(int maxElements)
{
        //CRIAR FILA
        Queue *Q;
        Q = (Queue *)malloc(sizeof(Queue));
        //INICIALIZAR PROPRIEDADES
        Q->elements = (int *)malloc(maxElements * sizeof(int));
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        //RETORNAR O PONTEIRO
        return Q;
}

void Dequeue(Queue *Q)
{
        //SE O TAMANHO DA FILA É 0, ELA ESTÁ VAZIA (NÃO HÁ NADA PARA REMOVER)
        if(Q->size==0)
        {
                printf("Queue is empty\n");
                return;
        }

        else
        {
                Q->size--;
                Q->front++;

                if(Q->front==Q->capacity)
                {
                        Q->front=0;
                }
        }
        return;
}

int Front(Queue *Q)
{
        if(Q->size==0)
        {
                printf("Queue is empty\n");
                exit(0);
        }
        //RETORNA O ELEMENTO NA PRIMEIRA POSIÇÃO DA FILA
        return Q->elements[Q->front];
}

void Enqueue(Queue *Q,int element)
{
        //SE A FILA ESTIVER CHEIA, NÃO HÁ ESPAÇO PARA INSERIR OUTRO ITEM
        if(Q->size == Q->capacity)
        {
                printf("Queue is full\n");
        }
        else
        {
                Q->size++;
                Q->rear = Q->rear + 1;

                if(Q->rear == Q->capacity)
                {
                        Q->rear = 0;
                }

                Q->elements[Q->rear] = element;
        }
        return;
}

void BFS(Graph *G, int Source)
{
    printf("--BFS Start--\n");
    G->Visited[Source] = 1;
    //PASSA POR TODOS OS VERTICES CONECTADOS AO Source E FAZ BFS NELES SE ELES AINDA NÃO FORAM VISITADOS
    Queue *Q = CreateQueue(G->vertices);
    Enqueue(Q,Source);
    while(Q->size)
    {
            Source = Front(Q);
            printf("Vertex -> %d\n",Source);
            Dequeue(Q);
            int i;
            for(i=0;i<G->vertices;i++)
            {
                    //SE O VERTICE AINDA NÃO FOI VISITADO, ELE É ADICIONADO À FILA
                    //COMO ELE AINDA NÃO FOI VISITADO, ELE SÓ POSSUI 1 MENOR CAMINHO DESDE O SOURCE
                    if( (G->AdjacencyMatrix[matrixOffset(G->vertices,Source,i)] == 1) && !G->Visited[i] )
                    {
                            G->Visited[i] = 1;
                            G->MinDistance[i] = G->MinDistance[Source] +1;
                            G->ShortestPaths[i] = G->ShortestPaths[Source];
                            Enqueue(Q,i);
                    }
                    //SE O VÉRTICE JÁ TIVER SIDO VISITADO, ELE NÃO SERÁ ADICIONADO À FILA
                    else if( (G->AdjacencyMatrix[matrixOffset(G->vertices,Source,i)] == 1) && G->Visited[i] )
                    {
                        //SE AS DISTÂNCIAS MÍNIMAS DOS DOIS VÉRTICES FOREM IGUAIS, A DISTÂNCIA MÍNIMA PARA ATINGIR
                        // O ALVO É A MESMA, MAS O NÚMERO DE MENORES CAMINHOS AUMENTA
                        if(G->MinDistance[i] == (G->MinDistance[Source] +1) )
                        {
                            G->ShortestPaths[i] = G->ShortestPaths[i] + G->ShortestPaths[Source];
                        }
                        //SE UM TIVER UMA DISTÂNCIA MÍNIMA MENOR, ENTÃO A DISTÂNCIA MÍNIMA PARA ATINGIR
                        //O ALVO E O NÚMERO DE MENORES CAMINHOS SERÃO ATUALIZADOS
                        else if(G->MinDistance[i] > (G->MinDistance[Source] +1) )
                        {
                            G->MinDistance[i] = G->MinDistance[Source]+1;
                            G->ShortestPaths[i] = G->ShortestPaths[Source];
                        }
                    }
            }
    }

    printf("--BFS End--\n");
    return;
}

void shortestPaths(Graph *G)
{

    //ALOCAR ESPAÇO PARA OS VETORES DE OPERAÇÃO
    G->Visited = (int*)malloc(G->vertices * sizeof(int));
    G->MinDistance = (int*)malloc(G->vertices * sizeof(int));
    G->ShortestPaths = (int*)malloc(G->vertices * sizeof(int));

    int i;
    //INICIALIZAR VALORES
    for(i=0;i<G->vertices;i++)
    {
        G->Visited[i] = 0;
        G->MinDistance[i] = 0;
        G->ShortestPaths[i] = 0;
    }

    //PARA O CALCULO DE MENORES CAMINHOS, OS VALORES DO VÉRTICE
    //DE ORIGEM (ZERO) SÃO DEFINIDOS COMO 1
    G->MinDistance[0] = 1;
    G->ShortestPaths[0] = 1;

    //REALIZAR BUSCA EM LARGURA
    BFS(G,0);

    return;
}
