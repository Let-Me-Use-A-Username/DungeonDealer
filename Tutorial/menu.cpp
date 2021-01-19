#include "menu.h"

bool Menu::loadGame()
{
	return pressedLoadGame;
}

bool Menu::exitGame()
{

	return pressedExitGame;
}

std::string Menu::mapLoaded()
{
	return std::string(GHOSTTOWN_LAYOUT) + "ghosttown";
}

void Menu::update() 
{
	if (graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		pressedLoadGame = true;
	}

	if (graphics::getKeyState(graphics::SCANCODE_ESCAPE))
	{
		pressedExitGame = true;
	}
}

void Menu::draw()
{

	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_HEIGHT , CANVAS_WIDTH, br);

	graphics::setFont(ASSET_PATH_FONTS);

	text.fill_color[0] = 0.0f;
	text.fill_color[1] = 0.0f;
	text.fill_color[2] = 0.0f;

	graphics::drawText(CANVAS_WIDTH / 2 - 75, CANVAS_HEIGHT / 2 - 100 , 30, "Load Game", text);
	graphics::drawText(CANVAS_WIDTH / 2 - 75, CANVAS_HEIGHT / 2 - 90, 10, "{Press Enter to Continue}", text);
	graphics::drawText(CANVAS_WIDTH / 2 - 75, CANVAS_HEIGHT / 2 + 100, 30, "Exit Game", text);
	graphics::drawText(CANVAS_WIDTH / 2 - 75, CANVAS_HEIGHT / 2 + 110, 10, "{Press Esc to Exit}", text);
}
