#include "hud.h"
#include "textureManager.h"
#include "fontManager.h"
#include "player.h"
#include "item.h"

#define TEXTORIGIN vector2f(tmpRect.width / 2.f, tmpRect.height) // center the size
//#define TEXTORIGIN vector2f(tmpRect.width / 2.f, 0.f) // center the string

typedef struct Hud {
	sfVector2f mainPos;
	sfIntRect mainRect;

	sfVector2f gasPos;
	sfIntRect gasRect;
	sfVector2f fullGasPos;
	sfIntRect fullGasRect;

	sfVector2f lightningPos;
	sfIntRect lightningRect;
	sfVector2f fullLightningPos;
	sfIntRect fullLightningRect;

	sfVector2f lifePos;
	sfIntRect lifeRect;

	sfVector2f shieldPos;
	sfIntRect shielRect;
}Hud;
Hud hud[MAX_PLAYER];

typedef struct TextHud {
	float gasTextSize;
	sfVector2f gasTextPos;

	float lightningTextSize;
	sfVector2f lightningTextPos;

	float mineTextSize;
	sfVector2f mineTextPos;

	float respawnTextSize;
	sfVector2f respawnTextPos;
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
	float multiplierTextAngle;

	float scoreTextSize;
	sfVector2f scoreTextPos;

	float countdownTextSize;
	sfVector2f countdownTextPos;
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

			hud[i].fullGasPos = vector2f(8.f, 1069.f);
			hud[i].fullGasRect = IntRect(0, 4564, 106, 0);

			//hud[i].lightningPos = vector2f(155.f, 980.f);
			//hud[i].lightningRect = IntRect(0, 4801, 53, 85);

			hud[i].lightningPos = vector2f(160.f, 981.f);
			hud[i].lightningRect = IntRect(0, 4716, 53, 85);

			hud[i].fullLightningPos = vector2f(158.f, 1070.f);
			hud[i].fullLightningRect = IntRect(0, 4801, 58, 0);

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

			hud[i].fullGasPos = vector2f(1805.f, 1069.f);
			hud[i].fullGasRect = IntRect(0, 4564, 106, 0);

			//hud[i].lightningPos = vector2f(1667.f, 984.f);
			//hud[i].lightningRect = IntRect(0, 4716, 53, 85);

			hud[i].lightningPos = vector2f(1672.f, 985.f);
			hud[i].lightningRect = IntRect(0, 4716, 53, 85);

			hud[i].fullLightningPos = vector2f(1670.f, 1074.f);
			hud[i].fullLightningRect = IntRect(0, 4801, 58, 0);

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
			textHud[i].gasTextPos = vector2f(64.f, 1000.f);
			textHud[i].gasTextSize = 11.f;

			textHud[i].lightningTextPos = vector2f(228.f, 1025.f);
			textHud[i].lightningTextSize = 24.f;

			textHud[i].mineTextPos = vector2f(388.f, 1032.f);
			textHud[i].mineTextSize = 24.f;

			textHud[i].respawnTextPos = vector2f(624.f, 1009.f);
			textHud[i].respawnTextSize = 18.f;
			break;
		case 1:
			// TODO
			textHud[i].gasTextPos = vector2f(1861.f, 1000.f);
			textHud[i].gasTextSize = 11.f;

			textHud[i].lightningTextPos = vector2f(1742.f, 1030.f);
			textHud[i].lightningTextSize = 24.f;

			textHud[i].mineTextPos = vector2f(1532.f, 1037.f);
			textHud[i].mineTextSize = 24.f;

			textHud[i].respawnTextPos = vector2f(1350.f, 1010.f);
			textHud[i].respawnTextSize = 18.f;
			break;
		default:
			break;
		}
	}

	commonTextHud.multiplierTextPos = vector2f(960.f, 962.f);
	commonTextHud.multiplierTextSize = 60.f;

	commonTextHud.scoreTextPos = vector2f(960.f, 1040.f);
	commonTextHud.scoreTextSize = 36.f;

	// TODO respawn

}

void updateHud(Window* _window)
{
	float udt = getUnscaledDeltaTime();

	for (int i = 0; i < nbPlayer; i++)
	{
		// dynamic life
		if (player[i].life <= 0) {
			hud[i].lifeRect.width = 0;
		}
		else if (player[i].life == 1) {
			hud[i].lifeRect.top = 4892;
			hud[i].lifeRect.width = 55;
		}
		else if (player[i].life == 2) {
			hud[i].lifeRect.top = 4925;
			hud[i].lifeRect.width = 113;
		}
		else if (player[i].life == 3) {
			hud[i].lifeRect.top = 4958;
			hud[i].lifeRect.width = 170;
		}

		// dynamic gas
		hud[i].fullGasPos.y = 1069.f - ((float)player[i].nbGas / 100.f * 152.f);
		hud[i].fullGasRect.height = player[i].nbGas * 152 / 100;
		hud[i].fullGasRect.top = 4716 - (player[i].nbGas * 152 / 100);

		//// dynamic lightning
		//hud[i].fullLightningPos.y = 1066.f - ((float)player[i].nbLightning / 100.f * 91.f);
		//hud[i].fullLightningRect.height = player[i].nbLightning * 91 / 100;
		//hud[i].fullLightningRect.top = 4892 - (player[i].nbLightning * 91 / 100);
		// 
		// dynamic lightning
		if (i)
			hud[i].fullLightningPos.y = 1074.f - ((LIGTHNING_SECONDS_REQUIRED - (float)player[i].nbLightning) / LIGTHNING_SECONDS_REQUIRED * 91.f);
		else
			hud[i].fullLightningPos.y = 1070.f - ((LIGTHNING_SECONDS_REQUIRED - (float)player[i].nbLightning) / LIGTHNING_SECONDS_REQUIRED * 91.f);
		
		hud[i].fullLightningRect.height = ((int)LIGTHNING_SECONDS_REQUIRED - player[i].nbLightning) * 91 / (int)LIGTHNING_SECONDS_REQUIRED;
		hud[i].fullLightningRect.top = 4892 - (((int)LIGTHNING_SECONDS_REQUIRED - player[i].nbLightning) * 91 / (int)LIGTHNING_SECONDS_REQUIRED);

	}

	if (common.multiplierTimer > 0.f) {
		common.multiplierTimer -= udt;

		if (common.multiplierTimer > MULTIPLIER_ITEM_DURATION - 1.f) {
			commonTextHud.multiplierTextSize = 60.f * (MULTIPLIER_ITEM_DURATION - common.multiplierTimer);
			commonTextHud.multiplierTextAngle = 90.f * (MULTIPLIER_ITEM_DURATION - common.multiplierTimer) - 90.f;
			//commonTextHud.multiplierTextAngle = (common.multiplierTimer - MULTIPLIER_ITEM_DURATION - 1.f) * 90.f + 180.f;
		}
		else {
			commonTextHud.multiplierTextSize = 60.f;
			commonTextHud.multiplierTextAngle = 0.f;
		}
		
	}
	else {
		common.multiplier = 1;
	}

}

void displayHud(Window* _window)
{
	for (int i = 0; i < nbPlayer; i++)
	{
		// main
		sfSprite_setPosition(hudSprite, hud[i].mainPos);
		sfSprite_setTextureRect(hudSprite, hud[i].mainRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// gas
		sfSprite_setPosition(hudSprite, hud[i].gasPos);
		sfSprite_setTextureRect(hudSprite, hud[i].gasRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

		sfSprite_setPosition(hudSprite, hud[i].fullGasPos);
		sfSprite_setTextureRect(hudSprite, hud[i].fullGasRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// lightning
		sfSprite_setPosition(hudSprite, hud[i].lightningPos);
		sfSprite_setTextureRect(hudSprite, hud[i].lightningRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

		sfSprite_setPosition(hudSprite, hud[i].fullLightningPos);
		sfSprite_setTextureRect(hudSprite, hud[i].fullLightningRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);

		// life
		sfSprite_setPosition(hudSprite, hud[i].lifePos);
		sfSprite_setTextureRect(hudSprite, hud[i].lifeRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		
		// shield
		if (player[i].hasShield) {
			sfSprite_setPosition(hudSprite, hud[i].shieldPos);
			sfSprite_setTextureRect(hudSprite, hud[i].shielRect);
			sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
		}
	}

	// multiplier
	if (common.multiplier > 1) {
		sfSprite_setPosition(hudSprite, commonHud.multiplierZonePos);
		sfSprite_setTextureRect(hudSprite, commonHud.multiplierZoneRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
	}
	
	// when needed
	if (0) {
		// click to join
		sfSprite_setPosition(hudSprite, commonHud.clickToJoinPos);
		sfSprite_setTextureRect(hudSprite, commonHud.clickToJoinRect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
	}



	sfFloatRect tmpRect;

	sfText_setColor(hudText, sfBlack);

	for (int i = 0; i < nbPlayer; i++)
	{
		// gas text
		sprintf(hudChar, "%d %%", player[i].nbGas);
		sfText_setString(hudText, hudChar);
		sfText_setCharacterSize(hudText, textHud[i].gasTextSize);
		sfText_setPosition(hudText, textHud[i].gasTextPos);
		tmpRect = sfText_getLocalBounds(hudText);
		sfText_setOrigin(hudText, TEXTORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		// lightning text
		sprintf(hudChar, "%ds", player[i].nbLightning);
		sfText_setString(hudText, hudChar);
		sfText_setCharacterSize(hudText, textHud[i].lightningTextSize);
		sfText_setPosition(hudText, textHud[i].lightningTextPos);
		tmpRect = sfText_getLocalBounds(hudText);
		sfText_setOrigin(hudText, TEXTORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		// mine text
		sprintf(hudChar, "%d / %d", player[i].nbMine, KILL_COUNT_REQUIRED);
		sfText_setString(hudText, hudChar);
		sfText_setCharacterSize(hudText, textHud[i].mineTextSize);
		sfText_setPosition(hudText, textHud[i].mineTextPos);
		tmpRect = sfText_getLocalBounds(hudText);
		sfText_setOrigin(hudText, TEXTORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		// respawn text
		sprintf(hudChar, "x%d", player[i].nbRespawn);
		sfText_setString(hudText, hudChar);
		sfText_setCharacterSize(hudText, textHud[i].respawnTextSize);
		sfText_setPosition(hudText, textHud[i].respawnTextPos);
		tmpRect = sfText_getLocalBounds(hudText);
		sfText_setOrigin(hudText, TEXTORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

	}

	// multiplier text
	if (common.multiplier > 1) {
		sprintf(hudChar, "x%d", common.multiplier);
		sfText_setString(hudText, hudChar);
		sfText_setCharacterSize(hudText, commonTextHud.multiplierTextSize);
		sfText_setPosition(hudText, commonTextHud.multiplierTextPos);
		sfText_setRotation(hudText, commonTextHud.multiplierTextAngle);
		tmpRect = sfText_getLocalBounds(hudText);
		sfText_setOrigin(hudText, TEXTORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
	}

	sfText_setRotation(hudText, 0.f);


	// score text
	sprintf(hudChar, "%d", common.score);
	sfText_setString(hudText, hudChar);
	sfText_setCharacterSize(hudText, commonTextHud.scoreTextSize);
	sfText_setPosition(hudText, commonTextHud.scoreTextPos);
	tmpRect = sfText_getLocalBounds(hudText);
	sfText_setOrigin(hudText, TEXTORIGIN);
	sfText_setColor(hudText, sfWhite);
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
	
	// countdown to join
	sprintf(hudChar, "%d", common.countdown);
	sfText_setString(hudText, hudChar);
	sfText_setCharacterSize(hudText, commonTextHud.countdownTextSize);
	sfText_setPosition(hudText, commonTextHud.countdownTextPos);
	tmpRect = sfText_getLocalBounds(hudText);
	sfText_setOrigin(hudText, TEXTORIGIN);
	sfText_setColor(hudText, sfWhite);
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
}

void deinitHud()
{
	sfSprite_destroy(hudSprite);
	sfText_destroy(hudText);
}