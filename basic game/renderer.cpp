void render_background() {
    unsigned int* pixel = (unsigned int*)rander_state.memory;
    for (int y = 0; y < rander_state.height; y++) {
        for (int x = 0; x < rander_state.width; x++) {
            *pixel++ = x * y;
        }
    }
}

void clear_screen(unsigned int color) {
    unsigned int* pixel = (unsigned int*)rander_state.memory;
    for (int y = 0; y < rander_state.height; y++) {
        for (int x = 0; x < rander_state.width; x++) {
            *pixel++ = color;
        }
    }
}

void draw_rect(int x0,int y0,int x1,int y1, unsigned  int color) {

}
