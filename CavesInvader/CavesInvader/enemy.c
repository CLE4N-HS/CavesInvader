#include "enemy.h"
#include "textureManager.h"
#include "player.h"


sfSprite* enemySprite;

sfTexture* enemyTexture;

sfTexture* deadEnemyTexture;


void initEnemy(Window* _window)
{
	enemiesList = STD_LIST_CREATE(Enemies, 0);

	enemySprite = sfSprite_create();

	enemyTexture = GetTexture("enemies");
	deadEnemyTexture = GetTexture("deadEnemies");

	sfSprite_setTexture(enemySprite, enemyTexture, sfTrue);
}

void addEnemy(enemyType _type, enemyState _state, enemyState _lastState, sfIntRect _rect, sfVector2f _origin, float _animTimer, float _timeBetweenFrames, sfVector2f _pos, sfVector2f _velocity, sfVector2f _forward, float _speed, int _life, int _damage, float _startFocusingPos, float _startAttackingMoment, float _startAttackingTimer)
{
	Enemies tmp;
	tmp.type = _type;
	tmp.state = _state;
	tmp.lastState = _lastState;
	tmp.rect = _rect;
	tmp.origin = _origin;
	tmp.animTimer = _animTimer;
	tmp.timeBetweenFrames = _timeBetweenFrames;
	tmp.pos = _pos;
	tmp.velocity = _velocity;
	tmp.forward = _forward;
	tmp.speed = _speed;
	tmp.life = _life;
	tmp.damage = _damage;
	tmp.vengefly.startFocusingPos = _startFocusingPos;
	tmp.vengefly.startAttackingMoment = _startAttackingMoment;
	tmp.vengefly.startAttackingTimer = _startAttackingTimer;
	tmp.bounds = FlRect(0.f, 0.f, 0.f, 0.f);

	STD_LIST_PUSHBACK(enemiesList, tmp);
}

void createEnemy(enemyType _type)
{
	enemyState state = FLYING;
	enemyState lastState = NO_STATE;
	sfIntRect rect = IntRect(0, 0, 0, 0);
	sfVector2f origin = VECTOR2F_NULL;
	sfVector2f pos = VECTOR2F_NULL;
	sfVector2f velocity = VECTOR2F_NULL;
	float speed = 0.f;
	sfVector2f forward = VECTOR2F_NULL;
	float animTimer = 0.f;
	float timeBetweenFrames = 0.f;
	int life = 0;
	int damage = 0;

	// unique parameters
	float startFocusingPos = 0.f;
	float startAttackingMoment = 0.f;
	float startAttackingTimer = 0.f;
	
	switch (_type)
	{
	case VENGELFY:
		//rect = IntRect(0, 411, 167, 124);
		//origin = vector2f(31.f, 75.f);
		pos = vector2f(1951.f, rand_float(31.f, 1031.f));
		//forward = Normalize(CreateVector(pos, getClosestPlayerPos(pos)));
		//speed = 100.f;
		//timeBetweenFrames = 0.1f;
		life = 2;
		damage = 1;
		startFocusingPos = rand_float(1400.f, 1780.f);
		startAttackingMoment = rand_float(1.f, 3.f);
		break;
	case ENRAGED_VENGEFLY:
		//*_rect = IntRect(0, 747, 259, 206);
		//*_origin = vector2f(48.f, 132.f);
		pos = vector2f(1968.f, rand_float(48.f, 1006.f));
		//forward = Normalize(CreateVector(pos, getClosestPlayerPos(pos)));
		//speed = 100.f;
		//timeBetweenFrames = 0.1f;
		life = 2;
		damage = 2;
		startFocusingPos = rand_float(1400.f, 1780.f);
		startAttackingMoment = rand_float(0.5f, 1.5f);
		break;
	default:
		break;
	}

	addEnemy(_type, state, lastState, rect, origin, animTimer, timeBetweenFrames, pos, velocity, forward, speed, life, damage, startFocusingPos, startAttackingMoment, startAttackingTimer);
}

void setupEnemy(enemyType _type, enemyState _state, sfIntRect* _rect, sfVector2f* _origin, float* _animTimer, float *_timeBetweenFrames, sfVector2f* _forward, sfVector2f _pos, float* _speed)
{

	switch (_type)
	{
	case VENGELFY:

		switch (_state)
		{
		case FLYING:
			*_rect = IntRect(0, 411, 167, 124);
			*_origin = vector2f(31.f, 75.f);
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			*_forward = vector2f(-1.f, 0.f);
			*_speed = 100.f;
			break;
		case FOCUSING:
			//*_rect = IntRect(0, 411, 167, 124);
			//*_origin = vector2f(31.f, 75.f);
			//*_rect = IntRect(0, 535, 166, 114);
			//*_origin = vector2f(31.f, 68.f);
			*_timeBetweenFrames = 0.1f;
			// no movement
			break;
		case ATTACKING:
			*_rect = IntRect(0, 535, 166, 114);
			*_origin = vector2f(31.f, 68.f);
			//*_rect = IntRect(0, 649, 166, 98);
			//*_origin = vector2f(31.f, 52.f);
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			*_speed = 700.f;
			*_forward = Normalize(CreateVector(_pos, getClosestPlayerPos(_pos)));
			break;
		case DEAD:
			*_rect = IntRect(0, 0, 166, 114);
			//*_origin = we'll see;
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			// no movement
			break;
		default:
			break;
		}
		break;

	case ENRAGED_VENGEFLY:

		switch (_state)
		{
		case FLYING:
			*_rect = IntRect(0, 747, 259, 206);
			*_origin = vector2f(48.f, 132.f);
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			*_forward = vector2f(-1.f, 0.f);
			*_speed = 200.f;
			break;
		case FOCUSING:
			// same as flying
			//*_rect = IntRect(0, 411, 167, 124);
			//*_origin = vector2f(31.f, 75.f);
			//*_rect = IntRect(0, 535, 166, 114);
			//*_origin = vector2f(31.f, 68.f);
			*_timeBetweenFrames = 0.1f;
			// no movement
			break;
		case ATTACKING:
			*_rect = IntRect(0, 953, 257, 177);
			*_origin = vector2f(48.f, 100.f); //,82.f
			//*_rect = IntRect(0, 649, 166, 98);
			//*_origin = vector2f(31.f, 52.f);
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			*_speed = 900.f;
			*_forward = Normalize(CreateVector(_pos, getClosestPlayerPos(_pos)));
			break;
		case DEAD:
			*_rect = IntRect(0, 114, 255, 175);
			//*_origin = we'll see;
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			// no movement
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void updateEnemy(Window* _window)
{
	float dt = getDeltaTime();

	static float timer = 0.f;
	timer += dt;

	//tmpPos.x -= dt * 1000.f;
	//if (tmpPos.x < -500.f) {
	//	tmpPos = vector2f(2000.f, rand_float(0.f, 800.f));
	//}

	if (isKeyboardOrControllerButtonPressed(sfKeyA, A_XBOX) && timer > 0.2f) {
		createEnemy(VENGELFY);
		timer = 0.f;
	}
	if (isKeyboardOrControllerButtonPressed(sfKeyE, B_XBOX) && timer > 0.2f) {
		createEnemy(ENRAGED_VENGEFLY);
		timer = 0.f;
	}


	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		Enemies tmp;



		sfVector2f tmpVelocity = VECTOR2F_NULL;

		if (GETDATA_ENEMIES->pos.x < (-GETDATA_ENEMIES->rect.width + GETDATA_ENEMIES->origin.y)) {
			enemiesList->erase(&enemiesList, i);
			continue;
		}

		if (GETDATA_ENEMIES->life <= 0) {
			GETDATA_ENEMIES->state = DEAD;
		}

		// players collisions
		if (GETDATA_ENEMIES->state != DEAD) {
			for (int j = 0; j < nbPlayer; j++)
			{
				if (sfFloatRect_intersects(&GETDATA_ENEMIES->bounds, &player[j].bounds, NULL)) {
					player[j].life -= GETDATA_ENEMIES->damage;
					GETDATA_ENEMIES->state = DEAD;
				}
			}
		}

		tmp.type = GETDATA_ENEMIES->type;
		tmp.state = GETDATA_ENEMIES->state;
		tmp.lastState = GETDATA_ENEMIES->lastState;

		if (tmp.state != tmp.lastState) {
			setupEnemy(GETDATA_ENEMIES->type, GETDATA_ENEMIES->state, &GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->origin, &GETDATA_ENEMIES->animTimer, &GETDATA_ENEMIES->timeBetweenFrames, &GETDATA_ENEMIES->forward, GETDATA_ENEMIES->pos, &GETDATA_ENEMIES->speed);
			GETDATA_ENEMIES->lastState = tmp.state;
		}

		if (tmp.type == VENGELFY || tmp.type == ENRAGED_VENGEFLY)
		{
			switch (tmp.state)
			{
			case FLYING:
				tmp.pos = GETDATA_ENEMIES->pos;
				tmpVelocity = MultiplyVector(GETDATA_ENEMIES->forward, dt * GETDATA_ENEMIES->speed);

				GETDATA_ENEMIES->pos = AddVectors(tmp.pos, tmpVelocity);

				if (tmp.pos.x < GETDATA_ENEMIES->vengefly.startFocusingPos)
					GETDATA_ENEMIES->state = FOCUSING;

				GETDATA_ENEMIES->animTimer += dt;

				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 3, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);
				// correct origin problems but it was a feature :0
				// 
				//int tmpFrameX = GETDATA_ENEMIES->rect.left;

				//if (tmpFrameX < 100.f)
				//	GETDATA_ENEMIES->origin = vector2f(31.f, 75.f);
				//else if (tmpFrameX < 200.f)
				//	GETDATA_ENEMIES->origin = vector2f(31.f, 79.f);
				//else
				//	GETDATA_ENEMIES->origin = vector2f(31.f, 74.f);
				break;

			case FOCUSING:
				GETDATA_ENEMIES->vengefly.startAttackingTimer += dt;

				if (GETDATA_ENEMIES->vengefly.startAttackingTimer > GETDATA_ENEMIES->vengefly.startAttackingMoment)
					GETDATA_ENEMIES->state = ATTACKING;


				GETDATA_ENEMIES->animTimer += dt;

				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 3, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);

				//Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 2, 0, GETDATA_ENEMIES->timeBetweenFrames, 0.f);
				break;

			case ATTACKING:
				tmpVelocity = MultiplyVector(GETDATA_ENEMIES->forward, dt * GETDATA_ENEMIES->speed);

				GETDATA_ENEMIES->pos = AddVectors(GETDATA_ENEMIES->pos, tmpVelocity);

				if (GETDATA_ENEMIES->rect.left < 50.f) {
					GETDATA_ENEMIES->animTimer += dt;

					if (GETDATA_ENEMIES->animTimer > GETDATA_ENEMIES->timeBetweenFrames) {
						GETDATA_ENEMIES->rect.left += GETDATA_ENEMIES->rect.width;
					}
				}

				//Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 2, 0, GETDATA_ENEMIES->timeBetweenFrames, 0.f);
				break;
			case DEAD:
				GETDATA_ENEMIES->animTimer += dt;

				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 4, 0, GETDATA_ENEMIES->timeBetweenFrames, 0.f);

				if (tmp.type == VENGELFY) {
					if (GETDATA_ENEMIES->rect.left > 340) {
						enemiesList->erase(&enemiesList, i);
						continue;
					}
				}
				else {
					if (GETDATA_ENEMIES->rect.left > 520) {
						enemiesList->erase(&enemiesList, i);
						continue;
					}
				}

				break;
			default:
				break;
			}
		}


			




	}
}

void displayEnemy(Window* _window)
{
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		if (GETDATA_ENEMIES->life > 0)
			sfSprite_setTexture(enemySprite, enemyTexture, sfFalse);
		else
			sfSprite_setTexture(enemySprite, deadEnemyTexture, sfFalse);

		sfSprite_setOrigin(enemySprite, GETDATA_ENEMIES->origin);
		sfSprite_setTextureRect(enemySprite, GETDATA_ENEMIES->rect);
		sfSprite_setPosition(enemySprite, GETDATA_ENEMIES->pos);
		sfRenderTexture_drawSprite(_window->renderTexture, enemySprite, NULL);

		GETDATA_ENEMIES->bounds = sfSprite_getGlobalBounds(enemySprite);
	}
}

void deinitEnemy()
{
	sfSprite_destroy(enemySprite);
}
