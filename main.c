#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main(int argc, char *argv[])
{
    FILE *InputFile = fopen(argv[1],"r");

    if (InputFile == NULL)
    {
        printf("Erro ao abrir o arquivo de entrada!\n");
        return 1;
    }

    FILE *OutputFile = fopen(argv[2],"w");
    fclose(OutputFile);
    OutputFile = fopen(argv[2],"a");

    if (OutputFile == NULL)
    {
        printf("Erro ao abrir o arquivo de saida!\n");
        return 2;
    }

    int Instances;
    int j,k;
    int target;
    City Johnville;
    Corner Aux;
    Graph Grid;
    char str[STR_MAX];

    //INÍCIO DA LEITURA DO ARQUIVO
    fgets(str,STR_MAX,InputFile);
    Instances = atoi(str);
    printf("Instances: %d\n",Instances);

    //LOOP DE INSTÂNCIAS
    for(k=0;k<Instances;k++)
    {
        printf("-------Instance %d-------\n",k+1);

        fgets(str,STR_MAX,InputFile);
        Johnville.x = atoi(strtok(str," "));
        Johnville.y = atoi(strtok(NULL," "));

        printf("City dimensions for this instance: %d (rows) x %d (columns)\n",Johnville.x,Johnville.y);

        //INICIALIZAR O GRAFO, E CRIAR GRADE (CIDADE)
        initializeGraph(&Grid,Johnville);
        buildGrid(&Grid);

        //REMOVER ESQUINAS BLOQUEADAS DA MATRIZ DE ADJACÊNCIA
        fgets(str,STR_MAX,InputFile);
        Johnville.BlockedCorners = atoi(strtok(str," "));
        printf("Blocked Corners: %d\n",Johnville.BlockedCorners);
        for(j=0;j<Johnville.BlockedCorners;j++)
        {
            fgets(str,STR_MAX,InputFile);
            Aux.x = atoi(strtok(str," "));
            Aux.y = atoi(strtok(NULL," "));
            removeVertex(translateToVertex(Aux.x,Aux.y,Johnville),&Grid);
        }

        //FAZER BUSCA EM LARGURA NO GRAFO, PARA DETERMINAR OS MENORES CAMINHOS PARA CADA VÉRTICE
        shortestPaths(&Grid);

        //LER ESQUINAS B
        fgets(str,STR_MAX,InputFile);
        Johnville.Destinations = atoi(strtok(str," "));
        printf("Targets: %d\n\n",Johnville.Destinations);

        //OBTER E IMPRIMIR RESULTADO PARA CADA ESQUINA B
        for(j=0;j<Johnville.Destinations;j++)
        {
            fgets(str,STR_MAX,InputFile);
            Aux.x = atoi(strtok(str," "));
            Aux.y = atoi(strtok(NULL," "));

            target = translateToVertex(Aux.x,Aux.y,Johnville);

            printf("Result for vertex %d -> %d shortest path(s)\n",target,Grid.ShortestPaths[target]);
            printf("Result for vertex %d -> Minimum distance: %d\n\n",target,Grid.MinDistance[target]);
            //IMPRIMIR NO ARQUIVO DE SAÍDA OS RESULTADOS
            fprintf(OutputFile,"%d\n",Grid.ShortestPaths[target]);
        }

        //IMPRIMIR LINHA PARA SEPARAR CADA INSTÂNCIA NO ARQUIVO DE SAIDA
        fprintf(OutputFile,"\n");

        //DESALOCAR
        free(Grid.AdjacencyMatrix);
        free(Grid.Visited);
        free(Grid.MinDistance);
        free(Grid.ShortestPaths);

    }
    fclose(InputFile);
    fclose(OutputFile);

    return 0;
}
