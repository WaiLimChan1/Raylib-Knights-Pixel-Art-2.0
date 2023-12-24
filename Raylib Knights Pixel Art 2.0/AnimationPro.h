#pragma once
#include "Header.h"

class AnimationPro
{
private:
	Texture2D texture;
	Rectangle source;
	float width;
	float height;
	float scale;

	float updateTime;
	float runningTime;
	int frame;

	int rowFrames;
	int colFrames;
	int totalFrames;

public:
	AnimationPro();
	AnimationPro(Texture2D texture, float scale, float updateTime, int rowFrames, int colFrames, int totalFrames);

	float getWidth();
	float getHeight();
	float getFrame();
	float getRowFrames();
	float getColFrames();
	float getTotalFrames();

	void draw(Vector2 pos, bool flip, Color color = WHITE);
	void updateFrame();

	bool atMidFrame();
	bool atLastFrame();

	void unloadTexture();
};