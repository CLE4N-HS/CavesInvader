#include "enemy.h"
#include "textureManager.h"
#include "player.h"
#include "item.h"
#include "bullets.h"

#define GETDATA_ENEMIES STD_LIST_GETDATA(enemiesList, Enemies, i)

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

void addEnemy(enemyType _type, enemyState _state, enemyState _lastState, sfIntRect _rect, sfVector2f _origin, sfVector2f _originToCenter, float _radius, float _animTimer, float _timeBetweenFrames, sfVector2f _pos, sfVector2f _velocity, sfVector2f _forward, float _speed, int _life, int _damage, int _scoreValue, float _startFocusingPos, float _startAttackingMoment, float _startAttackingTimer, float _focusingTimer, sfBool _upMovement)
{
	Enemies tmp;
	tmp.type = _type;
	tmp.state = _state;
	tmp.lastState = _lastState;
	tmp.rect = _rect;
	tmp.origin = _origin;
	tmp.originToCenter = _originToCenter;
	tmp.radius = _radius;
	tmp.animTimer = _animTimer;
	tmp.timeBetweenFrames = _timeBetweenFrames;
	tmp.pos = _pos;
	tmp.velocity = _velocity;
	tmp.forward = _forward;
	tmp.speed = _speed;
	tmp.life = _life;
	tmp.damage = _damage;
	tmp.scoreValue = _scoreValue;

	if (tmp.type == VENGELFY || tmp.type == ENRAGED_VENGEFLY) {
		tmp.vengefly.startFocusingPos = _startFocusingPos;
		tmp.vengefly.startAttackingMoment = _startAttackingMoment;
		tmp.vengefly.startAttackingTimer = _startAttackingTimer;
	}
	else if (tmp.type == HOPPER || tmp.type == ENRAGED_HOPPER) {
		tmp.hopper.startAttackingPos = _startFocusingPos;
		tmp.hopper.startAttackingMoment = _startAttackingMoment;
		tmp.hopper.startAttackingTimer = _startAttackingTimer;
		tmp.hopper.focusingTimer = _focusingTimer;
		tmp.hopper.upMovement = _upMovement;
	}


	tmp.bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	tmp.ftimeInAOE = 0.f;
	tmp.lastDamageSource = -1;

	STD_LIST_PUSHBACK(enemiesList, tmp);
}

void createEnemy(enemyType _type)
{
	enemyState state = FLYING;
	enemyState lastState = NO_STATE;
	sfIntRect rect = IntRect(0, 0, 0, 0);
	sfVector2f origin = VECTOR2F_NULL;
	sfVector2f originToCenter = VECTOR2F_NULL;
	float radius = 0.f;
	sfVector2f pos = VECTOR2F_NULL;
	sfVector2f velocity = VECTOR2F_NULL;
	float speed = 0.f;
	sfVector2f forward = VECTOR2F_NULL;
	float animTimer = 0.f;
	float timeBetweenFrames = 0.f;
	int life = 0;
	int damage = 0;
	int scoreValue = 0;

	// unique parameters
	float startFocusingPos = 0.f;
	float startAttackingMoment = 0.f;
	float startAttackingTimer = 0.f;
	
	float focusingTimer = 0.f;
	sfBool upMovement = sfFalse;

	switch (_type)
	{
	case VENGELFY:
		//rect = IntRect(0, 411, 167, 124);
		//origin = vector2f(31.f, 75.f);
		originToCenter = vector2f(36.f, -12.f);
		radius = 80.f;
		pos = vector2f(1951.f, rand_float(31.f, 1031.f)); // TODO right rand Y pos
		//forward = Normalize(CreateVector(pos, getClosestPlayerPos(pos)));
		//speed = 100.f;
		//timeBetweenFrames = 0.1f;
		life = 2;
		damage = 1;
		scoreValue = 10;
		startFocusingPos = rand_float(1400.f, 1780.f);
		startAttackingMoment = rand_float(1.f, 3.f);
		break;
	case ENRAGED_VENGEFLY:
		//*_rect = IntRect(0, 747, 259, 206);
		//*_origin = vector2f(48.f, 132.f);
		originToCenter = vector2f(48.f, -18.f);
		radius = 120.f;
		pos = vector2f(1968.f, rand_float(48.f, 1006.f)); // TODO right rand Y pos
		//forward = Normalize(CreateVector(pos, getClosestPlayerPos(pos)));
		//speed = 100.f;
		//timeBetweenFrames = 0.1f;
		life = 2;
		damage = 2;
		scoreValue = 25;
		startFocusingPos = rand_float(1400.f, 1780.f);
		startAttackingMoment = rand_float(0.5f, 1.5f);
		break;
	case HOPPER:
		origin = vector2f(38.f, 105.f);
		originToCenter = vector2f(68.f, -18.f);
		radius = 88.f;
		pos = vector2f(1958.f, rand_float(105.f, 928.f));
		life = 10;
		damage = 1;
		scoreValue = 20;
		rect = IntRect(0, 1283, 190, 257);
		animTimer = 0.f;
		timeBetweenFrames = 0.1f;

		startFocusingPos = rand_float(1400.f, 1770.f);
		startAttackingMoment = rand_float(1.5f, 2.5f);
		focusingTimer = 0.f;
		
		if (pos.y < 540.f)
			upMovement = sfFalse;
		else
			upMovement = sfTrue;

		break;
	case ENRAGED_HOPPER:
		origin = vector2f(56.f, 152.f);
		originToCenter = vector2f(96.f, -14.f);
		radius = 123.f;
		pos = vector2f(1976.f, rand_float(152.f, 761.f));
		life = 20;
		damage = 1;
		scoreValue = 50;
		rect = IntRect(0, 1540, 281, 371);
		animTimer = 0.f;
		timeBetweenFrames = 0.1f;

		startFocusingPos = rand_float(1400.f, 1700.f);
		startAttackingMoment = rand_float(0.75f, 1.5f);
		focusingTimer = 0.f;
		
		if (pos.y < 540.f)
			upMovement = sfFalse;
		else
			upMovement = sfTrue;

		break;
	default:
		break;
	}

	addEnemy(_type, state, lastState, rect, origin, originToCenter, radius, animTimer, timeBetweenFrames, pos, velocity, forward, speed, life, damage, scoreValue, startFocusingPos, startAttackingMoment, startAttackingTimer, focusingTimer, upMovement);
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
			*_speed = 900.f;
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
			*_speed = 1100.f;
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

	case HOPPER:

		switch (_state)
		{
		case FLYING:
			*_speed = 100.f;
			break;
		case ATTACKING:
			*_speed = 80.f;
			break;
		case DEAD:
			*_rect = IntRect(0, 289, 188, 256);
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
			break;
		default:
			break;
		}
		break;

	case ENRAGED_HOPPER:

		switch (_state)
		{
		case FLYING:
			*_speed = 200.f;
			break;
		case ATTACKING:
			*_speed = 140.f;
			break;
		case DEAD:
			*_rect = IntRect(0, 545, 271, 367);
			*_animTimer = 0.f;
			*_timeBetweenFrames = 0.1f;
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

	// to remove
	static float timer = 0.f;
	timer += dt;

	
	if (isKeyboardOrControllerButtonPressed(sfKeyA, A_XBOX) && timer > 0.2f) {
		createEnemy(VENGELFY);
		timer = 0.f;
	}
	if (isKeyboardOrControllerButtonPressed(sfKeyE, B_XBOX) && timer > 0.2f) {
		createEnemy(ENRAGED_VENGEFLY);
		timer = 0.f;
	}
	if (isKeyboardOrControllerButtonPressed(sfKeyR, X_XBOX) && timer > 0.2f) {
		createEnemy(HOPPER);
		timer = 0.f;
	}
	if (isKeyboardOrControllerButtonPressed(sfKeyT, Y_XBOX) && timer > 0.2f) {
		createEnemy(ENRAGED_HOPPER);
		timer = 0.f;
	}


	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		Enemies tmp;



		sfVector2f tmpVelocity = VECTOR2F_NULL;
		sfBool honorableKill = sfTrue;

		if (GETDATA_ENEMIES->pos.x < (-GETDATA_ENEMIES->rect.width + GETDATA_ENEMIES->origin.y)) {
			enemiesList->erase(&enemiesList, i);
			continue;
		}

		// players collisions
		if (GETDATA_ENEMIES->state != DEAD) {
			for (int j = 0; j < nbPlayer; j++)
			{
				if (sfFloatRect_intersects(&GETDATA_ENEMIES->bounds, &player[j].bounds, NULL)) {
					//player[j].life -= GETDATA_ENEMIES->damage;
					damagePlayer(j, GETDATA_ENEMIES->damage);
					GETDATA_ENEMIES->life = 0;
					honorableKill = sfFalse;
				}
			}
		}

		if (GETDATA_ENEMIES->life <= 0 && GETDATA_ENEMIES->state != DEAD) {
			if (honorableKill) {
				createItem(RANDOM_ITEM, GETDATA_ENEMIES->pos);

				common.score += GETDATA_ENEMIES->scoreValue * common.multiplier;

				if (GETDATA_ENEMIES->lastDamageSource >= 0) {
					increasePlayerKillCount(GETDATA_ENEMIES->lastDamageSource);
				}
			}

			GETDATA_ENEMIES->state = DEAD;
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
				break;

			case FOCUSING:
				GETDATA_ENEMIES->vengefly.startAttackingTimer += dt;

				if (GETDATA_ENEMIES->vengefly.startAttackingTimer > GETDATA_ENEMIES->vengefly.startAttackingMoment)
					GETDATA_ENEMIES->state = ATTACKING;


				GETDATA_ENEMIES->animTimer += dt;

				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 3, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);
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
				break;

			case DEAD:
				GETDATA_ENEMIES->animTimer += dt;

				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 4, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);

				if (getFrameX(GETDATA_ENEMIES->rect) > 3) {
					enemiesList->erase(&enemiesList, i);
					continue;
				}
				break;
			default:
				break;
			}
		}
		else if (tmp.type == HOPPER || tmp.type == ENRAGED_HOPPER) {

			switch (tmp.state)
			{
			case FLYING:
				GETDATA_ENEMIES->pos.x -= GETDATA_ENEMIES->speed * dt;

				if (GETDATA_ENEMIES->pos.x < GETDATA_ENEMIES->hopper.startAttackingPos)
					GETDATA_ENEMIES->state = ATTACKING;

				GETDATA_ENEMIES->animTimer += dt;
				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 3, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);
				break;

			case ATTACKING:
				tmpVelocity = MultiplyVector(GETDATA_ENEMIES->forward, dt * GETDATA_ENEMIES->speed);

				GETDATA_ENEMIES->pos = AddVectors(GETDATA_ENEMIES->pos, tmpVelocity);

				if (GETDATA_ENEMIES->hopper.upMovement)
					GETDATA_ENEMIES->hopper.focusingTimer -= dt;
				else
					GETDATA_ENEMIES->hopper.focusingTimer += dt;

				GETDATA_ENEMIES->forward.y = cosf(GETDATA_ENEMIES->hopper.focusingTimer);


				GETDATA_ENEMIES->hopper.startAttackingTimer += dt;

				if (GETDATA_ENEMIES->hopper.startAttackingTimer > GETDATA_ENEMIES->hopper.startAttackingMoment) {

					GETDATA_ENEMIES->hopper.startAttackingTimer = 0.f;
					if (tmp.type == HOPPER) {
						createBullets(ENEMY_YELLOW_BULLET, i, GETDATA_ENEMIES->pos, 180.f);
						GETDATA_ENEMIES->hopper.startAttackingMoment = rand_float(1.5f, 2.5f);
					}
					else {
						createBullets(ENEMY_GREEN_BULLET, i, GETDATA_ENEMIES->pos, 180.f);
						GETDATA_ENEMIES->hopper.startAttackingMoment = rand_float(0.75f, 1.5f);
					}
				}


				GETDATA_ENEMIES->animTimer += dt;
				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 3, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);
				break;

			case DEAD:
				GETDATA_ENEMIES->animTimer += dt;

				Animator(&GETDATA_ENEMIES->rect, &GETDATA_ENEMIES->animTimer, 4, 1, GETDATA_ENEMIES->timeBetweenFrames, 0.f);

				if (getFrameX(GETDATA_ENEMIES->rect) > 3) {
					enemiesList->erase(&enemiesList, i);
					continue;
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
	enemiesList->destroy(&enemiesList);
}
