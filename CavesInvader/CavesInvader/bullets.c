#include "bullets.h"
#include "textureManager.h"
#include "List.h"
#include "Vector.h"
#include "enemy.h"

#define GETDATA_PLAYERBULLETS STD_LIST_GETDATA(playerBulletsList, playerBullets, i)
#define GD_ENEMIES STD_LIST_GETDATA(enemiesList, Enemies, j)

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

void addBullets(bulletType _type, bulletId _id, sfVector2f _pos, sfVector2f _origin, sfIntRect _rect, sfVector2f _velocity, int _damage)
{
	playerBullets tmp;
	tmp.type = _type;
	tmp.id = _id;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.rect = _rect;
	tmp.velocity = _velocity;
	tmp.damage = _damage;
	tmp.bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	playerBulletsList->push_back(&playerBulletsList, &tmp);
}

void createPlayerBullets(bulletType _type, bulletId _id, sfVector2f _pos)
{
	sfVector2f pos = VECTOR2F_NULL;
	sfVector2f origin = VECTOR2F_NULL;
	sfIntRect rect = IntRect(0,0,0,0);
	sfVector2f velocity = VECTOR2F_NULL;
	bulletId id = PLAYER_ID_BULLET;
	int damage = 0;
	//vector2f(156.f, 48.f) playerOrigin 
	switch (_type)
	{
	case PLAYER_BASIC_BULLET:
		origin = vector2f(0.f, 6.f);
		pos = AddVectors(_pos, vector2f(50.f, 23.f)); // vector2f(240.f, 64.f)
		rect = IntRect(0, 2452, 23, 13);
		velocity = vector2f(800.f, 0.f);
		id = PLAYER_ID_BULLET;
		damage = 1;
		break;
	case PLAYER_CHARGED_BULLET:
		origin = vector2f(0.f, 15.f);
		pos = AddVectors(_pos, vector2f(50.f, 23.f)); // vector2f(185.f, 54.f)
		rect = IntRect(0, 2407, 56, 31);
		velocity = vector2f(1200.f, 0.f);
		id = PLAYER_ID_BULLET;
		damage = 5;
		break;
	default:
		break;
	}
	addBullets(_type, id, pos, origin, rect, velocity, damage);

}

void updateBullets(Window* _window)
{
	float dt = getDeltaTime();
	for (int i = 0; i < playerBulletsList->size(playerBulletsList); i++)
	{
		GETDATA_PLAYERBULLETS->pos = AddVectors(GETDATA_PLAYERBULLETS->pos, MultiplyVector(GETDATA_PLAYERBULLETS->velocity, dt));

		if (GETDATA_PLAYERBULLETS->pos.x > 1920.f + GETDATA_PLAYERBULLETS->origin.x) {
			playerBulletsList->erase(&playerBulletsList, i);
			continue;
		}

		// collsions
		if (GETDATA_PLAYERBULLETS->id == PLAYER_ID_BULLET) {
			for (int j = 0; j < enemiesList->size(enemiesList); j++)
			{
				if (sfFloatRect_intersects(&GETDATA_PLAYERBULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {
		
					GD_ENEMIES->life -= GETDATA_PLAYERBULLETS->damage;
		
					bulletType tmpType = GETDATA_PLAYERBULLETS->type;
		
					if (tmpType == PLAYER_BASIC_BULLET || tmpType == PLAYER_CHARGED_BULLET) {
						playerBulletsList->erase(&playerBulletsList, i);
						break;
					}
		
				}
			}
		}
		
	}
}

void displayBullets(Window* _window)
{
	for (int i = 0; i < playerBulletsList->size(playerBulletsList); i++)
	{
		sfSprite_setPosition(bulletsSprite, GETDATA_PLAYERBULLETS->pos);
		sfSprite_setOrigin(bulletsSprite, GETDATA_PLAYERBULLETS->origin);
		sfSprite_setTextureRect(bulletsSprite, GETDATA_PLAYERBULLETS->rect);
		sfRenderTexture_drawSprite(_window->renderTexture, bulletsSprite, NULL);

		GETDATA_PLAYERBULLETS->bounds = sfSprite_getGlobalBounds(bulletsSprite);
	}
}

void deinitBullets()
{
	sfSprite_destroy(bulletsSprite);
	free(playerBulletsList);
}
