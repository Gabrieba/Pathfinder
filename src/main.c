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
listedge_t createList(void) {
  return NULL;
}


// Return 1 if the list is empty, 0 otherwise
int isEmpty(listedge_t l) {
  return (l==NULL) ? 1 : 0;
}



// Add an edge at the end of the list
// Return the new modified list
listedge_t addElement(listedge_t l, edge_t e) {
  listedge_t tmp;
  listedge_t p = calloc(1, sizeof(*p));
  if (p == NULL) {
    warningMsg("Error when allocating memory to an edges list.");
    // Liberer les autres maillons ?
    return NULL;
  }
  p->val = e;
  p->next = NULL;
  if (isEmpty(l))
    return p;
  else {
    tmp = l;
    while (tmp->next != NULL)
      tmp = tmp->next;
    tmp->next = p;
  }
  return l;
}



// Delete the first edge of the list and free the allocated memory
// Return the new list
listedge_t deleteElement(listedge_t l) {
  listedge_t p;
  if (isEmpty(l)) {   // If the list is already empty
    warningMsg("List already empty");
    return NULL;
  }
  p = l->next;
  free(l);      // Free the allocated memory
  return p;
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
    while (!isEmpty(l))
      l = deleteElement(l);
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
    while (!isEmpty(pl)) {
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
    (graph->data[i]).name = calloc(sizeof(char), 32);   // Memory allocated to name
    if ((graph->data[i]).name == NULL) {
      errorMsg("Memory allocation error : string name.");
      for (j = 0; j < i; j++)     // Free memory allocated to previous names
        free((graph->data[j]).name);
      free(graph->data);        // Free memory  allocated to nodes array
      return 1;
    }
  }
  for (i = 0; i < nb; i++) {
    (graph->data[i]).line = calloc(sizeof(char), 32);   // Memory allocated to line
    if ((graph->data[i]).line == NULL) {
      errorMsg("Memory allocation error : string line.");
      for (j = 0; j < nb; j++)     // Free memory allocated to previous names
        free((graph->data[j]).name);
      for (j = 0; j < i; j++)     // Free memory allocated to previous lines
        free((graph->data[j]).line);
      free(graph->data);      // Free memory  allocated to nodes array
      return 1;
    }
    (graph->data[i]).edges = createList();    // Create a list of edges
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
  char line[32]; char name[32];
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
    fscanf(fp, "%d %lf %lf %s %s", &index, &latitude, &longitude, line, name);
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
    graph->data[dep].edges = addElement(graph->data[dep].edges, e);
  }
  fclose(fp);
  return 0;
}




// Check if arguments are correct, and return 0
// Return 0 if an error occured : Memory allocation error, no argument, too much arguments or wrong file name
int stringCheck(char* arg1, char* arg2) {
  const char* separator = ".";
  char cmd[21] = "ls Graphes/ | grep ";     // Unix command to check if the file exists
  char* string = calloc(sizeof(*string), 29);
  if (string == NULL) {
    errorMsg("Memory allocation error : string.");
    return 1;
  }
  char* command = calloc(sizeof(*command), 64);    // Memory allocation for Unix command
  if (command == NULL) {
    errorMsg("Memory allocation error : command.");
    free(string);           // Free allocated memory
    return 1;
  }
  strcpy(command, cmd);
  if (arg1 == NULL) {
    errorMsg("No argument specified. You need to give the filename containing the graph.");
    free(command);            // Free allocated memory
    free(string);
    return 1;
  }
  strcpy(string, arg1);     // Get again the filename
  char* strtoken = strtok(string, separator);   // Read filename without extension
  strtoken = strtok(NULL, separator);   // Get file extension
  if ((strtoken == NULL) || ((strcmp(strtoken, "txt") != 0) && (strcmp(strtoken, "csv") != 0)) ) {    // If there is no file extension, or something else than .tkt or .csv
    errorMsg("Incorrect file extension.");
    free(command);          // Free allocated memory
    free(string);
    return 1;
  }
  strcpy(string, arg1);     // Get again the filename
  strcat(command, string);    // Add filename to Unix command
  if (system(command) != 0) {
    printf("%s : ", arg1);
    errorMsg("File not found");
    free(command);          // Free allocated memory
    free(string);
    return 1;
  }
  if (arg2 != NULL) {   // Check if there is too much arguments
    warningMsg("Too much argument specified");
    free(command);      // Free allocated memory
    free(string);
    return 1;
  }
  free(command);    // Free allocated memory
  free(string);
  return 0;
}



// Main code + error handler
int main(int argc, char* argv[]) {
  graph_t graph;
  int code = 0;
  code = stringCheck(argv[1], argv[2]);
  if (code != 0)
    return EXIT_FAILURE;
  code = loadData(&graph, argv[1]);
  if (code != 0)
   return EXIT_FAILURE;
  printGraph(graph);
  destructGraph(&graph);

  infoMsg("END");
  return EXIT_SUCCESS;
}
