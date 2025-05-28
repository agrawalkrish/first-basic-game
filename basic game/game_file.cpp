internal void
simulate_game(Input& input) {
	
        clear_screen(0xff5500);

        if(input.buttons[BUTTON_UP].is_down)
            draw_rect(0, 0, 1, 1, 0xaa7348);
        draw_rect(30, 30, 5, 5, 0xaac348);
        draw_rect(-20, 20, 8, 3, 0xcc7348);
}