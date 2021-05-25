struct ButtonState {
	bool isDown;
	bool changed;
};

enum {
	BUTTONUP,
	BUTTONDOWN,
	BUTTON_W,
	BUTTON_S,

	BUTTONCOUNT,
};

struct Input {
	ButtonState buttons[BUTTONCOUNT];
};