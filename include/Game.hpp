#pragma once
#include "Food.hpp"
#include "Snake.hpp"
#include "raylib.h"

class Game
{
    public:
        Game();
        ~Game();
        
        void Draw() const;
        void Update();
        void GameOver();
        
        // Game constants
        static const int cellSize = 30;
        static const int cellCount = 25;
        static const int borderSize = 75;
        
        // Game objects
        Snake player1;
        Snake player2;
        Food food;
        
        // Game state
        int score;
        int score2;
        bool running;
        
    private:
        void CheckCollisionWithFood();
        void CheckCollisionWithEdges();
        void CheckCollisionWithTail();
        
        // Audio
        Sound consumptionSound;
        Sound deathSound;
};