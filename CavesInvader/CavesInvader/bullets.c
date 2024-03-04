#include "bullets.h"
#include "textureManager.h"
#include "List.h"
#include "Vector.h"
#include "enemy.h"
#include "particlesSystemManager.h"
#include "gamepad.h"
#include "player.h"

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

void addBullets(bulletType _type, bulletId _id, int _ownerId, sfVector2f _pos, sfVector2f _origin, sfIntRect _rect, sfVector2f _velocity, int _damage, float _fDamagePerSecond, float _angle, float _rotationSpeed, float _animTimer)
{
	playerBullets tmp;
	tmp.type = _type;
	tmp.id = _id;
	tmp.ownerId = _ownerId;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.rect = _rect;
	tmp.velocity = _velocity;
	tmp.damage = _damage;
	tmp.bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	tmp.angle = _angle;
	tmp.rotationSpeed = _rotationSpeed;
	tmp.animTimer = _animTimer;

	// unique parameters
	if (_type == PLAYER_MINES) {
		tmp.mine.radius = 35.f;
	}

	playerBulletsList->push_back(&playerBulletsList, &tmp);
}

void createPlayerBullets(bulletType _type, bulletId _id, int _ownerId, sfVector2f _pos)
{
	sfVector2f pos = VECTOR2F_NULL;
	sfVector2f origin = VECTOR2F_NULL;
	sfIntRect rect = IntRect(0,0,0,0);
	sfVector2f velocity = VECTOR2F_NULL;
	int damage = 0;
	float fDamagePerSecond = 0.f;
	float angle = 0.f;
	float rotationSpeed = 0.f;
	float animTimer = 0.f;
	//vector2f(156.f, 48.f) playerOrigin 
	switch (_type)
	{
	case PLAYER_BASIC_BULLET:
		origin = vector2f(0.f, 6.f);
		pos = AddVectors(_pos, vector2f(50.f, 23.f));
		rect = IntRect(0, 2452, 23, 13);
		velocity = vector2f(800.f, 0.f);  // TODO : check the nbBullet the player can shot to adjust the velocity
		damage = 1;
		break;
	case PLAYER_CHARGED_BULLET:
		origin = vector2f(0.f, 15.f);
		pos = AddVectors(_pos, vector2f(50.f, 23.f));
		rect = IntRect(0, 2407, 56, 31);
		velocity = vector2f(1200.f, 0.f);
		damage = 5;
		break;
	case PLAYER_LASER:
		origin = vector2f(0.f, 14.f);
		pos = AddVectors(_pos, vector2f(36.f, 24.f));
		rect = IntRect(0, 2172, 437, 28);
		fDamagePerSecond = 2.f;
		break;
	case PLAYER_MINES:
		pos = AddVectors(_pos, vector2f(50.f, 24.f));
		if (_ownerId) { // j2
			origin = vector2f(38.f, 41.f);
			rect = IntRect(0, 2332, 75, 75);
		}
		else { // j1
			origin = vector2f(39.f, 41.f);
			rect = IntRect(0, 2257, 77, 75); // IntRect(0, 2256, 77, 76)
		}
		velocity = vector2f(500.f, 0.f);
		damage = 5;
		rotationSpeed = 100.f;
		break;
	case PLAYER_FLAMETHROWER:
		pos = AddVectors(_pos, vector2f(50.f, 24.f));
		fDamagePerSecond = 2.f;
		break;
	default:
		break;
	}
	addBullets(_type, _id, _ownerId, pos, origin, rect, velocity, damage, fDamagePerSecond, angle, rotationSpeed, animTimer);

}

void updateBullets(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < playerBulletsList->size(playerBulletsList); i++)
	{
		if (GETDATA_PLAYERBULLETS->pos.x > 1920.f + GETDATA_PLAYERBULLETS->origin.x) {
			playerBulletsList->erase(&playerBulletsList, i);
			continue;
		}

		playerBullets tmp;

		tmp.type = GETDATA_PLAYERBULLETS->type;

		if (tmp.type == PLAYER_BASIC_BULLET || tmp.type == PLAYER_CHARGED_BULLET)
		{
			GETDATA_PLAYERBULLETS->pos = AddVectors(GETDATA_PLAYERBULLETS->pos, MultiplyVector(GETDATA_PLAYERBULLETS->velocity, dt));
		}
		else if (tmp.type == PLAYER_LASER)
		{
			GETDATA_PLAYERBULLETS->pos = AddVectors(getPlayerPos(GETDATA_PLAYERBULLETS->ownerId), vector2f(36.f, 24.f));

			if (Gamepad[GETDATA_PLAYERBULLETS->ownerId].TriggerL < 0.5f) {
				playerBulletsList->erase(&playerBulletsList, i);
				continue;
			}
		}
		else if (tmp.type == PLAYER_MINES)
		{
			if (GETDATA_PLAYERBULLETS->rect.top < 2460) {
				GETDATA_PLAYERBULLETS->pos = AddVectors(GETDATA_PLAYERBULLETS->pos, MultiplyVector(GETDATA_PLAYERBULLETS->velocity, dt));
				GETDATA_PLAYERBULLETS->angle += GETDATA_PLAYERBULLETS->rotationSpeed * dt;
			}
			else {
				GETDATA_PLAYERBULLETS->animTimer += dt;

				if (GETDATA_PLAYERBULLETS->animTimer > 0.1f) {

					GETDATA_PLAYERBULLETS->animTimer = 0.f;

					GETDATA_PLAYERBULLETS->rect.left += GETDATA_PLAYERBULLETS->rect.width;

					sfIntRect tmpRect = GETDATA_PLAYERBULLETS->rect;

					// mine radius for collisions
					if (tmpRect.top < 3000) {
						if (tmpRect.left > 4000) GETDATA_PLAYERBULLETS->mine.radius = 180.f;
						else if (tmpRect.left > 5000) GETDATA_PLAYERBULLETS->mine.radius = 260.f;
					}
					else {
						if (tmpRect.left < 10) GETDATA_PLAYERBULLETS->mine.radius = 310.f;
						else if (tmpRect.left < 2000) GETDATA_PLAYERBULLETS->mine.radius = 410.f;
						else GETDATA_PLAYERBULLETS->mine.radius = -1.f;
					}

					if (tmpRect.left > 6200) {
						if (tmpRect.top < 3000) {
							GETDATA_PLAYERBULLETS->rect.top += GETDATA_PLAYERBULLETS->rect.height;
						}
						else {
							playerBulletsList->erase(&playerBulletsList, i);
							continue;
						}
						GETDATA_PLAYERBULLETS->rect.left = 0;
					}
				}
			}
			
		}
		else if (tmp.type == PLAYER_FLAMETHROWER)
		{
			// estimated rect for collisions
		}


		// collsions
		if (GETDATA_PLAYERBULLETS->id == PLAYER_ID_BULLET) {
			for (int j = 0; j < enemiesList->size(enemiesList); j++)
			{
				if (GD_ENEMIES->state == DEAD || GD_ENEMIES->life <= 0)
					continue;

				if (tmp.type == PLAYER_BASIC_BULLET || tmp.type == PLAYER_CHARGED_BULLET)
				{
					if (sfFloatRect_intersects(&GETDATA_PLAYERBULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {

						GD_ENEMIES->life -= GETDATA_PLAYERBULLETS->damage;

						playerBulletsList->erase(&playerBulletsList, i);
						break;

					}
				}

				else if (tmp.type == PLAYER_MINES)
				{
					// basic mine
					if (GETDATA_PLAYERBULLETS->rect.top < 2460) {

						if (sfFloatRect_intersects(&GETDATA_PLAYERBULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {

							GD_ENEMIES->life -= GETDATA_PLAYERBULLETS->damage;

							GETDATA_PLAYERBULLETS->rect = IntRect(898, 2465, 898, 798);
							GETDATA_PLAYERBULLETS->angle = 0.f;
							GETDATA_PLAYERBULLETS->origin = vector2f(449.f, 399.f);

							GETDATA_PLAYERBULLETS->mine.radius = 35.f;
						}
					}
					// exploded mine
					else if (GETDATA_PLAYERBULLETS->mine.radius > 0.f) {
						if (GetSqrMagnitude(CreateVector(GETDATA_PLAYERBULLETS->pos, AddVectors(GD_ENEMIES->pos, GD_ENEMIES->originToCenter)))  <= GETDATA_PLAYERBULLETS->mine.radius * GETDATA_PLAYERBULLETS->mine.radius + GD_ENEMIES->radius * GD_ENEMIES->radius)
							GD_ENEMIES->life -= GETDATA_PLAYERBULLETS->damage;
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
		sfSprite_setRotation(bulletsSprite, GETDATA_PLAYERBULLETS->angle);
		sfSprite_setTextureRect(bulletsSprite, GETDATA_PLAYERBULLETS->rect);
		sfRenderTexture_drawSprite(_window->renderTexture, bulletsSprite, NULL);

		// if not flame thrower and mb laser too
		GETDATA_PLAYERBULLETS->bounds = sfSprite_getGlobalBounds(bulletsSprite);
	}
}

void deinitBullets()
{
	sfSprite_destroy(bulletsSprite);
	free(playerBulletsList);
}
