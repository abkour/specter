#pragma once
#include "math/vec2.hpp"

namespace specter {

struct MouseKeyboardState {
	// Special keys
	bool Alt;
	bool Comma;
	bool Control;
	bool Capslock;
	bool Enter;
	bool Escape;
	bool Period;
	bool Tab;

	// Alpha-numerical keys
	bool A;
	bool B;
	bool C;
	bool D;
	bool E;
	bool F;
	bool G;
	bool H;
	bool I;
	bool J;
	bool K;
	bool L;
	bool M;
	bool N;
	bool O;
	bool P;
	bool Q;
	bool R;
	bool S;
	bool T;
	bool U; 
	bool V;
	bool W;
	bool X; 
	bool Y;
	bool Z;
	bool Num_0;
	bool Num_1;
	bool Num_2;
	bool Num_3;
	bool Num_4;
	bool Num_5;
	bool Num_6;
	bool Num_7;
	bool Num_8;
	bool Num_9;

	// F keys
	bool F1;
	bool F2;
	bool F3;
	bool F4;
	bool F5;
	bool F6;
	bool F7;
	bool F8;
	bool F9;
	bool F10;
	bool F11;
	bool F12;

	// Mouse buttons
	bool initialEntry;
	bool LMB;
	bool MMB;
	bool RMB;
	float zoom;
	vec2i pos;
	vec2i delta;
};

}