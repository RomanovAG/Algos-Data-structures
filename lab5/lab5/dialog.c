#include "dialog.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int dialog(const char *msgs[], const int NMsgs)
{
    system("cls");
    for (int i = 0; i < NMsgs; i++)
        printf("%s\n", msgs[i]);
    printf("\nEnter the alternative\n");
    int flag;
    int temp;
    do
    {
        printf(">");
        flag = scanf("%d", &temp);
        if (flag < 0)
            return 0;
        if (flag == 0 || temp >= NMsgs)
        {
            printf("Wrong value. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (flag < 1 || temp >= NMsgs);
    scanf("%*[^\n]");
    scanf("%*c");
    return temp;
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
    {
        free(res);
        res = calloc(1, sizeof (char));
    }
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
    {
        free(res);
        res = calloc(1, sizeof (char));
    }
    return res;
}

int getInt(int *value)
{
    int flag;
    int temp;
    do
    {
        flag = scanf("%d", &temp);
        if (flag < 0)
            return -1;
        if (flag == 0)
        {
            printf("Wrong value. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (flag < 1);
    *value = temp;
    return 1;
}

int getDouble(double *value)
{
    int flag;
    double temp;
    do
    {
        flag = scanf("%lf", &temp);
        if (flag < 0)
            return -1;
        if (flag == 0)
        {
            printf("Wrong value. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (flag < 1);
    *value = temp;
    return 1;
}

char *addExtension(const char *file_name, const char *extension)
{
    if (file_name == NULL || extension == NULL)
        return NULL;
    char *file_name_extension = malloc(strlen(file_name) + strlen(extension) + 1);
    strcpy(file_name_extension, file_name);
    strcat(file_name_extension, extension);
    return file_name_extension;
}

int D_AddNode(Graph *graph)
{
    if (graph == NULL)
        return -1;
    printf("Enter identifier (null terminated string): ");
    char *identifier = getStr();
    if (identifier == NULL)
        return 1;
    printf("Enter x: ");
    double x;
    if (getDouble(&x) < 0)
    {
        free(identifier);
        return 1;
    }
    printf("Enter y: ");
    double y;
    if (getDouble(&y) < 0)
    {
        free(identifier);
        return 1;
    }
    if (addNode(graph, identifier, x, y) == 1)
    {
        printf("Duplicate identifier\n");
        getchar();
    }
    free(identifier);
    return 0;
}

int D_AddEdge(Graph *graph)
{
    if (graph == NULL)
        return -1;
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
    addEdge(graph, identifier_start, identifier_end);
    free(identifier_start), free(identifier_end);
    return 0;
}

int D_DeleteNode(Graph *graph)
{
    if (graph == NULL)
        return -1;
    printf("Enter identifier (null terminated string): ");
    char *identifier = getStr();
    if (identifier == NULL)
        return 1;
    if (deleteNode(graph, identifier) == 1)
    {
        printf("No such node\n");
        getchar();
    }
    return 0;
}

int D_DeleteEdge(Graph *graph)
{
    if (graph == NULL)
        return -1;
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
    if (deleteEdge(graph, identifier_start, identifier_end) > 0)
    {
        printf("No such edge\n");
        getchar();
    }
    free(identifier_start), free(identifier_end);
    return 0;
}

int D_Show(Graph *graph)
{
    if (graph == NULL)
        return -1;
    if (printGraph(graph, SIMPLE) == 1)
        printf("Graph is empty...");
    getchar();
    return 0;
}

int D_FindPathDFS(Graph *graph)
{
    if (graph == NULL)
        return -1;
    if (graph->num == 0)
    {
        printf("Graph is empty...");
        getchar();
        return 2;
    }
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
    DFS(graph);
    Node *end = findNode(graph, identifier_end);
    if (end == NULL)
    {
        printf("No path found");
        free(identifier_start);
        free(identifier_end);
        getchar();
        return 1;
    }
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
            free(identifier_start);
            free(identifier_end);
            getchar();
            return 0;
        }
        ptr = ptr->prev;
    }
    printf("No path found");
    free(identifier_start);
    free(identifier_end);
    getchar();
    return 1;
}

int D_FindMinPath(Graph *graph)
{
    if (graph == NULL)
        return -1;
    if (graph->num == 0)
    {
        printf("Graph is empty...");
        getchar();
        return 2;
    }
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
    if (BF(graph, findNode(graph, identifier_start)) == 1)
    {
        printf("Error...");
        free(identifier_start);
        free(identifier_end);
        getchar();
        return 1;
    }
    Node *end;
    if ((end = findNode(graph, identifier_end)) == NULL)
    {
        printf("End node hasn't been found...");
        free(identifier_start);
        free(identifier_end);
        getchar();
        return 1;
    }
    if (end->distance_to < __DBL_MAX__)
        printf("Distance from %s to %s: %.3f\n", identifier_start, identifier_end, end->distance_to);
    else
        printf("There's no path from %s to %s\n", identifier_start, identifier_end);
    getchar();
    free(identifier_start);
    free(identifier_end);
    return 0;
}

char *makeCommand(const char *gv_file, const char *png_file);
int writeToGV(Graph *graph, FILE *file, const char *file_name);

int D_Spec(Graph *graph)
{
    if (graph == NULL)
        return -1;
    if (graph->num == 0)
    {
        printf("Graph is empty...");
        getchar();
        return 2;
    }
    printf("Enter start identifier (null terminated string): ");
    char *identifier_start = getStr();
    if (identifier_start == NULL)
        return 1;
    if (BF(graph, findNode(graph, identifier_start)) == 1)
    {
        printf("Graph contains cycles with below zero weight\n");
        getchar();
        return 1;
    }
    Graph tree = {0, NULL};
    for (unsigned i = 0; i < graph->num; i++)
        if (graph->nodes[i]->distance_to < __DBL_MAX__)
            addNode(&tree, graph->nodes[i]->identifier, graph->nodes[i]->x, graph->nodes[i]->y);
    for (unsigned i = 0; i < tree.num; i++)
        if (findNode(graph, tree.nodes[i]->identifier)->prev != NULL)
            addEdge(&tree, findNode(graph, tree.nodes[i]->identifier)->prev->identifier, tree.nodes[i]->identifier);
    if (BF(&tree, findNode(&tree, identifier_start)) == 1)
    {
        printf("Graph contains cycles with below zero weight\n");
        getchar();
        return 1;
    }
    printGraph(&tree, DISTANCE);
    FILE *file = fopen("tree.gv", "w");
    writeToGV(&tree, file, "tree");
    fclose(file);
    char *command = makeCommand("tree.gv", "tree.png");
    system(command);
    free(command);
    system("tree.png");
    getchar();
    return 0;
}

char *makeCommand(const char *gv_file, const char *png_file)
{
    if (gv_file == NULL || png_file == NULL)
        return NULL;
    char *command = malloc(strlen("D:\\Programs\\Graphviz\\bin\\dot ")
                           + strlen(gv_file) + strlen(" -Tpng -o ") + strlen(png_file) + 1);
    strcpy(command, "D:\\Programs\\Graphviz\\bin\\dot ");
    strcat(command, gv_file);
    strcat(command, " -Tpng -o ");
    strcat(command, png_file);
    return command;
}

typedef struct FileNode
{
    Node *node;
    char var[16];
} FileNode;

int writeToGV(Graph *graph, FILE *file, const char *file_name)
{
    fseek(file, 0, SEEK_SET);
    fprintf(file, "digraph %s\n"
                  "{\n", file_name);
    fprintf(file, "node [shape = oval, fontname = consolas];\n"
                  "edge [fontname = consolas];\n\n");
    FileNode *node_list = calloc(graph->num, sizeof (FileNode));
    for (unsigned i = 0; i < graph->num; i++)
    {
        FileNode file_node;
        file_node.node = graph->nodes[i];
        snprintf(file_node.var, sizeof (file_node.var) - 1, "node%d", i);
        fprintf(file, "%s [label = \"%s\"];\n", file_node.var, file_node.node->identifier);
        node_list[i] = file_node;
    }
    fprintf(file, "\n");
    for (unsigned i = 0; i < graph->num; i++)
    {
        Adjacent *ptr = node_list[i].node->adjacent;
        while (ptr != NULL)
        {
            unsigned j;
            for (j = 0; j < graph->num; j++)
                if (strcmp(node_list[j].node->identifier, ptr->node->identifier) == 0)
                    break;
            fprintf(file, "%s -> %s [label = \"%.3f\"];\n", node_list[i].var, node_list[j].var, ptr->weight_prev);
            ptr = ptr->next;
        }
    }
    free(node_list);
    fprintf(file, "}");
    return 0;
}

int D_saveToPng(Graph *graph)
{
    if (graph == NULL)
        return -1;
    if (graph->num == 0)
    {
        printf("Graph is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    char *file_name_gv = addExtension(file_name, ".gv");
    FILE *file;
    file = fopen(file_name_gv, "w");

    writeToGV(graph, file, file_name);

    fclose(file);
    char *file_name_png = addExtension(file_name, ".png");
    char *command = makeCommand(file_name_gv, file_name_png);
    //printf("%s\n", command);
    system(command), free(command);
    system(file_name_png);
    free(file_name), free(file_name_gv), free(file_name_png);
    printf("Success\n");
    getchar();
    return 0;
}

int writeToTxt(Graph *graph, FILE *file)
{
    if (graph == NULL || file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    for (unsigned i = 0; i < graph->num; i++)
    {
        fprintf(file, "%s\n", graph->nodes[i]->identifier);
        fprintf(file, "%.3f\n%.3f\n", graph->nodes[i]->x, graph->nodes[i]->y);
    }
    fprintf(file, "#\n");
    for (unsigned i = 0; i < graph->num; i++)
    {
        Adjacent *ptr = graph->nodes[i]->adjacent;
        while (ptr != NULL)
        {
            fprintf(file, "%s\n%s\n", graph->nodes[i]->identifier, ptr->node->identifier);
            ptr = ptr->next;
        }
    }
    return 0;
}

int D_saveToTxt(Graph *graph)
{
    if (graph == NULL)
        return -1;
    if (graph->num == 0)
    {
        printf("Graph is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    char *file_name_txt = addExtension(file_name, ".txt");
    FILE *file;
    file = fopen(file_name_txt, "w");

    writeToTxt(graph, file);

    fclose(file);
    free(file_name), free(file_name_txt);
    printf("Success\n");
    getchar();
    return 0;
}

int readFromTxt(Graph *graph, FILE *file)
{
    if (graph == NULL || file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    char *identifier = NULL;
    while ((identifier = fgetStr(file)) != NULL && strcmp(identifier, "#") != 0)
    {
        double x, y;
        fscanf(file, "%lf%lf", &x, &y);
        fscanf(file, "%*c");
        addNode(graph, identifier, x, y);
        free(identifier);
    }
    free(identifier);
    while (!feof(file))
    {
        char *start = fgetStr(file);
        char *end = fgetStr(file);
        addEdge(graph, start, end);
        free(start), free(end);
    }
    return 0;
}

int D_loadFromTxt(Graph *graph)
{
    if (graph == NULL)
        return -1;
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    char *file_name_txt = addExtension(file_name, ".txt");
    FILE *file;
    file = fopen(file_name_txt, "r");
    if (file == NULL)
    {
        perror("Error occured while opening file");
        free(file_name), free(file_name_txt);
        getchar();
        return 1;
    }
    if (graph->num > 0)
        deleteGraph(graph);

    readFromTxt(graph, file);

    fclose(file);
    free(file_name), free(file_name_txt);
    //getchar();
    return 0;
}

int D_Random(Graph *graph)
{
    return -1;
}
