// Function to clear the screen by filling it with a uniform color  
internal void
clear_screen(u32 color) {
    u32* pixel = (u32*)rander_state.memory;
    // Iterate through each pixel in the buffer and set it to the given color  
    for (int y = 0; y < rander_state.height; y++) {
        for (int x = 0; x < rander_state.width; x++) {
            *pixel++ = color;
        }
    }
}

// Function to draw a rectangle directly using pixel coordinates  
internal void
draw_rect_inpixel(int x0, int y0, int x1, int y1, unsigned int color) {
    // Ensure the rectangle coordinates are within valid screen bounds  
    x0 = clamp(0, x0, rander_state.width);
    x1 = clamp(0, x1, rander_state.width);
    y0 = clamp(0, y0, rander_state.height);
    y1 = clamp(0, y1, rander_state.height);

    // Iterate over the pixels within the given rectangle bounds and color them  
    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*)rander_state.memory + x0 + y * rander_state.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

// Global variable for render scaling, used for transformations  
global_variable float renderscale = 0.01f;

// Function to draw a rectangle using float-based coordinate system  
internal void
draw_rect(float x, float y, float halfSize_x, float halfSize_y, u32 color) {
    // Convert from world space coordinates to pixel space  
    x *= rander_state.height * renderscale;
    y *= rander_state.height * renderscale;
    halfSize_x *= rander_state.height * renderscale;
    halfSize_y *= rander_state.height * renderscale;

    // Adjust coordinates relative to the screen center  
    x += rander_state.width / 2.f;
    y += rander_state.height / 2.f;

    // Convert float-based coordinates into pixel-based bounds  
    int x0 = x - halfSize_x;
    int x1 = x + halfSize_x;
    int y0 = y - halfSize_y;
    int y1 = y + halfSize_y;

    // Draw the rectangle using pixel coordinates  
    draw_rect_inpixel(x0, y0, x1, y1, color);
}