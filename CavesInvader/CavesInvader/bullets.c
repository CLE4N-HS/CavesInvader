#include "bullets.h"
#include "textureManager.h"
#include "List.h"
#include "Vector.h"

#define GETDATA_PLAYERBULLETS STD_LIST_GETDATA(playerBulletsList, playerBullets, i)

stdList* playerBulletsList;

sfSprite* bulletsSprite;

sfTexture* bulletsTexture;


void initBullets(Window* _window)
{
	bulletsSprite = sfSprite_create();

	bulletsTexture = GetTexture("bullets");

	sfSprite_setTexture(bulletsSprite, bulletsTexture, sfTrue);

	playerBulletsList = STD_LIST_CREATE(playerBullets, 0);

}

void addBullets(bulletType _type, bulletId _id, sfVector2f _pos, sfIntRect _rect, sfVector2f _velocity)
{
	playerBullets tmp;
	tmp.type = _type;
	tmp.id = _id;
	tmp.pos = _pos;
	tmp.rect = _rect;
	tmp.velocity = _velocity;
	playerBulletsList->push_back(&playerBulletsList, &tmp);
}

void createPlayerBullets(bulletType _type, bulletId _id, sfVector2f _pos)
{
	sfVector2f pos = VECTOR2F_NULL;
	sfIntRect rect = IntRect(0,0,0,0);
	sfVector2f velocity = VECTOR2F_NULL;
	bulletId id = PLAYER_ID_BULLET;
	//vector2f(156.f, 48.f) playerOrigin 
	switch (_type)
	{
	case PLAYER_BASIC_BULLET:
		pos = AddVectors(_pos, vector2f(184.f, 16.f)); // vector2f(240.f, 64.f)
		rect = IntRect(0, 2452, 23, 13);
		velocity = vector2f(800.f, 0.f);
		id = PLAYER_ID_BULLET;
		break;
	case PLAYER_CHARGED_BULLET:
		pos = AddVectors(_pos, vector2f(29.f, 6.f)); // vector2f(185.f, 54.f)
		rect = IntRect(0, 2407, 56, 31);
		velocity = vector2f(1200.f, 0.f);
		id = PLAYER_ID_BULLET;
		break;
	default:
		break;
	}
	addBullets(_type, id, pos, rect, velocity);

}

void updateBullets(Window* _window)
{
	float dt = getDeltaTime();
	for (int i = 0; i < playerBulletsList->size(playerBulletsList); i++)
	{
		// collsions
		if (GETDATA_PLAYERBULLETS->id == PLAYER_ID_BULLET) {
			//for 
		}

		GETDATA_PLAYERBULLETS->pos = AddVectors(GETDATA_PLAYERBULLETS->pos, MultiplyVector(GETDATA_PLAYERBULLETS->velocity, dt));
	}
}

void displayBullets(Window* _window)
{
	for (int i = 0; i < playerBulletsList->size(playerBulletsList); i++)
	{
		sfSprite_setPosition(bulletsSprite, GETDATA_PLAYERBULLETS->pos);
		sfSprite_setTextureRect(bulletsSprite, GETDATA_PLAYERBULLETS->rect);
		sfRenderTexture_drawSprite(_window->renderTexture, bulletsSprite, NULL);
	}
}

void deinitBullets()
{
	sfSprite_destroy(bulletsSprite);
	free(playerBulletsList);
}
