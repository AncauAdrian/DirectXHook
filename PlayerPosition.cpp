#include "PlayerPosition.h"

const uint* PlayerPosition::cPed_ptr = (uint*)0xB6F5F0;
const uint PlayerPosition::pos_struct_offset = 0x14;
const uint PlayerPosition::x_offset = 0x30;
const uint PlayerPosition::y_offset = 0x34;
const uint PlayerPosition::z_offset = 0x38;

float PlayerPosition::getX()
{
	if (*cPed_ptr == 0)
		return 0.0f;

	uint step = *cPed_ptr + pos_struct_offset;

	uint* step2 = (uint*)step;

	if (step2 == nullptr)
		return 0.0f;

	float* x = (float*)(*step2 + x_offset);

	if (x == nullptr)
		return 0.0f;

	return *x;
}

float PlayerPosition::getY()
{
	if (*cPed_ptr == 0)
		return 0.0f;

	uint step = *cPed_ptr + pos_struct_offset;

	uint* step2 = (uint*)step;

	if (step2 == nullptr)
		return 0.0f;

	float* y = (float*)(*step2 + y_offset);

	if (y == nullptr)
		return 0.0f;

	return *y;
}

float PlayerPosition::getZ()
{
	if (*cPed_ptr == 0)
		return 0.0f;

	uint step = *cPed_ptr + pos_struct_offset;

	uint* step2 = (uint*)step;

	if (step2 == nullptr)
		return 0.0f;

	float* z = (float*)(*step2 + z_offset);

	if (z == nullptr)
		return 0.0f;

	return *z;
}

void PlayerPosition::getPosition(float& retx, float& rety, float& retz)
{
	retx = 0, rety = 0, retz = 0;
	if (*cPed_ptr == 0)
		return;

	uint step = *cPed_ptr + pos_struct_offset;

	uint* step2 = (uint*)step;

	if (step2 == nullptr)
		return;

	float* x = (float*)(*step2 + x_offset);
	float* y = (float*)(*step2 + y_offset);
	float* z = (float*)(*step2 + z_offset);

	if (x != nullptr)
		retx = *x;

	if (y != nullptr)
		rety = *y;

	if (z != nullptr)
		retz = *z;

}

float PlayerPosition::debug()
{
	uint step = *cPed_ptr + pos_struct_offset;
	uint* step2 = (uint*)step;
	return *(float*)(*step2 + x_offset);
}
