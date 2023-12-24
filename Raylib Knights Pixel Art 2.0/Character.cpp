#include "Character.h"

Character::Character() {}
Character::Character(const char* name,
	AnimationPro idle, AnimationPro run, AnimationPro turnAround, AnimationPro attack1,
	AnimationPro attack2, AnimationPro attackCombo, AnimationPro crouching, AnimationPro crouched,
	AnimationPro crouchWalk, AnimationPro crouchAttack, AnimationPro dash, AnimationPro jump,
	AnimationPro roll, AnimationPro slide, AnimationPro hurt, AnimationPro death,
	float velocity, bool facingLeft, int health, float energyCap, float energyRegenSpeed, float energyCost)
	: name(name), current(idle), idle(idle), run(run), turnAround(turnAround), attack1(attack1), attack2(attack2), attackCombo(attackCombo), crouching(crouching), crouched(crouched),
	crouchWalk(crouchWalk), crouchAttack(crouchAttack), dash(dash), jump(jump), roll(roll), slide(slide), hurt(hurt), death(death), velocity(velocity), facingLeft(facingLeft), health(health), 
	energyCap(energyCap), energyRegenSpeed(energyRegenSpeed), energy(energyCap), energyCost(energyCost)
{
	lastStatus = IDLE;
	status = IDLE;

	this->pos = { SCREEN_WIDTH / 2 - current.getWidth() / 2, SCREEN_HEIGHT - current.getHeight() };

	velY = 0; 
	inAir = false;

	isCrouched = false;
	dead = false;
}

const char* Character::getName() { return name; }
Status Character::getStatus() { return status; }
Vector2 Character::getPos() { return pos; }
void Character::setPos(Vector2 pos) { this->pos = pos; }
int Character::getHealth() { return health; }
bool Character::getDead() { return dead; }
float Character::getEnergy() { return energy; }

Vector2 Character::getSyncCameraPos() { return { pos.x - (SCREEN_WIDTH / 2 - current.getWidth() / 2), 0 + current.getHeight() / 3}; }

Rectangle Character::getHitbox(Vector2 cameraPos)
{
	float hitboxWidth = current.getWidth() / 4;
	float hitboxHeight = current.getHeight() / 2;
	return Rectangle{ pos.x - cameraPos.x + 3 * hitboxWidth / 2, pos.y - cameraPos.y + hitboxHeight, hitboxWidth, hitboxHeight };
}
Rectangle Character::getCrouchedHitbox(Vector2 cameraPos)
{
	float hitboxWidth = current.getWidth() / 4;
	float hitboxHeight = current.getHeight() / 3;
	return Rectangle{ pos.x - cameraPos.x + 3 * hitboxWidth / 2, pos.y - cameraPos.y + 2 * hitboxHeight, hitboxWidth, hitboxHeight };
}
void Character::drawHitbox(Vector2 cameraPos) 
{ 
	if (isCrouched) DrawRectangleRec(getCrouchedHitbox(cameraPos), RED);
	else DrawRectangleRec(getHitbox(cameraPos), RED); 
}

Rectangle Character::getAttackBox(Vector2 cameraPos)
{
	float attackBoxWidth = current.getWidth() * 3 / 8;
	float attackBoxHeight = current.getHeight() / 2;
	float attackBoxX = pos.x - cameraPos.x;
	float attackBoxY = pos.y - cameraPos.y + attackBoxHeight;

	if (facingLeft) attackBoxX += attackBoxWidth * 1 / 3;
	else attackBoxX += attackBoxWidth * 4 / 3;
	return Rectangle{ attackBoxX, attackBoxY, attackBoxWidth, attackBoxHeight };
}
Rectangle Character::getCrouchedAttackbox(Vector2 cameraPos)
{
	float attackBoxWidth = current.getWidth() * 3 / 8;
	float attackBoxHeight = current.getHeight() / 3;
	float attackBoxX = pos.x - cameraPos.x;
	float attackBoxY = pos.y - cameraPos.y + 2 * attackBoxHeight;

	if (facingLeft) attackBoxX += attackBoxWidth * 1 / 3;
	else attackBoxX += attackBoxWidth * 4 / 3;
	return Rectangle{ attackBoxX, attackBoxY, attackBoxWidth, attackBoxHeight };
}
void Character::drawAttackbox(Vector2 cameraPos) 
{ 
	if (isCrouched) DrawRectangleRec(getCrouchedAttackbox(cameraPos), ORANGE);
	else DrawRectangleRec(getAttackBox(cameraPos), ORANGE); 
}

bool Character::atMidFrame() { return current.atMidFrame(); }
int Character::getFrame() { return current.getFrame(); }
bool Character::attackBoxesAreActive()
{
	return ((status == ATTACK1 || status == ATTACK2 || status == CROUCH_ATTACK) && current.atMidFrame()) //Single Attacks Are Active At Half Way Point In Animation
		    || (status == ATTACK_COMBO) && (current.getFrame() == 2 || current.getFrame() == 7); //Combo Attacks Requires Hard Coding The Frames At Which Attack Boxes Are Activated
}
bool Character::isHitByRec(Rectangle attackBox, Vector2 cameraPos)
{
	if (!dead && status != DEATH)
	{
		Rectangle hitbox;
		if (isCrouched) hitbox = getCrouchedHitbox(cameraPos);
		else hitbox = getHitbox(cameraPos);
		if (CheckCollisionRecs(attackBox, hitbox))
		{
			status = HURT;
			return true;
		}
	}
	return false;
}

void Character::draw(Vector2 cameraPos)
{
	if (status == HURT) current.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft, RED);
	else current.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft);
	if (!dead) current.updateFrame();
	endPlayOnceAnimations();
}

void Character::draw(Vector2 cameraPos, Color color)
{
	if (status == HURT) current.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft, RED);
	else current.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft, color);
	if (!dead) current.updateFrame();
	endPlayOnceAnimations();
}

void Character::endPlayOnceAnimations()
{
	if (current.atLastFrame()) // Animation finished
	{
		// Once finished go to IDLE standing
		if (status == TURN_AROUND || status == ATTACK1 || status == ATTACK2 || status == ATTACK_COMBO || status == DASH || status == JUMP || status == HURT)
		{
			if (status == TURN_AROUND) facingLeft = !facingLeft;
			if (status == JUMP && inAir) status = JUMP; //Special Case, JUMP remains jump as long as character is in air
			else
			{
				isCrouched = false;
				status = IDLE;
			}
		}

		// Once finished go to CROUNCHED
		if (status == CROUCH_ATTACK || status == ROLL || status == SLIDE)
		{
			isCrouched = true;
			status = CROUCHED;
		}

		// Crouching Goes Back And Forth Between The IDLE And CROUCHED
		if (status == CROUCHING)
		{
			if (isCrouched)
			{
				isCrouched = false;
				status = IDLE;
			}
			else
			{
				isCrouched = true;
				status = CROUCHED;
			}
		}

		if (status == DEATH)
		{
			dead = true;
		}
	}
}

void Character::takeInput()
{
	if (!dead && status != DEATH)
	{
		if (isCrouched && status != CROUCH_ATTACK && status != ROLL && status != SLIDE && status != CROUCHING && status != HURT ) // Crouching 
		{
			status = CROUCHED;
			if (IsKeyDown(KEY_A))
			{
				facingLeft = true;
				status = CROUCH_WALK;
			}
			if (IsKeyDown(KEY_D))
			{
				facingLeft = false;
				status = CROUCH_WALK;
			}
			if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_R) || IsKeyPressed(KEY_F)) status = CROUCH_ATTACK;
			if (IsKeyPressed(KEY_W)) status = ROLL;
			if (IsKeyPressed(KEY_SPACE)) status = SLIDE;
			if (IsKeyPressed(KEY_C)) status = CROUCHING;
		}
		else if (!isCrouched && status != TURN_AROUND && status != ATTACK1 && status != ATTACK2 && status != ATTACK_COMBO && status != DASH && status != JUMP && status != CROUCHING && status != HURT) // Standing
		{
			status = IDLE;
			if (IsKeyDown(KEY_A))
			{
				if (!facingLeft) status = TURN_AROUND;
				else status = RUN;
			}
			if (IsKeyDown(KEY_D))
			{
				if (facingLeft) status = TURN_AROUND;
				else status = RUN;
			}
			if (IsKeyPressed(KEY_E)) status = ATTACK1;
			if (IsKeyPressed(KEY_R)) status = ATTACK2;
			if ((IsKeyPressed(KEY_E) && IsKeyPressed(KEY_R)) || IsKeyPressed(KEY_F)) status = ATTACK_COMBO;
			if (IsKeyPressed(KEY_W)) status = DASH;
			if (IsKeyPressed(KEY_SPACE)) status = JUMP;
			if (IsKeyPressed(KEY_C)) status = CROUCHING;
		}
		//if (IsKeyPressed(KEY_Y)) status = HURT;
	}
}

void Character::takePlayer2Input()
{
	if (!dead && status != DEATH)
	{
		if (isCrouched && status != CROUCH_ATTACK && status != ROLL && status != SLIDE && status != CROUCHING && status != HURT) // Crouching 
		{
			status = CROUCHED;
			if (IsKeyDown(KEY_J))
			{
				facingLeft = true;
				status = CROUCH_WALK;
			}
			if (IsKeyDown(KEY_L))
			{
				facingLeft = false;
				status = CROUCH_WALK;
			}
			if (IsKeyPressed(KEY_U) || IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_H)) status = CROUCH_ATTACK;
			if (IsKeyPressed(KEY_I)) status = ROLL;
			if (IsKeyPressed(KEY_M)) status = SLIDE;
			if (IsKeyPressed(KEY_N)) status = CROUCHING;
		}
		else if (!isCrouched && status != TURN_AROUND && status != ATTACK1 && status != ATTACK2 && status != ATTACK_COMBO && status != DASH && status != JUMP && status != CROUCHING && status != HURT) // Standing
		{
			status = IDLE;
			if (IsKeyDown(KEY_J))
			{
				if (!facingLeft) status = TURN_AROUND;
				else status = RUN;
			}
			if (IsKeyDown(KEY_L))
			{
				if (facingLeft) status = TURN_AROUND;
				else status = RUN;
			}
			if (IsKeyPressed(KEY_U)) status = ATTACK1;
			if (IsKeyPressed(KEY_Y)) status = ATTACK2;
			if ((IsKeyPressed(KEY_U) && IsKeyPressed(KEY_Y)) || IsKeyPressed(KEY_H)) status = ATTACK_COMBO;
			if (IsKeyPressed(KEY_I)) status = DASH;
			if (IsKeyPressed(KEY_M)) status = JUMP;
			if (IsKeyPressed(KEY_N)) status = CROUCHING;
		}
		//if (IsKeyPressed(KEY_Y)) status = HURT;
	}
}

void Character::aiRandomInput()
{
	int randInput = rand() % 8;
	if (!dead && status != DEATH)
	{
		if (isCrouched && status != CROUCH_ATTACK && status != ROLL && status != SLIDE && status != CROUCHING && status != HURT) // Crouching 
		{
			status = CROUCHED;
			if (randInput == 0)
			{
				facingLeft = true;
				status = CROUCH_WALK;
			}
			if (randInput == 1)
			{
				facingLeft = false;
				status = CROUCH_WALK;
			}
			if (randInput == 2 || randInput == 3 || randInput == 4) status = CROUCH_ATTACK;
			if (randInput == 5) status = ROLL;
			if (randInput == 6) status = SLIDE;
			if (randInput == 7) status = CROUCHING;
		}
		else if (!isCrouched && status != TURN_AROUND && status != ATTACK1 && status != ATTACK2 && status != ATTACK_COMBO && status != DASH && status != JUMP && status != CROUCHING && status != HURT) // Standing
		{
			status = IDLE;
			if (randInput == 0)
			{
				if (!facingLeft) status = TURN_AROUND;
				else status = RUN;
			}
			if (randInput == 1)
			{
				if (facingLeft) status = TURN_AROUND;
				else status = RUN;
			}
			if (randInput == 2) status = ATTACK1;
			if (randInput == 3) status = ATTACK2;
			if ((randInput == 2 && randInput == 3) || randInput == 4) status = ATTACK_COMBO;
			if (randInput == 5) status = DASH;
			if (randInput == 6) status = JUMP;
			if (randInput == 7) status = CROUCHING;
		}
		//if (IsKeyPressed(KEY_Y)) status = HURT;
	}
}

void Character::changeAnimation()
{
	if (!dead)
	{
		if (lastStatus != status)
		{
			if ((status == DASH || status == JUMP || status == ROLL || status == SLIDE))
			{
				if (energy >= energyCost)
				{
					if (status == DASH) current = dash;
					if (status == JUMP)
					{
						velY = JUMPVAL;
						inAir = true;
						current = jump;
					}
					if (status == ROLL) current = roll;
					if (status == SLIDE) current = slide;
					energy -= energyCost;
				}
				else
				{
					if (isCrouched) status = CROUCHED;
					else status = IDLE;
				}
			}

			if (status == IDLE) current = idle;
			if (status == RUN) current = run;
			if (status == TURN_AROUND) current = turnAround;
			if (status == ATTACK1) current = attack1;
			if (status == ATTACK2) current = attack2;
			if (status == ATTACK_COMBO) current = attackCombo;
			if (status == CROUCHING) current = crouching;
			if (status == CROUCHED) current = crouched;
			if (status == CROUCH_WALK) current = crouchWalk;
			if (status == CROUCH_ATTACK) current = crouchAttack;
			
			if (status == HURT)
			{
				health--;
				if (health <= 0)
				{
					status = DEATH;
					current = death;
				}
				else current = hurt;
			}
			if (status == DEATH)
			{
				current = death;
			}
		}
		lastStatus = status;
	}
}

void Character::updatePosition()
{
	float dt = GetFrameTime();
	if (status == CROUCH_WALK)
	{
		if (facingLeft) pos.x -= velocity / 2 * dt;
		else pos.x += velocity / 2 * dt;
	}
	else if (status == RUN || status == ROLL || status == SLIDE)
	{
		if (facingLeft) pos.x -= velocity * dt;
		else pos.x += velocity * dt;
	}
	else if (status == DASH)
	{
		if (facingLeft) pos.x -= velocity * 2 * dt;
		else pos.x += velocity * 2 * dt;
	}
	else if (status == JUMP)
	{
		if (IsKeyDown(KEY_A))
		{
			facingLeft = true;
			pos.x -= velocity * dt;
		}
		else if (IsKeyDown(KEY_D))
		{
			facingLeft = false;
			pos.x += velocity * dt;
		}
	}
	if (inAir)
	{
		velY -= GRAVITY * dt;
		pos.y -= velY * dt;
	}
	checkBound();
	manageEnergy();
}

void Character::checkBound()
{
	if (pos.y >= SCREEN_HEIGHT - current.getHeight())
	{
		pos.y = SCREEN_HEIGHT - current.getHeight();
		velY = 0;
		inAir = false;
	}
}

void Character::manageEnergy()
{
	energy += energyRegenSpeed * GetFrameTime();
	if (energy > energyCap) energy = energyCap;
}

void Character::unloadTexture()
{
	idle.unloadTexture();
	run.unloadTexture();
	turnAround.unloadTexture();
	attack1.unloadTexture();
	attack2.unloadTexture();
	attackCombo.unloadTexture();
	crouching.unloadTexture();
	crouched.unloadTexture();
	crouchWalk.unloadTexture();
	crouchAttack.unloadTexture();
	dash.unloadTexture();
	jump.unloadTexture();
	roll.unloadTexture();
	slide.unloadTexture();
	hurt.unloadTexture();
	death.unloadTexture();
}