#include "game.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"
#include "common.h"

// Global variables for update loop

int g_last_input = INPUT_RIGHT; // last input, initialized to INPUT_RIGHT
// Global variables for game status

bool g_food_placed = false; // true if food is placed on the board 
extern int g_game_over; // game is not over
extern int g_score; // score is 0
extern int g_growing;
extern size_t g_name_len; // length of the player's name in characters
/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells_p: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(int* cells_p, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

   /**
     * Food Placement 
     */

    if (g_game_over == 1) {
        return;
    }
    // if there is no food on the board, place food 
    if (!g_food_placed) {
        
        place_food(cells_p, width, height);
        g_food_placed = true;
    }
    
    /**
     * Snake Movement
     */
    // index cells to the current position of the snake with g_snake_row and g_snake_col
    int head_index = get_snake_head_index(snake_p);
    int* tail_index = get_last(snake_p->snake_head);
    // conditionally check the direction to calculate the new position
    size_t next_position = transform_snake_index(snake_p, head_index, input);
    // if the snake hits a wall (or itself NYI), set g_game_over to 1
    if (cells_p[next_position] == FLAG_WALL || (cells_p[next_position] == FLAG_SNAKE && (*(tail_index + 0) * g_width + *(tail_index + 1)) != next_position)) {
    
    // if game is over, return early        
        g_game_over = 1;
        return;
    }
    
    
    // if the snake hits food, increment the score add a new snake cell to the tail (NYI)
    if (cells_p[next_position] == FLAG_FOOD) {
        g_score++;
        // if the snake eats food, place a new food on the board
        place_food(cells_p, width, height);
    }

    /// Cell Movement Handling 
    //
    // update the cell at the new position to FLAG_SNAKE
    // update the snake's position to the new position
    if (growing == 1 && cells_p[next_position] == FLAG_FOOD) {
        // if the snake is growing, don't free the last cell
        int* new_indices = get_new_indices(snake_p);
        insert_first(&snake_p->snake_head, new_indices, sizeof(int) * 2);
        free(new_indices);
        cells_p[next_position] = FLAG_SNAKE;

    }
    else{
        // if the snake isn't growing, free the last cell
        int* new_indices = get_new_indices(snake_p);
        insert_first(&snake_p->snake_head, new_indices, sizeof(int) * 2);
        int* old_data = remove_last(&snake_p->snake_head);
        cells_p[*(old_data + 0) * g_width + *(old_data + 1)] = FLAG_PLAIN_CELL;
        cells_p[*(new_indices + 0) * g_width + *(new_indices + 1)] = FLAG_SNAKE;

        free(old_data);
        free(new_indices);
    }
    
}


/** Gets the new index for the head of the snake based on its current direction.
 * Arguments:
 * - current_index: the current index of the snake's head
 * - direction: the direction the snake is moving
 */
size_t transform_snake_index(snake_t* snake_p, size_t current_index, enum input_key direction) {
    if (direction == INPUT_NONE) {
        direction = g_last_input; // Use the last input direction if no new input
        snake_p->direction = direction;
    }
    
    
    // Ignore the input if it is the opposite of the current direction
    if (((direction == INPUT_UP && g_last_input == INPUT_DOWN) ||
        (direction == INPUT_DOWN && g_last_input == INPUT_UP) ||
        (direction == INPUT_LEFT && g_last_input == INPUT_RIGHT) ||
        (direction == INPUT_RIGHT && g_last_input == INPUT_LEFT)) && length_list(snake_p->snake_head) > 1) {

        direction = g_last_input; // Ignore the reversing input  
    }
    
    // update the last input direction
    g_last_input = direction;
    snake_p->direction = direction;
        
    size_t next_position;
    switch (direction) {
        case INPUT_UP:
        
            next_position = (size_t)(current_index - g_width);
            snake_p->row--;
            break;
        case INPUT_DOWN:
            next_position = (size_t)(current_index + g_width);
            snake_p->row++;  
            break;
        case INPUT_LEFT:
            next_position = (size_t)(current_index - 1);
            snake_p->col--;
            break;
        case INPUT_RIGHT:
            next_position = (size_t)(current_index + 1);
            snake_p->col++;
            break;
        default:
            next_position = transform_snake_index(snake_p,current_index, g_last_input); // if no input, keep moving in the last direction
            break;
    }
        
    return next_position;
}
    


/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells_p: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells_p, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells_p + food_index) == FLAG_PLAIN_CELL) {
        *(cells_p + food_index) = FLAG_FOOD;
    } else {
        place_food(cells_p, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    // Prompt the user for their name
    // if the user presses enter without typing a name, prompt again
    while (1) {
        printf("Name > ");
        fflush(stdout);
        fgets(write_into, 1000, stdin);
        // remove the newline character from the end of the string
        write_into[strcspn(write_into, "\n")] = 0;
        if (strlen(write_into) > 0) {
            break; // if the user types a name, break out of the loop
        }
    }

    // save the length of the name
    g_name_len = strlen(write_into);
    // save the name dynamically
    g_name = malloc(g_name_len + 1);
    strncpy(g_name, write_into, g_name_len);

}





/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells_p: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells_p, snake_t* snake_p) {
    // Free the cells array
    free(cells_p);


    // Free the snake linked list
    if (snake_p && snake_p->snake_head) {
        node_t* current = snake_p->snake_head;
        while (current) {
            node_t* next = current->next;
            if (current->data) {
                free(current->data);
            }
            free(current);
            current = next;
        }
        snake_p->snake_head = NULL;
    }
}    