#include <stdio.h>

// Colors for text display
#define STYLE_BOLD    0x01
#define STYLE_OFF     0x00
#define COLOR_RED     31
#define COLOR_YELLOW  33
#define COLOR_BLUE    36

// SDL window dimensions
#define HEIGHT 590     // SDL window height
#define WIDTH 850      // SSL window width


// Data user storage (used by dataForm)
typedef struct {
int code;
int control;
char string;
} datauser_t;


// Index list description (used by A* algorithm)
typedef struct link_int {
int val;
struct link_int* next;
}* listindex_t;


// Edge description
typedef struct {
int departure, arrival;  // Index of start and end nodes at the extremities of this edge
double cost;        // Cost of this edge
} edge_t;


// Node description (used by A* algorithm)
typedef struct {
int numero;   // Index of the node
int parent;   // Index of parent node
char* name;
char* line;
double cost;
double x,y;
} node_t;


// Edges list description
typedef struct link_edge {
edge_t val;
struct link_edge * next;
}* listedge_t;


// Nodes list description
typedef struct link_node {
node_t val;
struct link_node * next;
}* listnode_t;


// Node description (used to store data)
typedef struct {
int numero;       // Index of node
char* name;      // Name of node
char* line;      // Name of line (for metro)
double x,y;       // Latitude and longitude coordinates
listedge_t edges;     // Edges list which start from this node
} vertex_t;


// Type graphe :
typedef struct {
int size_vertex;      // Number of vertex
int size_edges;       // Nombre of edges
vertex_t* data;       // Array of vertex
} graph_t;




// "main.c" function prototypes
void errorMsg(char* msg);
void warningMsg(char* msg);
void infoMsg(char* msg);
int dataCheck(char* filename, char* departurename, char* arrivalname, char* departureindex, char* arrivalindex);
void stringStandardise(char* string);
int initGraph(graph_t* graph, int nb);
int loadData(graph_t* graph, char* file);
listedge_t createListEdge(void);
listnode_t createListNode(void);
listindex_t createListIndex(void);
int isEmptyInteger(listindex_t l);
int isEmptyEdge(listedge_t l);
int isEmptyNode(listnode_t l);
listedge_t addEdges(listedge_t l, edge_t e);
listnode_t addNodes(listnode_t l, node_t n);
listindex_t addInteger(listindex_t l, int i);
listnode_t deleteNode(listnode_t l, node_t n);
listedge_t deleteEdge(listedge_t l);
listindex_t deleteInteger(listindex_t l);
void destructGraph(graph_t* graph);
void printGraph(graph_t graph);
int presentList(node_t n, listnode_t l);


// "astar.c" function prototypes
void pathfinder(graph_t graph, int dep, int arriv, void (*evaluationFunction)(node_t*, listedge_t, int, double, double, double));
void algoAstar(graph_t graph, int dep, int arriv);
void evaluationAStar(node_t* n, listedge_t l, int i_parent, double cost_parent, double lat, double longi);
void algoDikstra(graph_t graph, int dep, int arriv);
void evaluationDikstra(node_t* n, listedge_t l, int i_parent, double cost_parent, double lat, double longi);
void freeMemoryList(listnode_t l1, listnode_t l2, listindex_t l3);
void freeMemoryString(node_t n1, node_t n2);
void printPath(listindex_t l, graph_t graph);
void loadNode(node_t* n, vertex_t v, int i_parent);
int greatCircle(double lat1, double long1, double lat2, double long2);
void printNodeList(listnode_t l);
void printEdgeList(listedge_t l);


// "display.c" function prototypes
void processEvent(datauser_t* data);
int dataForm(char* filename, char* departurename, char* arrivalname, char* departureindex, char* arrivalindex);
