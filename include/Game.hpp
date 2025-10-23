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
        Snake snake;
        Food food;
        
        // Game state
        int score;
        bool running;
        
    private:
        void CheckCollisionWithFood();
        void CheckCollisionWithEdges();
        void CheckCollisionWithTail();
        
        // Audio
        Sound consumptionSound;
        Sound deathSound;
};