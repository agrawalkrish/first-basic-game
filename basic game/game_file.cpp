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

int player_1_score, player_2_score;
enum GameMode {
    GM_Menu,
    GM_Gameplay,
};
int hot_button = 0;
bool enemy_is_ai;

GameMode current_gamemode;

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


    if (current_gamemode == GM_Gameplay) {
        // Player 1 input and physics
        float player_1_ddp = 0.f;
        if (!enemy_is_ai){
            if (is_down(BUTTON_UP)) 
                player_1_ddp += 2000;
            if (is_down(BUTTON_DOWN))
                player_1_ddp -= 2000;
        }
        else {
            player_1_ddp = (ball_p_y - player_1_p) * 100;
            if (player_1_ddp > 1300)player_1_ddp = 1300;
            if (player_1_ddp < -1300)player_1_ddp = -1300;
        }
        
        simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);

        // Player 2 input and physics
        float player_2_ddp = 0.f;
        if (is_down(BUTTON_W))
            player_2_ddp += 2000;
        if (is_down(BUTTON_S))
            player_2_ddp -= 2000;

        simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

        // Ball movement
        ball_p_x += ball_dp_x * dt;
        ball_p_y += ball_dp_y * dt;

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
        //ball simulation

        {
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
                ball_p_x = 0;
                ball_p_y = 0;
                ball_dp_y = 0;
                ball_dp_x *= -1;
                player_1_score++;
            }
            else if (ball_p_x - ball_half_size < -arena_half_size_x)
            {
                ball_p_x = 0;
                ball_p_y = 0;
                ball_dp_y = 0;
                ball_dp_x *= -1;
                player_2_score++;
            }
        }
        
        
        draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
        draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);
        // Draw everything
        draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);      // ball
        draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);  // Right paddle
        draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // Left paddle
    }

    else{
        if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))
            hot_button = !hot_button;

        if (pressed(BUTTON_ENTER)) {
            current_gamemode = GM_Gameplay;
            enemy_is_ai = hot_button ? 0 : 1;
        }
        if (hot_button == 1) {
            draw_text("MULTI PLAYER", -80, -10, 1, 0xaaaaaa);
            draw_text("SINGLE PLAYER", 0, -10, 1, 0xff0000);
            
        }
        else {
            draw_text("MULTI PLAYER", -80, -10, 1, 0xff0000);
            draw_text("SINGLE PLAYER", 0, -10, 1, 0xaaaaaa);
            
        }

        draw_text("PING PONG", -50, 40, 2, 0xffffff);
            
        
    }
}