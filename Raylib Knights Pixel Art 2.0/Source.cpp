#include "Header.h"
#include "ConstGlobalVariables.cpp"
#include "MyCamera.h"
#include "Background.h"
#include "AnimationPro.h"
#include "Character.h"

// Global Variables
//---------------------------------------------------------------------------------------------------------------------------------------------------
Background background;
MyCamera camera;

vector<Character> knights;
vector<Color> colors;

int numOfEnemies = 1000;
//---------------------------------------------------------------------------------------------------------------------------------------------------

void CharacterSetUp()
{
	float scale = 4.0f;
	float updateTime = 1.0 / 15.0;

	Texture2D idleTexture = LoadTexture("knight/idle.png");
	AnimationPro idle = AnimationPro(idleTexture, scale, updateTime, 1, 10, 10);

	Texture2D runTexture = LoadTexture("knight/run.png");
	AnimationPro run = AnimationPro(runTexture, scale, updateTime, 1, 10, 10);

	Texture2D turnAroundTexture = LoadTexture("knight/turnAround.png");
	AnimationPro turnAround = AnimationPro(turnAroundTexture, scale, updateTime / 3, 1, 3, 3);

	Texture2D attack1Texture = LoadTexture("knight/attack1.png");
	AnimationPro attack1 = AnimationPro(attack1Texture, scale, updateTime, 1, 4, 4);

	Texture2D attack2Texture = LoadTexture("knight/attack2.png");
	AnimationPro attack2 = AnimationPro(attack2Texture, scale, updateTime, 1, 6, 6);

	Texture2D attackComboTexture = LoadTexture("knight/attackCombo.png");
	AnimationPro attackCombo = AnimationPro(attackComboTexture, scale, updateTime * 3 / 4, 1, 10, 10);

	Texture2D crouchingTexture = LoadTexture("knight/crouching.png");
	AnimationPro crouching = AnimationPro(crouchingTexture, scale, updateTime / 3, 1, 3, 3);

	Texture2D crouchedTexture = LoadTexture("knight/crouched.png");
	AnimationPro crouched = AnimationPro(crouchedTexture, scale, updateTime, 1, 1, 1);

	Texture2D crouchWalkTexture = LoadTexture("knight/crouchWalk.png");
	AnimationPro crouchWalk = AnimationPro(crouchWalkTexture, scale, updateTime, 1, 8, 8);

	Texture2D crouchAttackTexture = LoadTexture("knight/crouchAttack.png");
	AnimationPro crouchAttack = AnimationPro(crouchAttackTexture, scale, updateTime * 3 / 4, 1, 4, 4);

	Texture2D dashTexture = LoadTexture("knight/dash.png");
	AnimationPro dash = AnimationPro(dashTexture, scale, updateTime, 1, 2, 2);

	Texture2D jumpTexture = LoadTexture("knight/jump.png");
	AnimationPro jump = AnimationPro(jumpTexture, scale, updateTime, 1, 3, 3);

	Texture2D rollTexture = LoadTexture("knight/roll.png");
	AnimationPro roll = AnimationPro(rollTexture, scale, updateTime / 2, 1, 12, 12);

	Texture2D slideTexture = LoadTexture("knight/slide.png");
	AnimationPro slide = AnimationPro(slideTexture, scale, updateTime, 1, 4, 4);

	Texture2D hurtTexture = LoadTexture("knight/hurt.png");
	AnimationPro hurt = AnimationPro(hurtTexture, scale, updateTime, 1, 1, 1);

	Texture2D deathTexture = LoadTexture("knight/death.png");
	AnimationPro death = AnimationPro(deathTexture, scale, updateTime, 1, 10, 10);

	float velocity{ 600.0f };
	bool facingLeft{ false };
	int health{ 10 };
	float energyCap = 100;
	float energyRegenSpeed = 15;
	float energyCost = 30;

	knights.clear();
	Character knight1 = Character("Knight", idle, run, turnAround, attack1, attack2, attackCombo, crouching, crouched, crouchWalk,
		crouchAttack, dash, jump, roll, slide, hurt, death, velocity, facingLeft, health, energyCap, energyRegenSpeed, energyCost);
	Vector2 pos = { rand() % SCREEN_WIDTH - SCREEN_WIDTH, knight1.getPos().y };
	knight1.setPos(pos);
	knights.push_back(knight1);
	
	// Player 2
	/*
	knight1 = Character("Knight", idle, run, turnAround, attack1, attack2, attackCombo, crouching, crouched, crouchWalk,
		crouchAttack, dash, jump, roll, slide, hurt, death, velocity, facingLeft, health, energyCap, energyRegenSpeed, energyCost);
	knights.at(1).setPos(knights.at(0).getPos());
	knights.push_back(knight1);
	colors.push_back(WHITE);
	*/
	

	//Random Enemies
	
	colors.push_back(WHITE);
	for (int i = 0; i < numOfEnemies; i++)
	{
		Character enemyKnight = Character("Enemy Knight", idle, run, turnAround, attack1, attack2, attackCombo, crouching, crouched, crouchWalk,
			crouchAttack, dash, jump, roll, slide, hurt, death, velocity, facingLeft, health, energyCap, energyRegenSpeed, energyCost);
		Vector2 pos = { rand() % (SCREEN_WIDTH * numOfEnemies / 10), enemyKnight.getPos().y};
		enemyKnight.setPos(pos);
		knights.push_back(enemyKnight);
		colors.push_back({ (unsigned char)(rand() % 255), (unsigned char)(rand() % 255), (unsigned char)(rand() % 255), 255 });
	}
	
}

void SetUp()
{
	background = Background(LoadTexture("knight/DarkForest.png"), { 0,0 }, 1.1f);
	camera = MyCamera({ 0,0 }, 600.0f, true);

	CharacterSetUp();
}

void PrintKnightsInfo()
{
	char message[100];
	int offsetX = 10;
	int offsetY = 0;
	int fontSize = 30;
	Color color = WHITE;

	for (auto& knight : knights)
	{
		strcpy_s(message, knight.getName());
		strcat_s(message, "| Health: ");
		strcat_s(message, to_string((int)knight.getHealth()).c_str());
		strcat_s(message, " Energy: ");
		strcat_s(message, to_string((int)knight.getEnergy()).c_str());
		strcat_s(message, " Dead: ");
		strcat_s(message, to_string((int)knight.getDead()).c_str());
		DrawText(message, offsetX, offsetY, fontSize, color);
		offsetY += fontSize;
		break;
	}

	int counter = 0;
	for (auto& knight : knights) if (!knight.getDead()) counter++;
	strcpy_s(message, "Alive: ");
	strcat_s(message, to_string((int)counter).c_str());
	DrawText(message, SCREEN_WIDTH - MeasureText(message, fontSize) * 3/2, 0, fontSize, color);
}

void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);

	background.draw(camera.getPos());
	
	PrintKnightsInfo();

	//Draw Hitbox And Attack Box
	
	/*
	for (auto& knight : knights)
	{
		knight.drawHitbox(camera.getPos());
		if (knight.attackBoxesAreActive()) knight.drawAttackbox(camera.getPos());
	}
	*/
	

	//for (auto& knight : knights) knight.draw(camera.getPos());

	
	knights.at(0).draw(camera.getPos());
	for (int i = 1; i < knights.size(); i++)
	{
		knights.at(i).draw(camera.getPos(), colors.at(i));
	}
	

	EndDrawing();
}

void Input()
{
	knights.at(0).takeInput();
	//knights.at(1).takePlayer2Input(); //Player 2 Input

	for (int i = 1; i < knights.size(); i++) // Random Enemies Input 
		knights.at(i).aiRandomInput();

	camera.takeInput();

	if (IsKeyPressed(KEY_B)) CharacterSetUp(); // Reset
}

void Logic()
{
	// Check Hitboxes
	for (int attackerIndex = 0; attackerIndex < knights.size(); attackerIndex++)
	{
		for (int defenderIndex = 0; defenderIndex < knights.size(); defenderIndex++)
		{
			if (attackerIndex != defenderIndex)
			{
				Character& attacker = knights.at(attackerIndex);
				Character& defender = knights.at(defenderIndex);
				if (attacker.attackBoxesAreActive())
					defender.isHitByRec(attacker.getAttackBox(camera.getPos()), camera.getPos());
			}
		}
	}

	for (auto& knight : knights)
	{
		knight.changeAnimation();
		knight.updatePosition();
	}

	if (camera.getLocked()) camera.setPos(knights.at(0).getSyncCameraPos());
	camera.checkTopDownBound(background.getScaledSize());
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Knights Pixel Art 2.0");
	SetWindowState(FLAG_VSYNC_HINT);
	srand(time(0));

	SetUp();
	while (!WindowShouldClose())
	{
		Draw();
		Input();
		Logic();
	}

	knights.at(0).unloadTexture();
	background.unloadTexture();

	CloseWindow();

	return 0;
}