#include "bullets.h"
#include "textureManager.h"
#include "List.h"
#include "enemy.h"
#include "particlesSystemManager.h"
#include "gamepad.h"
#include "player.h"

#define GETDATA_BULLETS STD_LIST_GETDATA(bulletsList, Bullets, i)
#define GD_ENEMIES STD_LIST_GETDATA(enemiesList, Enemies, j)

stdList* bulletsList;

sfSprite* bulletsSprite;

sfTexture* bulletsTexture;
sfTexture* enemyBulletsTexture;
sfTexture* laserTexture;

sfRectangleShape* tmpRectangle; // to remove


void initBullets(Window* _window)
{
	bulletsSprite = sfSprite_create();

	bulletsTexture = GetTexture("bullets");
	enemyBulletsTexture = GetTexture("enemyBullets");
	laserTexture = GetTexture("laser");

	sfSprite_setTexture(bulletsSprite, bulletsTexture, sfTrue);

	bulletsList = STD_LIST_CREATE(Bullets, 0);

	tmpRectangle = sfRectangleShape_create();

}

void addBullets(bulletType _type, bulletId _id, int _ownerId, sfVector2f _pos, sfVector2f _origin, sfIntRect _rect, sfVector2f _forward, float _speed, sfVector2f _velocity, sfVector2f _scale, int _damage, float _fDamagePerSecond, float _angle, float _rotationSpeed, float _animTimer, sfBool _canDealDamages, sfVector2f _originToCenter, float _radius)
{
	Bullets tmp;
	tmp.type = _type;
	tmp.id = _id;
	tmp.ownerId = _ownerId;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.scale = _scale;
	tmp.rect = _rect;
	tmp.forward = _forward;
	tmp.speed = _speed;
	tmp.velocity = _velocity;
	tmp.damage = _damage;
	tmp.fDamagePerSecond = _fDamagePerSecond;
	tmp.bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	tmp.angle = _angle;
	tmp.rotationSpeed = _rotationSpeed;
	tmp.animTimer = _animTimer;
	tmp.originToCenter = _originToCenter;
	tmp.radius = _radius;

	tmp.canDealDamages = _canDealDamages;
	// unique parameters
	if (_type == PLAYER_BASIC_BULLET || _type == PLAYER_CHARGED_BULLET) {
		tmp.basicBullet.deathTimer = 0.f;
	}
	else if (_type == PLAYER_MINES) {
		tmp.mine.radius = 35.f;
	}
	else if (_type == PLAYER_FLAMETHROWER) {
		tmp.flamethrower.bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	}
	else if (_type == PLAYER_LASER) {
		tmp.laser.timer = 0.f;
		tmp.laser.timeLasering = 0.f;
	}
	else if (_type == ENEMY_YELLOW_BULLET || _type == ENEMY_GREEN_BULLET) {
		tmp.enemyBullet.deathTimer = 0.f;
	}

	bulletsList->push_back(&bulletsList, &tmp);
}

void createBullets(bulletType _type, int _ownerId, sfVector2f _pos, float _angle)
{
	bulletId id = 0;
	if (_type <= PLAYER_FLAMETHROWER)
		id = PLAYER_ID_BULLET;
	else
		id = ENEMY_ID_BULLET;

	sfVector2f forward = VECTOR2F_NULL;
	float speed = 0.f;

	sfVector2f pos = VECTOR2F_NULL;
	sfVector2f origin = VECTOR2F_NULL;
	sfIntRect rect = IntRect(0,0,0,0);
	sfVector2f velocity = VECTOR2F_NULL;
	sfVector2f scale = vector2f(1.f, 1.f);
	int damage = 0;
	float fDamagePerSecond = 0.f;
	float angle = 0.f;
	float rotationSpeed = 0.f;
	float animTimer = 0.f;
	sfBool canDealDamages = sfTrue;
	float radius = 0.f;
	sfVector2f originToCenter = VECTOR2F_NULL;
	//vector2f(156.f, 48.f) playerOrigin 
	switch (_type)
	{
	case PLAYER_BASIC_BULLET:
		origin = vector2f(0.f, 6.f);
		originToCenter = vector2f(/*19*/0.f, 0.f);
		radius = 7.f;
		pos = AddVectors(_pos, vector2f(50.f, 23.f));
		rect = IntRect(0, 2452, 23, 13);
		velocity = vector2f(800.f, 0.f);  // TODO : check the nbBullet the player can shot to adjust the velocity
		damage = 1;
		forward = PolarCoords(VECTOR2F_NULL, 1.f, _angle * DEG2RAD);
		speed = 800.f;
		break;
	case PLAYER_CHARGED_BULLET:
		origin = vector2f(0.f, 15.f);
		originToCenter = vector2f(/*44*/-44.f, 0.f);
		radius = 16.f;
		pos = AddVectors(_pos, vector2f(50.f, 23.f));
		rect = IntRect(0, 2407, 56, 31);
		velocity = vector2f(1200.f, 0.f);
		damage = 5;
		forward = PolarCoords(VECTOR2F_NULL, 1.f, _angle * DEG2RAD);
		speed = 1200.f;
		break;
	case PLAYER_LASER:
		origin = vector2f(0.f, 14.f);
		pos = AddVectors(_pos, vector2f(36.f, 24.f));
		//rect = IntRect(0, 2172, 437, 28);
		rect = IntRect(0, 0, 1275, 28);
		scale = vector2f(0.f, 1.f);
		fDamagePerSecond = 8.f;
		break;
	case PLAYER_MINES:
		pos = AddVectors(_pos, vector2f(-50.f, 24.f));
		if (_ownerId) { // j2
			origin = vector2f(38.f, 41.f);
			rect = IntRect(0, 2332, 75, 75);
		}
		else { // j1
			origin = vector2f(39.f, 41.f);
			rect = IntRect(0, 2257, 77, 75); // IntRect(0, 2256, 77, 76)
		}
		originToCenter = vector2f(-38.f, 0.f);
		radius = 38.f;
		velocity = vector2f(500.f, 0.f);
		damage = 5;
		rotationSpeed = 100.f;
		break;
	case PLAYER_FLAMETHROWER:
		pos = AddVectors(_pos, vector2f(50.f, 24.f));
		fDamagePerSecond = 10.f;
		break;
	case ENEMY_YELLOW_BULLET:
		pos = AddVectors(_pos, vector2f(-34.f, 20.f));
		rect = IntRect(0, 4639, 33, 33);
		origin = vector2f(33.f, 17.f);
		damage = 1;
		scale = vector2f(0.5f, 0.5f);
		velocity = vector2f(-450.f, 0.f);
		break;
	case ENEMY_GREEN_BULLET:
		pos = AddVectors(_pos, vector2f(-51.f, 30.f));
		rect = IntRect(0, 4680, 52, 36);
		origin = vector2f(52.f, 18.f);
		damage = 1;
		scale = vector2f(0.5f, 0.5f);
		velocity = vector2f(-600.f, 0.f);
		break;
	case ENEMY_RED_BULLET:
		pos = AddVectors(_pos, vector2f(-60.f, 125.f));
		rect = IntRect(0, 4535, 88, 86);
		origin = vector2f(44.f, 43.f);
		damage = 1;
		scale = vector2f(0.5f, 0.5f);
		velocity = vector2f(-500.f, 0.f);
		break;
	case ENEMY_WARNING_BULLET:
		pos = vector2f(0.f, rand_float(100.f, 674.f));
		rect = IntRect(0, 1254, 1920, 306);
		origin = vector2f(0.f, 0.f);
		damage = 1;
		canDealDamages = sfFalse;
		break;
	default:
		break;
	}
	addBullets(_type, id, _ownerId, pos, origin, rect, forward, speed, velocity, scale, damage, fDamagePerSecond, angle, rotationSpeed, animTimer, canDealDamages, originToCenter, radius);

}

void updateBullets(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < bulletsList->size(bulletsList); i++)
	{
		if (GETDATA_BULLETS->id == PLAYER_ID_BULLET) {
			if (GETDATA_BULLETS->pos.x > 1920.f + GETDATA_BULLETS->origin.x) {
				bulletsList->erase(&bulletsList, i);
				continue;
			}
		}
		else {
			if (GETDATA_BULLETS->pos.x < 0.f - GETDATA_BULLETS->rect.width + GETDATA_BULLETS->origin.x) {
				bulletsList->erase(&bulletsList, i);
				continue;
			}
		}


		Bullets tmp;
		
		tmp.type = GETDATA_BULLETS->type;

		if (tmp.type == PLAYER_BASIC_BULLET || tmp.type == PLAYER_CHARGED_BULLET)
		{
			if (GETDATA_BULLETS->canDealDamages) {
				//GETDATA_BULLETS->pos = AddVectors(GETDATA_BULLETS->pos, MultiplyVector(GETDATA_BULLETS->velocity, dt));
				GETDATA_BULLETS->pos = AddVectors(GETDATA_BULLETS->pos, MultiplyVector(GETDATA_BULLETS->forward, GETDATA_BULLETS->speed * dt));
			}

			else {
				GETDATA_BULLETS->basicBullet.deathTimer += dt;
				Animator(&GETDATA_BULLETS->rect, &GETDATA_BULLETS->basicBullet.deathTimer, 6, 1, 0.05f, 0.f);

				if (getFrameX(GETDATA_BULLETS->rect) > 5) {
					bulletsList->erase(&bulletsList, i);
					continue;
				}
			}
		}
		else if (tmp.type == PLAYER_LASER)
		{
			GETDATA_BULLETS->pos = AddVectors(getPlayerPos(GETDATA_BULLETS->ownerId), vector2f(-20.f, 24.f));

			GETDATA_BULLETS->laser.timer += dt;
			GETDATA_BULLETS->scale.y = sin(GETDATA_BULLETS->laser.timer) * 2.f;

			if (GETDATA_BULLETS->scale.x < 1.5f)
				GETDATA_BULLETS->scale.x += dt * 2.f;


			if (GETDATA_BULLETS->scale.y < 1.f) {
				GETDATA_BULLETS->scale.y = 1.f;
				GETDATA_BULLETS->laser.timer = 0.6f;
			}

			GETDATA_BULLETS->laser.timeLasering += dt;

			if (GETDATA_BULLETS->laser.timeLasering > 4.f) {
				player[GETDATA_BULLETS->ownerId].isLightning = sfFalse;
				player[GETDATA_BULLETS->ownerId].nbLightning = 15;

				for (int j = 0; j < enemiesList->size(enemiesList); j++)
				{
					GD_ENEMIES->isLasered = sfFalse;
				}

				bulletsList->erase(&bulletsList, i);
				continue;
			}


			//GETDATA_BULLETS->pos = AddVectors(getPlayerPos(GETDATA_BULLETS->ownerId), vector2f(-20.f, 24.f));
			//
			//if (GETDATA_BULLETS->scale.x < 4.2f)
			//	GETDATA_BULLETS->scale.x += dt * 5.f;
			//
			//GETDATA_BULLETS->laser.timer += dt;
			//
			//GETDATA_BULLETS->scale.y = sin(GETDATA_BULLETS->laser.timer) * 2.f;
			//
			//if (GETDATA_BULLETS->scale.y < 1.f) {
			//	GETDATA_BULLETS->scale.y = 1.f;
			//	GETDATA_BULLETS->laser.timer = 0.6f;
			//}


			//if (getTriggerValue(GETDATA_BULLETS->ownerId, sfTrue) <= 0.1f || player[GETDATA_BULLETS->ownerId].nbLightning >= LIGTHNING_SECONDS_REQUIRED) {
			//	player[GETDATA_BULLETS->ownerId].isLightning = sfFalse;
			//
			//	for (int j = 0; j < enemiesList->size(enemiesList); j++)
			//	{
			//		GD_ENEMIES->isLasered = sfFalse;
			//	}
			//
			//	bulletsList->erase(&bulletsList, i);
			//	continue;
			//}
		}
		else if (tmp.type == PLAYER_MINES)
		{
			if (GETDATA_BULLETS->rect.top < 2460) {
				GETDATA_BULLETS->pos = AddVectors(GETDATA_BULLETS->pos, MultiplyVector(GETDATA_BULLETS->velocity, dt));
				GETDATA_BULLETS->angle += GETDATA_BULLETS->rotationSpeed * dt;
			}
			else {
				GETDATA_BULLETS->animTimer += dt;

				if (GETDATA_BULLETS->animTimer > 0.1f) {

					GETDATA_BULLETS->animTimer = 0.f;

					GETDATA_BULLETS->rect.left += GETDATA_BULLETS->rect.width;

					sfIntRect tmpRect = GETDATA_BULLETS->rect;

					// mine radius for collisions
					if (tmpRect.top < 3000) {
						if (tmpRect.left > 4000) GETDATA_BULLETS->mine.radius = 180.f;
						else if (tmpRect.left > 5000) GETDATA_BULLETS->mine.radius = 260.f;
					}
					else {
						if (tmpRect.left < 10) GETDATA_BULLETS->mine.radius = 310.f;
						else if (tmpRect.left < 2000) GETDATA_BULLETS->mine.radius = 410.f;
						else GETDATA_BULLETS->mine.radius = -1.f;
					}

					if (tmpRect.left > 6200) {
						if (tmpRect.top < 3000) {
							GETDATA_BULLETS->rect.top += GETDATA_BULLETS->rect.height;
						}
						else {
							bulletsList->erase(&bulletsList, i);
							continue;
						}
						GETDATA_BULLETS->rect.left = 0;
					}
				}
			}
			
		}
		else if (tmp.type == PLAYER_FLAMETHROWER)
		{
			// estimated rect for collisions
			int tmpOwnerId = GETDATA_BULLETS->ownerId;
			sfVector2f tmpPos = getPlayerPos(tmpOwnerId);
			GETDATA_BULLETS->bounds.left = tmpPos.x + 100.f ;
			GETDATA_BULLETS->bounds.top = tmpPos.y - 74.f - getPlayerVelocity(tmpOwnerId).y / 10.f;
			GETDATA_BULLETS->bounds.width = 250.f - getPlayerVelocity(tmpOwnerId).x / 3.f;
			GETDATA_BULLETS->bounds.height = 200.f /*+ getPlayerVelocity(tmpOwnerId).y / 3.f * 0.f*/;

			// erase it because there shouldn't be any flames at this point
			if (!player[GETDATA_BULLETS->ownerId].isFlamethrowering) {
				bulletsList->erase(&bulletsList, i);
				continue;
			}
		}
		else if (tmp.type == ENEMY_YELLOW_BULLET || tmp.type == ENEMY_GREEN_BULLET || tmp.type == ENEMY_RED_BULLET)
		{
			if (GETDATA_BULLETS->canDealDamages) {

				sfVector2f tmpScale = GETDATA_BULLETS->scale;
				if (tmpScale.x < 1.f) {
					GETDATA_BULLETS->scale = AddVectors(tmpScale, vector2f(dt, dt));
				}
				else
					GETDATA_BULLETS->scale = vector2f(1.f, 1.f);
	
				GETDATA_BULLETS->pos.x += GETDATA_BULLETS->velocity.x * dt;
			}
			else {
				GETDATA_BULLETS->enemyBullet.deathTimer += dt;
				Animator(&GETDATA_BULLETS->rect, &GETDATA_BULLETS->enemyBullet.deathTimer, 6, 1, 0.05f, 0.f);

				if (getFrameX(GETDATA_BULLETS->rect) > 5) {
					bulletsList->erase(&bulletsList, i);
					continue;
				}
			}

		}
		else if (tmp.type == ENEMY_WARNING_BULLET)
		{
			GETDATA_BULLETS->animTimer += dt;

			if (GETDATA_BULLETS->animTimer > 0.2f) {
				GETDATA_BULLETS->rect.left += GETDATA_BULLETS->rect.width;

				if (GETDATA_BULLETS->rect.left > 4000) {
					GETDATA_BULLETS->rect.left = 0;
					GETDATA_BULLETS->rect.top += GETDATA_BULLETS->rect.height;
				}

				sfIntRect tmpRect = GETDATA_BULLETS->rect;

				if (tmpRect.top > 1800 && tmpRect.left > 100) {
					resetBossPosition();
					bulletsList->erase(&bulletsList, i);
					continue;
				}

				if (tmpRect.top > 1500 && tmpRect.top < 1600 && tmpRect.left < 2000)
					GETDATA_BULLETS->canDealDamages = sfTrue;
				else
					GETDATA_BULLETS->canDealDamages = sfFalse;


				if (tmpRect.top < 1300)
					GETDATA_BULLETS->animTimer = -0.2f;
				else 
					GETDATA_BULLETS->animTimer = 0.1f;
			}

		}


		// collsions
		if (GETDATA_BULLETS->id == PLAYER_ID_BULLET && GETDATA_BULLETS->canDealDamages) {
			for (int j = 0; j < enemiesList->size(enemiesList); j++)
			{
				if (GD_ENEMIES->state == DEAD || GD_ENEMIES->life <= 0)
					continue;

				enemyType tmpType = GD_ENEMIES->type;

				GD_ENEMIES->isLasered = sfFalse;

				if (tmp.type == PLAYER_BASIC_BULLET || tmp.type == PLAYER_CHARGED_BULLET)
				{
					if (tmpType == TAMER) {
						if (GetSqrMagnitude(CreateVector(GETDATA_BULLETS->pos, AddVectors(GD_ENEMIES->pos, GD_ENEMIES->originToCenter))) <= GETDATA_BULLETS->radius * GETDATA_BULLETS->radius + GD_ENEMIES->radius * GD_ENEMIES->radius) {
							if (canDamageEnemy(tmpType))
								GD_ENEMIES->life -= GETDATA_BULLETS->damage * getPlayerDamageFactor(GETDATA_BULLETS->ownerId);
					
							GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;
					
							GETDATA_BULLETS->canDealDamages = sfFalse;
							GETDATA_BULLETS->rect = IntRect(0, 1071, 69, 59);
							GETDATA_BULLETS->origin = vector2f(0.f, 30.f);
							break;
						}
					}

					else if (sfFloatRect_intersects(&GETDATA_BULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {

						if (canDamageEnemy(tmpType))
							GD_ENEMIES->life -= GETDATA_BULLETS->damage * getPlayerDamageFactor(GETDATA_BULLETS->ownerId);

						GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;

						GETDATA_BULLETS->canDealDamages = sfFalse;
						GETDATA_BULLETS->rect = IntRect(0, 1071, 69, 59);
						GETDATA_BULLETS->origin = vector2f(0.f, 30.f);

						//playerBulletsList->erase(&playerBulletsList, i);
						break;

					}
				}

				else if (tmp.type == PLAYER_LASER) {
					if (sfFloatRect_intersects(&GETDATA_BULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {

						float damageFactor = 1.f;
						if (getPlayerDamageFactor(GETDATA_BULLETS->ownerId) == 2) {
							damageFactor = 2.f;
						}
						GD_ENEMIES->ftimeInAOE += dt * GETDATA_BULLETS->fDamagePerSecond * damageFactor;
						GD_ENEMIES->isLasered = sfTrue;

						if (GD_ENEMIES->ftimeInAOE > 1.f) {
							GD_ENEMIES->ftimeInAOE = 0.f;
							if (canDamageEnemy(tmpType))
								GD_ENEMIES->life -= 1;

							GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;
						}
					}
				}

				else if (tmp.type == PLAYER_MINES)
				{
					// basic mine
					if (GETDATA_BULLETS->rect.top < 2460) {

						if (tmpType == TAMER) {
							if (GetSqrMagnitude(CreateVector(GETDATA_BULLETS->pos, AddVectors(GD_ENEMIES->pos, GD_ENEMIES->originToCenter))) <= GETDATA_BULLETS->radius * GETDATA_BULLETS->radius + GD_ENEMIES->radius * GD_ENEMIES->radius) {
								if (canDamageEnemy(tmpType))
									GD_ENEMIES->life -= GETDATA_BULLETS->damage * getPlayerDamageFactor(GETDATA_BULLETS->ownerId);

								GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;

								GETDATA_BULLETS->rect = IntRect(898, 2465, 898, 798);
								GETDATA_BULLETS->angle = 0.f;
								GETDATA_BULLETS->origin = vector2f(449.f, 399.f);

								GETDATA_BULLETS->mine.radius = 35.f;
							}
						}

						else if (sfFloatRect_intersects(&GETDATA_BULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {

							if (canDamageEnemy(tmpType))
								GD_ENEMIES->life -= GETDATA_BULLETS->damage;

							GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;

							GETDATA_BULLETS->rect = IntRect(898, 2465, 898, 798);
							GETDATA_BULLETS->angle = 0.f;
							GETDATA_BULLETS->origin = vector2f(449.f, 399.f);

							GETDATA_BULLETS->mine.radius = 35.f;
						}
					}
					// exploded mine
					else if (GETDATA_BULLETS->mine.radius > 0.f) {
						if (GetSqrMagnitude(CreateVector(GETDATA_BULLETS->pos, AddVectors(GD_ENEMIES->pos, GD_ENEMIES->originToCenter))) <= GETDATA_BULLETS->mine.radius * GETDATA_BULLETS->mine.radius + GD_ENEMIES->radius * GD_ENEMIES->radius) {
							if (canDamageEnemy(tmpType))
								GD_ENEMIES->life -= GETDATA_BULLETS->damage;

							GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;
						}
					}
				}

				else if (tmp.type == PLAYER_FLAMETHROWER) {
					
					if (sfFloatRect_intersects(&GETDATA_BULLETS->bounds, &GD_ENEMIES->bounds, NULL)) {

						float damageFactor = 1.f;
						if (getPlayerDamageFactor(GETDATA_BULLETS->ownerId) == 2) {
							damageFactor = 2.f;
						}
						GD_ENEMIES->ftimeInAOE += dt * GETDATA_BULLETS->fDamagePerSecond * damageFactor;

						if (GD_ENEMIES->ftimeInAOE > 1.f) {
							GD_ENEMIES->ftimeInAOE = 0.f;
							if (canDamageEnemy(tmpType))
								GD_ENEMIES->life -= 1;

							GD_ENEMIES->lastDamageSource = GETDATA_BULLETS->ownerId;
						}
					}
				}
			}
		}

		if (GETDATA_BULLETS->id == ENEMY_ID_BULLET && GETDATA_BULLETS->canDealDamages)
		{
			for (int j = 0; j < nbPlayer; j++)
			{
				if (sfFloatRect_intersects(&GETDATA_BULLETS->bounds, &player[j].bounds, NULL)) {

					//player[j].life -= GETDATA_BULLETS->damage;
					damagePlayer(j, GETDATA_BULLETS->damage);

					//bulletsList->erase(&bulletsList, i);
					GETDATA_BULLETS->canDealDamages = sfFalse;

					if (tmp.type == ENEMY_YELLOW_BULLET) {
						GETDATA_BULLETS->rect = IntRect(0, 1192, 66, 62);
						GETDATA_BULLETS->origin = vector2f(66.f, 31.f);
					}
					else if (tmp.type == ENEMY_GREEN_BULLET) {
						GETDATA_BULLETS->rect = IntRect(0, 1130, 66, 62);
						GETDATA_BULLETS->origin = vector2f(66.f, 31.f);
					}
					continue;
				}
			}
		}
		
	}
}

void displayBullets(Window* _window)
{
	for (int i = 0; i < bulletsList->size(bulletsList); i++)
	{


		if (GETDATA_BULLETS->type == PLAYER_FLAMETHROWER) {
			//sfRectangleShape_setPosition(tmpRectangle, vector2f(GETDATA_BULLETS->bounds.left, GETDATA_BULLETS->bounds.top));
			//sfRectangleShape_setSize(tmpRectangle, vector2f(GETDATA_BULLETS->bounds.width, GETDATA_BULLETS->bounds.height));
			//sfRectangleShape_setFillColor(tmpRectangle, color(255, 0, 0, 20));
			//sfRenderTexture_drawRectangleShape(_window->renderTexture, tmpRectangle, NULL);
			continue; // keep this but to remove all above
		}

		if (GETDATA_BULLETS->id == PLAYER_ID_BULLET) {
			if (GETDATA_BULLETS->type == PLAYER_LASER)
				sfSprite_setTexture(bulletsSprite, laserTexture, sfFalse);
			else
				sfSprite_setTexture(bulletsSprite, bulletsTexture, sfFalse);
		}
		else {
			if (GETDATA_BULLETS->type == ENEMY_WARNING_BULLET)
				sfSprite_setTexture(bulletsSprite, bulletsTexture, sfFalse);
			else if (GETDATA_BULLETS->canDealDamages)
				sfSprite_setTexture(bulletsSprite, enemyBulletsTexture, sfFalse);
			else
				sfSprite_setTexture(bulletsSprite, bulletsTexture, sfFalse);
		}

		sfSprite_setTextureRect(bulletsSprite, GETDATA_BULLETS->rect);
		sfSprite_setPosition(bulletsSprite, GETDATA_BULLETS->pos);
		sfSprite_setOrigin(bulletsSprite, GETDATA_BULLETS->origin);
		sfSprite_setRotation(bulletsSprite, GETDATA_BULLETS->angle);
		sfSprite_setScale(bulletsSprite, GETDATA_BULLETS->scale);
		sfRenderTexture_drawSprite(_window->renderTexture, bulletsSprite, NULL);

		GETDATA_BULLETS->bounds = sfSprite_getGlobalBounds(bulletsSprite);
	}
}

void deinitBullets()
{
	sfSprite_destroy(bulletsSprite);
	bulletsList->destroy(&bulletsList);
}
