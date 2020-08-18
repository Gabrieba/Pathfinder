#include <stdio.h>
#include <stdlib.h>

# include "../inc/main.h"


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
  if (isEmptyNode(p))           // If the list 'l' is already empty
    return l;
  if (isEmptyNode(p->next)) {           // If the list 'l' has only one node
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
  while (!isEmptyNode(pp)) {            // Otherwise : if 'n' position in the list is greater than the first position
    if ((pp->val).numero == n.numero) {    // If the following node is the node 'n'
      p->next = pp->next;
      free(pp);                 // Free the memory allocated to the node 'n'
      return l;
    }
    p = p->next;
    pp = pp->next;
  }
  return l;     // Node 'n' is missing in the list
}
