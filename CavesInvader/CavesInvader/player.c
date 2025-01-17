#include "player.h"
#include "customMath.h"
#include "gamepadx.h"
#include "bullets.h"
#include "particlesSystemManager.h"
#include "game.h"
#include "soundManager.h"

sfSprite* playerSprite;

sfTexture* playerHitTexture;
sfTexture* playerShapeTexture;

void initPlayer(Window* _window)
{
	playerSprite = sfSprite_create();

	playerHitTexture = GetTexture("playerHit");
	playerShapeTexture = sfTexture_createFromFile("../Ressources/Textures/Game/PLAYERS/shape.png", NULL);
	
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		switch (i)
		{
		case 0:
			player[i].texture = GetTexture("player1");
			player[i].pos = vector2f(300.f, 300.f);
			player[i].flame.texture = GetTexture("flame1");
			break;
		case 1:
			player[i].texture = GetTexture("player2");
			player[i].pos = vector2f(300.f, 600.f);
			player[i].flame.texture = GetTexture("flame2");
			break;
		default:
			break;
		}
		player[i].life = 3; // CHANGE to 3
		player[i].speed = PLAYER_SPEED;
		player[i].velocity = VECTOR2F_NULL;
		player[i].forward = VECTOR2F_NULL;
		player[i].drag = 1.f;
		player[i].isMoving = sfFalse;
		player[i].timeMoving = 0.f;
		player[i].anothertimer = 0.f;
		player[i].wasAlreadyMoving = sfFalse;
		player[i].nbBullet = 1;
		player[i].bulletTimer = 0.f;
		player[i].origin = vector2f(156.f, 48.f);
		player[i].particlesTimer = 0.f;
		
		player[i].flame.pos = VECTOR2F_NULL;
		player[i].flame.origin = vector2f(72.f, 21.f);
		player[i].flame.scale = vector2f(1.f, 1.f);
		player[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);

		player[i].nbGas = 50;
		player[i].fGasTimer = 0.f;
		player[i].nbLightning = LIGTHNING_SECONDS_REQUIRED;
		player[i].fLightningTimer = 0.f;
		player[i].isLightning = sfFalse;
		player[i].isFlamethrowering = sfFalse;
		player[i].flameThroweringTimer = 1.f;
		player[i].damageTimer = 0.f;
		player[i].damageFactor = 1;
		player[i].nbMine = 0;
		player[i].nbRespawn = 2; // CHANGE to 2
		player[i].hasShield = sfFalse;
		player[i].invulnerabilityTimer = 0.f;
		player[i].color = color(255, 255, 255, 255);

		player[i].ISMOVING = sfFalse;

		//player[i].cid = 

	
		
		for (int j = 0; j < NB_SHADOWS; j++)
		{
			player[i].shadow[j].pos = player[i].pos;

		}
		player[i].shadowsTimer = 0.f;


		//if (i >= nbPlayer)
		//	break;
	}

	common.multiplier = 1;
	common.multiplierTimer = 0.f;
	common.score = 0;
	common.countdown = 5;
	common.fcountdownTimer = 0.f;
}

void updatePlayer(Window* _window)
{
	float dt = getDeltaTime();

	int nbPlayerAlive = nbPlayer;

	for (int i = 0; i < nbPlayer; i++)
	{
		// death
		if (player[i].nbRespawn < 0) {
			nbPlayerAlive -= 1;
			player[i].pos = vector2f(-1000.f, -1000.f);
			player[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
			if (nbPlayerAlive <= 0) {
				isGameOver = sfTrue;
			}
			continue;
		}


		// shadows
		player[i].shadowsTimer += dt;

		if (player[i].shadowsTimer > 0.02f) {

			for (int j = 0; j < NB_SHADOWS; j++)
			{
				if (j < NB_SHADOWS - 1) {
					player[i].shadow[j].pos = player[i].shadow[j + 1].pos;
				}
				else {
					player[i].shadow[j].pos = player[i].pos;
				}
			}

			player[i].shadowsTimer = 0.f;
		}




		float LStickYValue = getStickPos(i, sfTrue, sfFalse);
		float LStickXValue = getStickPos(i, sfTrue, sfTrue);

		player[i].isMoving = sfFalse;

		player[i].bulletTimer += dt;
		player[i].particlesTimer += dt;

		if (LStickYValue < -10.f) {
			if (!player[i].ISMOVING) {
				player[i].forward = VECTOR2F_NULL;
			}
			player[i].forward.y -= dt * LStickYValue;
			player[i].isMoving = sfTrue;
			player[i].ISMOVING = sfTrue;
		}
		if (LStickYValue > 10.f) {
			if (!player[i].ISMOVING) {
				player[i].forward = VECTOR2F_NULL;
			}
			player[i].forward.y += dt * -LStickYValue;
			player[i].isMoving = sfTrue;
			player[i].ISMOVING = sfTrue;
		}
		if (LStickXValue < -10.f) {
			if (!player[i].ISMOVING) {
				player[i].forward = VECTOR2F_NULL;
			}
			player[i].forward.x -= dt * -LStickXValue;
			player[i].isMoving = sfTrue;
			player[i].ISMOVING = sfTrue;
		}
		if (LStickXValue > 10.f) {
			if (!player[i].ISMOVING) {
				player[i].forward = VECTOR2F_NULL;
			}
			player[i].forward.x += dt * LStickXValue;
			player[i].isMoving = sfTrue;
			player[i].ISMOVING = sfTrue;
		}

		


		player[i].forward = Normalize(player[i].forward);
		if (player[i].isMoving) {
			player[i].timeMoving += dt;
			player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
			player[i].anothertimer += dt;
			player[i].anothertimer = MIN(player[i].anothertimer, 1.f);
			player[i].anothertimer = player[i].timeMoving;
			
			if (player[i].wasAlreadyMoving) {
				player[i].velocity = VECTOR2F_NULL;
				player[i].wasAlreadyMoving = sfFalse;
			}

			player[i].previousForward = player[i].forward;
		}
		else {

			player[i].ISMOVING = sfFalse;

			player[i].wasAlreadyMoving = sfTrue;
			player[i].timeMoving -= getDeltaTime();
			player[i].velocity = LerpVector(VECTOR2F_NULL, player[i].velocity, player[i].timeMoving);

			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
			player[i].forward = LerpVector(VECTOR2F_NULL, player[i].previousForward, player[i].timeMoving);
		}
		player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
		player[i].timeMoving = MAX(player[i].timeMoving, 0.f);

		player[i].velocity = MultiplyVector(player[i].forward, player[i].speed * player[i].timeMoving);

		if (player[i].velocity.x < 0.001f && player[i].velocity.x > -0.001f && !player[i].isMoving) {
			player[i].velocity.x = 0.f;
		}
		if (player[i].velocity.y < 0.001f && player[i].velocity.y > -0.001f && !player[i].isMoving) {
			player[i].velocity.y = 0.f;
		}
		player[i].pos = AddVectors(player[i].pos, MultiplyVector(player[i].velocity, dt));



		if (player[i].pos.x < 156.f) {
			player[i].pos.x = 156.f;
			//player[i].timeMoving -= dt * 2.f;
		}
		if (player[i].pos.x > 1829.f) {
			player[i].pos.x = 1829.f;
			//player[i].timeMoving -= dt * 2.f;
			//player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.y < 48.f) {
			player[i].pos.y = 48.f;
			//player[i].timeMoving -= dt * 2.f;
			//player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.y > 990.f) {
			player[i].pos.y = 990.f;
			//player[i].timeMoving -= dt * 2.f;
			//player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}





		// invulnerabilty
		if (player[i].invulnerabilityTimer > 0.f) {

			if (player[i].invulnerabilityTimer > 2.7f) {
				setVibration(i, 0.7f, 0.7f);
			}
			else {
				setVibration(i, 0.f, 0.f);
			}

			if (player[i].invulnerabilityTimer > 2.9f) {
				player[i].color.g = 0;
				player[i].color.b = 0;
			}
			else {
				player[i].color.g = 255;
				player[i].color.b = 255;
			}

			float fColor = fabs(cosf(player[i].invulnerabilityTimer));
			//float fColor = fabs(cosf(player[i].invulnerabilityTimer)) * 0.5f + 0.5f;
			//fColor -= 0.5f;
			//fColor *= 2.f;
			sfUint8 color = fColor * 255;
			player[i].color.a = color;
			player[i].invulnerabilityTimer -= dt;

		}
		else {
			player[i].color = color(255, 255, 255, 255);
		}


		// Shots
		
		player[i].fGasTimer += dt;

		//if (player[i].isLightning) {
		//	player[i].fLightningTimer += dt * 3.f;
		//
		//	if (player[i].fLightningTimer > 1.f) {
		//		player[i].nbLightning += 1;
		//		player[i].fLightningTimer = 0.f;
		//	}
		//}
		//else if (player[i].nbLightning > 0 && player[i].nbLightning <= LIGTHNING_SECONDS_REQUIRED) {
		//	player[i].fLightningTimer += dt;
		//	if (player[i].fLightningTimer > 1.f) {
		//		player[i].nbLightning -= 1;
		//		player[i].fLightningTimer = 0.f;
		//	}
		//
		//}

		if (player[i].nbLightning > 0) {
			player[i].fLightningTimer += dt;
			if (player[i].fLightningTimer > 1.f) {
				player[i].nbLightning -= 1;
				player[i].fLightningTimer = 0.f;
			}

		}

		if (isAControllerButtonPressedOrKeyboard(i, sfKeySpace, LB) && player[i].bulletTimer > 0.2f && !player[i].isLightning && !player[i].isFlamethrowering) {
			PlayASound("blaster", sfFalse);
			if (player[i].bulletTimer > 2.f) {
				if (player[i].nbBullet == 1) {
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 0.f);
				}
				else if (player[i].nbBullet == 2) {
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, -5.f);
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 5.f);
				}
				else if (player[i].nbBullet == 3) {
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, -5.f);
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 0.f);
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 5.f);
				}
				player[i].bulletTimer = -0.1f;
			}
			else {
				if (player[i].nbBullet == 1) {
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 0.f);
				}
				else if (player[i].nbBullet == 2) {
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, -5.f);
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 5.f);
				}
				else if (player[i].nbBullet == 3) {
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, -5.f);
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 0.f);
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 5.f);
				}
				player[i].bulletTimer = 0.0f;
			}

		}
		// buttons to change
		// TODO cheks if PC or controller for releasing a button or mb if both or released, yeah better, i agree, thanks man, am i alone or what
		else if (getTriggerValue(i, sfTrue) > 0.1f && player[i].nbLightning <= 0 && !player[i].isLightning && !player[i].isFlamethrowering) { // no timer but 15 seconds condition
			PlayASound("laserSfx", sfFalse);
			createBullets(PLAYER_LASER, i, player[i].pos, 0.f);
			player[i].isLightning = sfTrue;
		}
		else if (isAControllerButtonPressedOrKeyboard(i, sfKeyM, RB) && player[i].nbMine >= KILL_COUNT_REQUIRED && !player[i].isLightning && !player[i].isFlamethrowering) { // no timer but 15 kills condition
			createBullets(PLAYER_MINES, i, player[i].pos, 0.f);
			player[i].nbMine = 0;
		}
		else if (getTriggerValue(i, sfFalse) > 0.1f && player[i].nbGas > 0 && player[i].fGasTimer > 0.02f && !player[i].isLightning) { // and the gauge is not empty
			int random = iRand(0, 1);
			int randomDirection = iRand(0, 1);
			float direction = 300.f;
			if (randomDirection)
				direction *= -1;
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 20.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 2.f, 2.f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 20.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 1.f, 1.f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f + (player[i].velocity.y / 25.f), 25.f + (player[i].velocity.y / 25.f), direction, 10.f, 4000.f + (player[i].velocity.x * 5.f), 4000.f + (player[i].velocity.x * 5.f), 30.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.1f);
			CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 5000.f, 5000.f, 30.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 2, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.1f);
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f + (player[i].velocity.y / 15.f), 25.f + (player[i].velocity.y / 15.f), direction, 10.f, 3000.f + (player[i].velocity.x * 2.f) + (fabs(player[i].velocity.y) * 2.f), 3000.f + (player[i].velocity.x * 2.f) + (fabs(player[i].velocity.y) * 2.f), 25.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
			if (!player[i].isFlamethrowering) {
				player[i].isFlamethrowering = sfTrue;
				createBullets(PLAYER_FLAMETHROWER, i, player[i].pos, 0.f);
			}
			player[i].fGasTimer = 0.f;
			player[i].nbGas -= 1;
			player[i].flameThroweringTimer = 0.f;
		}

		if (player[i].isLightning)
			player[i].speed = PLAYER_SPEED * 7.f / 10.f;
		else
			player[i].speed = PLAYER_SPEED;

		// TODO know better when you're not flamethrowering lmao
		player[i].flameThroweringTimer += dt;
		if (player[i].flameThroweringTimer > 0.3f)
			player[i].isFlamethrowering = sfFalse; // will erase the bullet


		// increase damage item
		if (player[i].damageTimer > 0.f) {
			player[i].damageTimer -= dt;
			player[i].damageFactor = 2;
		}
		else
			player[i].damageFactor = 1;



		// Particles
		if (player[i].particlesTimer > 0.1f) {
			int random = iRand(0, 1);
			int randomDirection = iRand(0, 1);
			float direction = 500.f;
			if (randomDirection)
				direction *= -1;
			CreateParticles(SubstractVectors(player[i].pos, vector2f(116.f, 2.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), 160.f, 200.f, direction, 10.f, 100.f, 300.f, 1.f, color(255, 255, 255, 255), color(0, 0, 0, 0), 0.5f, 1.f, 1, "particles", IntRect(0, 17 * random + (34 * i), 19, 17), NULL, 0.f, 0.f, 0.5f);
			player[i].particlesTimer = 0.f;
		}

		// Flame
		player[i].flame.pos = AddVectors(player[i].pos, vector2f(-113.f, 1.f));
		player[i].flame.scale = LerpVector(vector2f(1.0f, 1.0f), vector2f(0.5f, 0.5f), 1.f - player[i].timeMoving);

	}
}

void damagePlayer(int _playerId, int _damage)
{
	if (player[_playerId].invulnerabilityTimer > 0.f) {
		NULL;
	}
	else if (player[_playerId].hasShield) {
		player[_playerId].hasShield = sfFalse;
	}
	else {
		player[_playerId].life -= _damage;
		
		player[_playerId].invulnerabilityTimer = 3.f;

		if (player[_playerId].life <= 0) {
			player[_playerId].life = 3;
			player[_playerId].nbRespawn -= 1;
			player[_playerId].pos = vector2f(300.f, 450.f);
			player[_playerId].timeMoving = 0.f;
		}
	}
}

void increasePlayerKillCount(int _playerId)
{
	for (int i = 0; i < nbPlayer; i++)
	{
		if (i == _playerId) {
			if (player[_playerId].nbMine < KILL_COUNT_REQUIRED)
				player[_playerId].nbMine += 1;
		}
	}

}

void displayPlayer(Window* _window)
{
	if (!isGameOver) {
		for (int i = 0; i < nbPlayer; i++)
		{
			if (player[i].nbRespawn < 0) {
				continue;
			}
			sfSprite_setOrigin(playerSprite, player[i].origin);
			sfSprite_setTexture(playerSprite, playerShapeTexture, sfTrue);

			for (int j = 0; j < NB_SHADOWS; j++)
			{
				sfSprite_setPosition(playerSprite, player[i].shadow[j].pos);
				sfSprite_setColor(playerSprite, color(255, 255, 255, j * 255 / NB_SHADOWS / NB_SHADOWS * 2));
				sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);
			}

			sfSprite_setColor(playerSprite, player[i].color);

			sfSprite_setTexture(playerSprite, player[i].flame.texture, sfTrue);
			sfSprite_setPosition(playerSprite, player[i].flame.pos);
			sfSprite_setOrigin(playerSprite, player[i].flame.origin);
			sfSprite_setScale(playerSprite, player[i].flame.scale);
			sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

			//if (player[i].invulnerabilityTimer > 2.9f)
			//	sfSprite_setTexture(playerSprite, playerHitTexture, sfTrue);
			//else
			sfSprite_setTexture(playerSprite, player[i].texture, sfTrue);

			sfSprite_setPosition(playerSprite, player[i].pos);
			sfSprite_setOrigin(playerSprite, player[i].origin);
			sfSprite_setScale(playerSprite, vector2f(1.f, 1.f));
			sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

			player[i].bounds = sfSprite_getGlobalBounds(playerSprite);
		}
	}
}

void deinitPlayer()
{
	sfSprite_destroy(playerSprite);
	//free(player[0].texture);
	//free(player[1].texture);
}

sfVector2f getClosestPlayerPos(sfVector2f _pos)
{
	sfVector2f pos = player[0].pos;
	if (nbPlayer <= 1)
		return pos;

	sfVector2f between = CreateVector(pos, _pos);
	float mag = GetSqrMagnitude(between);


	sfVector2f closestPos = pos;

	for (int i = 1; i < nbPlayer; i++)
	{
		sfVector2f otherPos = player[i].pos;

		sfVector2f otherBetween = CreateVector(otherPos, _pos);
		float otherMag = GetSqrMagnitude(otherBetween);

		if (otherMag < mag)
			closestPos = otherPos;
	}

	return closestPos;
}

sfVector2f getPlayerPos(int _playerId)
{
	return player[_playerId].pos;
}

sfVector2f getPlayerVelocity(int _playerId)
{
	return player[_playerId].velocity;
}

int getPlayerDamageFactor(int _playerId)
{
	return player[_playerId].damageFactor;
}
