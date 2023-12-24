#pragma once
#include "Header.h"
#include "ConstGlobalVariables.cpp"
#include "AnimationPro.h"

class Character
{
private:
	const char* name;

	AnimationPro current;

	AnimationPro idle; //Interruptable 
	AnimationPro run; //Interruptable 

	AnimationPro turnAround;

	AnimationPro attack1;
	AnimationPro attack2;
	AnimationPro attackCombo;

	AnimationPro crouching;
	AnimationPro crouched; //Interruptable
	AnimationPro crouchWalk; //Interruptable

	AnimationPro crouchAttack;

	AnimationPro dash;
	AnimationPro jump;
	AnimationPro roll;
	AnimationPro slide;

	AnimationPro hurt; //Can Interrupt Everything
	AnimationPro death; //Can Interrupt Everything

	Status lastStatus;
	Status status;

	Vector2 pos;

	float velocity;

	float velY;
	bool inAir;

	bool facingLeft;

	int health;
	bool isCrouched;
	bool dead; //Dead and finished DEATH animation

	float energyCap;
	float energyRegenSpeed;
	float energyCost;
	float energy; 

public:
	Character();
	Character(const char* name,
		AnimationPro idle, AnimationPro run, AnimationPro turnAround, AnimationPro attack1,
		AnimationPro attack2, AnimationPro attackCombo, AnimationPro crouching, AnimationPro crouched,
		AnimationPro crouchWalk, AnimationPro crouchAttack, AnimationPro dash, AnimationPro jump,
		AnimationPro roll, AnimationPro slide, AnimationPro hurt, AnimationPro death,
		float velocity, bool facingLeft, int health, float energyCap, float energyRegenSpeed, float energyCost);

	const char* getName();
	Status getStatus();
	Vector2 getPos();
	void setPos(Vector2 pos);
	int getHealth();
	bool getDead();
	float getEnergy();

	Vector2 getSyncCameraPos();

	Rectangle getHitbox(Vector2 cameraPos);
	Rectangle getCrouchedHitbox(Vector2 cameraPos);
	void drawHitbox(Vector2 cameraPos);

	Rectangle getAttackBox(Vector2 cameraPos);
	Rectangle getCrouchedAttackbox(Vector2 cameraPos);
	void drawAttackbox(Vector2 cameraPos);

	bool atMidFrame();
	int getFrame();

	bool attackBoxesAreActive();
	bool isHitByRec(Rectangle attackBox, Vector2 cameraPos);

	void draw(Vector2 cameraPos);
	void draw(Vector2 cameraPos, Color color);
	void endPlayOnceAnimations();
	void takeInput();
	void takePlayer2Input();
	void aiRandomInput();
	void changeAnimation();

	void updatePosition();
	void checkBound();

	void manageEnergy();

	void unloadTexture();
};