#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include "common.h"

/**
 * Contains function headers for game.c
 */

void read_name(char* write_into);
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing);
void place_food(int* cells, size_t width, size_t height);
void teardown(int* cells, snake_t* snake_p);
size_t transform_snake_index(snake_t* snake_p, size_t current_index, enum input_key direction);
#endif
