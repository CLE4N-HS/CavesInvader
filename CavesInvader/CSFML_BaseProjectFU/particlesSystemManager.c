#include "particlesSystemManager.h"

#define GETDATA_PARTICLES STD_LIST_GETDATA(list, Particles, i)
#define PARTICLES_LIFETIME GETDATA_PARTICLES->timerAlive / GETDATA_PARTICLES->timeToDie
#define GP GETDATA_PARTICLES

sfSprite* spr;
sfCircleShape* circle;

sfTexture* tmpTexture;
sfIntRect tmpIntRect;

void initParticlesSystem()
{
	//list = stdList_Create(sizeof(Particles), 0);
	list = STD_LIST_CREATE(Particles, 0);
	spr = sfSprite_create();
	circle = sfCircleShape_create();
	tmpTexture = sfTexture_createFromFile(TEXTURE_PATH"coffre32.png", NULL);
	tmpIntRect = IntRect(0, 0, 17, 23);
}

void AddParticles(sfVector2f _pos, sfVector2f _startScale, sfVector2f _endScale, sfVector2f _origin, float _angle, float _rotationSpeed, float _angularDrag, float _force, float _endDrag, sfColor _startColor, sfColor _endColor, float _timeToDie, TypeParticles _type, char* _name, sfIntRect _rect, sfRenderStates* _renderState, float _timeToSpawn, float psTimeToDie)
{
	Particles tmp;
	tmp.pos = _pos;
	tmp.startPos = _pos;
	tmp.scale = _startScale;
	tmp.startScale = tmp.scale;
	tmp.endScale = _endScale;
	tmp.origin = _origin;
	tmp.circle.radius = 1.f;
	tmp.circle.startRadius = tmp.circle.radius;
	tmp.circle.endRadius = 1.f;
	//tmp.origin = vector2f(tmp.circle.radius, tmp.circle.radius);
	tmp.sprite.rect;
	tmp.angle = _angle;
	tmp.startAngle = tmp.angle;
	tmp.angleRotation = tmp.angle;
	tmp.rotationSpeed = _rotationSpeed;
	tmp.angularDrag = _angularDrag;
	tmp.endAngularDrag = tmp.angularDrag;
	tmp.drag = 0.f;
	tmp.endDrag = _endDrag;
	tmp.force = _force;

	tmp.circle.color = _startColor;
	tmp.circle.startColor = tmp.circle.color;
	tmp.circle.endColor = _endColor;
	tmp.timerAlive = 0.f;
	tmp.timeToDie = _timeToDie;
	tmp.timerToSpawn = _timeToSpawn;
	tmp.type = _type;
	strcpy(tmp.name, _name);
	tmp.renderState = _renderState;
	
	tmp.psTimeToDie = psTimeToDie;

	tmp.mass = 1.f;

	list->push_back(&list, &tmp);
}


void CreateParticles(sfVector2f _pos, sfVector2f _startScale, sfVector2f _endScale, sfVector2f _origin, float _angleMin, float _angleMax, float _rotationSpeed, float _angularDrag, float _forceMin, float _forceMax, float _endDrag, sfColor _startColor, sfColor _endColor, float _timeToDieMin, float _timeToDieMax, int _nbParticles, char* _name, sfIntRect _rect, sfRenderStates* _renderState, float _timeToSpawnMin, float _timeToSpawnMax, float psTimeToDie)
{
	for (int i = 0; i < _nbParticles; i++)
	{
		float angle = rand_float(_angleMin, _angleMax);
		float force = rand_float(_forceMin, _forceMax);
		float timeToDie = rand_float(_timeToDieMin, _timeToDieMax);
		float timeToSpawn = rand_float(_timeToSpawnMin, _timeToSpawnMax);
		sfTexture* tmpTexture;
		tmpTexture = GetTexture(_name);
		TypeParticles type;
		if (tmpTexture == NULL)
			type = CIRCLE_PARTICLES;
		else
			type = SPRITE_PARTICLES;
		AddParticles(_pos, _startScale, _endScale, _origin, angle, _rotationSpeed, _angularDrag, force, _endDrag, _startColor, _endColor, timeToDie, type, _name, _rect, _renderState, timeToSpawn, psTimeToDie);
	}
}

void updateParticlesSystem(Window* _window)
{
	float dt = getDeltaTime();
	for (int i = 0; i < list->size(list); i++)
	{
		GETDATA_PARTICLES->timerToSpawn -= getDeltaTime();
		GETDATA_PARTICLES->psTimeToDie -= dt;

		if (GETDATA_PARTICLES->timerToSpawn > 0)
			continue;

		GETDATA_PARTICLES->timerAlive += dt;

		GETDATA_PARTICLES->drag = Lerp(0.f, GETDATA_PARTICLES->endDrag, PARTICLES_LIFETIME);
		GETDATA_PARTICLES->angularDrag = Lerp(0.f, GETDATA_PARTICLES->endAngularDrag, PARTICLES_LIFETIME);

		sfVector2f normVecAngle = PolarCoords(vector2f(0.0f,0.0f), 1.f, GETDATA_PARTICLES->angle * DEG2RAD);

		GETDATA_PARTICLES->velocity = MultiplyVector(normVecAngle, GETDATA_PARTICLES->force);
		GETDATA_PARTICLES->pos = AddVectors(GETDATA_PARTICLES->pos, MultiplyVector(GETDATA_PARTICLES->velocity, dt / (1.f + GETDATA_PARTICLES->drag)));

		GETDATA_PARTICLES->circle.color = color_lerp(GETDATA_PARTICLES->circle.startColor, GETDATA_PARTICLES->circle.endColor, PARTICLES_LIFETIME);

		GETDATA_PARTICLES->angleRotation += GETDATA_PARTICLES->rotationSpeed * dt / (1.f + GETDATA_PARTICLES->angularDrag);

		GETDATA_PARTICLES->scale = LerpVector(GETDATA_PARTICLES->startScale, GETDATA_PARTICLES->endScale, PARTICLES_LIFETIME);

		// BIG SWITCH

		if (GETDATA_PARTICLES->timerAlive > GETDATA_PARTICLES->timeToDie)
		{
			if (GP->psTimeToDie > 0)
			{
				AddParticles(GP->startPos, GP->startScale, GP->endScale, GP->origin, GP->angle, GP->rotationSpeed, GP->angularDrag, GP->force, GP->endDrag, GP->circle.startColor, GP->circle.endColor, GP->timeToDie, GP->type, GP->name, GP->sprite.rect, GP->renderState, 0.f, GP->psTimeToDie);
			}

			list->erase(&list, i);
		}

	}
}

void displayParticlesSystem(Window* _window)
{
	for (int i = 0; i < list->size(list); i++)
	{
		if (GETDATA_PARTICLES->timerToSpawn > 0)
			continue;

		if (STD_LIST_GETDATA(list, Particles, i)->type == CIRCLE_PARTICLES)
		{
			sfCircleShape_setPosition(circle, GETDATA_PARTICLES->pos);
			sfCircleShape_setScale(circle, GETDATA_PARTICLES->scale);
			sfCircleShape_setRadius(circle, 1.f);
			sfCircleShape_setFillColor(circle, GETDATA_PARTICLES->circle.color);
			sfCircleShape_setOrigin(circle, GETDATA_PARTICLES->origin);
			sfCircleShape_setRotation(circle, GETDATA_PARTICLES->angleRotation);
			sfRenderTexture_drawCircleShape(_window->renderTexture, circle, GETDATA_PARTICLES->renderState);
		}
		else if (GETDATA_PARTICLES->type == SPRITE_PARTICLES)
		{
			sfSprite_setPosition(spr, GETDATA_PARTICLES->pos);
			sfSprite_setOrigin(spr, GETDATA_PARTICLES->origin);
			//sfSprite_setTextureRect(spr) we might need a texture rect
			sfSprite_setScale(spr, GETDATA_PARTICLES->scale);
			sfSprite_setTexture(spr, GetTexture(GETDATA_PARTICLES->name), sfTrue);
			sfSprite_setRotation(spr, GETDATA_PARTICLES->angleRotation);
			sfRenderTexture_drawSprite(_window->renderTexture, spr, GETDATA_PARTICLES->renderState);
		}
	}
}


sfVector2f AddForceParticles(sfVector2f _initialVelocity, sfVector2f _force, float _mass)
{
	if (_mass == 0.f) _mass = 1.f;
	return AddVectors(_initialVelocity, MultiplyVector(_force, 1.f / _mass));
}