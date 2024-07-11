#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "D:/labs/AISD/lab5/lab5/graph.h"

#include <time.h>

char *getStr();
char *fgetStr(FILE *file);

int main()
{
    FILE *file_nodes, *file_edges;
    while (1)
    {
        system("cls");
        printf("Enter file name to load nodes: ");
        char *file_name_nodes = getStr();
        if (file_name_nodes == NULL)
            return 0;
        if ((file_nodes = fopen(file_name_nodes, "rb")) == NULL)
        {
            perror("Error occured while opening file");
            free(file_name_nodes);
            getchar();
            continue;
        }
        free(file_name_nodes);
        printf("Enter file name to load edges: ");
        char *file_name_edges = getStr();
        if (file_name_edges == NULL)
            return 0;
        if ((file_edges = fopen(file_name_edges, "rb")) == NULL)
        {
            perror("Error occured while opening file");
            free(file_name_edges);
            getchar();
            continue;
        }
        free(file_name_edges);
        break;
    }
    Graph graph = {0, NULL};
    clock_t start = clock();
    while (!feof(file_nodes))
    {
        char *line = fgetStr(file_nodes);
        line[strlen(line) - 1] = '\0';
        char *identifier = strtok(line, " ");
        char *x_str = strtok(NULL, " ");
        char *y_str = strtok(NULL, " ");
        double x, y;
        sscanf(x_str, "%lf", &x);
        sscanf(y_str, "%lf", &y);
        //printf("%s %.3f %.3f\n", identifier, x, y);
        addNode(&graph, identifier, x, y);
        free(line);
    }
    fclose(file_nodes);
    while (!feof(file_edges))
    {
        char *line = fgetStr(file_edges);
        line[strlen(line) - 1] = '\0';
        char *ID = strtok(line, " ");
        ID[strlen(ID)] = '\0';
        char *identifier_start = strtok(NULL, " ");
        char *identifier_end = strtok(NULL, " ");
        //printf("%s %s %s\n", ID, identifier_start, identifier_end);
        addEdge(&graph, identifier_start, identifier_end);
        free(line);
    }
    fclose(file_edges);
    clock_t end = clock();
    printf("time: %f\n", (double) (end - start) / CLOCKS_PER_SEC);
    //printGraph(&graph, SIMPLE);
    system("pause");
    while (1)
    {
        system("cls");
        printf("Enter start identifier (null terminated string): ");
        char *identifier_start = getStr();
        if (identifier_start == NULL)
            return 1;
        printf("Enter end identifier (null terminated string): ");
        char *identifier_end = getStr();
        if (identifier_end == NULL)
        {
            free(identifier_start);
            return 1;
        }
        clock_t start = clock();
        if (BF(&graph, findNode(&graph, identifier_start)) == 1)
        {
            printf("Error...");
            getchar();
            free(identifier_start);
            free(identifier_end);
            break;
        }
        Node *end;
        if ((end = findNode(&graph, identifier_end)) == NULL)
        {
            printf("End node hasn't been found...");
            getchar();
            free(identifier_start);
            free(identifier_end);
            break;
        }
        clock_t endt = clock();
        if (end->distance_to < __DBL_MAX__)
        {
            printf("Distance from %s to %s: %.3f\n", identifier_start, identifier_end, end->distance_to);
            Node *ptr = end;
            while (ptr != NULL)
            {
                if (strcmp(identifier_start, ptr->identifier) == 0)
                {
                    Node *_ptr = end;
                    printf("%s", _ptr->identifier);
                    _ptr = _ptr->prev;
                    while (_ptr != NULL)
                    {
                        printf(" <-- %s", _ptr->identifier);
                        if (strcmp(_ptr->identifier, identifier_start) == 0)
                            break;
                        _ptr = _ptr->prev;
                    }
                    printf("\n");
                    getchar();
                    break;
                }
                ptr = ptr->prev;
            }
        }
        else
        {
            printf("There's no path from %s to %s\n", identifier_start, identifier_end);
        }
        printf("time: %f\n", (double) (endt - start) / CLOCKS_PER_SEC);
        free(identifier_start);
        free(identifier_end);
        getchar();
    }
    system("pause");
    deleteGraph(&graph);
    return 0;
}

char *getStr()
{
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do
    {
        n = scanf("%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
                return NULL;
        }
        else if (n > 0)
        {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else
            scanf("%*c");
    } while (n > 0);
    if (len > 0)
        res[len] = '\0';
    else
        res = calloc(1, sizeof(char));
    return res;
}

char *fgetStr(FILE *file)
{
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do
    {
        n = fscanf(file, "%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
                return NULL;
        }
        else if (n > 0)
        {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else
            fscanf(file, "%*c");
    } while (n > 0);
    if (len > 0)
        res[len] = '\0';
    else
        res = calloc(1, sizeof(char));
    return res;
}
