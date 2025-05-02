#include "game_setup.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern int g_snake_dir;
extern int g_snake_length;
extern int g_snake_row;
extern int g_snake_col;

extern int g_game_over;
extern int g_score;
extern bool g_food_placed;

// Pointers to the values of width and height
extern size_t g_width;
extern size_t g_height;
// Pointer to the first element in the cells array
extern int* g_cells;


// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p, size_t* height_p) {
    // Initializes pointers to width and height values
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    g_cells = *cells_p;

    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p, size_t* height_p, snake_t* snake_p, char* board_rep) {
    enum board_init_status status;

    // If board_rep is NULL, initialize the default board
    if (board_rep == NULL) {
        status = initialize_default_board(cells_p, width_p, height_p);
        // Initialize snake struct
        initialize_snake(snake_p, 2, 2);

    }
    // If board_rep is not NULL, decompress the board_rep string
    else {
        status = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
        if (*cells_p == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for board\n");
        return INIT_UNIMPLEMENTED; // or an appropriate error
            }

        // Check the status of the decompression
        switch(status) {
            case INIT_ERR_INCORRECT_DIMENSIONS:
                printf("Error: Incorrect dimensions\n");
                break;
            case INIT_ERR_WRONG_SNAKE_NUM:
                printf("Error: Wrong snake number\n");
                break;
            case INIT_ERR_BAD_CHAR:
                printf("Error: Bad character\n");
                break;
            case INIT_UNIMPLEMENTED:
                printf("Error: Unimplemented\n");
                break;
            case INIT_SUCCESS:
                break;
            default:
                break;
        }
    }
    // Initialize global variables
    g_width = *width_p; // pointer to the width variable we'd like to initialize.
    g_height = *height_p; // pointer to the height variable we'd like to initialize.
    g_cells = *cells_p; // pointer to the first element in the cells array

    g_game_over = 0; // game is not over
    g_score = 0; // score is 0
    
    place_food(g_cells, *width_p, *height_p);
    g_food_placed = true;


    

return status;

}

// Helper function to extract the dimensions from the compressed string, using a pointer to an index, and the pointer to the compressed string
int extract_rle_dims(int *index, char* compressed){
    int rle_dim = 0;

    while (compressed[*index] != '|' && compressed[*index] != 'x') {
        if (compressed[*index] >= DIGIT_START && compressed[*index] <= DIGIT_END) {
            // shift the current value to the left by multiplying by 10 and adding the new digit
            rle_dim = rle_dim * 10 + (compressed[*index] - DIGIT_START);
        } 
        else {
            fprintf(stderr, "Unexpected character in compressed string: %c\n", compressed[*index]);
            return INIT_ERR_BAD_CHAR;
        }
        (*index)++;
    }
    (*index)++; // move to the next character

    return rle_dim;
}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */
enum board_init_status decompress_board_str(int** cells_p, size_t* width_p, size_t* height_p, snake_t* snake_p, char* compressed) {

    // Initialize struct to keep track of the current character and the number of times it should be repeated
    struct RLE_char {
        char character;
        int count;
    } current_char = { ' ', 0 };

    // initialize dummy snake because  of dumb memory interactions
    // Lets us know if snake has been initialized at all
    int g_snake_initialized = 0; // if 0 or greater than 1, return INIT_ERR_WRONG_SNAKE_NUM
    // Delimiter counter to keep track of the number of '|' characters
    int delimiter_counter = 0;
    // initialize snake struct
    snake_p->snake_head = NULL;
    // Ensure first character is a B 
    if (compressed[0] != 'B') {
        fprintf(stderr, "Unexpected character in compressed string: %c\n", compressed[0]);
        return INIT_ERR_BAD_CHAR;
    }
    
    // Extract the dimensions from the compressed string 
    int compressed_index = 1;
    *height_p = extract_rle_dims(&compressed_index, compressed);
    *width_p = extract_rle_dims(&compressed_index, compressed);
    // Delimiter should finally be a '|', so increment the delimiter counter
    delimiter_counter++;

    // Check if the dimensions are valid (non-zero)
    if (*height_p == 0 || *width_p == 0) {
        fprintf(stderr, "Row or Column in first RLE section amounts to zero.\n");
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // Initialize the global width and height variables
    g_width = *width_p;
    g_height = *height_p;
    
    // Initialize the cells array using the compressed string
    int* cells = malloc(*width_p * *height_p * sizeof(int));
    if (!cells) {
        fprintf(stderr, "Memory allocation failed.\n");
        return INIT_ERR_MEM_ALLOC;
    }
    *cells_p = cells;
    g_cells = *cells_p;
    // Initialize the cells array to FLAG_PLAIN_CELL to avoid uninitialized values later
    for (size_t i = 0; i < *width_p * *height_p; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }     

    // Counter for the number of columns in the current row, will be checked to ensure it matches the width, as specified by each delimited row
    int current_column_count = 0;
    // Pointer to keep track of current cell_p memory slot as we generate the grid in-line 
    int* current_cell = cells;
    // Initialize the global snake variables
    int current_row = 0;

    // Loop through the compressed string to initialize the cells array
    while (compressed[compressed_index] != '\0') {
        // '|' Case: if the current character is a '|', increment the delimiter counter
        if (compressed[compressed_index] == '|') {
            delimiter_counter++;

            // If the delimiter is greater than the amount of rows, return an error
            if ((size_t)delimiter_counter > *height_p) {
                fprintf(stderr, "More rows than specified in the compressed string.\n");
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            compressed_index++; // move to the next character in compressed message
            current_column_count = 0;
            current_row++;
            continue;
        }
        // 'W', 'E', or 'S' Case: create a RLE_char character to the current character and amount specified
        if (compressed[compressed_index] == 'S' || compressed[compressed_index] == 'E' || compressed[compressed_index] == 'W') {
            current_char.character = compressed[compressed_index];
            compressed_index++; // move to the next character
            current_char.count = 0;
            while (compressed[compressed_index] >= DIGIT_START && compressed[compressed_index] <= DIGIT_END){
                // shift the current value to the left by multiplying by 10 and adding the new digit
                current_char.count = current_char.count * 10 + (compressed[compressed_index] - DIGIT_START);
                compressed_index++; // move to the next character
                continue;
            }
            // For each `current_char.character`, assign to `current_char.count` amount of cell grids  
            for(int i = 0; i < current_char.count; i++){
                // If there's more columns than specified 
                if ((size_t)current_column_count > *width_p) {
                    fprintf(stderr, "More columns than specified in the compressed string.\n");
                    return INIT_ERR_INCORRECT_DIMENSIONS;
                }
                switch(current_char.character){
                    case 'W':
                    *current_cell = FLAG_WALL;
                    break;
                    case 'E':
                    *current_cell = FLAG_PLAIN_CELL;
                    break;
                    case 'S': 
                    *current_cell = FLAG_SNAKE;
                    
                    g_snake_initialized++;
                    if (g_snake_initialized > 1) {
                        fprintf(stderr, "More than one snake initialized.\n");
                        return INIT_ERR_WRONG_SNAKE_NUM;
                    }
                    // Initialize the snake struct
                    initialize_snake(snake_p, current_row, current_column_count);
                    break;
                }
                current_cell++; // Increments the pointer to the next cell
                current_column_count++;
            }
            // if the next character to decompress is a new row delimiter or the end of line and we're short of the designated width, throw an error
            if ((compressed[compressed_index] == '|' || compressed[compressed_index] == '\0') && (size_t)current_column_count < *width_p) {
                fprintf(stderr, "Not enough columns in the compressed string.\n");
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            continue;
        }
        // If the current character is not a '|', 'W', 'E', or 'S', return an error
        fprintf(stderr, "Unexpected character in compressed string.\n");
        return INIT_ERR_BAD_CHAR;
    }

    // Check if the number of rows matches the specified height
    if ((size_t)delimiter_counter != *height_p) {
        fprintf(stderr, "Number of rows does not match the specified height.\n");
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    // Check if the snake has been initialized exactly once
    if (g_snake_initialized != 1) {
        fprintf(stderr, "Incorrect number of snakes initialized.\n");
        return INIT_ERR_WRONG_SNAKE_NUM;
    }

    return INIT_SUCCESS;
}

