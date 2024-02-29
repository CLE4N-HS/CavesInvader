#include "player.h"
#include "customMath.h"
#include "gamepadx.h"
#include "bullets.h"

typedef struct Players {
	sfTexture* texture;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f speed;
	sfVector2f velocity;
	sfVector2f forward;
	sfVector2f previousForward;
	float drag;
	float bulletTimer;
	sfBool isMoving;
	float timeMoving;
	float anothertimer;
	sfBool wasalreadymovingtbh;
	sfBool wasnt;
}Players;
Players player[MAX_PLAYER];

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
			break;
		case 1:
			player[i].texture = GetTexture("player2");
			player[i].pos = vector2f(300.f, 600.f);
			break;
		default:
			break;
		}
		player[i].speed = vector2f(600.f, 600.f);
		player[i].velocity = VECTOR2F_NULL;
		player[i].forward = VECTOR2F_NULL;
		player[i].drag = 1.f;
		player[i].isMoving = sfFalse;
		player[i].timeMoving = 0.f;
		player[i].anothertimer = 0.f;
		player[i].wasalreadymovingtbh = sfFalse;
		player[i].bulletTimer = 0.f;
		player[i].origin = vector2f(156.f, 48.f);

		//if (i >= nbPlayer)
		//	break;
	}

	sfSprite_setOrigin(playerSprite, player[0].origin);
}

void updatePlayer(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		float gamepadUp = Gamepad_isJoystickMoved(i, STICKLY);
		float gamepadDown = Gamepad_isJoystickMoved(i, STICKLY);
		float gamepadLeft = Gamepad_isJoystickMoved(i, STICKLX);
		float gamepadRight = Gamepad_isJoystickMoved(i, STICKLX);

		player[i].isMoving = sfFalse;

		player[i].forward = VECTOR2F_NULL;
		player[i].bulletTimer += dt;

		//player[i].velocity = VECTOR2F_NULL;
		if (gamepadUp < -30.f) {
			player[i].forward.y -= dt * -gamepadUp;
			player[i].isMoving = sfTrue;
		}
		if (gamepadDown > 30.f) {
			player[i].forward.y += dt * gamepadDown;
			player[i].isMoving = sfTrue;
		}
		if (gamepadLeft < -30.f) {
			player[i].forward.x -= dt * -gamepadLeft;
			player[i].isMoving = sfTrue;
		}
		if (gamepadRight > 30.f) {
			player[i].forward.x += dt * gamepadLeft;
			player[i].isMoving = sfTrue;
		}

		//if (player[i].isMoving) {
		//	player[i].timerMove += dt;
		//	player[i].timerMove = MAX(player[i].timerMove, 1.f);
		//	player[i].previousForward = Normalize(player[i].velocity);
		//	if (player[i].previousForward.x < 10.f && player[i].previousForward.x > 0.f) {
		//		player[i].previousForward.x = 10.f;
		//	}
		//	if (player[i].previousForward.y < 10.f && player[i].previousForward.y > 0.f) {
		//		player[i].previousForward.y = 10.f;
		//	}
		//	if (player[i].previousForward.x > -10.f && player[i].previousForward.x < 0.f) {
		//		player[i].previousForward.x = -10.f;
		//	}
		//	if (player[i].previousForward.y > -10.f && player[i].previousForward.y < 0.f) {
		//		player[i].previousForward.y = -10.f;
		//	}
		//	player[i].previousForward = Normalize(player[i].previousForward);
		//}
		//else {
		//	player[i].previousForward = MultiplyVector(player[i].forward, player[i].timerMove);
		//	player[i].timerMove -= dt;
		//}


		if (i == 0)
		{
			if (sfKeyboard_isKeyPressed(sfKeyZ) || Gamepad_isJoystickMoved(0, CROSSY_XBOX) > 30.f) {
				gamepadUp = -100.f;
				player[i].forward.y += dt * gamepadUp;
				player[i].isMoving = sfTrue;
			}
			if (sfKeyboard_isKeyPressed(sfKeyS) || Gamepad_isJoystickMoved(0, CROSSY_XBOX) < -30.f) {
				gamepadDown = 100.f;
				player[i].forward.y += dt * gamepadDown;
				player[i].isMoving = sfTrue;
			}
			if (sfKeyboard_isKeyPressed(sfKeyQ) || Gamepad_isJoystickMoved(0, CROSSX_XBOX) < -30.f) {
				gamepadLeft = -100.f;
				player[i].forward.x += dt * gamepadLeft;
				player[i].isMoving = sfTrue;
			}
			if (sfKeyboard_isKeyPressed(sfKeyD) || Gamepad_isJoystickMoved(0, CROSSX_XBOX) > 30.f) {
				gamepadRight = 100.f;
				player[i].forward.x += dt * gamepadRight;
				player[i].isMoving = sfTrue;
			}
		}
		else if (i == 1)
		{
			if (sfKeyboard_isKeyPressed(sfKeyUp)) {
				player[i].velocity.y -= player[i].speed.y * dt;
				//player[i].velocity = AddVectors(player[i].velocity, MultiplyVector(Normalize(player[i].speed)
				player[i].isMoving = sfTrue;
				gamepadDown = -100.f;
			}
			if (sfKeyboard_isKeyPressed(sfKeyDown)) {
				player[i].velocity.y += player[i].speed.y * dt;
				player[i].isMoving = sfTrue;
				gamepadDown = 100.f;
			}
			if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
				player[i].velocity.x -= player[i].speed.x * dt;
				player[i].isMoving = sfTrue;
				gamepadRight = -100.f;
			}
			if (sfKeyboard_isKeyPressed(sfKeyRight)) {
				player[i].velocity.x += player[i].speed.x * dt;
				player[i].isMoving = sfTrue;
				gamepadRight = 100.f;
			}
		}



		player[i].forward = Normalize(player[i].forward);
		//printf("%f, %f\n", player[i].forward.x, player[i].forward.y);
		if (player[i].isMoving) {
			//player[i].timeMoving += 1.f;
			player[i].timeMoving += dt;
			player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
			player[i].anothertimer += dt;
			player[i].anothertimer = MIN(player[i].anothertimer, 1.f);
			player[i].anothertimer = player[i].timeMoving;
			//
			if (player[i].wasalreadymovingtbh) {
				player[i].velocity = VECTOR2F_NULL;
				//player[i].timeMoving = 0.f;
				player[i].wasalreadymovingtbh = sfFalse;
				//player[i].forward = VECTOR2F_NULL;
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

		//player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * player[i].timeMoving)
		player[i].velocity = MultiplyVector(player[i].forward, player[i].speed.x * dt * player[i].anothertimer);

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
		player[i].pos = AddVectors(player[i].pos, player[i].velocity);

		if (player[i].pos.x < 0.f) {
			player[i].pos.x = 0.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.x > 1642.f) {
			player[i].pos.x = 1642.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.y < 0.f) {
			player[i].pos.y = 0.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}
		if (player[i].pos.y > 941.f) {
			player[i].pos.y = 941.f;
			player[i].timeMoving -= dt * 2.f;
			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
		}

		printf("%f\n", player[0].timeMoving);
		//printf("%f, %f\n", player[0].velocity.x, player[0].velocity.y);

		//player[i].pos = vector2f(MAX(player[i].pos.x, 0.f), MAX(player[i].pos.y, 0.f));
		//player[i].pos = vector2f(MIN(player[i].pos.x, 1920.f - 278.f), MIN(player[i].pos.y, 1080.f - 139.f));









		if (isKeyboardOrControllerButtonPressed(sfKeySpace, LB_XBOX) && player[i].bulletTimer > 0.2f) {

			if (player[i].bulletTimer > 2.f) {
				createPlayerBullets(PLAYER_CHARGED_BULLET, PLAYER_ID_BULLET, player[i].pos);
				player[i].bulletTimer = -0.1f;
			}
			else {
				createPlayerBullets(PLAYER_BASIC_BULLET, PLAYER_ID_BULLET, player[i].pos);
				player[i].bulletTimer = 0.0f;
			}

		}




		if (nbPlayer <= 1)
			break;
	}

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

void displayPlayer(Window* _window)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		sfSprite_setTexture(playerSprite, player[i].texture, sfTrue);
		sfSprite_setPosition(playerSprite, player[i].pos);
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

		if (nbPlayer <= 1)
			break;
	}
}

void deinitPlayer()
{
	sfSprite_destroy(playerSprite);
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

