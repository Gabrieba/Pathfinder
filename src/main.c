#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/main.h"


void errorMsg(char* msg) {
  fprintf(stdout, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_RED);   // Set to bold red letters
  printf("ERROR : %s\n", msg);
  fprintf(stdout, "%c[%dm", 0x1B, 0);   // Reset
}


void warningMsg(char* msg) {
  fprintf(stdout, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_YELLOW);   // Set to bold yellow letters
  printf("WARNING : %s\n", msg);
  fprintf(stdout, "%c[%dm", 0x1B, 0);   // Reset
}


void infoMsg(char* msg) {
  fprintf(stdout, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_BLUE);   // Set to bold blue letters
  printf("INFO : %s\n", msg);
  fprintf(stdout, "%c[%dm", 0x1B, 0);   // Reset
}



// Create a list (to contain edges)
listedge_t createListEdge(void) {
  return NULL;
}


// Create a list (to contain nodes)
listnode_t createListNode(void) {
  return NULL;
}


// Create a list (to contains index)
listindex_t createListIndex(void) {
  return NULL;
}

// Return 1 if the list of integers is empty, 0 otherwise
int isEmptyInteger(listindex_t l) {
  return (l==NULL) ? 1 : 0;
}


// Return 1 if the list of edges is empty, 0 otherwise
int isEmptyEdge(listedge_t l) {
  return (l==NULL) ? 1 : 0;
}


// Return 1 if the list of nodes is empty, 0 otherwise
int isEmptyNode(listnode_t l) {
  return (l==NULL) ? 1 : 0;
}



// Add an edge at the end of the list
// Return the new modified list
listedge_t addEdges(listedge_t l, edge_t e) {
  listedge_t tmp;
  listedge_t p = calloc(1, sizeof(*p));
  if (p == NULL) {
    warningMsg("Error when allocating memory to an edge.");
    // Liberer les autres maillons ?
    return NULL;
  }
  p->val = e;
  p->next = NULL;
  if (isEmptyEdge(l))
    return p;

  tmp = l;
  while (tmp->next != NULL)
    tmp = tmp->next;
  tmp->next = p;
  return l;
}



// Add an integer at the beginning of the list
// Return the new modified list
listindex_t addInteger(listindex_t l, int i) {
  listindex_t p = calloc(1, sizeof(*p));
  if (isEmptyInteger(p)) {
    warningMsg("Error when allocating memory to an integer.");
    return NULL;
  }
  p->val = i;
  p->next = l;
  return p;
}



// Add a node at the beginning of the list
// Return the new modified list
listnode_t addNodes(listnode_t l, node_t n) {
  listnode_t p = calloc(1, sizeof(*p));
  if (p == NULL) {
    warningMsg("Error when allocating memory to a node.");
    // Liberer les autres maillons ?
    return NULL;
  }
  p->val = n;
  p->next = l;
  return p;
}



// Return 1 if the node 'n' is present in the list of nodes 'l'
// Return 0 otherwise
int presentList(node_t n, listnode_t l) {
  listnode_t p = l;
  while (!isEmptyNode(p)) {
    if ((p->val).numero == n.numero)
      return 1;
    p = p->next;
  }
  return 0;
}



// Delete the first edge of the list and free the allocated memory
// Return the new list
listedge_t deleteEdge(listedge_t l) {
  listedge_t p;
  if (isEmptyEdge(l)) {   // If the list is already empty
    warningMsg("List already empty");
    return NULL;
  }
  p = l->next;
  free(l);      // Free the allocated memory
  return p;
}



// Delete the first integer of the list and free the allocated memory
// Return the new list
listindex_t deleteInteger(listindex_t l) {
  listindex_t p;
  if (isEmptyInteger(l)) {   // If the list is already empty
    warningMsg("List already empty");
    return NULL;
  }
  p = l->next;
  free(l);      // Free the allocated memory
  return p;
}



// Delete the node 'n' from the list 'l' and free the allocated memory
// Return the new modified list
// Return 'l' if 'n' is missing
listnode_t deleteNode(listnode_t l, node_t n) {
  listnode_t p = l;
  listnode_t pp;
  if (isEmptyNode(p))     // If the list 'l' is already empty
    return l;
  if (isEmptyNode(p->next)) {   // If the list 'l' has only one node
    if ((p->val).numero == n.numero)           // If this single node is the node 'n'
      return NULL;
    return l;           // Node 'n' is missing in the list in this case
  }
  if ((p->val).numero == n.numero) {    // If node 'n' is the first node of the list
    pp = p->next;
    free(p);
    return pp;
  }
  pp = p->next;
  while (!isEmptyNode(pp)) {    // Otherwise : if 'n' position in the list is greater than first
    if ((pp->val).numero == n.numero) {    // If the following node is the node 'n'
      p->next = pp->next;
      free(pp);      // Free the memory allocated to the node 'n'
      return l;
    }
    p = p->next;
    pp = pp->next;
  }
  puts("huh");
  return l;     // Node 'n' is missing in the list
}



// Free the memory allocated to the graph
// Return nothing
void destructGraph(graph_t* graph) {
  int i;
  listedge_t l;
  int nb = graph->size_vertex;    // Number of nodes
  for (i = 0; i < nb; i++) {
    free((graph->data[i]).name);
    free((graph->data[i]).line);
    l = (graph->data[i]).edges;
    while (!isEmptyEdge(l))
      l = deleteEdge(l);
  }
  free(graph->data);
}



// Print all the data from a graph
// Return nothing
void printGraph(graph_t graph) {
  int i;
  listedge_t pl;
  printf("size_vertex = %d \t size_egdes = %d\n", graph.size_vertex, graph.size_edges);
  printf("NODES : X - Y - line - name \n");
  for (i = 0; i < graph.size_vertex; i++) {
    printf("%d %lf %lf %s %s\n", (graph.data[i]).numero, (graph.data[i]).x, (graph.data[i]).y, (graph.data[i]).line, (graph.data[i]).name);
  }
  puts("");
  printf("EDGES : departure - arrival - cost \n");
  for (i = 0; i < graph.size_vertex; i++) {
    pl = (graph.data[i]).edges;
    while (!isEmptyEdge(pl)) {
      printf("%d %d %lf\n", (pl->val).departure, (pl->val).arrival, (pl->val).cost);
      pl = pl->next;
    }
  }
  puts("");
}



// Allocate memory to the graph, nodes and edges
// nb is the total number of nodes
// Return 0 where applicable, 0 otherwise
int initGraph(graph_t* graph, int nb) {
  int i, j;
  graph->data = calloc(sizeof(vertex_t), nb);   // Memory allocated to an array of nodes
  if (graph->data == NULL) {
    errorMsg("Memory allocation error : nodes array.");
    return 1;
  }
  for (i = 0; i < nb; i++) {
    (graph->data[i]).name = calloc(sizeof(char), 64);   // Memory allocated to name
    if ((graph->data[i]).name == NULL) {
      errorMsg("Memory allocation error : string name.");
      for (j = 0; j < i; j++)     // Free memory allocated to previous names
        free((graph->data[j]).name);
      free(graph->data);        // Free memory  allocated to nodes array
      return 1;
    }
  }
  for (i = 0; i < nb; i++) {
    (graph->data[i]).line = calloc(sizeof(char), 16);   // Memory allocated to line
    if ((graph->data[i]).line == NULL) {
      errorMsg("Memory allocation error : string line.");
      for (j = 0; j < nb; j++)     // Free memory allocated to previous names
        free((graph->data[j]).name);
      for (j = 0; j < i; j++)     // Free memory allocated to previous lines
        free((graph->data[j]).line);
      free(graph->data);      // Free memory  allocated to nodes array
      return 1;
    }
    (graph->data[i]).edges = createListEdge();    // Create a list of edges
  }
  return 0;
}



// Init the graph and fill it with data from datafile
// Return 0 where applicable, 1 otherwise
int loadData(graph_t* graph, char* file) {
  int code = 0; int nb_nodes, nb_edges, i, index, dep, arriv;
  double latitude, longitude, cost;
  edge_t e;
  listedge_t l;
  char string[64];
  char line[16]; char name[64];
  char filename[64] = "Graphes/";
  strcat(filename, file);
  FILE* fp = fopen(filename, "r");      // Open the file containing the graph
  if (fp == NULL) {
    errorMsg("Error when opening data file.");
    return 1;
  }
  fscanf(fp, "%d %d", &nb_nodes, &nb_edges);    // Read the number of nodes and edges
  code = initGraph(graph, nb_nodes);
  if (code != 0)
    return 1;
  graph->size_vertex = nb_nodes;
  graph->size_edges = nb_edges;
  fgets(string, 2, fp);   // Read the carriage return at the end of the precedent line
  fgets(string, 64, fp);          // Read the phrase
  for (i = 0; i < nb_nodes; i++) {
    fscanf(fp, "%d %lf %lf %s", &index, &latitude, &longitude, line);
    fgets(name, 64, fp);
    stringStandardise(name);     // To delete special caracters
    stringStandardise(line);     // To delete special caracters
    (graph->data[i]).numero = index;
    (graph->data[i]).x = latitude;
    (graph->data[i]).y = longitude;
    strcpy((graph->data[i]).line, line);
    strcpy((graph->data[i]).name, name);
  }
  fgets(string, 2, fp);   // Read the carriage return at the end of the precedent line
  fgets(string, 64, fp);         // Read the phrase

  for (i = 0; i < nb_edges; i++) {
    fscanf(fp, "%d %d %lf", &dep, &arriv, &cost);
    e.departure = dep;
    e.arrival = arriv;
    e.cost = cost;
    graph->data[dep].edges = addEdges(graph->data[dep].edges, e);
  }
  fclose(fp);
  return 0;
}



// Delete the following caracters from the string chain : ' ', '\t', '\n'
// Return nothing
void stringStandardise(char* string) {
  char buffer[64];
  int i = 0; int j = 0;
  strcpy(buffer, string);
  while (buffer[i] != '\0') {     // While the end of 'string' is not reached
    if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n') {       // If the string caracter isn't a space, a tabulation or a carriage return
      string[j] = buffer[i];
      j++;
    }
    i++;
  }
  string[j] = '\0';
}



// Check if arguments are correct, and return 0
// Return 0 if an error occured : not enough arguments, too much arguments, wrong file name, wrong file extension, incorrect argument type
int stringCheck(char** tab, int num) {
  int i;
  const char* separator = ".";      // Separator to isolate the file extension
  char cmd[64] = "ls Graphes/ | grep ";     // Unix command to check if the file exists
  char filename[32];
  if (num < 4) {      // If not enough arguments
    errorMsg("Not enough arguments. You need to specified the filename containing the graph, the departure index and the arrival index : \n ./path char* int int");
    return 1;
  }
  if (num > 4) {    // If too much arguments
    errorMsg("Too much arguments. You only need to specified the filename containing the graph, the departure index and the arrival index : \n ./path char* int int");
    return 1;
  }
  strcpy(filename, tab[1]);
  char* strtoken = strtok(filename, separator);   // Read filename without extension
  strtoken = strtok(NULL, separator);   // Get file extension
  if ((strtoken == NULL) || ((strcmp(strtoken, "txt") != 0) && (strcmp(strtoken, "csv") != 0)) ) {    // If there is no file extension, or something else than .tkt or .csv
    errorMsg("Incorrect file extension.");
    return 1;
  }
  strcat(cmd, tab[1]);
  if (system(cmd) != 0) {       // Check if the file exists in folder 'Graphes'
    printf("%s : ", tab[1]);
    errorMsg("File not found");
    return 1;
  }
  i = 0;
  while (tab[2][i] != '\0') {
    if (tab[2][i] < 48 || tab[2][i] > 57) {   // Check if argument 2 is an integer
      errorMsg("Arguments 2 and 3 must be integers (departure and arrival node index)");
      return 1;
    }
    i++;
  }
  i = 0;
  while (tab[3][i] != '\0') {
    if (tab[3][i] < 48 || tab[3][i] > 57) {     // Check if argument 3 is an integer
      errorMsg("Arguments 2 and 3 must be integers (departure and arrival node index)");
      return 1;
    }
    i++;
  }
  return 0;
}



// Main code + error handler
int main(int argc, char* argv[]) {
  graph_t graph;
  int code = 0;
  int dep, arriv;
  code = stringCheck(argv, argc);
  if (code != 0)
    return EXIT_FAILURE;
  code = loadData(&graph, argv[1]);
  if (code != 0)
   return EXIT_FAILURE;
  sscanf(argv[2], "%d", &dep);
  sscanf(argv[3], "%d", &arriv);
  if (dep > graph.size_vertex || arriv > graph.size_vertex) {
    errorMsg("Arguments 2 and 3 (departure and arrival node index) cannot be greater than the maximal node index");
    destructGraph(&graph);
    return EXIT_FAILURE;
  }
  infoMsg("#### A-STAR ALGORITHM ####");
  puts("");
  algoAstar(graph, dep, arriv);
  //printGraph(graph);
  destructGraph(&graph);
  puts("");
  infoMsg("END OF A-STAR ALGORITHM");
  return EXIT_SUCCESS;
}
