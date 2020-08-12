#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../inc/main.h"

// DEBUG FUNCTION
// Print the nodes list in the shell
// Return nothing
void printNodeList(listnode_t l) {
  listnode_t p = l;
  while (!isEmptyNode(p)) {
    printf("n°%d - %s - cost = %lf\n",(p->val).numero, (p->val).name, (p->val).cost);
    p = p->next;
  }
  puts("");
}

// DEBUG FUNCTION
// Print the nodes list in the shell
// Return nothing
void printEdgeList(listedge_t l) {
  listedge_t p = l;
  while (!isEmptyEdge(p)) {
    printf("n°%d ---> n°%d \t\t cost = %lf\n",(p->val).departure, (p->val).arrival, (p->val).cost);
    p = p->next;
  }
  puts("");
}



// Return the great circle distance between two points on the Earth surface
// Distances given in kilometers
// 6371 is the average radius of the Earth
int greatCircle(double lat1, double long1, double lat2, double long2) {
  return (6371*acos(cos(lat1)*cos(lat2)*cos(long2-long1) + sin(lat1)*sin(lat2)));
}



// Free the memory allocated to lists (opened, closed and final)
// Return nothing
void freeMemory(listnode_t l1, listnode_t l2, listindex_t l3) {
  while (!isEmptyInteger(l3))   // Free the memory allocated to index
    l3 = deleteInteger(l3);
  while (!isEmptyNode(l2))    // Free the memory allocated to nodes in 'closed' list
    l2 = deleteNode(l2, l2->val);
  while (!isEmptyNode(l1))     // Free the memory allocated to nodes in 'opened' list
    l1 = deleteNode(l1, l1->val);
  free(l1);
  free(l2);
  free(l3);
}



// Load data from a vertex_t variable (v) to a node_t variable (n)
// Need some extra data from the parent node (its index 'i_parent' and its cost 'cost_parent') and from the arrival node ('lat' and 'long')
// Return 1 if an error occured, 0 otherwise
int loadNode(node_t* n, vertex_t v, listedge_t l, int i_parent, double cost_parent, double lat, double longi) {
  double cost_edge;       // Cost of the edge to reach the node 'n' from its parent node
  n->numero = v.numero;
  strcpy(n->name, v.name);
  strcpy(n->line, v.line);
  n->x = v.x;
  n->y = v.y;
  n->parent = i_parent;

  if (i_parent == -1)     // If the node 'n' is the departure node (so there is no parent node)
    cost_edge = 0;
  else {
    cost_edge = (l->val).cost;
    if ((l->val).departure != i_parent || (l->val).arrival != v.numero) {
      errorMsg("Error when loading data to node from edges list");
      return 1;
    }
  }
  n->cost = cost_parent + cost_edge + greatCircle(n->x, n->y, lat, longi);    // Evaluation function = the real cost to reach this node from the starting point + heuristic value
  return 0;
}




// Compute the shortest path in the graph, between vertex 'dep' and 'arriv'
// Print the path in the shell
// Return nothing
void algoAstar(graph_t graph, int dep, int arriv) {
  listnode_t closed = createListNode();
  listnode_t opened = createListNode();
  listnode_t p; listnode_t pp;
  listedge_t l;
  listindex_t final = createListIndex();    // Store the node indexes which belong to the final path
  listindex_t f;                            // Used to go through the 'final' list
  double cost; int code; int num;
  node_t current_node;    // Store the current node
  node_t n;   // Store the neighbour nodes of the current node

  current_node.name = calloc(sizeof(*(current_node.name)), 64);
  if (current_node.name == NULL) {
    errorMsg("Error when allocating memory to name of current node");
    return;
  }
  current_node.line = calloc(sizeof(*(current_node.line)), 16);
  if (current_node.line == NULL) {
    errorMsg("Error when allocating memory to line of current node");
    free(current_node.name);
    return;
  }
  n.name = calloc(sizeof(*(n.name)), 64);
  if (n.name == NULL) {
    errorMsg("Error when allocating memory to name of neighbour node");
    free(current_node.name);
    free(current_node.line);
    return;
  }
  n.line = calloc(sizeof(*(n.line)), 16);
  if (n.line == NULL) {
    errorMsg("Error when allocating memory to line of neighbour node");
    free(current_node.name);
    free(current_node.line);
    free(n.name);
    return;
  }
  code = loadNode(&current_node, graph.data[dep], NULL, -1, 0, graph.data[arriv].x, graph.data[arriv].y);    // Node departure
  if (code != 0) {
    free(current_node.name);
    free(current_node.line);
    free(n.name);
    free(n.line);
    return;
  }
  opened = addNodes(opened, current_node);

  while (!isEmptyNode(opened)) {
    p = opened;
    pp = p;
    cost = (opened->val).cost;
    while (!isEmptyNode(p)) {     // Goes through the opened list to find the node that has the smallest evaluation function
      if ((p->val).cost < cost) {
        cost = (p->val).cost;
        pp = p;
      }
      p = p->next;
    }

    current_node = (pp->val);   // Update the current node
    opened = deleteNode(opened, current_node);    // Delete the current node from the opened list

    if (current_node.numero == arriv) {   // If the current node is the final node
      closed = addNodes(closed, current_node);     // Add the last node to the closed list
      num = arriv;
      while (num != dep) {
        final = addInteger(final, num);
        p = closed;
        while ((p->val).numero != num)
          p = p->next;
        num = (p->val).parent;
      }
      final = addInteger(final, dep);
      f = final;
      while (!isEmptyInteger(f)) {
        printf("%d \t %s", f->val, (graph.data[f->val]).name);
        num = strlen((graph.data[f->val]).name);
        while(num < 41) {
          num = num + 8;
          printf("\t");
        }
        printf("%s\n", (graph.data[f->val]).line);
        f = f->next;
      }
      puts("");
      free(current_node.name);
      free(current_node.line);
      free(n.name);
      free(n.line);
      freeMemory(opened, closed, final);
      return;
    }

    l = (graph.data[current_node.numero]).edges;    // Get the list of edges of the node 'current_node'
    while (!isEmptyEdge(l)) {     // Goes through the list of edges to get all the neighbour nodes of node 'current_node'
      code = loadNode(&n, graph.data[(l->val).arrival], l, current_node.numero, cost, graph.data[arriv].x, graph.data[arriv].y);
      if (code != 0) {
        free(current_node.name);
        free(current_node.line);
        free(n.name);
        free(n.line);
        freeMemory(opened, closed, final);
        return;
      }

      p = opened;
      while (!isEmptyNode(p) && ((p->val).numero != n.numero)) {   // Goes through the opened list to check if the neighbour node 'n' is already present
        p = p->next;
      }
      if (!isEmptyNode(p)) {      // If the neighbour node is already present in the opened list (so 'p' is pointing to this node and not 'NULL'), we get its old cost value
        cost = (p->val).cost;
      }

      if (!presentList(n, closed)) {   // If the neighbour node is not present in the closed list
        if (presentList(n, opened)) {    // If the neighbour node is present in the opened list
          if (n.cost > cost) {                    // If the new cost is greater than the old cost
            opened = deleteNode(opened, p->val);    // Delete the neighbour node with an old cost value
            opened = addNodes(opened, n);           // Add the new neighbour node 'n' with the new cost value
          }
        }
        else {
          opened = addNodes(opened, n);           // Add the new neighbour node 'n' with the new cost value
        }
      }
      l = l->next;    // Goes through the list of edges to get all the neighbour nodes
    }
    closed = addNodes(closed, current_node);    // Add the current node to the closed list
  }
  warningMsg("There is apparently no possible path...");
  free(current_node.name);
  free(current_node.line);
  free(n.name);
  free(n.line);
  freeMemory(opened, closed, final);
}
