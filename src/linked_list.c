#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * In this file, you will find the partial implementation of common doubly
 * linked list functions.
 *
 * Your first task is to debug some of the functions!
 *
 * After you have found all of the bugs, you will be writing three doubly
 * linked list functions and test them.
 *
 */

/**
 * find and return the length of the list
 *
 * given a pointer to the head of list
 */
int length_list(node_t* head_list) {
  int len = 0;  
  if (!head_list) {
    return len;
  }
  node_t* current = head_list;
  while (current) {
    // ERROR: current is never updated to the next node
    // current = current->next;
    current = current->next;
    len++;
  }
  return len;
}

/**
 * returns the value of the head of the list
 *
 * given pointer to the head of the list
 */
void* get_first(node_t* head_list) { 
  
  if (!head_list) {
    return NULL;
  }

  return head_list->data; }

/** returns the value of the last element of the list
 *
 * given a pointer to the head of the list
 */
void* get_last(node_t* head_list) {
  if (!head_list) {
    return NULL;
  }
  // start at head
  node_t* curr = head_list;
  // traverse until there's no valid next node
  while (curr->next) {
    curr = curr->next;
  }
  return curr->data;
}

/** TODO: implement this!
 * inserts element at the beginning of the list
 *
 * given a pointer to the head of the list, a void pointer representing the
 * value to be added, and the size of the data pointed to
 *
 * returns nothing
 */
void insert_first(node_t** head_list, void* to_add, size_t size) { 
  if (!to_add) {
    return;
  }

  node_t* new_element = (node_t*)malloc(sizeof(node_t));
  void* new_data = malloc(size);
  memcpy(new_data, to_add, size);
  new_element->data = new_data;

  // if head_list is NULL, then the list is empty
  if (!(*head_list)) {
    *head_list = new_element;
    new_element->prev = NULL;
    new_element->next = NULL;
    return;
  }
  else if (*head_list) {
    (*head_list)->prev = new_element;
    new_element->next = *head_list;
    new_element->prev = NULL;
    *head_list = new_element;
  }
  return; 
}

/**
 * inserts element at the end of the linked list
 *
 * given a pointer to the head of the list, a void pointer representing the
 * value to be added, and the size of the data pointed to
 *
 * returns nothing
 */
void insert_last(node_t** head_list, void* to_add, size_t size) {
  if (!to_add) {
    return;
  }
  node_t* new_element = (node_t*)malloc(sizeof(node_t));
  void* new_data = malloc(size);
  memcpy(new_data, to_add, size);
  new_element->data = new_data;

  if (!(*head_list)) {  // means the list is empty
    *head_list = new_element;
    new_element->prev = NULL;
    new_element->next = NULL;
    return;
  }
  
  // traverse to the end of the list
  node_t* curr = *head_list;
  while (curr->next) {
    curr = curr->next;
  }

  curr->next = new_element;
  new_element->prev = curr;
  new_element->next = NULL;
}

/** TODO: implement this!
 * gets the element from the linked list
 *
 * given a pointer to the head of the list and an index into the linked list
 * you need to check to see if the index is out of bounds (negative or longer
 * than linked list)
 *
 * returns the string associated with an index into the linked list
 */
void* get(node_t* head_list, int index) { 
if (index < 0 || !head_list || length_list(head_list) <= index ) {
    return NULL;
  }
  node_t* curr = head_list;
  while (index > 0) {
    curr = curr->next;
    index--;
  }
  if (!curr->data){
    return NULL;
  }
  return curr->data;
} 

/**
 * removes element from linked list
 *
 * given a pointer to the head of list, a void pointer of the node to remove
 * you need to account for if the void pointer doesn't exist in the linked list
 *
 * returns 1 on success and 0 on failure of removing an element from the linked
 * list
 */
int remove_element(node_t **head_list, void *to_remove, size_t size) {
  if (!(*head_list)) {
    return 0;  // element doesn't exist
  }

  node_t* curr = *head_list;

  while (curr) {
    if (!memcmp(curr->data, to_remove, size)) {  // found the element to remove
      if (curr->next) {
        curr->next->prev = curr->prev;
      }
      if (curr == *head_list) {
        *head_list = curr->next;
      } else {
        curr->prev->next = curr->next;
      }
      free(curr->data);
      free(curr);
      return 1;
    }
    curr = curr->next;
  }

  return 0;
}

/**
 * reverses the list given a double pointer to the first element
 * 
 * returns nothing
 */
void reverse_helper(node_t** head_list) {
  node_t* curr = *head_list;
  node_t* placeholder = NULL;
  node_t* next_node = NULL;
  while (curr) {
    if (curr->next == NULL) {
      curr->next = placeholder;
      curr->prev = NULL;
      *head_list = curr;
      return;
      }
      next_node = curr->next;
      curr->next = placeholder;
      curr->prev = next_node;
      placeholder = curr;
      curr = next_node;
    }
  }

/**
 * calls a helper function that reverses the linked list
 *
 * given a pointer to the first element
 *
 * returns nothing
 */
void reverse(node_t** head_list) {
  if (head_list) {
    reverse_helper(head_list);
  }
}

/**
 * removes the first element of the linked list if it exists
 *
 * given a pointer to the head of the linked list
 *
 * returns the void pointer of the element removed
 *
 */
void* remove_first(node_t** head_list) {
  if (!(*head_list)) {
    return NULL;
  }
  node_t* curr = *head_list;
  *head_list = (*head_list)->next;

  void* data = curr->data;
  if (*head_list) { 
    (*head_list)->prev = NULL;
  }

//  free(curr->data);
// dont free the curr->data value as that will be freed by the test case
  free(curr);

  return data;
}

/** TODO: implement this!
 * removes the last element of the linked list if it exists
 *
 * given a pointer to the head of the linked list
 *
 * returns the void pointer of the element removed
 *
 */
void* remove_last(node_t** head_list) { 
  if (!(*head_list)) {
    return NULL;
  }
  node_t* curr = *head_list;
  while (curr->next) {
    curr = curr->next;
  }
  // if current has a previous node, it's not the head of the list, so remove the next of its previous, then make the current node's previous node null
  if (curr->prev) {
    curr->prev->next = NULL;
    curr->prev = NULL;
  }
  // if curr is the head of the list, make the head of the list null
  else if( curr == *head_list) {
    *head_list = NULL;
  }
  // return the void pointer of the data of the node that was removed, then free the node itself
  void* data = curr->data;
  free(curr);

  return data; 
  }
