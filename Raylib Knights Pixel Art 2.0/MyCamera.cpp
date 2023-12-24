#include "MyCamera.h"

MyCamera::MyCamera() {}
MyCamera::MyCamera(Vector2 pos, float speed, bool locked)
	: pos(pos), speed(speed), locked(locked) {}

Vector2 MyCamera::getPos() { return pos; }
bool MyCamera::getLocked() { return locked; }
void MyCamera::setPos(Vector2 pos) { this->pos = pos; }

void MyCamera::takeInput()
{
	float dt = GetFrameTime();
	if (IsKeyDown(KEY_LEFT)) pos.x -= speed * dt;
	if (IsKeyDown(KEY_RIGHT)) pos.x += speed * dt;
	if (IsKeyDown(KEY_UP)) pos.y -= speed * dt;
	if (IsKeyDown(KEY_DOWN)) pos.y += speed * dt;

	if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) locked = false;
	if (IsKeyPressed(KEY_L)) locked = !locked;
}

void MyCamera::checkTopDownBound(Vector2 mapSize)
{
	if (pos.y < 0) pos.y = 0;
	else if (pos.y > mapSize.y - SCREEN_HEIGHT) pos.y = mapSize.y - SCREEN_HEIGHT;
}