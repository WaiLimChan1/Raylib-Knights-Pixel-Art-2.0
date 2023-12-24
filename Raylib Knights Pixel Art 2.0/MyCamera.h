#pragma once
#include "Header.h"
#include "ConstGlobalVariables.cpp"

class MyCamera
{
private:
	Vector2 pos;
	float speed;
	bool locked;

public:
	MyCamera();
	MyCamera(Vector2 pos, float speed, bool locked);

	Vector2 getPos();
	bool getLocked();
	void setPos(Vector2 pos);

	void takeInput();
	void checkTopDownBound(Vector2 mapSize);
};