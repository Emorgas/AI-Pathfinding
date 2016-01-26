#pragma once

#include "Obstacle.h"

class ChargePack
{
private:
	float _xPos, _yPos, _zPos;
	float _redColor, _greenColor, _blueColor;
	float _size;

	position _currentPosition;
	position _offset;

	bool	 _spawnState;
public:
	ChargePack(float x, float y, float z, float red, float green, float blue, float size);
	void Render();
	void Collect();
	void Spawn();
	position GetPosition();
	bool IsSpawned() { return _spawnState; }
};