#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
    struct ScoreEntry
    {
        std::string name;
        int score;
    };

    struct Game
    {
        Player player;
        Apple* apples = nullptr;
        int appleCount = 0;

        Rock rocks[NUM_ROCK];

        int numEatenApples = 0;
        bool isGameFinished = false;
        float timeSinceGameFinish = 0.f;
        bool isRestartReady = false;

        sf::RectangleShape backgroundShape;

        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;

        sf::SoundBuffer appleEatSoundBuffer;
        sf::SoundBuffer deathSoundBuffer;
        sf::Sound appleEatSound;
        sf::Sound deathSound;

        sf::Font gameFont;
        sf::Text gameOverText;
        sf::Text restartText;

        int gameMode = 0;

        int selectedModeIndex = 0;
        sf::Text modeTexts[3];
        bool isChoosingMode = true;
        bool wasEnterReleased = true;

        std::vector<ScoreEntry> leaderboard;
        sf::Text leaderboardTexts[6]; // 5 ноунеймов + игрок
        bool leaderboardGenerated = false;
    };

    void RestartGame(Game& game);
    void InitGame(Game& game);
    void UpdateGame(Game& game, float deltaTime);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void FreeGame(Game& game);
}


/*▒▒▒▒▒█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█
▒▒▒▒▒█░▒▒▒▒▒▒▒▓▒▒▓▒▒▒▒▒▒▒░█
▒▒▒▒▒█░▒▒▓▒▒▒▒▒▒▒▒▒▄▄▒▓▒▒░█░▄▄
▄▀▀▄▄█░▒▒▒▒▒▒▓▒▒▒▒█░░▀▄▄▄▄▄▀░░█
█░░░░█░▒▒▒▒▒▒▒▒▒▒▒█░░░░░░░░░░░█
▒▀▀▄▄█░▒▒▒▒▓▒▒▒▓▒█░░░█▒░░░░█▒░░█
▒▒▒▒▒█░▒▓▒▒▒▒▓▒▒▒█░░░░░░░▀░░░░░█
▒▒▒▄▄█░▒▒▒▓▒▒▒▒▒▒▒█░░█▄▄█▄▄█░░█
▒▒▒█░░░█▄▄▄▄▄▄▄▄▄▄█░█▄▄▄▄▄▄▄▄▄█
▒▒▒█▄▄█░░█▄▄█░░░░░░█▄▄█░░█▄▄█*/

