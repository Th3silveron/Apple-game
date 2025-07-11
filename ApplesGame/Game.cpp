/*●▬▬▬▬▬▬▬▬▬▬▬▬▬ஜ۩۞۩ஜ▬▬▬▬▬▬▬▬▬▬▬▬▬●
  ░░░░░░░░░░░ЗДРАТУТИ ░░░░░░░░░░░░░░
  ●▬▬▬▬▬▬▬▬▬▬▬▬▬ஜ۩۞۩ஜ▬▬▬▬▬▬▬▬▬▬▬▬▬●
*/

#include "Game.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include <cassert>
#include <cstdlib>

namespace ApplesGame
{
    void GenerateLeaderboard(Game& game)
    {
        static const std::vector<std::string> fakeNames = {
            "Bot_Max", "NeoHunter", "RedWolf", "CyberFox", "Ghost777"
        };

        game.leaderboard.clear();

        for (int i = 0; i < 5; ++i)
        {
            ScoreEntry entry;
            entry.name = fakeNames[i];
            entry.score = rand() % 100 + 1;
            game.leaderboard.push_back(entry);
        }

        // Обозначение игрока
        ScoreEntry playerEntry{ "Player", game.numEatenApples };
        game.leaderboard.push_back(playerEntry);

        // СортировОчка (пузырьком)
        for (size_t i = 0; i < game.leaderboard.size(); ++i)
        {
            for (size_t j = i + 1; j < game.leaderboard.size(); ++j)
            {
                if (game.leaderboard[j].score > game.leaderboard[i].score)
                    std::swap(game.leaderboard[j], game.leaderboard[i]);
            }
        }

        for (size_t i = 0; i < game.leaderboard.size(); ++i)
        {
            std::string text = game.leaderboard[i].name + " - " + std::to_string(game.leaderboard[i].score);
            game.leaderboardTexts[i].setFont(game.gameFont);
            game.leaderboardTexts[i].setCharacterSize(30);
            game.leaderboardTexts[i].setFillColor(sf::Color::White);
            game.leaderboardTexts[i].setString(text);
            game.leaderboardTexts[i].setPosition(100.f, 600.f + i * 40.f);
        }

        game.leaderboardGenerated = true;
    }

    void RestartGame(Game& game)
    {
        InitPlayer(game.player, game);

        if (game.apples)
            delete[] game.apples;

        game.apples = new Apple[game.appleCount];
        for (int i = 0; i < game.appleCount; ++i)
            InitApple(game.apples[i], game.appleTexture);

        for (int i = 0; i < NUM_ROCK; ++i)
            InitRock(game.rocks[i], game.rockTexture);

        game.numEatenApples = 0;
        game.isGameFinished = false;
        game.timeSinceGameFinish = 0.f;
        game.isRestartReady = false;
        game.backgroundShape.setFillColor(sf::Color::Black);

        game.isChoosingMode = true;
        game.leaderboardGenerated = false;
    }

    void InitGame(Game& game)
    {
        assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "Player.png"));
        assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));

        assert(game.appleEatSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"));
        assert(game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"));

        game.appleEatSound.setBuffer(game.appleEatSoundBuffer);
        game.deathSound.setBuffer(game.deathSoundBuffer);

        assert(game.gameFont.loadFromFile(GAME_FONT_PATH));

        game.gameOverText.setFont(game.gameFont);
        game.gameOverText.setString("GAME OVER");
        game.gameOverText.setCharacterSize(60);
        game.gameOverText.setFillColor(sf::Color::Yellow);
        game.gameOverText.setOutlineColor(sf::Color::Black);
        game.gameOverText.setOutlineThickness(3.f);
        sf::FloatRect textRect = game.gameOverText.getLocalBounds();
        game.gameOverText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
        game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 40.f);

        game.restartText.setFont(game.gameFont);
        game.restartText.setString("PRESS ENTER TO RESTART");
        game.restartText.setCharacterSize(40);
        game.restartText.setFillColor(sf::Color::White);
        game.restartText.setOutlineColor(sf::Color::Black);
        game.restartText.setOutlineThickness(2.f);
        sf::FloatRect restartRect = game.restartText.getLocalBounds();
        game.restartText.setOrigin(restartRect.left + restartRect.width / 2.f, restartRect.top + restartRect.height / 2.f);
        game.restartText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 40.f);

        game.backgroundShape.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        game.backgroundShape.setFillColor(sf::Color::Black);
        game.backgroundShape.setPosition(0.f, 0.f);

        const char* labels[3] = { "Mode 1: 50 Apples + Speed", "Mode 2: Endless No Speed", "Mode 3: Endless + Speed" };
        for (int i = 0; i < 3; ++i)
        {
            game.modeTexts[i].setFont(game.gameFont);
            game.modeTexts[i].setString(labels[i]);
            game.modeTexts[i].setCharacterSize(50);
            game.modeTexts[i].setFillColor(sf::Color::White);
            game.modeTexts[i].setPosition(SCREEN_WIDTH / 2.f - 250.f, 300.f + i * 100.f);
        }
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        if (game.isChoosingMode)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                game.selectedModeIndex = (game.selectedModeIndex + 2) % 3;
                sf::sleep(sf::milliseconds(150));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                game.selectedModeIndex = (game.selectedModeIndex + 1) % 3;
                sf::sleep(sf::milliseconds(150));
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                game.wasEnterReleased = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && game.wasEnterReleased)
            {
                switch (game.selectedModeIndex)
                {
                case 0:
                    game.gameMode = MODE_FINITE_APPLES;
                    game.appleCount = 10;
                    break;
                case 1:
                    game.gameMode = MODE_INFINITE_NO_SPEED;
                    game.appleCount = 20;
                    break;
                case 2:
                    game.gameMode = MODE_INFINITE_WITH_SPEED;
                    game.appleCount = 20;
                    break;
                }

                RestartGame(game);
                game.isChoosingMode = false;
                game.wasEnterReleased = false;
            }
            return;
        }

        if (!game.isGameFinished)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) game.player.direction = PlayerDirection::Right;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) game.player.direction = PlayerDirection::Left;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) game.player.direction = PlayerDirection::Up;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) game.player.direction = PlayerDirection::Down;

            switch (game.player.direction)
            {
            case PlayerDirection::Right: game.player.position.x += game.player.speed * deltaTime; break;
            case PlayerDirection::Left:  game.player.position.x -= game.player.speed * deltaTime; break;
            case PlayerDirection::Up:    game.player.position.y -= game.player.speed * deltaTime; break;
            case PlayerDirection::Down:  game.player.position.y += game.player.speed * deltaTime; break;
            }

            UpdatePlayerDirection(game.player);

            for (int i = 0; i < game.appleCount; ++i)
            {
                if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f, game.apples[i].position, APPLE_SIZE / 2.f))
                {
                    game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
                    game.apples[i].sprite.setPosition(game.apples[i].position.x, game.apples[i].position.y);
                    ++game.numEatenApples;

                    if (game.gameMode & MODE_FINITE_APPLES || game.gameMode & MODE_INFINITE_WITH_SPEED)
                        game.player.speed += ACCELERATION;

                    game.appleEatSound.play();

                    if ((game.gameMode & MODE_FINITE_APPLES) && game.numEatenApples >= WINNING_APPLES)
                    {
                        game.isGameFinished = true;
                        game.timeSinceGameFinish = 0.f;
                        break;
                    }
                }
            }

            for (int i = 0; i < NUM_ROCK; ++i)
            {
                if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE + 5.f, PLAYER_SIZE + 5.f },
                    game.rocks[i].position, { ROCK_SIZE + 5.f, ROCK_SIZE + 5.f }))
                {
                    game.isGameFinished = true;
                    game.timeSinceGameFinish = 0.f;
                    game.deathSound.play();
                }
            }

            if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
                game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
                game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
                game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
            {
                game.isGameFinished = true;
                game.timeSinceGameFinish = 0.f;
                game.deathSound.play();
            }
        }
        else
        {
            if (!game.isRestartReady)
            {
                game.timeSinceGameFinish += deltaTime;
                if (game.timeSinceGameFinish >= PAUSE_LENGTH)
                    game.isRestartReady = true;
            }
            else
            {
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    game.wasEnterReleased = true;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && game.wasEnterReleased)
                {
                    game.isChoosingMode = true;
                    game.wasEnterReleased = false;
                }
            }

            game.backgroundShape.setFillColor(sf::Color::Red);

            if (!game.leaderboardGenerated)
                GenerateLeaderboard(game);
        }
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.backgroundShape);

        if (game.isChoosingMode)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (i == game.selectedModeIndex)
                {
                    game.modeTexts[i].setFillColor(sf::Color::Yellow);
                    game.modeTexts[i].setOutlineColor(sf::Color::Black);
                    game.modeTexts[i].setOutlineThickness(3.f);
                }
                else
                {
                    game.modeTexts[i].setFillColor(sf::Color::White);
                    game.modeTexts[i].setOutlineThickness(0.f);
                }

                window.draw(game.modeTexts[i]);
            }
            return;
        }

        DrawPlayer(game.player, window);

        for (int i = 0; i < game.appleCount; ++i)
            window.draw(game.apples[i].sprite);

        for (int i = 0; i < NUM_ROCK; ++i)
            window.draw(game.rocks[i].sprite);

        if (game.isGameFinished)
        {
            window.draw(game.gameOverText);
            if (game.isRestartReady)
                window.draw(game.restartText);

            for (size_t i = 0; i < game.leaderboard.size(); ++i)
                window.draw(game.leaderboardTexts[i]);
        }
    }

    void FreeGame(Game& game)
    {
        if (game.apples)
        {
            delete[] game.apples;
            game.apples = nullptr;
            game.appleCount = 0;
        }
    }
}

/*
__________$$____________________$$_______
__$$_____$__$__________________$__$_____$$
_$__$____$__$__________________$__$____$__$
__$__$___$__$__________________$__$___$__$
___$__$__$__$__________________$__$__$__$
____$__$_$__$__________________$__$_$__$
____$$__$$__$ _________________ $__$$__$$
__$$__$_$$$$$______Ты______ _$$$$$_$__$$
_$__$__$_____$____супер_____$_____$__$__$
$_$__$__$$$___$___клeвый___$__$$$__$__$_$
_$_$__$__$_$__$__человечек_$__$_$__$__$_$
_$_$__$$_$___$______________$___$_$$__$_$
__$_$$______$________________$______$$_$
___$_______$__________________$_______$
____$______$___________________$______$
___$$$$$$$$$$_________________$$$$$$$$$$
__$$$$$$$$$$$$$______________$$$$$$$$$$$$
*/