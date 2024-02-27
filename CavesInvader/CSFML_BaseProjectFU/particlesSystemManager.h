#pragma once
#include "tools.h"
#include "customMath.h"
#include "List.h"
#include "windowManager.h"
#include "textureManager.h"

typedef enum TypeParticles {
	SPRITE_PARTICLES = 0,
	CIRCLE_PARTICLES
}TypeParticles;

typedef struct CircleParticles {
	float radius;
	float startRadius;
	float endRadius;

	sfColor color;
	sfColor startColor;
	sfColor endColor;
}CircleParticles;

typedef struct SpriteParticles {
	sfIntRect rect;
	sfIntRect startRect;
	sfIntRect endRect;

	float opacity;
	float startOpacity;
	float endOpacity;

}SpriteParticles;

typedef struct Particles Particles;
struct Particles {

	TypeParticles type;
	char name[30];
	union Type
	{
		CircleParticles circle;
		SpriteParticles sprite;
	};

	sfVector2f pos;
	sfVector2f startPos;
	sfVector2f origin;
	sfVector2f scale;
	sfVector2f startScale;
	sfVector2f endScale;
	sfVector2f velocity;
	float angle;
	float angleRotation;
	float startAngle;
	float rotationSpeed;
	float angularDrag;
	float endAngularDrag;
	float force;
	float drag;
	float endDrag;
	float timerAlive;
	float timeToDie;
	float timerToSpawn;

	sfRenderStates* renderState;

	float psTimeToDie;

	float gravity;
	float mass;
};

stdList* list;

void initParticlesSystem();

void CreateParticles(sfVector2f _pos, sfVector2f _startScale, sfVector2f _endScale, sfVector2f _origin, float _angleMin, float _angleMax, float _rotationSpeed, float _angularDrag, float _forceMin, float _forceMax, float _endDrag, sfColor _startColor, sfColor _endColor, float _timeToDieMin, float _timeToDieMax, int _nbParticles, char* _name, sfIntRect _rect, sfRenderStates* _renderState, float _timeToSpawnMin, float _timeToSpawnMax, float psTimeToDie);

void updateParticlesSystem(Window* _window);

void displayParticlesSystem(Window* _window);

sfVector2f AddForceParticles(sfVector2f _initialVelocity, sfVector2f _force, float _mass);
