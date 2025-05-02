#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include "linked_list.h"
/**
 * Contains definitions for the input_key enum and snake struct, 
 * declarations for global variables defined in common.c, 
 * and function headers for common.c
 */


// Bitflags enable us to store cell data in integers!
#define FLAG_PLAIN_CELL 0b0001  // equals 1
#define FLAG_SNAKE 0b0010       // equals 2
#define FLAG_WALL 0b0100        // equals 4
#define FLAG_FOOD 0b1000        // equals 8

/**
 * Enumerated types, also known as "enums", are a way to create a set of named
 * constants! This enum represents the different possible inputs in our snake
 * game. The type corresponding to this enum is `enum input_key` and variables
 * of this type can take on the following values:
 * INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, and INPUT_NONE.
 */
enum input_key { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_NONE };



// Global variables for Snake
extern int g_snake_dir;  // Snake starts moving right
extern int g_snake_length;
extern int g_snake_row;
extern int g_snake_col;
extern int g_growing; // 0 if the snake does not grow on eating, 1 if it does



/** Global variables for game status.
 *
 * `g_` prefix used by convention to emphasize that these are global.
 *
 * You may need to add variables here in part 2 of the project!
 *
 * Variables:
 *  - g_game_over: 1 if game is over, 0 otherwise
 *  - g_score: current game score. Starts at 0. 1 point for every food eaten.
 */
extern int g_game_over;  // 1 if game is over, 0 otherwise
extern int g_score;      // game score: 1 point for every food eaten
extern size_t  g_name_len; // length of the player's name in characters
extern char *g_name; // player's name
extern size_t g_width; // amount of columns
extern size_t g_height; // amount of rows
extern int* g_cells; // array of cells in memory, pointer to first element

/** Snake struct. This struct is not needed until part 2!
 * Fields:
 *  - direction: the direction the snake is moving
 * - snake_length: the length of the snake's body
 * - row: the row of the snake's head
 * - col: the column of the snake's head
 */
typedef struct snake {
    int direction;  // Snake starts moving right
    int snake_length;  // Snake starts with length 1
    int row; // Snake starts at position (2,2) in default, or variable for board input
    int col; 

    node_t *snake_head; // list of the snake, a doubly linked list of snake cells
} snake_t;


/*
  // FOUR ELEMENT LIST
  char* check[] = {"hello", "world", "how", "cool"};
  node_t* four_list = malloc(sizeof(node_t));
  four_list->data = "hello";
  node_t* fl2 = malloc(sizeof(node_t));
  fl2->data = "world";
  node_t* fl3 = malloc(sizeof(node_t));
  fl3->data = "how";
  node_t* fl4 = malloc(sizeof(node_t));
  fl4->data = "cool";
  four_list->prev = NULL;
  four_list->next = fl2;
  fl2->prev = four_list;
  fl2->next = fl3;
  fl3->prev = fl2;
  fl3->next = fl4;
  fl4->prev = fl3;
  fl4->next = NULL;
  for (int i = 0; i < 4; i++) {
    assert(!strcmp(get(four_list, i), check[i]));
  }
  free(four_list);
  free(fl2);
  free(fl3);
  free(fl4);

*/


// Snake struct function headers
void initialize_snake(snake_t* snake_p, int row, int col);
void set_snake_indices(snake_t* snake_p, size_t next_position, int growing);


int get_snake_head_index(snake_t* snake_p);
void set_snake_row(snake_t* snake_p, int row);
void set_snake_col(snake_t* snake_p, int col);
int* get_new_indices(snake_t* snake_p);

// Function headers for seeding 
void set_seed(unsigned seed);
unsigned generate_index(unsigned size);

#endif
