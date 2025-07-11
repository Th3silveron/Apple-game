#include "Player.h"
#include "Game.h"

namespace ApplesGame
{
    void InitPlayer(Player& player, const Game& game)
    {
        player.position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
        player.speed = INITIAL_SPEED;
        player.direction = PlayerDirection::Right;

        player.sprite.setTexture(game.playerTexture);
        SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
        SetSpriteRelativeOrigin(player.sprite, 0.5f, 0.5f);
    }

    void DrawPlayer(Player& player, sf::RenderWindow& window)
    {
        player.sprite.setPosition(player.position.x, player.position.y);
        window.draw(player.sprite);
    }

    void UpdatePlayerDirection(Player& player)
    {
        float angle = 0.f;
        switch (player.direction)
        {
        case PlayerDirection::Right:
            angle = 0.f;
            break;
        case PlayerDirection::Up:
            angle = -90.f;
            break;
        case PlayerDirection::Left:
            angle = 180.f;
            break;
        case PlayerDirection::Down:
            angle = 90.f;
            break;
        }
        player.sprite.setRotation(angle);
    }
}


