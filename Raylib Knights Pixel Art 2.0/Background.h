#pragma once
#include "Header.h"

class Background
{
private:
	Texture2D texture;
	Vector2 pos;
	float scale;

public:
	Background();
	Background(Texture2D texture, Vector2 pos, float scale);
	Vector2 getScaledSize();
	void draw(Vector2 cameraPos);
	void unloadTexture();
};