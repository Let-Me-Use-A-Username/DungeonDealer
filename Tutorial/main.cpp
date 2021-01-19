#include "graphics.h"
#include "game.h"
#include "config.h"
#include "GameMap.h"

Game * mygame;
GameMap* nighttown;

// The custom callback function that the library calls 
// to check for and set the current application state.
void update(float ms)
{
    mygame->update();
}

// The window content drawing function.
void draw()
{
    mygame->draw();
}

int main()
{

    mygame = new Game();

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");

    graphics::setUserData(mygame);

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();

    mygame->~Game();

    return 0;
}
