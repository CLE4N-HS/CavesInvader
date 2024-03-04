#include "hud.h"
#include "textureManager.h"
#include "fontManager.h"

typedef struct Hud {
	sfVector2f mainPos;
	sfIntRect mainRect;

	sfVector2f gasPos;
	sfIntRect gasRect;

	sfVector2f lightningPos;
	sfIntRect lightningRect;

	sfVector2f lifePos;
	sfIntRect lifeRect;

	sfVector2f shieldPos;
	sfIntRect shielRect;
}Hud;
Hud hud[MAX_PLAYER];

typedef struct TextHud {
	float gasTextSize;
	sfVector2f gasTextPos;
	sfVector2f gasTextOrigin;

	float lightningTextSize;
	sfVector2f lightningTextPos;
	sfVector2f lightningTextOrigin;

	float mineTextSize;
	sfVector2f mineTextPos;
	sfVector2f mineTextOrigin;

	float lifeTextSize;
	sfVector2f lifeTextPos;
	sfVector2f lifeTextOrigin;
}TextHud;
TextHud textHud[MAX_PLAYER];

typedef struct CommonHud {
	sfVector2f multiplierZonePos;
	sfIntRect multiplierZoneRect;

	sfVector2f clickToJoinPos;
	sfIntRect clickToJoinRect;
}CommonHud;
CommonHud commonHud;

typedef struct CommonTextHud {
	float multiplierTextSize;
	sfVector2f multiplierTextPos;
	sfVector2f multiplierTextOrigin;

	float scoreTextSize;
	sfVector2f scoreTextPos;
	sfVector2f scoreTextOrigin;
}CommonTextHud;
CommonTextHud commonTextHud;

sfSprite* hudSprite;

sfTexture* hudTexture;

sfText* hudText;

sfFont* hudFont;

sfColor hudTextColor;

char hudChar[20];

void initHud(Window* _window)
{
	hudSprite = sfSprite_create();

	hudTexture = GetTexture("hud");

	sfSprite_setTexture(hudSprite, hudTexture, sfTrue);

	hudText = sfText_create();

	hudFont = GetFont("nasalization-rg");

	sfText_setFont(hudText, hudFont);

	hudTextColor = sfBlack;

	sfText_setColor(hudText, hudTextColor);


	for (int i = 0; i < MAX_PLAYER; i++)
	{
		switch (i)
		{
		case 0:
			hud[i].mainPos = vector2f(0.f, 897.f);
			hud[i].mainRect = IntRect(0, 4048, 750, 183);

			hud[i].gasPos = vector2f(8.f, 917.f);
			hud[i].gasRect = IntRect(0, 4412, 106, 152);

			hud[i].lightningPos = vector2f(155.f, 980.f);
			hud[i].lightningRect = IntRect(0, 4801, 53, 85);

			hud[i].lifePos = vector2f(521.f, 1032.f);
			hud[i].lifeRect = IntRect(0, 4958, 170, 33);

			hud[i].shieldPos = vector2f(520.f, 1031.f);
			hud[i].shielRect = IntRect(0, 4991, 173, 35);
			break;
		case 1:
			hud[i].mainPos = vector2f(1172.f, 899.f);
			hud[i].mainRect = IntRect(0, 4231, 758, 181);

			hud[i].gasPos = vector2f(1805.f, 917.f);
			hud[i].gasRect = IntRect(0, 4412, 106, 152);

			hud[i].lightningPos = vector2f(1667.f, 984.f);
			hud[i].lightningRect = IntRect(0, 4801, 53, 85);

			hud[i].lifePos = vector2f(1232.f, 1032.f);
			hud[i].lifeRect = IntRect(0, 4958, 170, 33);

			hud[i].shieldPos = vector2f(1231.f, 1031.f);
			hud[i].shielRect = IntRect(0, 4991, 173, 35);
			break;
		default:
			break;
		}
	}

	commonHud.multiplierZonePos = vector2f(908.f, 917.f); // vector2f(908.f, 917.f)
	commonHud.multiplierZoneRect = IntRect(0, 5082, 105, 92); // IntRect(0, 5082, 105, 92)

	commonHud.clickToJoinPos = vector2f(1304.f, 1003.f);
	commonHud.clickToJoinRect = IntRect(0, 5026, 574, 56);

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		switch (i)
		{
		case 0:
			textHud[i].gasTextPos = vector2f(48.f, 986.f);
			textHud[i].gasTextOrigin = vector2f(0.f, -6.f);
			textHud[i].gasTextSize = 11.f;
		default:
			break;
		}
	}

	sprintf(hudChar, "%d %%", 100);
}

void updateHud(Window* _window)
{
	
	sfText_setString(hudText, hudChar);
}

void displayHud(Window* _window)
{
	for (int i = 0; i < /*nbPlayer*/2; i++)
	{
		// main
		sfSprite_setPosition(hudSprite, hud[i].mainPos);
		sfSprite_setTextureRect(hudSprite, hud[i].mainRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// gas
		sfSprite_setPosition(hudSprite, hud[i].gasPos);
		sfSprite_setTextureRect(hudSprite, hud[i].gasRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// lightning
		sfSprite_setPosition(hudSprite, hud[i].lightningPos);
		sfSprite_setTextureRect(hudSprite, hud[i].lightningRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// life
		sfSprite_setPosition(hudSprite, hud[i].lifePos);
		sfSprite_setTextureRect(hudSprite, hud[i].lifeRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// shield
		sfSprite_setPosition(hudSprite, hud[i].shieldPos);
		sfSprite_setTextureRect(hudSprite, hud[i].shielRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
	}

	// score multiplier
	sfSprite_setPosition(hudSprite, commonHud.multiplierZonePos);
	sfSprite_setTextureRect(hudSprite, commonHud.multiplierZoneRect);
	sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

	// when needed
	if (0) {
		// click to join
		sfSprite_setPosition(hudSprite, commonHud.clickToJoinPos);
		sfSprite_setTextureRect(hudSprite, commonHud.clickToJoinRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
	}



	sfText_setColor(hudText, sfBlack);

	for (int i = 0; i < nbPlayer; i++)
	{
		sfText_setCharacterSize(hudText, textHud[i].gasTextSize);
		sfText_setPosition(hudText, textHud[i].gasTextPos);
		sfText_setOrigin(hudText, textHud[i].gasTextOrigin);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);


		sfText_setCharacterSize(hudText, textHud[i].gasTextSize);
		sfText_setPosition(hudText, textHud[i].gasTextPos);
		sfText_setOrigin(hudText, textHud[i].gasTextOrigin);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		sfText_setCharacterSize(hudText, textHud[i].gasTextSize);
		sfText_setPosition(hudText, textHud[i].gasTextPos);
		sfText_setOrigin(hudText, textHud[i].gasTextOrigin);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		sfText_setCharacterSize(hudText, textHud[i].gasTextSize);
		sfText_setPosition(hudText, textHud[i].gasTextPos);
		sfText_setOrigin(hudText, textHud[i].gasTextOrigin);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		sfText_setCharacterSize(hudText, textHud[i].gasTextSize);
		sfText_setPosition(hudText, textHud[i].gasTextPos);
		sfText_setOrigin(hudText, textHud[i].gasTextOrigin);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
	}

	sfText_setCharacterSize(hudText, commonTextHud.multiplierTextSize);
	sfText_setPosition(hudText, commonTextHud.multiplierTextPos);
	sfText_setOrigin(hudText, commonTextHud.multiplierTextOrigin);
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	sfText_setCharacterSize(hudText, commonTextHud.scoreTextSize);
	sfText_setPosition(hudText, commonTextHud.scoreTextPos);
	sfText_setOrigin(hudText, commonTextHud.scoreTextOrigin);
	sfText_setColor(hudText, sfWhite);
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

}