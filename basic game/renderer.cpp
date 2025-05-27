#pragma once
#include <windows.h>

struct Render_State {
    int width, height;
    void* memory;
    BITMAPINFO bit_map_info;
};


void render_background(Render_State& rander_state)
{
    unsigned int* pixel = (unsigned int*)rander_state.memory;
    for (int y = 0; y < rander_state.height; y++) {
        for (int x = 0; x < rander_state.width; x++) {
            *pixel++ = x * y;
        }
    }
}

//removed struct keyword (this is C++,  not C)
void clear_screen(Render_State& rander_state, unsigned int color) {
    unsigned int* pixel = (unsigned int*)rander_state.memory;
    for (int y = 0; y < rander_state.height; y++) {
        for (int x = 0; x < rander_state.width; x++) {
            *pixel++ = color;
        }
    }
}

void draw_rect(struct Render_State& rander_state, int x0, int y0, int x1, int y1, unsigned  int color) {
    
    for (int y = y0; y < y1; y++) {
        unsigned int* pixel = (unsigned int*)rander_state.memory + x0 + y * rander_state.width;

        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}