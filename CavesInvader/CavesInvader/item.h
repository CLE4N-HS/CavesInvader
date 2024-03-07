#pragma once
#include "tools.h"
#include "stateManager.h"

#define MULTIPLIER_ITEM_DURATION 15.f

typedef enum ItemType {
	RANDOM_ITEM,
	SHIELD_ITEM,
	DAMAGE_ITEM,
	GAS_ITEM,
	BULLET_ITEM,
	LIFE_ITEM,
	TIMES2_ITEM,
	TIMES3_ITEM,
}ItemType;

typedef struct Items {
	ItemType type;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f scale;
	sfIntRect rect;
	float speed;

	sfFloatRect bounds;
	sfBool isCollectible;
	float deathTimer;

}Items;

void initItem(Window* _window);

void createItem(ItemType _type, sfVector2f _pos);

void updateItem(Window* _window);

void displayItem(Window* _window);

void deinitItem();
