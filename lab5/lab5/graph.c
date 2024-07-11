#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

double calculateWeight(const double x1, const double y1, const double x2, const double y2)
{
    double a = (x2 - x1);
    double b = (y2 - y1);
    return sqrt(a * a + b * b);
}

Node *findNode(Graph *graph, const char *identifier)
{
    for (unsigned i = 0; i < graph->num; i++)
        if (strcmp(graph->nodes[i]->identifier, identifier) == 0)
            return graph->nodes[i];
    return NULL;
}

Adjacent *findAdj(Adjacent *ptr, const char *identifier)
{
    while (ptr != NULL)
    {
        if (strcmp(ptr->node->identifier, identifier) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

int freeAllAdjs(Node *node)
{
    Adjacent *ptr = node->adjacent;
    while (ptr != NULL)
    {
        Adjacent *temp = ptr;
        ptr = ptr->next;
        free(temp);
    }
    node->adjacent = NULL;
    return 0;
}

int delAdjFrom(Node *node, Adjacent *adj)
{
    if (adj == NULL)
        return -1;
    if (adj == node->adjacent)
        node->adjacent = adj->next;
    else
    {
        Adjacent *ptr = node->adjacent;
        while (ptr->next != adj)
            ptr = ptr->next;
        ptr->next = adj->next;
    }
    free(adj);
    return 0;
}

int addNode(Graph *graph, const char *identifier, const double x, const double y)
{
    if (graph == NULL) return -1;
    if (identifier == NULL) return -2;
    if (findNode(graph, identifier) != NULL)
        return 1;
    Node *node;
    if ((node = malloc(sizeof (Node))) == NULL) return -3;
    node->identifier = strdup(identifier);
    node->x = x;
    node->y = y;
    node->adjacent = NULL;
    node->prev = NULL;
    node->color = WHITE;
    graph->nodes = realloc(graph->nodes, (graph->num + 1) * sizeof (Node *));
    graph->nodes[graph->num++] = node;
    return 0;
}

int addEdge(Graph *graph, const char *identifier_start, const char *identifier_end)
{
    if (graph == NULL) return -1;
    if (identifier_start == NULL || identifier_end == NULL) return -2;
    Node *node_start = findNode(graph, identifier_start);
    Node *node_end = findNode(graph, identifier_end);
    if (node_start == NULL || node_end == NULL)
        return 2;
    if (findAdj(node_start->adjacent, identifier_end) != NULL)
        return 1;
    Adjacent *adj = malloc(sizeof (Adjacent));
    adj->node = node_end;
    adj->weight_prev = calculateWeight(node_start->x, node_start->y, node_end->x, node_end->y);
    adj->next = NULL;
    if (node_start->adjacent == NULL)
        node_start->adjacent = adj;
    else
    {
        Adjacent *ptr = node_start->adjacent;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = adj;
    }
    return 0;
}



int deleteNode(Graph *graph, const char *identifier)
{
    if (graph == NULL) return -1;
    if (identifier == NULL) return -2;
    Node *to_delete;
    if ((to_delete = findNode(graph, identifier)) == NULL)
        return 1;
    //printf("to del: %s\n", to_delete->identifier);
    for (unsigned i = 0; i < graph->num; i++)
    {
        Adjacent *adj_to_delete;
        if ((adj_to_delete = findAdj(graph->nodes[i]->adjacent, identifier)) != NULL)
            delAdjFrom(graph->nodes[i], adj_to_delete);
    }
    freeAllAdjs(to_delete);
    unsigned del_pos;
    for (del_pos = 0; del_pos < graph->num; del_pos++)
        if (graph->nodes[del_pos] == to_delete)
            break;
    //printf("del pos: %d\n", del_pos + 1);
    if (graph->num - 1 == 0)
    {
        free(graph->nodes[0]->identifier);
        free(graph->nodes[0]);
        free(graph->nodes);
        graph->nodes = NULL;
        graph->num = 0;
    }
    else
    {
        //printf("num: %d\n", graph->num);
        if (del_pos < graph->num - 1)
        {
            //printf("<\n");
            memmove(&graph->nodes[del_pos], &graph->nodes[del_pos + 1], (graph->num - del_pos - 1) * sizeof (Node *));
        }
        graph->nodes = realloc(graph->nodes, (graph->num - 1) * sizeof (Node *));
        graph->num--;
        free(to_delete->identifier);
        free(to_delete);
    }
    //getchar();
    return 0;
}

int deleteEdge(Graph *graph, const char *identifier_start, const char *identifier_end)
{
    if (graph == NULL) return -1;
    if (identifier_start == NULL || identifier_end == NULL) return -2;
    Node *node_start = findNode(graph, identifier_start);
    Node *node_end = findNode(graph, identifier_end);
    if (node_start == NULL || node_end == NULL)
        return 2;
    if (findAdj(node_start->adjacent, identifier_end) == NULL)
        return 1;
    delAdjFrom(node_start, findAdj(node_start->adjacent, identifier_end));
    return 0;
}

int deleteGraph(Graph *graph)
{
    if (graph == NULL)
        return 0;
    unsigned num = graph->num;
    for (unsigned i = 0; i < num; i++)
        deleteNode(graph, graph->nodes[0]->identifier);
    return 0;
}

int printGraph(const Graph *graph, PrintSettings s)
{
    if (graph == NULL)
        return -1;
    if (graph->num == 0)
        return 1;
    for (unsigned i = 0; i < graph->num; i++)
    {
        switch (s)
        {
        case SIMPLE:
        {
            printf("%d: %s (%.3f, %.3f)\n",
                   i + 1, graph->nodes[i]->identifier, graph->nodes[i]->x, graph->nodes[i]->y);
            Adjacent *ptr = graph->nodes[i]->adjacent;
            while (ptr != NULL)
            {
                printf("   --%.3f--> %s (%.3f, %.3f)\n", ptr->weight_prev, ptr->node->identifier, ptr->node->x, ptr->node->y);
                ptr = ptr->next;
            }
            break;
        }
        case DISTANCE:
        {
            printf("%d: %s (%.3f, %.3f) distance: %.3f\n",
                   i + 1, graph->nodes[i]->identifier, graph->nodes[i]->x, graph->nodes[i]->y, graph->nodes[i]->distance_to);
            Adjacent *ptr = graph->nodes[i]->adjacent;
            while (ptr != NULL)
            {
                printf("   --%.3f--> %s (%.3f, %.3f)\n", ptr->weight_prev, ptr->node->identifier, ptr->node->x, ptr->node->y);
                ptr = ptr->next;
            }
            break;
        }
        }

    }
    return 0;
}

int DFSVisit(Node *u/*, const char *identifier_end, Node **path, unsigned *path_len*/)
{
    u->color = GRAY;
    /*//path = realloc(path, (*path_len + 1) * sizeof (Node *));
    //path[(*path_len)++] = u;
    //if (u->identifier == identifier_end)
    //{
        //path = realloc(path, (*path_len + 1) * sizeof (Node *));
        //path[(*path_len)++] = NULL;
        //return path;
    //}*/
    //(*time)++;
    //u->start_time = *time;
    Adjacent *ptr = u->adjacent;
    while (ptr != NULL)
    {
        if (ptr->node->color == WHITE)
        {
            ptr->node->prev = u;
            DFSVisit(ptr->node/*, identifier_end, path, path_len*/);
        }
        ptr = ptr->next;
    }
    u->color = BLACK;
    //(*time)++;
    //u->end_time = *time;
    return 0;
}

int DFS(Graph *graph/*, const char *identifier_start, const char *identifier_end*/)
{
    //*time = 0;
    for (unsigned i = 0; i < graph->num; i++)
    {
        graph->nodes[i]->color = WHITE;
        graph->nodes[i]->prev = NULL;
    }
    //unsigned i;
    //for (i = 0; i < graph->num; i++)
    //    if (strcmp(graph->nodes[i]->identifier, identifier_start) == 0)
    //        break;
    //if (i == graph->num)
        //return NULL;
//    Node **path = NULL;
//    unsigned path_len = 0;
    /*path = */
    //DFSVisit(graph->nodes[i]/*, identifier_end, path, &path_len*/);
    for (unsigned i = 0; i < graph->num; i++)
        if (graph->nodes[i]->color == WHITE)
            DFSVisit(graph->nodes[i]);
//    if (strcmp(path[path_len - 1]->identifier, identifier_end) == 0)
//    {
//        path = realloc(path, (path_len + 1) * sizeof (Node *));
//        path[path_len++] = NULL;
//        return path;
//    }
//    free(path);
    return 0;
}

int processAdjEdges(Node *base)
{
    if (base == NULL)
        return -1;
    Adjacent *ptr = base->adjacent;
    while (ptr != NULL)
    {
        double sum;
        if (base->distance_to == DBL_MAX)
            sum = DBL_MAX;
        else
            sum = base->distance_to + ptr->weight_prev;
        if (ptr->node->distance_to > sum)
        {
            ptr->node->distance_to = sum;
            ptr->node->prev = base;
        }
        ptr = ptr->next;
    }
    return 0;
}

int checkAdjEdges(Node *base)
{
    if (base == NULL)
        return -1;
    Adjacent *ptr = base->adjacent;
    while (ptr != NULL)
    {
        double sum;
        if (base->distance_to == DBL_MAX)
            sum = DBL_MAX;
        else
            sum = base->distance_to + ptr->weight_prev;
        if (ptr->node->distance_to > sum)
            return 1;
        ptr = ptr->next;
    }
    return 0;
}

int BF(Graph *graph, Node *start)
{
    if (graph == NULL)
        return -1;
    if (start == NULL)
        return 1;
    for (unsigned i = 0; i < graph->num; i++)
    {
        graph->nodes[i]->distance_to = DBL_MAX;
        graph->nodes[i]->prev = NULL;
    }
    start->distance_to = 0;
    for (unsigned v = 0; v < graph->num; v++)
        for (unsigned i = 0; i < graph->num; i++)
            processAdjEdges(graph->nodes[i]);
    for (unsigned i = 0; i < graph->num; i++)
        if (checkAdjEdges(graph->nodes[i]) == 1)
            return 2;
    return 0;
}
