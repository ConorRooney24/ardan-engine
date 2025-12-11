// input.h
//
// handles everything to do with user input

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#define INPUT_KEY_COUNT 256

void input_init();
void input_restore();

void input_update();

bool input_key_pressed(int key); // true if a key was pressed since last call

#endif