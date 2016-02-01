#ifndef FLATCLOTH_INPUT_H
#define FLATCLOTH_INPUT_H

#include <SDL2/SDL.h>

void input_update(void);
int key_pressed(Uint8 key);
int key_release(Uint8 key);
int key_just_release(Uint8 key);
int key_just_pressed(Uint8 key);

#endif // FLATCLOTH_INPUT_H
