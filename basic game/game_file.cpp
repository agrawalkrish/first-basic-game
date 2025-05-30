#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
// Function to handle game simulation and rendering based on player input  


float player_pos_x = 0.f;
float player_pos_y = 0.f;

internal void
simulate_game(Input* input, float dt) {

    // Clears the screen with a solid background color (hex: 0xff5500, a shade of orange)  
    clear_screen(0xff5500);

    draw_rect(0, 0, 85, 45, 0xffaa33);
    float speed = 50.f;
    // Check if the "Up" button is pressed  
    if (is_down(BUTTON_UP)) player_pos_y += speed * dt;
    if (is_down(BUTTON_DOWN)) player_pos_y -= speed * dt;
    if (is_down(BUTTON_RIGHT)) player_pos_x += speed * dt;
    if (is_down(BUTTON_LEFT)) player_pos_x -= speed * dt;
        // Draws a small rectangle at the center when the "Up" button is pressed  
    //draw_rect(player_pos_x, player_pos_y, 1, 1, 0xaa7348);

    // Draws additional rectangles regardless of input
    draw_rect(0, 0, 1, 1, 0xffffff);
    draw_rect(80, player_pos_y, 2.5, 12, 0xff0000); // A rectangle at (30,30) with size (5,5)  
    draw_rect(-80, 0, 2.5, 12, 0xff0000); // A rectangle at (-20,20) with size (8,3)  
}