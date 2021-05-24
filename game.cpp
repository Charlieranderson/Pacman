#define is_down(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

float playerPosX = 0.f;
float playerPosY = 0.f;

internal void simulateGame(Input* input, float dt) {
	ClearScreen(0xff5500);

	float speed = 50.0f; //Units per second
	if (is_down(BUTTONUP)) playerPosY += speed * dt;
	if (is_down(BUTTONDOWN)) playerPosY -= speed * dt;
	if (is_down(BUTTONLEFT)) playerPosX -= speed * dt;
	if (is_down(BUTTONRIGHT)) playerPosX += speed * dt;

	DrawRectangle(playerPosX, playerPosY, 5, 5, 0x00ff22);
	DrawRectangle(20, 25, 3, 4, 0x0000ff);
}