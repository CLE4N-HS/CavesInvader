#include "player.h"
#include "customMath.h"
#include "gamepadx.h"
#include "bullets.h"
#include "particlesSystemManager.h"


sfSprite* playerSprite;

sfTexture* playerHitTexture;

void initPlayer(Window* _window)
{
	playerSprite = sfSprite_create();

	playerHitTexture = GetTexture("playerHit");
	
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
		player[i].life = 3;
		player[i].speed = PLAYER_SPEED;
		player[i].velocity = VECTOR2F_NULL;
		player[i].forward = VECTOR2F_NULL;
		player[i].drag = 1.f;
		player[i].isMoving = sfFalse;
		player[i].timeMoving = 0.f;
		player[i].anothertimer = 0.f;
		player[i].wasalreadymovingtbh = sfFalse;
		player[i].nbBullet = 1;
		player[i].bulletTimer = 0.f;
		player[i].origin = vector2f(156.f, 48.f);
		player[i].particlesTimer = 0.f;
		
		player[i].flame.pos = VECTOR2F_NULL;
		player[i].flame.origin = vector2f(72.f, 21.f);
		player[i].flame.scale = vector2f(1.f, 1.f);
		player[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);

		player[i].nbGas = 0;
		player[i].fGasTimer = 0.f;
		player[i].nbLightning = LIGTHNING_SECONDS_REQUIRED;
		player[i].fLightningTimer = 0.f;
		player[i].isLightning = sfFalse;
		player[i].isFlamethrowering = sfFalse;
		player[i].nbMine = 0;
		player[i].nbRespawn = 3;
		player[i].hasShield = sfFalse;
		player[i].invulnerabilityTimer = 0.f;
		player[i].color = color(255, 255, 255, 255);


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

	for (int i = 0; i < nbPlayer; i++)
	{
		float LStickYValue = getStickPos(i, sfTrue, sfFalse);
		float LStickXValue = getStickPos(i, sfTrue, sfTrue);

		player[i].isMoving = sfFalse;

		//player[i].forward = VECTOR2F_NULL;
		player[i].bulletTimer += dt;
		player[i].particlesTimer += dt;

		//player[i].velocity = VECTOR2F_NULL;
		if (LStickYValue < -10.f) {
			player[i].forward.y -= dt * LStickYValue;
			player[i].isMoving = sfTrue;
		}
		if (LStickYValue > 10.f) {
			player[i].forward.y += dt * -LStickYValue;
			player[i].isMoving = sfTrue;
		}
		if (LStickXValue < -10.f) {
			player[i].forward.x -= dt * -LStickXValue;
			player[i].isMoving = sfTrue;
		}
		if (LStickXValue > 10.f) {
			player[i].forward.x += dt * LStickXValue;
			player[i].isMoving = sfTrue;
		}

		player[i].forward = Normalize(player[i].forward);
		if (player[i].isMoving) {
			player[i].timeMoving += dt;
			player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
			player[i].anothertimer += dt;
			player[i].anothertimer = MIN(player[i].anothertimer, 1.f);
			player[i].anothertimer = player[i].timeMoving;
			//
			if (player[i].wasalreadymovingtbh) {
				player[i].velocity = VECTOR2F_NULL;
				player[i].wasalreadymovingtbh = sfFalse;
			}

			player[i].wasnt = sfTrue;

			player[i].previousForward = player[i].forward;
		}
		else {

			if (player[i].wasnt) {
				player[i].wasnt = sfFalse;
				//player[i].timeMoving = 0.f;
			}


			player[i].wasalreadymovingtbh = sfTrue;
			player[i].timeMoving -= getDeltaTime();
			player[i].velocity = LerpVector(VECTOR2F_NULL, player[i].velocity, player[i].timeMoving);

			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
			player[i].forward = LerpVector(VECTOR2F_NULL, player[i].previousForward, player[i].timeMoving);
		}
		player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
		player[i].timeMoving = MAX(player[i].timeMoving, 0.f);

		// TODO CLE4N that and don't stop the player but mb add a drag that is more consequent if you don't move


		//player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * player[i].timeMoving)
		player[i].velocity = MultiplyVector(player[i].forward, player[i].speed * player[i].anothertimer);

		//player[i].velocity.x += gamepadRight * dt * 5.f;
		//player[i].velocity.y += gamepadDown * dt * 5.f;

		//player[i].velocity = MultiplyVector(player[i].velocity, 1.f / (1.f +  (dt * player[i].drag * 10000.f)
		//player[i].velocity = AddVectors(player[i].velocity, force);
		//player[i].velocity = MultiplyVector(player[i].velocity, 1.f - dt);
		// drag is useless now
		if (player[i].velocity.x < 0.001f && player[i].velocity.x > -0.001f && !player[i].isMoving) {
			player[i].velocity.x = 0.f;
		}
		if (player[i].velocity.y < 0.001f && player[i].velocity.y > -0.001f && !player[i].isMoving) {
			player[i].velocity.y = 0.f;
		}
		player[i].pos = AddVectors(player[i].pos, MultiplyVector(player[i].velocity, dt));



		if (player[i].pos.x < 156.f) {
			player[i].pos.x = 156.f;
			player[i].timeMoving -= dt * 2.f;
		}
		if (player[i].pos.x > 1829.f) {
			player[i].pos.x = 1829.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.y < 48.f) {
			player[i].pos.y = 48.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.y > 990.f) {
			player[i].pos.y = 990.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}

		//printf("%f\n", player[0].timeMoving);
		//printf("%f, %f\n", player[0].velocity.x, player[0].velocity.y);

		//player[i].pos = vector2f(MAX(player[i].pos.x, 0.f), MAX(player[i].pos.y, 0.f));
		//player[i].pos = vector2f(MIN(player[i].pos.x, 1920.f - 278.f), MIN(player[i].pos.y, 1080.f - 139.f));





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

			float fColor = fabs(cosf(player[i].invulnerabilityTimer)) * 0.5f + 0.5f;
			fColor -= 0.5f;
			fColor *= 2.f;
			sfUint8 color = fColor * 255;
			player[i].color.a = color;
			player[i].invulnerabilityTimer -= dt;

		}
		else {
			player[i].color = color(255, 255, 255, 255);
		}


		// Shots
		
		player[i].fGasTimer += dt;

		if (player[i].isLightning) {
			player[i].fLightningTimer += dt * 3.f;

			if (player[i].fLightningTimer > 1.f) {
				player[i].nbLightning += 1;
				player[i].fLightningTimer = 0.f;
			}
		}
		else if (player[i].nbLightning > 0 && player[i].nbLightning <= LIGTHNING_SECONDS_REQUIRED) {
			player[i].fLightningTimer += dt;
			if (player[i].fLightningTimer > 1.f) {
				player[i].nbLightning -= 1;
				player[i].fLightningTimer = 0.f;
			}

		}

		// TODO : priority order between shots
		if (isAControllerButtonPressedOrKeyboard(i, sfKeySpace, LB) && player[i].bulletTimer > 0.2f) {
			if (player[i].bulletTimer > 2.f) {
				if (player[i].nbBullet == 1) {
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 0.f);
				}
				else if (player[i].nbBullet == 2) {
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, -10.f);
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 10.f);
				}
				else if (player[i].nbBullet == 3) {
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, -10.f);
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 0.f);
					createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 10.f);
				}
				player[i].bulletTimer = -0.1f;
			}
			else {
				if (player[i].nbBullet == 1) {
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 0.f);
				}
				else if (player[i].nbBullet == 2) {
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, -10.f);
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 10.f);
				}
				else if (player[i].nbBullet == 3) {
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, -10.f);
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 0.f);
					createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 10.f);
				}
				player[i].bulletTimer = 0.0f;
			}

		}
		// buttons to change
		// TODO cheks if PC or controller for releasing a button or mb if both or released, yeah better, i agree, thanks man, am i alone or what
		else if (getTriggerValue(i, sfTrue) > 0.1f && player[i].nbLightning <= 0 && !player[i].isLightning) { // no timer but 15 seconds condition
			createBullets(PLAYER_LASER, i, player[i].pos, 0.f);
			player[i].isLightning = sfTrue;
		}
		else if (isAControllerButtonPressedOrKeyboard(i, sfKeyM, RB) && player[i].nbMine >= KILL_COUNT_REQUIRED) { // no timer but 15 kills condition
			createBullets(PLAYER_MINES, i, player[i].pos, 0.f);
			player[i].nbMine = 0;
		}
		else if (getTriggerValue(i, sfFalse) > 0.1f && player[i].nbGas > 0 && player[i].fGasTimer > 0.02f) { // and the gauge is not empty
			int random = iRand(0, 1);
			int randomDirection = iRand(0, 1);
			float direction = 300.f;
			if (randomDirection)
				direction *= -1;
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 20.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 2.f, 2.f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 20.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 1.f, 1.f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f + (player[i].velocity.y / 25.f), 25.f + (player[i].velocity.y / 25.f), direction, 10.f, 4000.f + (player[i].velocity.x * 5.f), 4000.f + (player[i].velocity.x * 5.f), 30.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.1f);
			CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 5000.f, 5000.f, 30.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.1f);
			//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f + (player[i].velocity.y / 15.f), 25.f + (player[i].velocity.y / 15.f), direction, 10.f, 3000.f + (player[i].velocity.x * 2.f) + (fabs(player[i].velocity.y) * 2.f), 3000.f + (player[i].velocity.x * 2.f) + (fabs(player[i].velocity.y) * 2.f), 25.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
			if (!player[i].isFlamethrowering) {
				player[i].isFlamethrowering = sfTrue;
				createBullets(PLAYER_FLAMETHROWER, i, player[i].pos, 0.f);
			}
			player[i].fGasTimer = 0.f;
			player[i].nbGas -= 1;
		}

		if (player[i].isLightning)
			player[i].speed = PLAYER_SPEED * 7.f / 10.f;
		else
			player[i].speed = PLAYER_SPEED;


		// TODO know better when you're not flamethrowering lmao
		if (player[i].bulletTimer > 0.3f)
			player[i].isFlamethrowering = sfFalse; // will erase the bullet




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








		//float gamepadUp = Gamepad_isJoystickMoved(i, STICKLY);
		//float gamepadDown = Gamepad_isJoystickMoved(i, STICKLY);
		//float gamepadLeft = Gamepad_isJoystickMoved(i, STICKLX);
		//float gamepadRight = Gamepad_isJoystickMoved(i, STICKLX);
		//
		//player[i].isMoving = sfFalse;
		//
		//player[i].forward = VECTOR2F_NULL;
		//player[i].bulletTimer += dt;
		//player[i].particlesTimer += dt;
		//
		////player[i].velocity = VECTOR2F_NULL;
		//if (gamepadUp < -15.f) {
		//	player[i].forward.y -= dt * -gamepadUp;
		//	player[i].isMoving = sfTrue;
		//}
		//if (gamepadDown > 15.f) {
		//	player[i].forward.y += dt * gamepadDown;
		//	player[i].isMoving = sfTrue;
		//}
		//if (gamepadLeft < -15.f) {
		//	player[i].forward.x -= dt * -gamepadLeft;
		//	player[i].isMoving = sfTrue;
		//}
		//if (gamepadRight > 15.f) {
		//	player[i].forward.x += dt * gamepadLeft;
		//	player[i].isMoving = sfTrue;
		//}
		//
		////if (player[i].isMoving) {
		////	player[i].timerMove += dt;
		////	player[i].timerMove = MAX(player[i].timerMove, 1.f);
		////	player[i].previousForward = Normalize(player[i].velocity);
		////	if (player[i].previousForward.x < 10.f && player[i].previousForward.x > 0.f) {
		////		player[i].previousForward.x = 10.f;
		////	}
		////	if (player[i].previousForward.y < 10.f && player[i].previousForward.y > 0.f) {
		////		player[i].previousForward.y = 10.f;
		////	}
		////	if (player[i].previousForward.x > -10.f && player[i].previousForward.x < 0.f) {
		////		player[i].previousForward.x = -10.f;
		////	}
		////	if (player[i].previousForward.y > -10.f && player[i].previousForward.y < 0.f) {
		////		player[i].previousForward.y = -10.f;
		////	}
		////	player[i].previousForward = Normalize(player[i].previousForward);
		////}
		////else {
		////	player[i].previousForward = MultiplyVector(player[i].forward, player[i].timerMove);
		////	player[i].timerMove -= dt;
		////}
		//
		//
		//if (i == 0)
		//{
		//	if (sfKeyboard_isKeyPressed(sfKeyZ) || Gamepad_isJoystickMoved(0, CROSSY_XBOX) > 30.f) {
		//		gamepadUp = -100.f;
		//		player[i].forward.y += dt * gamepadUp;
		//		player[i].isMoving = sfTrue;
		//	}
		//	if (sfKeyboard_isKeyPressed(sfKeyS) || Gamepad_isJoystickMoved(0, CROSSY_XBOX) < -30.f) {
		//		gamepadDown = 100.f;
		//		player[i].forward.y += dt * gamepadDown;
		//		player[i].isMoving = sfTrue;
		//	}
		//	if (sfKeyboard_isKeyPressed(sfKeyQ) || Gamepad_isJoystickMoved(0, CROSSX_XBOX) < -30.f) {
		//		gamepadLeft = -100.f;
		//		player[i].forward.x += dt * gamepadLeft;
		//		player[i].isMoving = sfTrue;
		//	}
		//	if (sfKeyboard_isKeyPressed(sfKeyD) || Gamepad_isJoystickMoved(0, CROSSX_XBOX) > 30.f) {
		//		gamepadRight = 100.f;
		//		player[i].forward.x += dt * gamepadRight;
		//		player[i].isMoving = sfTrue;
		//	}
		//}
		//else if (i == 1)
		//{
		//	if (sfKeyboard_isKeyPressed(sfKeyUp)) {
		//		player[i].velocity.y -= player[i].speed * dt;
		//		//player[i].velocity = AddVectors(player[i].velocity, MultiplyVector(Normalize(player[i].speed)
		//		player[i].isMoving = sfTrue;
		//		gamepadDown = -100.f;
		//	}
		//	if (sfKeyboard_isKeyPressed(sfKeyDown)) {
		//		player[i].velocity.y += player[i].speed * dt;
		//		player[i].isMoving = sfTrue;
		//		gamepadDown = 100.f;
		//	}
		//	if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
		//		player[i].velocity.x -= player[i].speed * dt;
		//		player[i].isMoving = sfTrue;
		//		gamepadRight = -100.f;
		//	}
		//	if (sfKeyboard_isKeyPressed(sfKeyRight)) {
		//		player[i].velocity.x += player[i].speed * dt;
		//		player[i].isMoving = sfTrue;
		//		gamepadRight = 100.f;
		//	}
		//}
		//
		//
		//
		//player[i].forward = Normalize(player[i].forward);
		////printf("%f, %f\n", player[i].forward.x, player[i].forward.y);
		//if (player[i].isMoving) {
		//	//player[i].timeMoving += 1.f;
		//	player[i].timeMoving += dt;
		//	player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
		//	player[i].anothertimer += dt;
		//	player[i].anothertimer = MIN(player[i].anothertimer, 1.f);
		//	player[i].anothertimer = player[i].timeMoving;
		//	//
		//	if (player[i].wasalreadymovingtbh) {
		//		player[i].velocity = VECTOR2F_NULL;
		//		//player[i].timeMoving = 0.f;
		//		player[i].wasalreadymovingtbh = sfFalse;
		//		//player[i].forward = VECTOR2F_NULL;
		//	}
		//
		//
		//	player[i].wasnt = sfTrue;
		//
		//	player[i].previousForward = player[i].forward;
		//}
		//else {
		//
		//	if (player[i].wasnt) {
		//		player[i].wasnt = sfFalse;
		//		//player[i].timeMoving = 0.f;
		//	}
		//
		//
		//	player[i].wasalreadymovingtbh = sfTrue;
		//	player[i].timeMoving -= getDeltaTime();
		//	player[i].velocity = LerpVector(VECTOR2F_NULL, player[i].velocity, player[i].timeMoving);
		//	
		//	player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		//	player[i].forward = LerpVector(VECTOR2F_NULL, player[i].previousForward, player[i].timeMoving);
		//}
		//player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
		//player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		//
		//// TODO CLE4N that and don't stop the player but mb add a drag that is more consequent if you don't move
		//
		//
		////player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * player[i].timeMoving)
		//player[i].velocity = MultiplyVector(player[i].forward, player[i].speed * player[i].anothertimer);
		//
		////player[i].velocity.x += gamepadRight * dt * 5.f;
		////player[i].velocity.y += gamepadDown * dt * 5.f;
		//
		////player[i].velocity = MultiplyVector(player[i].velocity, 1.f / (1.f +  (dt * player[i].drag * 10000.f)
		////player[i].velocity = AddVectors(player[i].velocity, force);
		////player[i].velocity = MultiplyVector(player[i].velocity, 1.f - dt);
		//// drag is useless now
		//if (player[i].velocity.x < 0.001f && player[i].velocity.x > -0.001f && !player[i].isMoving) {
		//	player[i].velocity.x = 0.f;
		//}
		//if (player[i].velocity.y < 0.001f && player[i].velocity.y > -0.001f && !player[i].isMoving) {
		//	player[i].velocity.y = 0.f;
		//}
		//player[i].pos = AddVectors(player[i].pos, MultiplyVector(player[i].velocity, dt));
		//
		//
		//
		//if (player[i].pos.x < 156.f) {
		//	player[i].pos.x = 156.f;
		//	player[i].timeMoving -= dt * 2.f;
		//}
		//if (player[i].pos.x > 1829.f) {
		//	player[i].pos.x = 1829.f;
		//	player[i].timeMoving -= dt * 2.f;
		//	player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		//}
		//if (player[i].pos.y < 48.f) {
		//	player[i].pos.y = 48.f;
		//	player[i].timeMoving -= dt * 2.f;
		//	player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		//}
		//if (player[i].pos.y > 990.f) {
		//	player[i].pos.y = 990.f;
		//	player[i].timeMoving -= dt * 2.f;
		//	player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		//}
		//
		////printf("%f\n", player[0].timeMoving);
		////printf("%f, %f\n", player[0].velocity.x, player[0].velocity.y);
		//
		////player[i].pos = vector2f(MAX(player[i].pos.x, 0.f), MAX(player[i].pos.y, 0.f));
		////player[i].pos = vector2f(MIN(player[i].pos.x, 1920.f - 278.f), MIN(player[i].pos.y, 1080.f - 139.f));
		//
		//
		//
		//
		//
		//// invulnerabilty
		//if (player[i].invulnerabilityTimer > 0.f) {
		//
		//	if (player[i].invulnerabilityTimer > 2.9f) {
		//		player[i].color.g = 0;
		//		player[i].color.b = 0;
		//	}
		//	else {
		//		player[i].color.g = 255;
		//		player[i].color.b = 255;
		//	}
		//
		//	float fColor = fabs(cosf(player[i].invulnerabilityTimer)) * 0.5f + 0.5f;
		//	fColor -= 0.5f;
		//	fColor *= 2.f;
		//	sfUint8 color = fColor * 255;
		//	player[i].color.a = color;
		//	player[i].invulnerabilityTimer -= dt;
		//
		//}
		//else
		//	player[i].color = color(255, 255, 255, 255);
		//
		//
		//// Shots
		//// TODO : priority order between shots
		//if (isKeyboardOrControllerButtonPressed(sfKeySpace, LB_XBOX) && player[i].bulletTimer > 0.2f) {
		//	if (player[i].bulletTimer > 2.f) {
		//		if (player[i].nbBullet == 1) {
		//			createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 0.f);
		//		}
		//		else if (player[i].nbBullet == 2) {
		//			createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, -10.f);
		//			createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 10.f);
		//		}
		//		else if (player[i].nbBullet == 3) {
		//			createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, -10.f);
		//			createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 0.f);
		//			createBullets(PLAYER_CHARGED_BULLET, i, player[i].pos, 10.f);
		//		}
		//		player[i].bulletTimer = -0.1f;
		//	}
		//	else {
		//		if (player[i].nbBullet == 1) {
		//			createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 0.f);
		//		}
		//		else if (player[i].nbBullet == 2) {
		//			createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, -10.f);
		//			createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 10.f);
		//		}
		//		else if (player[i].nbBullet == 3) {
		//			createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, -10.f);
		//			createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 0.f);
		//			createBullets(PLAYER_BASIC_BULLET, i, player[i].pos, 10.f);
		//		}
		//		player[i].bulletTimer = 0.0f;
		//	}
		//
		//}
		//// buttons to change
		//// TODO cheks if PC or controller for releasing a button or mb if both or released, yeah better, i agree, thanks man, am i alone or what
		//else if (isKeyboardOrControllerButtonMoved(sfKeyL, TRIGGER_L2_XBOX, sfFalse, 10.f) && player[i].nbLightning <= 0 && !player[i].isLightning) { // no timer but 15 seconds condition
		//	createBullets(PLAYER_LASER, i, player[i].pos, 0.f);
		//	player[i].isLightning = sfTrue;
		//}
		//else if (isKeyboardOrControllerButtonPressed(sfKeyM, RB_XBOX) && player[i].bulletTimer > 0.5f) { // no timer but 15 kills condition
		//	createBullets(PLAYER_MINES, i, player[i].pos, 0.f);
		//	player[i].bulletTimer = 0.f;
		//}
		//else if (isKeyboardOrControllerButtonMoved(sfKeyF, TRIGGER_R2_XBOX, sfFalse, 10.f) && player[i].bulletTimer > 0.02f) { // and the gauge is not empty
		//	int random = iRand(0, 1);
		//	int randomDirection = iRand(0, 1);
		//	float direction = 300.f;
		//	if (randomDirection)
		//		direction *= -1;
		//	//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 20.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 2.f, 2.f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
		//	//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 2000.f /*+ (player[i].velocity.x * 7000.f)*/, 20.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 1.f, 1.f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
		//	//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f + (player[i].velocity.y / 25.f), 25.f + (player[i].velocity.y / 25.f), direction, 10.f, 4000.f + (player[i].velocity.x * 5.f), 4000.f + (player[i].velocity.x * 5.f), 30.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.1f);
		//	CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f, 25.f, direction, 10.f, 5000.f, 5000.f, 30.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.1f);
		//	//CreateParticles(AddVectors(player[i].pos, vector2f(50.f, 23.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), -25.f + (player[i].velocity.y / 15.f), 25.f + (player[i].velocity.y / 15.f), direction, 10.f, 3000.f + (player[i].velocity.x * 2.f) + (fabs(player[i].velocity.y) * 2.f), 3000.f + (player[i].velocity.x * 2.f) + (fabs(player[i].velocity.y) * 2.f), 25.f, color(0, 0, 0, 0), color(0, 0, 0, 0), 0.5f, 0.5f, 1, "particles", IntRect(0, 68 + 17 * random, 19, 17), NULL, 0.f, 0.f, 0.5f);
		//	if (!player[i].isFlamethrowering) {
		//		player[i].isFlamethrowering = sfTrue;
		//		createBullets(PLAYER_FLAMETHROWER, i, player[i].pos, 0.f);
		//	}
		//	player[i].bulletTimer = 0.f;
		//}
		//
		//if (player[i].isLightning)
		//	player[i].speed = PLAYER_SPEED * 7.f / 10.f;
		//else
		//	player[i].speed = PLAYER_SPEED;
		//
		//
		//// TODO know better when you're not flamethrowering lmao
		//if (player[i].bulletTimer > 0.3f)
		//	player[i].isFlamethrowering = sfFalse; // will erase the bullet
		//
		//
		//
		//
		//// Particles
		//if (player[i].particlesTimer > 0.1f) {
		//	int random = iRand(0, 1);
		//	int randomDirection = iRand(0, 1);
		//	float direction = 500.f;
		//	if (randomDirection)
		//		direction *= -1;
		//	CreateParticles(SubstractVectors(player[i].pos, vector2f(116.f, 2.f)), vector2f(1.f, 1.f), vector2f(0.f, 0.f), vector2f(10.f, 12.f), 160.f, 200.f, direction, 10.f, 100.f, 300.f, 1.f, color(255, 255, 255, 255), color(0, 0, 0, 0), 0.5f, 1.f, 1, "particles", IntRect(0, 17 * random + (34 * i), 19, 17), NULL, 0.f, 0.f, 0.5f);
		//	player[i].particlesTimer = 0.f;
		//}
		//
		//// Flame
		//player[i].flame.pos = AddVectors(player[i].pos, vector2f(-113.f, 1.f));
		//player[i].flame.scale = LerpVector(vector2f(1.0f, 1.0f), vector2f(0.5f, 0.5f), 1.f - player[i].timeMoving);

	//}

	//for (int i = 0; i < MAX_PLAYER; i++)
	//{
	//	float gamepadUp = Gamepad_isJoystickMoved(i, STICKLY);
	//	float gamepadDown = Gamepad_isJoystickMoved(i, STICKLY);
	//	float gamepadLeft = Gamepad_isJoystickMoved(i, STICKLX);
	//	float gamepadRight = Gamepad_isJoystickMoved(i, STICKLX);
	
	//	player[i].isMoving = sfFalse;
	
	//	//player[i].velocity = VECTOR2F_NULL;
	//	if (gamepadUp < -10.f) {
	//		player[i].velocity.y -= player[i].speed.y * -gamepadUp ;
	//		player[i].isMoving = sfTrue;
	//	}
	//	if (gamepadDown > 10.f) {
	//		player[i].velocity.y += player[i].speed.y * gamepadDown ;
	//		player[i].isMoving = sfTrue;
	//	}
	//	if (gamepadLeft < -10.f) {
	//		player[i].velocity.x -= player[i].speed.x * -gamepadLeft ;
	//		player[i].isMoving = sfTrue;
	//	}
	//	if (gamepadRight > 10.f) {
	//		player[i].velocity.x += player[i].speed.x * gamepadLeft;
	//		player[i].isMoving = sfTrue;
	//	}


	//	//if (player[i].isMoving) {
	//	//	player[i].timerMove += dt;
	//	//	player[i].timerMove = MAX(player[i].timerMove, 1.f);
	//	//	player[i].previousForward = Normalize(player[i].velocity);
	//	//	if (player[i].previousForward.x < 10.f && player[i].previousForward.x > 0.f) {
	//	//		player[i].previousForward.x = 10.f;
	//	//	}
	//	//	if (player[i].previousForward.y < 10.f && player[i].previousForward.y > 0.f) {
	//	//		player[i].previousForward.y = 10.f;
	//	//	}
	//	//	if (player[i].previousForward.x > -10.f && player[i].previousForward.x < 0.f) {
	//	//		player[i].previousForward.x = -10.f;
	//	//	}
	//	//	if (player[i].previousForward.y > -10.f && player[i].previousForward.y < 0.f) {
	//	//		player[i].previousForward.y = -10.f;
	//	//	}
	//	//	player[i].previousForward = Normalize(player[i].previousForward);
	//	//}
	//	//else {
	//	//	player[i].previousForward = MultiplyVector(player[i].forward, player[i].timerMove);
	//	//	player[i].timerMove -= dt;
	//	//}


	//	if (i == 0)
	//	{
	//		if (sfKeyboard_isKeyPressed(sfKeyZ)) {
	//			player[i].velocity.y -= player[i].speed.y * dt;
	//			//player[i].velocity = AddVectors(player[i].velocity, MultiplyVector(Normalize(player[i].speed), dt));
	//			player[i].isMoving = sfTrue;
	//			gamepadDown = -100.f;
	//		}
	//		if (sfKeyboard_isKeyPressed(sfKeyS)) {
	//			player[i].velocity.y += player[i].speed.y * dt;
	//			player[i].isMoving = sfTrue;
	//			gamepadDown = 100.f;
	//		}
	//		if (sfKeyboard_isKeyPressed(sfKeyQ)) {
	//			player[i].velocity.x -= player[i].speed.x * dt;
	//			player[i].isMoving = sfTrue;
	//			gamepadRight = -100.f;
	//		}
	//		if (sfKeyboard_isKeyPressed(sfKeyD)) {
	//			player[i].velocity.x += player[i].speed.x * dt;
	//			player[i].isMoving = sfTrue;
	//			gamepadRight = 100.f;
	//		}
	//	}
	//	else if (i == 1)
	//	{
	//		if (sfKeyboard_isKeyPressed(sfKeyUp)) {
	//			player[i].velocity.y -= player[i].speed.y * dt;
	//			//player[i].velocity = AddVectors(player[i].velocity, MultiplyVector(Normalize(player[i].speed), dt));
	//			player[i].isMoving = sfTrue;
	//			gamepadDown = -100.f;
	//		}
	//		if (sfKeyboard_isKeyPressed(sfKeyDown)) {
	//			player[i].velocity.y += player[i].speed.y * dt;
	//			player[i].isMoving = sfTrue;
	//			gamepadDown = 100.f;
	//		}
	//		if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
	//			player[i].velocity.x -= player[i].speed.x * dt;
	//			player[i].isMoving = sfTrue;
	//			gamepadRight = -100.f;
	//		}
	//		if (sfKeyboard_isKeyPressed(sfKeyRight)) {
	//			player[i].velocity.x += player[i].speed.x * dt;
	//			player[i].isMoving = sfTrue;
	//			gamepadRight = 100.f;
	//		}
	//	}




	//	player[i].forward = Normalize(player[i].velocity);
	//	if (player[i].isMoving) {
	//		player[i].timeMoving += getDeltaTime();
	//		player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
	//		//player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * timeMoving);

	//		player[i].previousForward = player[i].forward;
	//	}
	//	else {
	//		player[i].timeMoving -= getDeltaTime();

	//		player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
	//		//player[i].velocity = Normalize(player[i].velocity);
	//	}
	//	//player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * player[i].timeMoving); // right one
	//	player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * player[i].timeMoving);

	//	player[i].velocity.x += gamepadRight * dt * 5.f;
	//	player[i].velocity.y += gamepadDown * dt * 5.f;

	//	//player[i].velocity = MultiplyVector(player[i].velocity, 1.f / (1.f +  (dt * player[i].drag * 10000.f)));
	//	//player[i].velocity = AddVectors(player[i].velocity, force);
	//	player[i].velocity = MultiplyVector(player[i].velocity, 1.f - dt);
	//	// drag is useless now
	//	if (player[i].velocity.x < 0.001f && player[i].velocity.x > -0.001f && !player[i].isMoving) {
	//		player[i].velocity.x = 0.f;
	//	}
	//	if (player[i].velocity.y < 0.001f && player[i].velocity.y > -0.001f && !player[i].isMoving) {
	//		player[i].velocity.y = 0.f;
	//	}
	//	player[i].pos = AddVectors(player[i].pos, player[i].velocity);
	//	
	//	if (player[i].pos.x < 0.f) {
	//		player[i].pos.x = 0.f;
	//		player[i].timeMoving = 0.f;
	//	}
	//	if (player[i].pos.x > 1642.f) {
	//		player[i].pos.x = 1642.f;
	//		player[i].timeMoving = 0.f;
	//	}
	//	if (player[i].pos.y < 0.f) {
	//		player[i].pos.y = 0.f;
	//		player[i].timeMoving = 0.f;
	//	}
	//	if (player[i].pos.y > 941.f) {
	//		player[i].pos.y = 941.f;
	//		player[i].timeMoving = 0.f;
	//	}

	//	printf("%f, %f\n", player[0].velocity.x, player[0].velocity.y);

	//	//player[i].pos = vector2f(MAX(player[i].pos.x, 0.f), MAX(player[i].pos.y, 0.f));
	//	//player[i].pos = vector2f(MIN(player[i].pos.x, 1920.f - 278.f), MIN(player[i].pos.y, 1080.f - 139.f));


	//	if (nbPlayer <= 1)
	//		break;
	//}

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
		player[_playerId].nbMine = 0;
		
		//if (player[_playerId].life > 0) {
			player[_playerId].invulnerabilityTimer = 3.f;
		//}
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
	for (int i = 0; i < nbPlayer; i++)
	{
		sfSprite_setTexture(playerSprite, player[i].flame.texture, sfTrue);
		sfSprite_setPosition(playerSprite, player[i].flame.pos);
		sfSprite_setOrigin(playerSprite, player[i].flame.origin);
		sfSprite_setScale(playerSprite, player[i].flame.scale);
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

		//if (player[i].invulnerabilityTimer > 2.9f)
		//	sfSprite_setTexture(playerSprite, playerHitTexture, sfTrue);
		//else
			sfSprite_setTexture(playerSprite, player[i].texture, sfTrue);

		sfSprite_setColor(playerSprite, player[i].color);
		sfSprite_setPosition(playerSprite, player[i].pos);
		sfSprite_setOrigin(playerSprite, player[i].origin);
		sfSprite_setScale(playerSprite, vector2f(1.f, 1.f));
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

		player[i].bounds = sfSprite_getGlobalBounds(playerSprite);

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
