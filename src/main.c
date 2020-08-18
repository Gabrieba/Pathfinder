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



// Free the memory allocated to strings used to retrieve user data
// Return nothing
void destructStringData(char* filename, char* departureindex, char* arrivalindex, char* departurename, char* arrivalname) {
  if (filename != NULL)
    free(filename);
  if (departureindex != NULL)
    free(departureindex);
  if (arrivalindex != NULL)
    free(arrivalindex);
  if (departurename != NULL)
    free(departurename);
  if (arrivalname != NULL)
    free(arrivalname);
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
    if (buffer[i] != '\t' && buffer[i] != '\n') {       // If the string caracter isn't a space, a tabulation or a carriage return
      string[j] = buffer[i];
      j++;
    }
    i++;
  }
  string[j] = '\0';
}



// Check if user data are correct
//Return 0 if successful, 0 otherwise
int dataCheck(graph_t graph, char* filename, char* departurename, char* arrivalname, char* departureindex, char* arrivalindex, int* dep, int* arriv) {
  int bit = 0; int i;
  if (departureindex[0] != '\0')        // If the user has given a departure index
    sscanf(departureindex, "%d", dep);     // get the departure index
  else {                                            // If the user has given a departure name instead
    for (i = 0; i < graph.size_vertex; i++) {         // Go through the nodes list to check if this node name exists
      if (strcmp(graph.data[i].name, departurename) == 0) {
        bit = 1;        // Bit = 1 means that the given node name exists
        break;
      }
    }
    if (!bit) {       // If the given node name doesn't exist
      printf("%s : ", departurename);
      warningMsg("this node name doesn't exist");
      return 1;
    }
    else
      *dep = i;      // If the node name exists, we get its index
  }
  bit = 0;
  if (arrivalindex[0] != '\0')          // If the user has given an arrival index
    sscanf(arrivalindex, "%d", arriv);     // Get the arrival index
  else {                                        // If the user has given an arrival name instead
    for (i = 0; i < graph.size_vertex; i++) {       // Go through the nodes list to check if this node name exists
      if (strcmp(graph.data[i].name, arrivalname) == 0) {
        bit = 1;              // Bit = 1 means that the given node name exists
        break;
      }
    }
    if (!bit) {               // If the given node name doesn't exist
      printf("%s : ", arrivalname);
      warningMsg("this node name doesn't exist");
      return 1;
    }
    else
      *arriv = i;             // If the node name exists, we get its index
  }

  if (*dep > graph.size_vertex || *arriv > graph.size_vertex) {
    errorMsg("Departure and arrival node indexes cannot be greater than the maximal node index");
    return 1;
  }
  return 0;
}



// Check if arguments are correct, and return 0
// Return 0 if an error occured : not enough arguments, too much arguments, wrong file name, wrong file extension, incorrect argument type
int dataPreCheck(char* filename, char* departurename, char* arrivalname, char* departureindex, char* arrivalindex, void (*algo)(graph_t graph, int dep, int arriv, double (*heuristic)(double nx, double ny, double lat, double longi)), double (*heuristic)(double nx, double ny, double lat, double longi)) {
  int i;
  const char* separator = ".";      // Separator to isolate the file extension
  char cmd[64] = "ls Graphes/ | grep ";     // Unix command to check if the file exists
  char string[32];

  if (algo == NULL) {
    errorMsg("You didn't choose an algorithm");
    return 1;
  }

  strcpy(string, filename);
  char* strtoken = strtok(string, separator);   // Read filename without extension
  strtoken = strtok(NULL, separator);   // Get file extension
  if ((strtoken == NULL) || ((strcmp(strtoken, "txt") != 0) && (strcmp(strtoken, "csv") != 0)) ) {    // If there is no file extension, or something else than .tkt or .csv
    errorMsg("Incorrect file extension.");
    return 1;
  }
  strcat(cmd, filename);
  if (system(cmd) != 0) {       // Check if the file exists in folder 'Graphes'
    printf("%s : ", filename);
    errorMsg("File not found");
    return 1;
  }
  i = 0;
  while (departureindex[i] != '\0') {
    if (departureindex[i] < 48 || departureindex[i] > 57) {   // Check if argument 2 is an integer
      errorMsg("Arguments 2 and 3 must be integers (departure and arrival node index)");
      return 1;
    }
    i++;
  }
  i = 0;
  while (arrivalindex[i] != '\0') {
    if (arrivalindex[i] < 48 || arrivalindex[i] > 57) {     // Check if argument 3 is an integer
      errorMsg("Arguments 2 and 3 must be integers (departure and arrival node index)");
      return 1;
    }
    i++;
  }
  if (departurename[0] == '\0' && departureindex[0] == '\0') {
    errorMsg("You must specify a name or an index for the departure node");
    return 1;
  }
  if (arrivalname[0] == '\0' && arrivalindex[0] == '\0') {
    errorMsg("You must specify a name or an index for the arrival node");
    return 1;
  }
  return 0;
}



// Main code + error handler
// Return EXIT_SUCCESS in case of succes, EXIT_FAILURE otherwise
int main(int argc, char* argv[]) {
  graph_t graph;
  int code = 0;
  int dep, arriv, i;
  double (*heuristic)(double nx, double ny, double lat, double longi);
  void (*algo)(graph_t graph, int dep, int arriv, double (*heuristic)(double nx, double ny, double lat, double longi));

  char* filename = calloc(32, sizeof(char));
  if (filename == NULL) {
    errorMsg("Error when allocating memory to filename");
    exit(EXIT_FAILURE);
  }
  char* departurename = calloc(32, sizeof(char));
  if (departurename == NULL) {
    errorMsg("Error when allocating memory to departurename");
    free(filename);
    exit(EXIT_FAILURE);
  }
  char* arrivalname = calloc(32, sizeof(char));
  if (arrivalname == NULL) {
    errorMsg("Error when allocating memory to arrivalname");
    destructStringData(filename, NULL, NULL, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }
  char* departureindex = calloc(8, sizeof(char));
  if (departureindex == NULL) {
    errorMsg("Error when allocating memory to departureindex");
    destructStringData(filename, departureindex, NULL, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }
  char* arrivalindex = calloc(8, sizeof(char));
  if (arrivalindex == NULL) {
    errorMsg("Error when allocating memory to arrivalindex");
    destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }

  if (argc > 1) {       // If too much arguments have been specified
    warningMsg("You don't need to specify arguments after './path'.");
    destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }
  code = dataForm(filename, departurename, arrivalname, departureindex, arrivalindex, &algo, &heuristic);   // Get user data through a graphic interface
  if (code != 0) {
    destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }
  code = dataPreCheck(filename, departurename, arrivalname, departureindex, arrivalindex, algo, heuristic);      // Check if user data are correct
  if (code != 0) {
    destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }
  code = loadData(&graph, filename);        // Load graph data from file
  if (code != 0) {
    destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }
  code = dataCheck(graph, filename, departurename, arrivalname, departureindex, arrivalindex, &dep, &arriv);        // Load graph data from file
  if (code != 0) {
    destructGraph(&graph);
    destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
    exit(EXIT_FAILURE);
  }

  infoMsg("########## PATHFINDER ALGORITHM ##########");
  puts("");
  (*algo)(graph, dep, arriv, heuristic);      // Execute the pathfinder algorithm

  destructGraph(&graph);
  destructStringData(filename, departureindex, arrivalindex, departurename, arrivalname);
  puts("");
  infoMsg("########## END OF PATHFINDER ALGORITHM ##########");
  exit(EXIT_SUCCESS);
}
