#define is_down(b) input->buttons[b].is_down                                  // Button held
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)   // Just pressed
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed) // Just released

// Player variables
float player_1_p, player_1_dp;
float player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;

// Ball variables - Initialized for a consistent start
float ball_p_x = 0, ball_p_y = 0;
float ball_dp_x = 130, ball_dp_y = 0;
float ball_half_size = 1;
internal void simulate_player(float* p, float* dp, float ddp, float dt)
{
    ddp -= *dp * 10.f; // P1 friction
    *p += *dp * dt + 0.5f * ddp * dt * dt;
    *dp += ddp * dt;

    // Player 1 wall collision
    if (*p + player_half_size_y > arena_half_size_y)
    {
        *p = arena_half_size_y - player_half_size_y;
        *dp *= 0;
    }
    else if (*p - player_half_size_y < -arena_half_size_y)
    {
        *p = -arena_half_size_y + player_half_size_y;
        *dp *= 0;
    }
};

internal bool aabb(float p1x, float p1y, float hs1x, float hs1y,
    float p2x, float p2y, float hs2x, float hs2y)
{
    // Check for overlap on the X axis AND the Y axis
    return (p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y - hs1y < p2y + hs2y);
};
internal void simulate_game(Input* input, float dt)
{
    clear_screen(0xff5500);                                          // Orange background
    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // Main playfield

    // Player 1 input and physics
    float player_1_ddp = 0;
    if (is_down(BUTTON_UP))
        player_1_ddp += 2000;
    if (is_down(BUTTON_DOWN))
        player_1_ddp -= 2000;

    simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);

    // Player 2 input and physics
    float player_2_ddp = 0;
    if (is_down(BUTTON_W))
        player_2_ddp += 2000;
    if (is_down(BUTTON_S))
        player_2_ddp -= 2000;

    simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

    // Ball movement
    ball_p_x += ball_dp_x * dt;
    ball_p_y += ball_dp_y * dt;

    // Ball collision detection and response
    // Player 1 (Right Paddle) collision check
    if (aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y))
    {
        ball_p_x = 80 - player_half_size_x - ball_half_size;
        ball_dp_x *= -1;
        ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f; // Add vertical speed based on impact point
    }
    // Player 2 (Left Paddle) collision check
    else if (aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y))
    {
        ball_p_x = -80 + player_half_size_x + ball_half_size;
        ball_dp_x *= -1;
        ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f; // Add vertical speed based on impact point
    }

    // Ball collision with top and bottom walls
    if (ball_p_y + ball_half_size > arena_half_size_y)
    {
        ball_p_y = arena_half_size_y - ball_half_size;
        ball_dp_y *= -1;
    }
    else if (ball_p_y - ball_half_size < -arena_half_size_y)
    {
        ball_p_y = -arena_half_size_y + ball_half_size;
        ball_dp_y *= -1;
    }

    // Ball collision with side walls (scoring and reset)
    if (ball_p_x + ball_half_size > arena_half_size_x)
    {
        // Player 2 scores, reset ball
        ball_p_x = 0;
        ball_p_y = 0;
        ball_dp_y = 0;
        ball_dp_x *= -1; // Serve to the other player
    }
    else if (ball_p_x - ball_half_size < -arena_half_size_x)
    {
        // Player 1 scores, reset ball
        ball_p_x = 0;
        ball_p_y = 0;
        ball_dp_y = 0;
        ball_dp_x *= -1; // Serve to the other player
    }

    // Draw everything
    draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);      // ball
    draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);  // Right paddle
    draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // Left paddle
}