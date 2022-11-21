#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
    _In_ LPSTR szCmdLine, _In_ int iCmdShow)
{
    Game *gameData = new Game();
    gameData->Run(iCmdShow);
    delete gameData;
    return 0;
}