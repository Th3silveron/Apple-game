#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace ApplesGame
{
    enum class PlayerDirection
    {
        Right = 0,
        Up = 1,
        Left = 2,
        Down = 3
    };

    struct Player
    {
        Position2D position;
        float speed = INITIAL_SPEED;
        PlayerDirection direction = PlayerDirection::Right;
        sf::Sprite sprite;
    };

    struct Game;

    void InitPlayer(Player& player, const Game& game);
    void DrawPlayer(Player& player, sf::RenderWindow& window);
    void UpdatePlayerDirection(Player& player);
}
