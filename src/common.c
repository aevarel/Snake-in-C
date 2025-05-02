#include "common.h"

#include <stdlib.h>
#include <stdio.h>
// TODO: define global variables needed for your snake! (part 1A)

// Definition of global variables for snake.
//int g_snake_dir = INPUT_RIGHT;  // Snake starts moving right
//int g_snake_length = 1;  // Snake starts with length 1
//int g_snake_row;    // Snake starts at position (2,2) in default, or variable for board input
//int g_snake_col;

// Definition of global variables for game status.
int g_game_over = 0;    // Game is not over at the start
int g_score = 0;        // Starting score

// Definition of global variables for game board.
size_t g_width;
size_t g_height;
int* g_cells;

size_t g_name_len;
char *g_name;


extern int g_growing;


void initialize_snake(snake_t* snake_p, int row, int col) {
    // Initialize the snake struct
    snake_p->direction = INPUT_RIGHT;
    snake_p->snake_length = 1;
    snake_p->row = row;
    snake_p->col = col;

    // Allocate memory for the snake head node
    node_t* snake_head = malloc(sizeof(node_t));
    if (!snake_head) {
        fprintf(stderr, "Memory allocation failed for snake head\n");
        exit(1);
    }

    // Allocate memory for the head data
    snake_head->data = malloc(sizeof(int) * 2);
    if (!snake_head->data) {
        fprintf(stderr, "Memory allocation failed for head data\n");
        free(snake_head);
        exit(1);
    }

    // Initialize the head data
    int* head_data = (int*)snake_head->data;
    head_data[0] = row;
    head_data[1] = col;

    // Set the snake head node in the snake struct
    snake_p->snake_head = snake_head;
    snake_p->snake_head->next = NULL;
    snake_p->snake_head->prev = NULL;
}

int* get_new_indices(snake_t* snake_p){
    int* new_indices = malloc(sizeof(int) * 2);
    new_indices[0] = snake_p->row;
    new_indices[1] = snake_p->col;
    return new_indices;
}


int get_snake_head_index(snake_t* snake_p) {
    int* data = (int*)snake_p->snake_head->data;
    // Get the index of the snake's head in the cells array
    return *(data + 0) * g_width + *(data + 1);
}


/** Sets the seed for random number generation.
 * Arguments:
 *  - `seed`: the seed.
 */
void set_seed(unsigned seed) {
    /* DO NOT MODIFY THIS FUNCTION */
    srand(seed);
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Returns a random index in [0, size)
 * Arguments:
 *  - `size`: the upper bound for the generated value (exclusive).
 */
unsigned generate_index(unsigned size) {
    /* DO NOT MODIFY THIS FUNCTION */
    return rand() % size;
    /* DO NOT MODIFY THIS FUNCTION */
}
