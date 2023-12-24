#include "Background.h"

Background::Background() {}
Background::Background(Texture2D texture, Vector2 pos, float scale)
	: texture(texture), pos(pos), scale(scale) {}
Vector2 Background::getScaledSize() { return { scale * texture.width, scale * texture.height }; }
void Background::draw(Vector2 cameraPos) 
{ 
	if (cameraPos.x >= texture.width * scale + pos.x) pos = { cameraPos.x, 0 };
	if (cameraPos.x <= -texture.width * scale + pos.x) pos = { cameraPos.x, 0 };

	Vector2 temp{ pos.x - 2 * texture.width * scale, pos.y };
	for (int i = 0; i < 5; i++)
	{
		DrawTextureEx(texture, { temp.x - cameraPos.x, temp.y - cameraPos.y }, 0, scale, WHITE);
		temp.x += texture.width * scale;
	}
}
void Background::unloadTexture() { UnloadTexture(texture); }

