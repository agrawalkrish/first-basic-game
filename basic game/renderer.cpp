// Function to clear the screen by filling it with a uniform color
internal void
clear_screen(u32 color)
{
    u32 *pixel = (u32 *)rander_state.memory;
    // Iterate through each pixel in the buffer and set it to the given color
    for (int y = 0; y < rander_state.height; y++)
    {
        for (int x = 0; x < rander_state.width; x++)
        {
            *pixel++ = color;
        }
    }
}

// Function to draw a rectangle directly using pixel coordinates
internal void
draw_rect_inpixel(int x0, int y0, int x1, int y1, unsigned int color)
{
    // Ensure the rectangle coordinates are within valid screen bounds
    x0 = clamp(0, x0, rander_state.width);
    x1 = clamp(0, x1, rander_state.width);
    y0 = clamp(0, y0, rander_state.height);
    y1 = clamp(0, y1, rander_state.height);

    // Iterate over the pixels within the given rectangle bounds and color them
    for (int y = y0; y < y1; y++)
    {
        u32 *pixel = (u32 *)rander_state.memory + x0 + y * rander_state.width;
        for (int x = x0; x < x1; x++)
        {
            *pixel++ = color;
        }
    }
}

// Global variable for render scaling, used for transformations
global_variable float renderscale = 0.01f;

// Function to draw a rectangle using float-based coordinate system
internal void
draw_rect(float x, float y, float halfSize_x, float halfSize_y, u32 color)
{
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

const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

internal void
draw_text(const char* text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}


internal void draw_number(int number, float x, float y, float size, u32 color) {
    float half_size = size * .5f;
    float full_height = size * 2.5f;
    float half_height = size * 1.25f;
    float digit_width = 4 * size;

    do {
        int digit = number % 10;
        number /= 10;

        switch (digit) {
        case 0: {
            draw_rect(x - size, y, half_size, full_height, color);
            draw_rect(x + size, y, half_size, full_height, color);
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
        } break;

        case 1: {
            draw_rect(x + size, y, half_size, full_height, color);
        } break;

        case 2: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
            draw_rect(x + size, y + half_height, half_size, half_height, color);
            draw_rect(x - size, y - half_height, half_size, half_height, color);
        } break;

        case 3: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
            draw_rect(x + size, y, half_size, full_height, color);
        } break;

        case 4: {
            draw_rect(x + size, y, half_size, full_height, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x - size, y + half_height, half_size, half_height, color);
        } break;

        case 5: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
            draw_rect(x - size, y + half_height, half_size, half_height, color);
            draw_rect(x + size, y - half_height, half_size, half_height, color);
        } break;

        case 6: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
            draw_rect(x - size, y, half_size, full_height, color);
            draw_rect(x + size, y - half_height, half_size, half_height, color);
        } break;

        case 7: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x + size, y, half_size, full_height, color);
        } break;

        case 8: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
            draw_rect(x - size, y, half_size, full_height, color);
            draw_rect(x + size, y, half_size, full_height, color);
        } break;

        case 9: {
            draw_rect(x, y + full_height, size, half_size, color);
            draw_rect(x, y, size, half_size, color);
            draw_rect(x, y - full_height, size, half_size, color);
            draw_rect(x + size, y, half_size, full_height, color);
            draw_rect(x - size, y + half_height, half_size, half_height, color);
        } break;
        }

        x -= digit_width;

    } while (number);
}
