#include <stdio.h>

// Colors for text display
#define STYLE_BOLD    0x01
#define STYLE_OFF     0x00
#define COLOR_RED     31
#define COLOR_YELLOW  33
#define COLOR_BLUE    36


// Edge description
typedef struct {
int departure, arrival;  // Index of start and end nodes at the extremities of this edge
double cost;        // Cost of this edge
} edge_t;


// Edges list description
typedef struct link_edge {
edge_t val;
struct link_edge * next;
}* listedge_t;


// Node description
typedef struct {
int numero;       // Index of node
char* name;      // Name of node
char* line;      // Name of line (for metro)
double x,y;       // Latitude and longitude coordinates
listedge_t edges;     // Edges list which start from this node
} vertex_t;


// Type graphe :
typedef struct {
int size_vertex;      // Nombre de sommets
int size_edges;       // Nombre d’arcs
vertex_t* data;       // Tableau des sommets alloue dynamiquement
} graph_t;




// Fonction prototypes
void errorMsg(char* msg);
void warningMsg(char* msg);
void infoMsg(char* msg);
int stringCheck(char* filename, char* arg2);
int initGraph(graph_t* graph, int nb);
int loadData(graph_t* graph, char* file);
listedge_t createList(void);
int isEmpty(listedge_t l);
listedge_t addElement(listedge_t l, edge_t e);
listedge_t deleteElement(listedge_t l);
void destructGraph(graph_t* graph);
void printGraph(graph_t graph);
