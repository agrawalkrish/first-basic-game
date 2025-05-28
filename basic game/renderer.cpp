//removed struct keyword (this is C++,  not C)
internal void
clear_screen(u32 color) {
    u32* pixel = (u32*)rander_state.memory;
    for (int y = 0; y < rander_state.height; y++) {
        for (int x = 0; x < rander_state.width; x++) {
            *pixel++ = color;
        }
    }
}

internal void
draw_rect_inpixel(int x0, int y0, int x1, int y1, unsigned  int color) {
    x0 = clamp(0, x0, rander_state.width);
    x1 = clamp(0, x1, rander_state.width);
    y0 = clamp(0, y0, rander_state.height);
    y1 = clamp(0, y1, rander_state.height);
    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*)rander_state.memory + x0 + y * rander_state.width;

        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

global_variable float renderscale = 0.01f;

internal void
draw_rect(float x, float y, float halfSize_x, float halfSize_y, u32 color) {
    x *= rander_state.height* renderscale;
    y *= rander_state.height * renderscale;
    halfSize_x *= rander_state.height * renderscale;
    halfSize_y *= rander_state.height * renderscale;

    x += rander_state.width / 2.f;
    y += rander_state.height / 2.f;


    // change to pixel and add to pixel
    int x0 = x - halfSize_x;
    int x1= x + halfSize_x;
    int y0 = y - halfSize_y;
    int y1 = y + halfSize_y;

    draw_rect_inpixel(x0, y0, x1, y1, color);
}