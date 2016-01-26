#include "ChargePack.h"

ChargePack::ChargePack(float x, float y, float z, float red, float green, float blue, float size)
{
	_xPos = x;// +NUDGE;
	_yPos = y;
	_zPos = z;// -NUDGE;

	_redColor = red;
	_greenColor = green;
	_blueColor = blue;

	_offset.x = 0.0f;
	_offset.z = 0.0f;

	_size = size;

	_currentPosition.x = x;// +NUDGE;
	_currentPosition.z = z;// -NUDGE;

	_spawnState = true;
}

void ChargePack::Render()
{
	glPushMatrix();
		glTranslatef(_currentPosition.x + _offset.x + NUDGE, _yPos, _currentPosition.z - _offset.z - NUDGE);
		glColor3f(_redColor, _greenColor, _blueColor);
		glutSolidCube(_size);
	glPopMatrix();
}

void ChargePack::Collect()
{
	_spawnState = false;
}

void ChargePack::Spawn()
{
	_spawnState = true;
}

position ChargePack::GetPosition()
{
	position tempPosition;

	tempPosition.x = _currentPosition.x + _offset.x;// +NUDGE;
	tempPosition.z = _currentPosition.z - _offset.z;// -NUDGE;

	return tempPosition;
}