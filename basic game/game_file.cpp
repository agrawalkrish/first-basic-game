#define is_down(b) input->buttons[b].is_down                                  // Button held
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)   // Just pressed
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed) // Just released

float player_1_p, player_1_dp;
float player_2_p, player_2_dp;
float arean_half_size_x = 85, arean_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;

float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y;

internal void simulate_game(Input *input, float dt)
{
    clear_screen(0xff5500);                                          // Orange background
    draw_rect(0, 0, arean_half_size_x, arean_half_size_y, 0xffaa33); // Main playfield

    float player_1_ddp = 0;
    float player_2_ddp = 0;

    if (is_down(BUTTON_UP))
        player_1_ddp += 2000; // P1 move up
    if (is_down(BUTTON_DOWN))
        player_1_ddp -= 2000; // P1 move down

    if (is_down(BUTTON_W))
        player_2_ddp += 2000; // P2 move up
    if (is_down(BUTTON_S))
        player_2_ddp -= 2000; // P2 move down

    player_1_ddp -= player_1_dp * 10.f; // P1 friction
    player_1_p += player_1_dp * dt + 0.5f * player_1_ddp * dt * dt;
    player_1_dp += player_1_ddp * dt;

    // p1 collision
    if (player_1_p + player_half_size_y > arean_half_size_y)
    {
        player_1_p = arean_half_size_y - player_half_size_y;
        player_1_dp *= 0;//change to 0 for no bounce NOTE
    }
    else if (player_1_p - player_half_size_y < -arean_half_size_y)
    {
        player_1_p = -arean_half_size_y + player_half_size_y;
        player_1_dp *= 0;//change to 0 for no bounce NOTE
    }

    player_2_ddp -= player_2_dp * 10.f; // P2 friction
    player_2_p += player_2_dp * dt + 0.5f * player_2_ddp * dt * dt;
    player_2_dp += player_2_ddp * dt;

    // p2 collision
    if (player_2_p + player_half_size_y > arean_half_size_y)
    {
        player_2_p = arean_half_size_y - player_half_size_y;
        player_2_dp *= 0;//change to 0 for no bounce NOTE
    }
    else if (player_2_p - player_half_size_y < -arean_half_size_y)
    {
        player_2_p = -arean_half_size_y + player_half_size_y;
        player_2_dp *= 0;//change to 0 for no bounce NOTE
    }

    ball_p_x += ball_dp_x * dt;
    ball_p_y += ball_dp_y * dt;

    draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff);// ball
    draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);  // Right paddle
    draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // Left paddle
}