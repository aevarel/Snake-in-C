#ifndef GAME_SETUP_H
#define GAME_SETUP_H

#include "common.h"
#include "game.h"

/**
 * Contains the definition for the board_init_status enum and function headers for game_setup.c
 */


/** Enum to communicate board initialization status.
 * Values include INIT_SUCCESS, INIT_ERR_INCORRECT_DIMENSIONS,
 * INIT_ERR_WRONG_SNAKE_NUM, and INIT_ERR_BAD_CHAR.
 */
enum board_init_status {
    INIT_SUCCESS,                   // no errors were thrown
    INIT_ERR_INCORRECT_DIMENSIONS,  // dimensions description was not formatted
                                    // correctly, or too many rows/columns are
                                    // specified anywhere in the string for the
                                    // given dimensions
    INIT_ERR_WRONG_SNAKE_NUM,  // no snake or multiple snakes are on the board
    INIT_ERR_BAD_CHAR,  // any other part of the compressed string was formatted
                        // incorrectly
    INIT_UNIMPLEMENTED,  // only used in stencil, no need to handle this
    INIT_ERR_MEM_ALLOC  // memory allocation failed
};

enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep);

enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed);
/**
 * Initializes the cells array to a default board with 10 rows and 20 columns. 
 * The snake is initialized at the 3rd row down and 3rd column from the left within the cells array. 
 * You do not need to modify this function. */                                            
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p);

#endif
