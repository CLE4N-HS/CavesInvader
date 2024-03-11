#include "item.h"
#include "textureManager.h"
#include "List.h"
#include "player.h"
#include "CustomMath.h"

#define GETDATA_ITEM STD_LIST_GETDATA(itemList, Items, i)

stdList* itemList;

sfSprite* itemSprite;

sfTexture* itemTexture;
sfTexture* deadItemTexture;


void addItem(ItemType _type, sfVector2f _pos, sfVector2f _origin, sfIntRect _rect, float _speed)
{
	Items tmp;
	tmp.type = _type;
	tmp.pos = _pos;
	tmp.origin = _origin;
	tmp.rect = _rect;
	tmp.speed = _speed;
	
	tmp.bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	tmp.isCollectible = sfTrue;
	tmp.deathTimer = 0.f;

	STD_LIST_PUSHBACK(itemList, tmp);
}

void initItem(Window* _window)
{
	itemSprite = sfSprite_create();

	itemTexture = GetTexture("items");
	deadItemTexture = GetTexture("deadItems");

	itemList = STD_LIST_CREATE(Items, 0);

}

void createItem(ItemType _type, sfVector2f _pos)
{
	if (_type == RANDOM_ITEM) {
		int random = iRand(0, 99);
		if (random < 3) _type = BULLET_ITEM;
		else if (random < 10) _type = SHIELD_ITEM;
		else if (random < 20) _type = LIFE_ITEM;
		else if (random < 30) _type = GAS_ITEM;
		else if (random < 40) _type = TIMES3_ITEM;
		else if (random < 50) _type = DAMAGE_ITEM;
		else _type = TIMES2_ITEM;
	}


	sfIntRect rect = IntRect(0, 0, 0, 0);
	sfVector2f origin = VECTOR2F_NULL;
	float speed = 250.f;

	switch (_type)
	{
	case SHIELD_ITEM:
		rect = IntRect(0, 3453, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	case DAMAGE_ITEM:
		rect = IntRect(0, 3538, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	case GAS_ITEM:
		rect = IntRect(0, 3623, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	case BULLET_ITEM:
		rect = IntRect(0, 3708, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	case LIFE_ITEM:
		rect = IntRect(0, 3793, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	case TIMES2_ITEM:
		rect = IntRect(0, 3878, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	case TIMES3_ITEM:
		rect = IntRect(0, 3963, 81, 85);
		origin = vector2f(41.f, 43.f);
		break;
	default:
		break;
	}

	addItem(_type, _pos, origin, rect, speed);
}

void collectItem(int i)
{
	GETDATA_ITEM->isCollectible = sfFalse;
	GETDATA_ITEM->rect = IntRect(0, 912, 153, 159);
	GETDATA_ITEM->origin = vector2f(77.f, 80.f);
}

void updateItem(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < itemList->size(itemList); i++)
	{
		if (!GETDATA_ITEM->isCollectible) {
			GETDATA_ITEM->deathTimer += dt;

			if (GETDATA_ITEM->deathTimer > 0.1f) {
				GETDATA_ITEM->deathTimer = 0.f;
				GETDATA_ITEM->rect.left += GETDATA_ITEM->rect.width;
				if (GETDATA_ITEM->rect.left > 350) {
					itemList->erase(&itemList, i);
				}
				
			}
			continue;
		}

		if (GETDATA_ITEM->pos.x < -GETDATA_ITEM->rect.width + GETDATA_ITEM->origin.x) {
			itemList->erase(&itemList, i);
			continue;
		}


		GETDATA_ITEM->pos.x -= dt * GETDATA_ITEM->speed;
	
		for (int j = 0; j < nbPlayer; j++)
		{
			if (sfFloatRect_intersects(&player[j].bounds, &GETDATA_ITEM->bounds, NULL)) {

				ItemType tmpType = GETDATA_ITEM->type;

				switch (tmpType)
				{
				case SHIELD_ITEM:
					if (!player[j].hasShield) {
						player[j].hasShield = sfTrue;
						collectItem(i);
					}
					break;
				case DAMAGE_ITEM:
					player[j].damageTimer = 10.f;
					collectItem(i);
					break;
				case GAS_ITEM:
					if (player[j].nbGas < 100) {
						player[j].nbGas = 100;
						collectItem(i);
					}
					break;
				case BULLET_ITEM:
					if (player[j].nbBullet < 3) {
						player[j].nbBullet += 1;
						collectItem(i);
					}
					break;
				case LIFE_ITEM:
					if (player[j].life < 3) {
						player[j].life += 1;
						collectItem(i);
					}
					break;
				case TIMES2_ITEM:
					if (common.multiplier <= 2) {
						common.multiplier = 2;
						common.multiplierTimer = MULTIPLIER_ITEM_DURATION;
						collectItem(i);
					}
					break;
				case TIMES3_ITEM:
					common.multiplier = 3;
					common.multiplierTimer = MULTIPLIER_ITEM_DURATION;
					collectItem(i);
					break;
				default:
					break;
				}
			}
		}
	}
}

void displayItem(Window* _window)
{
	for (int i = 0; i < itemList->size(itemList); i++)
	{
		if (GETDATA_ITEM->isCollectible)
			sfSprite_setTexture(itemSprite, itemTexture, sfFalse);
		else
			sfSprite_setTexture(itemSprite, deadItemTexture, sfFalse);
	
		sfSprite_setTextureRect(itemSprite, GETDATA_ITEM->rect);
		sfSprite_setPosition(itemSprite, GETDATA_ITEM->pos);
		sfSprite_setOrigin(itemSprite, GETDATA_ITEM->origin);
		sfRenderTexture_drawSprite(_window->renderTexture, itemSprite, NULL);
		
		GETDATA_ITEM->bounds = sfSprite_getGlobalBounds(itemSprite);
	}
}

void deinitItem()
{
	sfSprite_destroy(itemSprite);
	itemList->destroy(&itemList);
}
