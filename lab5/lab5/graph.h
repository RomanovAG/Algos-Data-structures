#ifndef GRAPH_H
#define GRAPH_H

struct Node;

typedef enum {WHITE, GRAY, BLACK} Color;

typedef enum {SIMPLE, DISTANCE} PrintSettings;

typedef struct Adjacent
<%
    double weight_prev;
    struct Node *node;
    struct Adjacent *next;
%> Adjacent;

typedef struct Node
<%
    char *identifier;
    double x, y;
    Adjacent *adjacent;
    Color color;
    struct Node *prev;
    double distance_to;
    //unsigned start_time, end_time;
%> Node;

typedef struct Graph
<%
    unsigned num;
    Node **nodes;
%> Graph;

int addNode(Graph *graph, const char *identifier, const double x, const double y);
int addEdge(Graph *graph, const char *identifier_start, const char *identifier_end);
int deleteNode(Graph *graph, const char *identifier);
int deleteEdge(Graph *graph, const char *identifier_start, const char *identifier_end);
int deleteGraph(Graph *graph);
int DFS(Graph *graph);
int BF(Graph *graph, Node *start);
int printGraph(const Graph *graph, PrintSettings s);

Node *findNode(Graph *graph, const char *identifier);

#endif // GRAPH_H
