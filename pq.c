/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Vivek Bhagavatula
 * Email: bhagavav@oregonstate.edu
 *
 * this program implements a priority queue based on a min_heap(implied priorities) wheere the underlying data structure being used is a dynamic array. We build the tree for the heap using array representation of a tree. 
 */


#include <stdlib.h>
#include "pq.h"
#include "dynarray.h"

/*
 * This structure represents a priority queue element, containing both the
 * value and its priority.
 */
struct node {
    void* value;
    int priority;
};

/*
 * This struct represents the priority queue. It holds a dynamic
 * array to store thhe min_heap which is made up of nodes
 */
struct pq {
    struct dynarray* heap;
};


/*
 * This function allocates a PQ
 */
struct pq* pq_create() {
    struct pq* pq = malloc(sizeof(struct pq));
    pq->heap = dynarray_create();
    return pq;
}


/*
 * This function frees the memory allocated to th given priority queue.
 */
void pq_free(struct pq* pq) {
    dynarray_free(pq->heap);
    free(pq);
}


/*
 * This function returns 1 if the specified priority queue is empty and
 * 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
    return dynarray_size(pq->heap) == 0;
}


/*
 * This function inserts a given element into a priority queue with a
 * specified priority value.  LOWER priority values are assigned to elements
 * with HIGHER priority.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
    struct node* new_node = malloc(sizeof(struct node));// new node to add to the queue
    new_node->value = value;
    new_node->priority = priority;

    dynarray_insert(pq->heap, new_node);// add node to the end of the dynarray where it will be subsequently heapified

    // heapify the new node as high as you can
    int curr = dynarray_size(pq->heap) - 1;//
    while (curr > 0) {
        int par = (curr - 1) / 2;// gets the parent of the node
        struct node* parent_node = dynarray_get(pq->heap, par);
        struct node* current_node = dynarray_get(pq->heap, curr);

        if (current_node->priority < parent_node->priority) {
            // Swap the node with its parent if it has higher priority
            dynarray_set(pq->heap, curr, parent_node);
            dynarray_set(pq->heap, par, current_node);
            curr = par;
        } else {
            break;
        }
    }
}


/*
 * This function returns the value of the first item (lowest priority) in a
 * priority queue. The queue itself is not modified.
 */
void* pq_first(struct pq* pq) {
    struct node* first = dynarray_get(pq->heap, 0);
    return first->value;
}


/*
 * This function returns the priority value of the first item (lowest priority)
 * in a priority queue. The queue itself is not modified.
 */
int pq_first_priority(struct pq* pq) {
    struct node* first = dynarray_get(pq->heap, 0);
    return first->priority;
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 */
void* pq_remove_first(struct pq* pq) {

    int heapSize = dynarray_size(pq->heap) - 1;

    // Get the root node 
    struct node* rootNode = dynarray_get(pq->heap, 0);
    void* value = rootNode->value;

    //set the last node at the root and remove it from it's original place
    struct node* lastNode = dynarray_get(pq->heap, heapSize);
    dynarray_set(pq->heap, 0, lastNode);
    dynarray_remove(pq->heap, heapSize);

    // Heapify down from the root to maintain the heap property
    int curr = 0;
    while (1) {
        int LC = 2 * curr + 1;//gets the left child
        int RC = 2 * curr + 2;//gets the right child
        int smallest = curr;// sets the smallest calculation to a base of current because it might not change

        if (LC < heapSize && ((struct node*)dynarray_get(pq->heap, LC))->priority < ((struct node*)dynarray_get(pq->heap, smallest))->priority) {// if the LC currently exissts in the heap and it is smaller
            smallest = LC;// set the smallest to LC
        }

        if (RC < heapSize && ((struct node*)dynarray_get(pq->heap, RC))->priority < ((struct node*)dynarray_get(pq->heap, smallest))->priority) {//same for right
            smallest = RC;
        }

        if (smallest == curr) {// if there was nothing smaller than the current node, stop swapping and exit the heapify loop
            break; 
        }

        // Swap the nodes
        struct node* temp = dynarray_get(pq->heap, curr);
        dynarray_set(pq->heap, curr, dynarray_get(pq->heap, smallest));
        dynarray_set(pq->heap, smallest, temp);
        curr = smallest;
    }

    free(rootNode);// free the root we took off at the very beginning 

    return value;//return the value of the removal
}
