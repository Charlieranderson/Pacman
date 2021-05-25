#define is_down(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

//float playerPosX = 0.f;
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5f, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;

int player_1_score = 0;
int player_2_score = 0;

internal void SimulatePlayer(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt *.5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
		//Collision!
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool aabbVSaabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs1x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}

internal void simulateGame(Input* input, float dt) {
	ClearScreen(0xff5500);
	DrawRectangle(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);

	float player_1_ddp = 0.f;
	if (is_down(BUTTONUP)) player_1_ddp += 2000;
	if (is_down(BUTTONDOWN)) player_1_ddp -= 2000;

	float player_2_ddp = 0.f;
	if (is_down(BUTTON_W)) player_2_ddp += 2000;
	if (is_down(BUTTON_S)) player_2_ddp -= 2000;

	SimulatePlayer(&player_1_p, &player_1_dp, player_1_ddp, dt);
	SimulatePlayer(&player_2_p, &player_2_dp, player_2_ddp, dt);

	//Simulate Ball
	{
		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		if (aabbVSaabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y))
		{
			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * 0.75f;
		}

		else if (aabbVSaabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
			ball_p_x = -80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * 0.75f;
		}

		//hits the top, bounce ball
		if (ball_p_y + ball_half_size > arena_half_size_y) {
			ball_p_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -1;
		}
		else if (ball_p_y - ball_half_size < -arena_half_size_y) {
			ball_p_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -1;
		}

		//Hits a side, reset ball
		if (ball_p_x + ball_half_size > arena_half_size_x) {
			ball_p_x = 0, ball_p_y = 0;
			ball_dp_x *= -1;
			ball_dp_y *= 0;
			player_2_score++;
		}
		else if (ball_p_x - ball_half_size < -arena_half_size_x) {
			ball_p_x = 0, ball_p_y = 0;
			ball_dp_x *= -1;
			ball_dp_y *= 0;
			player_1_score++;
		}

	}

	DrawRectangle(ball_p_x, ball_p_y, 1, 1, 0xffffff);
	DrawRectangle(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);
	DrawRectangle(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000);

}