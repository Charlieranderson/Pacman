struct ButtonState {
	bool isDown;
	bool changed;
};

enum {
	BUTTONUP,
	BUTTONDOWN,
	BUTTONLEFT,
	BUTTONRIGHT,

	BUTTONCOUNT,
};

struct Input {
	ButtonState buttons[BUTTONCOUNT];
};