#include "player.h"

sfSprite* playerSprite;

void initPlayer(Window* _window)
{
	playerSprite = sfSprite_create();
	
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
		player[i].speed = 100.f;


		//if (i >= nbPlayer)
		//	break;
	}
}

void updatePlayer(Window* _window)
{


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
