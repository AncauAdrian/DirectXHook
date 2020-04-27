#pragma once

typedef unsigned int uint;

class PlayerPosition
{
private:
	static const uint* cPed_ptr;
	static const uint pos_struct_offset;
	static const uint x_offset;
	static const uint y_offset;
	static const uint z_offset;
	PlayerPosition() {}

public:
	static float getX();
	static float getY();
	static float getZ();
	static void getPosition(float& x, float& y, float& z);
	static float debug();
};


