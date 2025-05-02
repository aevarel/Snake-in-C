# Snake Game in C

This project is a simple implementation of the classic Snake game written in C. The game features a dynamic board, a snake that grows as it eats food, and the ability to navigate with keyboard input, along with run-length encoding to implement custom level designs. The game is rendered in the terminal using the `ncurses` library, and the game logic, including movement and collision detection, is implemented with a linked list for the snake's body.

## Features

- Classic Snake gameplay with snake movement and growth
- Board rendering using the `ncurses` library
- Keyboard controls for snake direction
- Game-over condition when the snake hits a wall or itself
- Clean memory management with proper initialization and cleanup

## Development

This project is built upon a stencil code provided as part of an assignment in Shaun Wallace's OS & Networks course. The initial structure for the game state, basic game loop, and memory management were adapted from the provided stencil, and the functionality was extended to implement the complete Snake game with linked list-based snake segments, dynamic rendering, and ncurses-based visualization.

## Requirements

- GCC (GNU Compiler Collection)
- `ncurses` library (install using `sudo apt-get install libncurses5-dev` on Linux or `brew install ncurses` on macOS)

## Building the Project

To compile and build the project, run:

```
make
```
This will create an executable file named `snake`, which can then be run.

To clean up the build files, run:

```
make clean
```

## Running the Game

To start the game, run the following command in your terminal:

```
snake <GROWS: 0|1> "[BOARD STRING]"
```
- `GROWS` is a flag that indicates whether the snake should grow when it eats food (1 for yes, 0 for no).
- `"[BOARD STRING]"` is a string that represents the initial state of the board using run-length encoding.

## Custom Board Designs

You can create custom board designs using run-length encoding. The format is as follows:

- Boards are specified with a header: B<rows>x<cols>
- Rows are separated using the | character

- Cell types include:
    - W: Wall
    - E: Empty
    - S1: Snake (must start with exactly one snake cell)

- Example (7x10 board with walls and a single snake):
```B7x10|W10|W1E4W5|W2E7W1|W1E8W1|W1E4W1E3W1|W1E2S1E1W1E3W1|W10```

### Error Handling

The program validates custom board strings and reports the following errors:
- `INIT_ERR_INCORRECT_DIMENSIONS`: Actual cell count does not match specified dimensions
- `INIT_ERR_WRONG_SNAKE_NUM`: Not exactly one snake cell found
- `INIT_ERR_BAD_CHAR`: Unexpected/invalid characters in the string

This feature enables quick sharing and loading of custom levels while keeping the input format compact and readable.

## File Structure
- src/snake.c: Entry point and main game loop
- src/game.c: Game logic including snake movement and collision detection
- src/game_setup.c: Initializes the game board and sets up the initial game state
- src/render.c: Handles rendering the game board and snake using the ncurses library
- src/common.c: Defines common functions and structures
- src/linked_list.c: Implements the linked list functionality for managing the snake's body
- src/mbstrings.c: Handles multibyte string operations (if applicable)
