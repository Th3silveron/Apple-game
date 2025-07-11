#pragma once
#include <string>

namespace ApplesGame
{
    const int SCREEN_WIDTH = 1940;
    const int SCREEN_HEIGHT = 1280;
    const float INITIAL_SPEED = 150.f;
    const float PLAYER_SIZE = 30.f;
    const float ACCELERATION = 50.f;
    const float APPLE_SIZE = 30.f;
    const int NUM_ROCK = 10;
    const float ROCK_SIZE = 30.f;
    const float PAUSE_LENGTH = 3.f;
    const std::string RESOURCES_PATH = "C:/Source/HW/ApplesGame/Resources/";
    const std::string GAME_FONT_PATH = "C:/Source/HW/ApplesGame/Resources/Fonts/Roboto-Bold.ttf";

    // Битки для модов
    const int MODE_FINITE_APPLES = 1 << 0; // Mode 1
    const int MODE_INFINITE_NO_SPEED = 1 << 1; // Mode 2
    const int MODE_INFINITE_WITH_SPEED = 1 << 2; // Mode 3

    const int MAX_APPLES_LIMIT = 100;
    const int WINNING_APPLES = 50;
}
