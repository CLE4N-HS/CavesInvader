#include "player.h"
#include "CustomMath.h"
#include "gamepadx.h"

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
		player[i].speed = vector2f(300.f, 300.f);
		player[i].velocity = VECTOR2F_NULL;
		player[i].forward = VECTOR2F_NULL;
		player[i].drag = 1.f;
		player[i].isMoving = sfFalse;
		player[i].timeMoving = 0.f;


		//if (i >= nbPlayer)
		//	break;
	}
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

		//player[i].velocity = VECTOR2F_NULL;
		if (gamepadUp < -10.f) {
			player[i].velocity.y -= player[i].speed.y * -gamepadUp ;
			player[i].isMoving = sfTrue;
		}
		if (gamepadDown > 10.f) {
			player[i].velocity.y += player[i].speed.y * gamepadDown ;
			player[i].isMoving = sfTrue;
		}
		if (gamepadLeft < -10.f) {
			player[i].velocity.x -= player[i].speed.x * -gamepadLeft ;
			player[i].isMoving = sfTrue;
		}
		if (gamepadRight > 10.f) {
			player[i].velocity.x += player[i].speed.x * gamepadLeft;
			player[i].isMoving = sfTrue;
		}

		if (i == 0)
		{
			if (sfKeyboard_isKeyPressed(sfKeyZ)) {
				player[i].velocity.y -= player[i].speed.y * dt;
				//player[i].velocity = AddVectors(player[i].velocity, MultiplyVector(Normalize(player[i].speed), dt));
				player[i].isMoving = sfTrue;
				gamepadDown = -100.f;
			}
			if (sfKeyboard_isKeyPressed(sfKeyS)) {
				player[i].velocity.y += player[i].speed.y * dt;
				player[i].isMoving = sfTrue;
				gamepadDown = 100.f;
			}
			if (sfKeyboard_isKeyPressed(sfKeyQ)) {
				player[i].velocity.x -= player[i].speed.x * dt;
				player[i].isMoving = sfTrue;
				gamepadRight = -100.f;
			}
			if (sfKeyboard_isKeyPressed(sfKeyD)) {
				player[i].velocity.x += player[i].speed.x * dt;
				player[i].isMoving = sfTrue;
				gamepadRight = 100.f;
			}
		}
		else if (i == 1)
		{
			if (sfKeyboard_isKeyPressed(sfKeyUp)) {
				player[i].velocity.y -= player[i].speed.y * dt;
				//player[i].velocity = AddVectors(player[i].velocity, MultiplyVector(Normalize(player[i].speed), dt));
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


		player[i].forward = Normalize(player[i].velocity);
		if (player[i].isMoving) {
			player[i].timeMoving += getDeltaTime();
			player[i].timeMoving = MIN(player[i].timeMoving, 1.f);
			//player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * timeMoving);

			player[i].previousForward = player[i].forward;
		}
		else {
			player[i].timeMoving -= getDeltaTime();

			player[i].timeMoving = MAX(player[i].timeMoving, 0.f);
			//player[i].velocity = Normalize(player[i].velocity);
		}
		player[i].velocity = MultiplyVector(player[i].forward, dt * player[i].speed.x * player[i].timeMoving);

		player[i].velocity.x += gamepadRight * dt * 5.f;
		player[i].velocity.y += gamepadDown * dt * 5.f;

		//player[i].velocity = MultiplyVector(player[i].velocity, 1.f / (1.f +  (dt * player[i].drag * 10000.f)));
		//player[i].velocity = AddVectors(player[i].velocity, force);
		player[i].velocity = MultiplyVector(player[i].velocity, 1.f - dt);
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
			player[i].timeMoving = 0.f;
		}
		if (player[i].pos.x > 1642.f) {
			player[i].pos.x = 1642.f;
			player[i].timeMoving = 0.f;
		}
		if (player[i].pos.y < 0.f) {
			player[i].pos.y = 0.f;
			player[i].timeMoving = 0.f;
		}
		if (player[i].pos.y > 941.f) {
			player[i].pos.y = 941.f;
			player[i].timeMoving = 0.f;
		}

		//player[i].pos = vector2f(MAX(player[i].pos.x, 0.f), MAX(player[i].pos.y, 0.f));
		//player[i].pos = vector2f(MIN(player[i].pos.x, 1920.f - 278.f), MIN(player[i].pos.y, 1080.f - 139.f));


		//if (nbPlayer <= 1)
		//	break;
	}

}

void displayPlayer(Window* _window)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		sfSprite_setTexture(playerSprite, player[i].texture, sfTrue);
		sfSprite_setPosition(playerSprite, player[i].pos);
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

		//if (nbPlayer <= 1)
		//	break;
	}
}

void deinitPlayer()
{
	sfSprite_destroy(playerSprite);
}
