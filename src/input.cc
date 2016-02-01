#include "input.h"

static Uint8 keyboard[323];
static Uint8 keyboard_prev[323];

void input_update(void)
{
        memcpy(keyboard_prev, keyboard, 323);
        memcpy(keyboard, SDL_GetKeyboardState(NULL), 323);
}

int key_pressed(Uint8 key)
{
        return keyboard[key] == SDL_PRESSED;
}

int key_release(Uint8 key)
{
        return keyboard[key] == SDL_RELEASED;
}

int key_just_release(Uint8 key)
{
        return keyboard_prev[key] == SDL_PRESSED && keyboard[key] == SDL_RELEASED;
}

int key_just_pressed(Uint8 key)
{
        return keyboard_prev[key] == SDL_RELEASED && keyboard[key] == SDL_PRESSED;
}
