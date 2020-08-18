#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../inc/main.h"



// Print the entire path at the end of the pathfinder algorithm
// Node indexs are arranged in order in the list 'l'
// Return nothing
void printPath(listindex_t l, graph_t graph) {
  listindex_t f = l; int i;
  while (!isEmptyInteger(f)) {      // As long as there are vertices to display
    printf("%d \t %s", f->val, (graph.data[f->val]).name);      // Print the node index and the node name
    i = strlen((graph.data[f->val]).name);
    while(i < 41) {         // To align vertically all line names
      i = i + 8;            // Because 1 tabulation = 8 spaces
      printf("\t");         // Display 1 tabulation
    }
    printf("%s\n", (graph.data[f->val]).line);      // Print the line name
    f = f->next;
  }
  puts("");
}



// Return the great circle distance between two points on the Earth surface
// Distances given in kilometers
// 6371 is the average radius of the Earth
double greatCircle(double lat1, double long1, double lat2, double long2) {
  return (6371*acos(cos(lat1)*cos(lat2)*cos(long2-long1) + sin(lat1)*sin(lat2)));
}



// Return the Manhattan distance between two points
// Distance given in kilometers
double manhattan(double lat1, double long1, double lat2, double long2) {
  puts("Not implemented yet !");
  return (0);
}



// Return the Euclidean distance between two points
// Distance given in kilometers
double euclidean(double lat1, double long1, double lat2, double long2) {
  puts("Not implemented yet !");
  return (0);
}



// Return the Octile distance between two points
// Distance given in kilometers
double octile(double lat1, double long1, double lat2, double long2) {
  puts("Not implemented yet !");
  return (0);
}



// Return the Chebyshev distance between two points
// Distance given in kilometers
double chebyshev(double lat1, double long1, double lat2, double long2) {
  puts("Not implemented yet !");
  return (0);
}



// Free the memory allocated to strings (node name and node line name)
// Return nothing
void freeMemoryString(node_t n1, node_t n2) {
  if (n1.name != NULL)
    free(n1.name);       // Free the memory allocated to the node name
  if (n1.line != NULL)
    free(n1.line);      // Free the memory allocated to the node line name
  // puts("OK");
  // if (n2.name != NULL)
  //   free(n2.name);      // Free the memory allocated to the node name
  // puts("OK");
  // if (n2.line != NULL)
  //   free(n2.line);      // Free the memory allocated to the node line name
}



// Free the memory allocated to the lists (opened, closed and final)
// Return nothing
void freeMemoryList(listnode_t l1, listnode_t l2, listindex_t l3) {
  while (!isEmptyInteger(l3))   // Free the memory allocated to index
    l3 = deleteInteger(l3);
  while (!isEmptyNode(l2))    // Free the memory allocated to nodes in 'closed' list
    l2 = deleteNode(l2, l2->val);
  while (!isEmptyNode(l1))     // Free the memory allocated to nodes in 'opened' list
    l1 = deleteNode(l1, l1->val);
}



// Load data from a vertex_t variable (v) to a node_t variable (n)
// Need some extra data from the parent node (its index 'i_parent' and its cost 'cost_parent') and from the arrival node ('lat' and 'long') and the heuristic function to compute the node cost
// Return nothing
void loadNode(node_t* n, vertex_t v, listedge_t l, int i_parent, double cost_parent, double lat, double longi, double (*heuristic)(double nx, double ny, double lat, double longi)) {
  double cost_edge;
  n->numero = v.numero;     // Get the index node
  strcpy(n->name, v.name);      // Get the node name
  strcpy(n->line, v.line);      // Get the line name
  n->x = v.x;           // Get the node coordinates
  n->y = v.y;
  n->parent = i_parent;     // Get the parent node index
  if (i_parent == -1)         // If the current node is the departure node (so there is no parent node)
    cost_edge = 0;
  else                    // If the current node is not the departure node
    cost_edge = (l->val).cost;
  if ((*heuristic) != NULL)        // If the algorithm is not Dijkstra
    n->cost = cost_parent + cost_edge + (*heuristic)(n->x, n->y, lat, longi);    // Evaluation function = the real cost to reach this node from the starting point + heuristic value
  else
    n->cost = cost_parent + cost_edge;    // No heuristic function for Dijkstra algorithm
}



// Compute the shortest path in the graph, between vertex 'dep' and 'arriv' according to A Star and Dijkstra algorithm
// Print the path in the shell
// Return nothing
void pathfinder1(graph_t graph, int dep, int arriv, double (*heuristic)(double nx, double ny, double lat, double longi)) {
  listnode_t closed = createListNode();       // List of nodes that are already treated
  listnode_t opened = createListNode();       // List of nodes neighbouring to nodes that are the 'closed' list
  listnode_t p; listnode_t pp;            // Nodes list used to go through 'opened' and 'closed' lists
  listedge_t l;                           // Edges list used to go through 'graph.val[...].edges' lists
  listindex_t final = createListIndex();    // Store the node indexes which belong to the final path
  listindex_t f;                            // Used to go through the 'final' list
  double cost;                      // 'cost' used to contain the smallest cost value of 'opened' list
  int num;                          // 'num' is used to store node index
  node_t current_node;          // Store the current node
  node_t node;                  // Store the neighbour nodes of the current node

  (current_node).name = calloc(64, sizeof(*(current_node.name)));     // Allocate memory to node name
  if ((current_node).name == NULL) {
    errorMsg("Error when allocating memory to name of current node");
    return;
  }
  (current_node).line = calloc(16, sizeof(*(current_node.line)));     // Allocate memory to node line name
  if ((current_node).line == NULL) {
    errorMsg("Error when allocating memory to line of current node");
    free((current_node).name);
    return;
  }
  node.name = calloc(64, sizeof(char));                     // Allocate memory to node name
  if (node.name == NULL) {
    errorMsg("Error when allocating memory to name of neighbour node");
    freeMemoryString((current_node), node);         // Free the memory allocated to the node names
    return;
  }
  node.line = calloc(16, sizeof(char));                 // Allocate memory to node line name
  if (node.line == NULL) {
    errorMsg("Error when allocating memory to line of neighbour node");
    freeMemoryString((current_node), node);         // Free the memory allocated to the node names
    return;
  }

  loadNode(&current_node, graph.data[dep], NULL, -1, 0, graph.data[arriv].x, graph.data[arriv].y, heuristic);    // Load data from vertex 'graph.data[dep]' to current node (which is the departure node) ; parent node index = -1 means that it has no parent node
  opened = addNodes(opened, current_node);      // Add the current node (which is the starting node) in the 'opened' list

  while (!isEmptyNode(opened)) {      // As long as there are nodes in the 'opened' list (otherwise there will be no possible path)
    p = opened;       // 'p' is used to go through the 'opened' list
    pp = p;         // 'pp' is used to store the node that has the smallest cost value
    cost = (opened->val).cost;
    while (!isEmptyNode(p)) {     // Goes through the opened list to find the node that has the smallest evaluation function
      if ((p->val).cost < cost) {
        cost = (p->val).cost;       // Update the new cost value
        pp = p;
      }
      p = p->next;
    }
    current_node = (pp->val);   // Update the current node
    opened = deleteNode(opened, current_node);    // Delete the current node from the opened list

    if (current_node.numero == arriv) {   // If the current node is the final node
      closed = addNodes(closed, current_node);     // Add the last node to the closed list
      num = arriv;
      while (num != dep) {                    // As long as the starting node has not been reached
        final = addInteger(final, num);        // Add this index node in the 'final' list
        p = closed;
        while ((p->val).numero != num)      // Go through the nodes list 'closed' to find its parent node index
          p = p->next;
        num = (p->val).parent;      // Update the 'num' value with the parent node index
      }
      final = addInteger(final, dep);     // Add the last node (which is the starting node) to the 'final' list
      printPath(final, graph);            // Display the path in the shell
      freeMemoryString((current_node), node);         // Free the memory allocated to the node names
      freeMemoryList(opened, closed, final);      // Free the memory allocated to remaining nodes and integers in the lists
      return;
    }

    l = (graph.data[current_node.numero]).edges;    // Get the list of edges of the node 'current_node'
    while (!isEmptyEdge(l)) {                      // Goes through the list of edges to get all the neighbour nodes of node 'current_node'
      loadNode(&node, graph.data[(l->val).arrival], l, current_node.numero, cost, graph.data[arriv].x, graph.data[arriv].y, heuristic);

      p = opened;
      while (!isEmptyNode(p) && ((p->val).numero != node.numero)) {   // Goes through the opened list to check if the neighbour node 'n' is already present
        p = p->next;
      }
      if (!isEmptyNode(p)) {      // If the neighbour node is already present in the opened list (so 'p' is pointing to this node and not 'NULL'), we get its old cost value
        cost = (p->val).cost;
      }
      if (!presentList(node, closed)) {   // If the neighbour node is not present in the closed list
        if (presentList(node, opened)) {    // If the neighbour node is present in the opened list
          if (node.cost > cost) {                    // If the new cost is greater than the old cost
            opened = deleteNode(opened, p->val);    // Delete the neighbour node with an old cost value
            opened = addNodes(opened, node);           // Add the new neighbour node 'n' with the new cost value
          }
        }
        else {
          opened = addNodes(opened, node);           // Add the new neighbour node 'n' with the new cost value
        }
      }
      l = l->next;    // Goes through the list of edges to get all the neighbour nodes
    }
    closed = addNodes(closed, current_node);    // Add the current node to the closed list
  }
  warningMsg("There is apparently no possible path...");
  freeMemoryString((current_node), node);         // Free the memory allocated to the node names
  freeMemoryList(opened, closed, final);      // Free the memory allocated to remaining nodes and integers in the lists
}




// Compute the shortest path in the graph, between vertex 'dep' and 'arriv' according to BFS algorithm
// Print the path in the shell
// Return nothing
void pathfinder2(graph_t graph, int dep, int arriv, double (*heuristic)(double nx, double ny, double lat, double longi)) {
  puts("BFS algorithm not implemented yet !");
  return;
}



// Compute the shortest path in the graph, between vertex 'dep' and 'arriv' according to DFS algorithm
// Print the path in the shell
// Return nothing
void pathfinder3(graph_t graph, int dep, int arriv, double (*heuristic)(double nx, double ny, double lat, double longi)) {
  puts("DFS algorithm not implemented yet !");
  return;
}
