#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "game.h"
#include "game_over.h"
#include "game_setup.h"
#include "mbstrings.h"
#include "render.h"
#include "common.h"

// Global variables for game board
extern size_t g_width;
extern size_t g_height;
extern int* g_cells;

// Global variables for snake
//extern int g_snake_dir;
//extern int g_snake_length;
//extern int g_snake_row;
//extern int g_snake_col;

// Global variables for game status
extern int g_game_over;
extern int g_score;
extern size_t g_name_len;
/** Gets the next input from the user, or returns INPUT_NONE if no input is
 * provided quickly enough.
 */
enum input_key get_input() {
    /* DO NOT MODIFY THIS FUNCTION */
    int input = getch();

    if (input == KEY_UP) {
        return INPUT_UP;
    } else if (input == KEY_DOWN) {
        return INPUT_DOWN;
    } else if (input == KEY_LEFT) {
        return INPUT_LEFT;
    } else if (input == KEY_RIGHT) {
        return INPUT_RIGHT;
    } else {
        // if the input isn't an arrow key, we treat it as no input (could add
        // other keys in if we want other commands, like 'pause' or 'quit')
        return INPUT_NONE;
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Helper function that procs the GAME OVER screen and final key prompt.
 * `snake_p` is not needed until Part 2!
 */
void end_game(int* cells, size_t width, size_t height, snake_t* snake_p) {
    // Game over!

    // Free any memory we've taken
    teardown(cells, snake_p);

    // ****************** UNCOMMENT THIS CODE IN PART 2B ***********************
    /*
    // Render final GAME OVER PRESS ANY KEY TO EXIT screen
    render_game_over(width, height);
    usleep(1000 * 1000);  // 1000ms
    cbreak(); // Leave halfdelay mode
    getch();
    */

    // tell ncurses that we're done
    endwin();
}


int main(int argc, char** argv) {
    // Main program function — this is what gets called when you run the
    // generated executable file from the command line!

    // Board data
    size_t width;   // the width of the board.
    size_t height;  // the height of the board.
    int* cells;     // a pointer to the first integer in an array of integers
                    // representing each board cell.
    char* board_rep; // pointer to the first character in the board string
    

    // snake data (only used in part 2!)
    snake_t snake;    // your snake struct. (not used until part 2!)
    int snake_grows;  // 0 if the snake does not grow on eating, 1 if it does
    enum board_init_status status;

    // initialize board from command line arguments
    switch (argc) {
        case (2):
            snake_grows = atoi(argv[1]);
            if (snake_grows != 1 && snake_grows != 0) {
                printf(
                    "snake_grows must be either 1 (grows) or 0 (does not "
                    "grow)\n");
                return 0;
            }
            status = initialize_game(&cells, &width, &height, &snake, NULL);
            break;
        case (3):
        // if snake grows or not handling
            snake_grows = atoi(argv[1]);
            board_rep = argv[2];
            if (snake_grows != 1 && snake_grows != 0) {
                printf(
                    "snake_grows must be either 1 (grows) or 0 (does not "
                    "grow)\n");
                return 0;
            // if board string is empty
            } else if (*argv[2] == '\0') {
                status = initialize_game(&cells, &width, &height, &snake, NULL);
                break;
            }
            // if board string is not empty
            status = initialize_game(&cells, &width, &height, &snake, argv[2]);
            if (status != INIT_SUCCESS) {
                return EXIT_FAILURE;
            }
            break;
        case (1):
        default:
            printf("usage: snake <GROWS: 0|1> [BOARD STRING]\n");
            return 0;
    }

    // ----------- DO NOT MODIFY ANYTHING IN `main` ABOVE THIS LINE -----------

    // Check validity of the board before rendering!
    // TODO: Implement (in Part 1C)
    // if ( ? board is not valid ? ) { return EXIT_FAILURE; }

    // Read in the player's name & save its name and length
    char name_buffer[1000];
    read_name(name_buffer);
    g_name_len = mbslen(name_buffer);
    // ? save name_buffer ?
    // ? save mbslen(name_buffer) ?

    initialize_window(width, height);
    
    // TODO: implement the game loop here (Part 1A)!
    while (g_game_over == 0) {
        // render the game
        update(g_cells, g_width, g_height, &snake, snake.direction, snake_grows);
        if (g_game_over == 1) {
            continue;
        }
        render_game(g_cells, g_width, g_height);
        // get the next input
        // wait 
        usleep(1000 * 750); // 100ms
        snake.direction = get_input();

    }

    end_game(cells, width, height, &snake);
}
    
    


