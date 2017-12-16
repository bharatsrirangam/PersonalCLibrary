	/**
	 * CS 2110 - Fall 2017 - Homework #10
	 *
	 * @author Bharat Srirangam
	 *
	 * list.c: Complete the functions!
	 */

	/**
	 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 * For any function that must use malloc, if malloc returns NULL, the function
	 * itself should return NULL if needs to return something (or return false if
	 * the function returns a bool).
	 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

	// This is just to remove compiler warnings associated with unused variables.
	// Delete calls to this as you implement functions.
#define UNUSED_PARAMETER(x) (void)(x)

	/* The node struct. Has a next pointer, and data. */
	/* DO NOT MODIFY THIS STRUCTURE */
	/* Only this file should deal with the node struct */
typedef struct _list_node
{
    struct _list_node *next;
    void *data;
} node;

	/* You should NOT have any global variables. */

	/* The create_node function is static because this is the only file that should
	   have knowledge about the nodes backing the linked list. */
static node *create_node(void *data);

	/** create_node
	  *
	  * Helper function that creates a node by allocating memory for it on the heap.
	  * Be sure to set its next pointer to NULL.
	  *
	  * If malloc returns NULL, you should return NULL to indicate failure.
	  *
	  * @param data a void pointer to data the user wants to store in the list
	  * @return a node
	  */
static node* create_node(void *data)
{
	node* newNode = (node*)malloc(sizeof(node));
	if (NULL == newNode) {
		return NULL;	
	}
	newNode->data = data;
	newNode->next = NULL;
    return newNode;
}

	/** create_list
	  *
	  * Creates a list by allocating memory for it on the heap.
	  * Be sure to initialize size to zero and head and tail to NULL.
	  *
	  * If malloc returns NULL, you should return NULL to indicate failure.
	  *
	  * @return a pointer to a new list or NULL on failure
	  */
list *create_list(void)
{
	list* newList = malloc(sizeof(list));
	if (NULL == newList) {
		return NULL;
	}
	newList->head = NULL;
	newList->tail = NULL;
	newList->size = 0;
    return newList;
}

	/** shallow_copy_list
	  *
	  * Creates a shallow copy of the list. This means a new list structure and
	  * nodes should be created, but each node will point to the same data as the
	  * list passed in.
	  *
	  * if malloc fails or the list is NULL, return NULL to indicate failure. If
	  * any memory allocations failures occur, your code must not leak memory. That
	  * is, if create_node() returns NULL, you must go back and free any nodes
	  * you've allocated so far and the new list.
	  *
	  * @param listToCopy the list to shallow copy
	  * @return a pointer to the new list or NULL on failure
		typedef struct _list
			{
				struct _list_node *head;
				struct _list_node *tail;
				int size;
			} list;

		QUESTION ONE: Do we have to deal with no contiguous memory? 
		QUESTION TWO: Can we use other methods from the linkedlist library? 
	  */
list *shallow_copy_list(list *listToCopy)
{	
	if (listToCopy == NULL) {
		return NULL;
	}
	
	list* newList = create_list();
	if (NULL == newList) {
		return NULL;
	}
	int size = listToCopy->size;
	if (size == 0) {
		return newList;
	}
	
	if (size == 1) {
		node* newNode = create_node((listToCopy->head)->data);
		if (newNode == NULL) {
			//TODO
			free(newList);
			return NULL;
		}
		newList->head = newNode;
		newList->tail = newNode;
		newList->size = 1;
		return newList;
	}

	node* current = listToCopy->head;
	for (int i = 0; i < size; i++) {
		if (false == push_back(newList, current->data)) {
			//TODO
			void* dataOut = NULL;
			for (int i = 0; i < newList->size; i++) {
			 	pop_front(newList, &dataOut);
			}
			free(newList);
			return NULL;
		}
		current = current->next;
	}
   return newList;
}

	/** deep_copy_list
	  *
	  * Create a new list structure, new nodes, and new copies of the data by using
	  * the copy function. Its implementation for any structure must copy
	  * EVERYTHING!
	  *
	  * If listToCopy or copyFunc is NULL, return NULL. For memory allocations
	  * failures, including if copyFunc() returns false (indicating a memory
	  * allocation failure), your code must not leak memory. This means that if any
	  * memory allocation failures occur, before you return NULL, you must go back
	  * and free all data in the new list, nodes in the new list, and the new list
	  * itself.
	  *
	  * @param listToCopy A pointer to the list structure to make a copy of
	  * @param copyFunc A function pointer to a function that makes a copy of the
	  *                 data that's being used in this list, allocating space for
	  *                 every part of that data on the heap. This is some function
	  *                 you must write yourself for testing, tailored specifically
	  *                 to whatever context you're using the list for in your test.
	  *                 If copyFunc returns false for any element, deep_copy_list()
	  *                 should return NULL.
	  * @param freeFunc function used to free the nodes' data in the case of a
	  *                 memory allocation failure when copying (see above)
	  * @return The list structure created by copying the old one, or NULL on
	  *         failure
	  */
list *deep_copy_list(list *listToCopy, list_copy copyFunc, list_op freeFunc)
{
	if (listToCopy == NULL) {
		return NULL;	
	}
	if (copyFunc == NULL) {
		return NULL;
	}
	if (freeFunc == NULL) {
		return NULL;
	}

	list* newList = create_list();
	if (NULL == newList) {
		return NULL;
	}

	int size = listToCopy->size;

	if (size == 0) {
		return newList;
	}
	//Need to free the list as well
	node* cursor = listToCopy->head;
	if (size == 1) {
		void* dataOut = NULL;
		if (copyFunc(cursor->data, &dataOut) == false) {
			free(newList);
			return NULL;
		}
		node* newNode = create_node(dataOut);
		newList->head = newNode;
		newList->tail = newNode;
		newList->size = 1;
		return newList;			
	}

	//node* cursor = listToCopy->head;
	void* dataOut = NULL;
	for (int i = 0; i < size; i++) {
		if (copyFunc(cursor->data, &dataOut) == false) {
			//TODO
			empty_list(newList, freeFunc);
			free(newList);
			return NULL;
		}

		if (push_back(newList, dataOut) == false) {
			//TODO
			freeFunc(dataOut);
			empty_list(newList, freeFunc);
			free(newList);
			return NULL;	
		}
		cursor = cursor->next;
	}

	return newList;
}

	/** push_front
	  *
	  * Adds the element at the front of the list.
	  *
	  * @param listToPrependTo a pointer to the list structure.
	  * @param data the data to add to the list.
	  * @return false if the list is NULL, true if successful.
	  */
bool push_front(list *listToPrependTo, void *data)
{

	if (listToPrependTo == NULL) {
		return false;
	}

	node* newNode = create_node(data);
	if (NULL == newNode) {
		return false;
	}    
	
	int size = listToPrependTo->size; 
	if (size == 0) {
		listToPrependTo->head = newNode;
		listToPrependTo->tail = newNode;
		listToPrependTo->size = size + 1;
	} else {
		newNode->next = listToPrependTo->head;
		listToPrependTo->head = newNode;
		listToPrependTo->size = size + 1;
	}

	return true;
}

	/** push_back
	  *
	  * Adds the element to the back of the list.
	  *
	  * @param listToAppendTo a pointer to the list structure.
	  * @param data the data to add to the list.
	  * @return false if the list is NULL, true if successful.
	  */
bool push_back(list *listToAppendTo, void *data)
{
	if (listToAppendTo == NULL) {
		return false;
	}

    node* newNode = create_node(data);
	if (NULL == newNode) {
		return false;
	}    
	
	int size = listToAppendTo->size; 
	if (size == 0) {
		listToAppendTo->head = newNode;
		listToAppendTo->tail = newNode;
		listToAppendTo->size = size + 1;
	} else {
		(listToAppendTo->tail)->next = newNode;
		listToAppendTo->tail = (listToAppendTo->tail)->next;
		listToAppendTo->size = size + 1;
	}

	return true;
}

	/** add
	  *
	  * Add the element at the specified index in the list. This index must lie in
	  * the inclusive range [0,size].
	  * For example, if you have no elements in the list,
	  * you should be able to add to index 0 but no further.
	  * If you have two elements in the list,
	  * you should be able to add to index 2 but no further.
	  *
	  * @param listToAddTo a pointer to the list structure
	  * @param index 0-based, starting from the head in the inclusive range
	  *              [0,size]
	  * @param data the data to add to the list
	  * @return false if the index is out of bounds or the list is NULL
	  *         (do not add the data in this case)
	  *         otherwise return true
	  */
bool add(list *listToAddTo, int index, void *data)
{
    if (NULL == listToAddTo) {
		return false;
	}
	if (index < 0) {
		return false;
	}
	
	int size = listToAddTo->size;
	
	if (index > size || index < 0) {
		return false;
	}

	if (size == 0) {
		return push_front(listToAddTo, data);
	}

	if (index == 0) {
		return push_front(listToAddTo, data);
	}

	if (index == size) {
		return push_back(listToAddTo, data);
	}

	node* newNode = create_node(data);

	if (newNode == NULL) {
		return false;
	}

	node* current = listToAddTo->head;
	for (int i = 0; i < index-1; i++) {
		current = current->next;
	}
	node* temp = current->next;
	current->next = newNode;
	(current->next)->next = temp;
	listToAddTo->size = listToAddTo->size + 1;
    return true;
}

	/** front
	  *
	  * Gets the data at the front of the list
	  * If the list is empty or the list is NULL, return false.
	  *
	  * @param listToGetFrontOf a pointer to the list
	  * @param dataOut A pointer to a pointer used to return the data at the first
	  *                node in the list or NULL on failure.
	  * @return false if dataOut is NULL or the list is NULL or empty, true if
	  *         successful.

		QUESTION: Can I dereference a double void pointer once?
	  */
bool front(list *listToGetFrontOf, void **dataOut)
{
	if (NULL == dataOut) {
		return false;
	}
    if (NULL == listToGetFrontOf) {
		*dataOut = NULL;
		return false;
	} 
	if (listToGetFrontOf->size == 0) {
		*dataOut = NULL;
		return false;
	}

	*dataOut = (listToGetFrontOf->head)->data;
    return true;
}

	/** back
	  *
	  * Gets the data at the back of the list
	  * If the list is empty or the list is NULL, return false.
	  *
	  * @param listToGetBackOf a pointer to the list
	  * @param dataOut A pointer to a pointer used to return the data at the last
	  *                node in the list or NULL on failure.
	  * @return false if dataOut is NULL or the list is NULL or empty, true
	  *         otherwise
	  */
bool back(list *listToGetBackOf, void **dataOut)
{
    if (NULL == dataOut) {
		return false;
	}
    if (NULL == listToGetBackOf) {
		*dataOut = NULL;
		return false;
	} 
	if (listToGetBackOf->size == 0) {
		*dataOut = NULL;
		return false;
	}

	*dataOut = (listToGetBackOf->tail)->data;
    return true;
}

	/** get
	  *
	  * Gets the data at the specified index in the list
	  *
	  * @param listToGetDataFrom a pointer to the list structure
	  * @param index 0-based, starting from the head.
	  * @param dataOut A pointer to a pointer used to return the data from the
	  *        specified index in the list or NULL on failure.
	  * @return false if dataOut is NULL or index is out of range of the list or
	  *         the list is NULL, true otherwise
	  */
bool get(list *listToGetDataFrom, int index, void **dataOut)
{
	if (NULL == dataOut) {
		return false;
	}
	if (NULL == listToGetDataFrom) {
		*dataOut = NULL;
		return false; 
	} 
	if (listToGetDataFrom->size <= index) {
		*dataOut = NULL;
		return false;
	}

	if (0 > index) {
		*dataOut = NULL;
		return false;
	}

    node* current = listToGetDataFrom->head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}
	*dataOut = current->data;
    return true;
}

	/** size
	  *
	  * Gets the size of the list
	  *
	  * if the list is NULL, return false.
	  *
	  * @param listToGetSizeOf a pointer to the list structure
	  * @param A pointer used to return the size of the list, set to -1 on NULL list
	  * @return false if sizeOut is NULL or list is NULL, true otherwise
	  */
bool size(list *listToGetSizeOf, int *sizeOut)
{
	if (NULL == sizeOut) {
		return false;
	}
	if (NULL == listToGetSizeOf) {
		*sizeOut = -1;
		return false;
	}
	
	*sizeOut = listToGetSizeOf->size;
	return true;
}

	/** contains
	  *
	  * Traverses the list, trying to see if the list contains some data.
	  *
	  * The "data" parameter may not necessarily point to the same address as the
	  * equal data you are returning from this function, it's just data which the
	  * comparator says is equal. For instance, if you have a list of person structs:
	  *   (Andrew, 26), (Nick, 24), (Collin, 23), (Marie, 23)
	  * And you want to return any person whose age is 24, you could create a new
	  * person struct (<NULL>, 24) with a comparator that returns 0 if age == 24 and
	  * pass that into this function as "data" along with comparator. contains()
	  * would then return (Nick, 24) In other words, data will be something with
	  * value equality to what is in the list, but not necessarily something with
	  * reference equality. Value equality will be defined by comparator.
	  *
	  * If there are multiple pieces of data in the list which are equal to the
	  * "data" parameter, return any one of them.
	  *
	  * @param listToSearch a pointer to the list structure
	  * @param data The data, to see if it exists in the list
	  * @param comparator A function written by the user that will tell if two pieces
	  *                of data are equal. Returns 0 if equal, something else
	  *                otherwise. Imagine subtracting two numbers. Think of
	  *                a comparator's compare function from Java.
	  * @param dataOut A pointer to a pointer used to return the data contained in
	  *                the list or NULL on failure
	  * @return false if dataOut is NULL, list is NULL, comparator is NULL, or the
	  *               list does not contain data, else true
		
		QUESTION: What happens when you have a struct with a generic self reference as apposed to a pointer self reference? 
	  */
bool contains(list *listToSearch, void *data, list_eq comparator, void **dataOut)
{
	if(NULL == dataOut) {
		return false;
	}
	if(NULL == listToSearch) {
		*dataOut = NULL;
		return false;
	}
	if(NULL == comparator) {
		*dataOut = NULL;
		return false;
	}
	if(listToSearch->size == 0) {
		*dataOut = NULL;
		return false;
	}

	if(listToSearch->size == 1) {
		node* cursor = listToSearch->head;
		if (0 == comparator(data, (cursor->data))) {
			*dataOut = cursor->data;
			return true;
		}
		*dataOut = NULL;
		return false;
	}

	int size = listToSearch->size;
	node* cursor = listToSearch->head;
	for (int i = 0; i < size; i++) {
		if (0 == comparator(data, (cursor->data))) {
			*dataOut = cursor->data;
			return true;
		}
		cursor = cursor->next;
	}
	*dataOut = NULL;
    return false;
}


	/** pop_front
	  *
	  * Removes the node at the front of the list, and returns its data to the user
	  *
	  * @param listToPopFrom a pointer to the list.
	  * @param dataOut A pointer to a pointer used to return the data in the first
	  *                node or NULL on failure
	  * @return false if dataOut is NULL or the list is NULL or empty, else true
		
		QUESTION: IS THIS HOW YOU ACTUALLY USE FREE PROPERLY? 
	  */
bool pop_front(list *listToPopFrom, void **dataOut)
{
	if(NULL == dataOut) {
		return false;
	}
	if(NULL == listToPopFrom) {
		*dataOut = NULL;
		return false;
	}
	if(listToPopFrom->size == 0) {
		*dataOut = NULL;
		return false;
	}

	if(listToPopFrom->size == 1) {
		node* toRemove = listToPopFrom->head;
		listToPopFrom->head = NULL;
		listToPopFrom->tail = NULL;
		listToPopFrom->size = 0;
		
		*dataOut = toRemove->data;
		free(toRemove);
		return true;
	}

	node* toRemove = listToPopFrom->head;
	listToPopFrom->size = listToPopFrom->size - 1;
	listToPopFrom->head = (listToPopFrom->head)->next;
	
	*dataOut = toRemove->data;

	free(toRemove);
    return true;
}

	/** pop_back
	  *
	  * Removes the node at the back of the list, and returns its data to the user
	  *
	  * @param listToPopFrom a pointer to the list.
	  * @param dataOut A pointer to a pointer used to return the data in the last
	  *                node or NULL on failure
	  * @return false if dataOut is NULL or the list is NULL or empty, else true

		QUESTION: IS THIS HOW YOU ACTUALLY USE FREE PROPERLY? 
	  */
bool pop_back(list *listToPopFrom, void **dataOut)
{
    if(NULL == dataOut) {
		return false;
	}
	if(NULL == listToPopFrom) {
		*dataOut = NULL;
		return false;
	}
	if(listToPopFrom->size == 0) {
		*dataOut = NULL;
		return false;
	}

	if(listToPopFrom->size == 1) {
		node* toRemove = listToPopFrom->head;
		listToPopFrom->size = 0;
		listToPopFrom->head = NULL;
		listToPopFrom->tail = NULL;
		
		*dataOut = toRemove->data;
		free(toRemove);
		return true;
	}
	
	int size = listToPopFrom->size;

	node* current = listToPopFrom->head;
	for (int i = 0; i < size-2; i++) {
		current = current->next;
	}

	*dataOut = (current->next)->data;
	node* toRemove = current->next;
	current->next = NULL;
	listToPopFrom->tail = current;
	listToPopFrom->size = size - 1;
	free(toRemove);
    return true;
}

	/** list_remove
	  *
	  * Removes the data from the specified index in the list
	  *
	  * @param listToRemoveFrom a pointer to the list structure
	  * @param index 0-based, starting from the head.
	  * @param dataOut A pointer to a pointer used to return the data from the
	  *                specified index in the list or NULL on failure
	  * @return false if dataOut is NULL or list is NULL or index is out of range
	  *         of the list, else true
	  */
bool list_remove(list *listToRemoveFrom, int index, void **dataOut)
{
    if (NULL == dataOut) {
		return false;
	}
	if (NULL == listToRemoveFrom) {
		*dataOut = NULL;
		return false;
	}
	if (listToRemoveFrom->size <= index) {
		*dataOut = NULL;
		return false;
	}

	if (0 > index) {
		*dataOut = NULL;
		return false;
	}

	if (listToRemoveFrom->size == 1) {
	    return pop_back(listToRemoveFrom, dataOut);
	}

	if (listToRemoveFrom->size == (index + 1)) {
	    return pop_back(listToRemoveFrom, dataOut);
	}

	if (index == 0) {
		return pop_front(listToRemoveFrom, dataOut);
	}
	
	node* current = listToRemoveFrom->head;
	for (int i = 0; i < index - 1; i++) {
		current = current->next;
	}
	node* toRemove = current->next;
	current->next = (current->next)->next;
	listToRemoveFrom->size = listToRemoveFrom->size - 1;
	
	*dataOut = toRemove->data;
	free(toRemove);
	return true;
}


	/** empty_list
	  *
	  * Empties the list. After this is called, the list should be empty.
	  * This does not free the list struct itself, just all nodes and data within.
	  * Make sure to check that listToEmpty and freeFunc are not NULL before using
	  * them.
	  *
	  * @param listToEmpty a pointer to the list structure
	  * @param freeFunc function used to free the nodes' data.
	  */
void empty_list(list *listToEmpty, list_op freeFunc)
{
    if ((listToEmpty == NULL) || (freeFunc == NULL) || (listToEmpty->size == 0)) {
		return;
	}
	int size = listToEmpty->size;
	if (size == 1) {
		node* toRemove = listToEmpty->head;
		freeFunc(toRemove->data);
		free(toRemove);
		listToEmpty->head = NULL;
		listToEmpty->tail = NULL;
		listToEmpty->size = 0;
	} else {
		node* cursor = NULL;
		for (int i = 0; i < size; i++) {
			cursor = listToEmpty->head;
			listToEmpty->head = (listToEmpty->head)->next;
			freeFunc(cursor->data);				
			free(cursor);
		}
		listToEmpty->head = NULL;
		listToEmpty->tail = NULL;
		listToEmpty->size = 0;
	}	
}

	/** traverse
	  *
	  * Traverses the list, calling a function on each node's data.
	  *
	  * @param listToTraverse a pointer to the list structure
	  * @param doFunc a function that does something to each node's data.
	  * @return true if the traversal was a success, false otherwise
	  *         (listToTraverse1 or doFunc is null)
	  */
bool traverse(list *listToTraverse, list_op doFunc)
{
	if (listToTraverse == NULL) {
		return false;
	}
	if (doFunc == NULL) {
		return false;
	}	

	int size = listToTraverse->size;
	if (0==size) {
		return true;
	}
	
	node* cursor = listToTraverse->head;
	for (int i = 0; i < (size); i++) {
		doFunc((cursor->data));
		cursor = cursor->next;
	}
    return true;
}

	/** reverse
	  *
	  * Reverses the singly linked list in place.
	  *
	  * @param listToReverse to append to.
	  * @return true if the operation was a success, false otherwise (listToReverse
	  *         was NULL)
	  */
bool reverse(list *listToReverse)
{
	if (listToReverse == NULL) {
		return false; 	
	}
	if (listToReverse->size < 2) {
		return true;
	}

	int size = listToReverse->size;
	node* cursor = listToReverse->head;

	for (int i = 0; i<size-1; i++) {
		node* temp = cursor->next;
		cursor->next = (cursor->next)->next;
		temp->next = listToReverse->head;
		listToReverse->head = temp;
	}

	listToReverse->tail = cursor;
    return true;
}

	/** concat
	  *
	  * Concatinates firstList and secondList. This function should append
	  * secondList to the end of firstList.
	  *
	  * NOTE: this function will effectively empty lastList onto firstList which
	  *       means that lastList will be empty (head == tail == NULL and size == 0)
	  *       at the end of the function and firstList will contain all the elements
	  *       from both lists. Be sure to update the size, head, and tail pointers
	  *       accordingly.
	  *
	  * example: if firstList is a->b->c and second list is d->e->f
	  * after the function call, firstList will be a->b->c->d->e->f and
	  * secondList will be empty.
	  *
	  *
	  * @param firstList to append to.
	  * @param secondList the list to append to firstList
	  * @return true if the operation was a success, false otherwise (firstList or
	  *         secondList is null)
	  */
bool concat(list *firstList, list *lastList)
{
	if(NULL == firstList) {
		return false;
	}
	if(NULL == lastList) {
		return false; 
	}
	
	if(firstList->size == 0) {
		firstList->head = lastList->head;
		firstList->tail = lastList->tail;
		firstList->size = lastList->size;

		lastList->head = NULL;
		lastList->tail = NULL;
		lastList->size = 0;
		return true;
	}

	if(lastList->size == 0) {
		return true;
	}

	node* connect = lastList->head;
	
	(firstList->tail)->next = connect;
	firstList->tail = lastList->tail;
	firstList->size = firstList->size + lastList->size;

	lastList->size = 0;
	lastList->head = NULL;
	lastList->tail = NULL;

    return true;
}