// Function to handle game simulation and rendering based on player input  
internal void
simulate_game(Input& input) {

    // Clears the screen with a solid background color (hex: 0xff5500, a shade of orange)  
    clear_screen(0xff5500);

    // Check if the "Up" button is pressed  
    if (input.buttons[BUTTON_UP].is_down)
        // Draws a small rectangle at the center when the "Up" button is pressed  
        draw_rect(0, 0, 1, 1, 0xaa7348);

    // Draws additional rectangles regardless of input  
    draw_rect(30, 30, 5, 5, 0xaac348); // A rectangle at (30,30) with size (5,5)  
    draw_rect(-20, 20, 8, 3, 0xcc7348); // A rectangle at (-20,20) with size (8,3)  
}