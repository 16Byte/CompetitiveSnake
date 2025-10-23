#pragma once
#include "raylib.h"
#include <deque>

class Food
{
    public:
        Food(const std::deque<Vector2>& snakeBody, int cellCount);
        ~Food();
        
        void Draw(int cellSize, int borderSize) const;
        Vector2 GenerateRandomPos(const std::deque<Vector2>& snakeBody);
        
        Vector2 position;
    
    private:
        Vector2 GenerateRandomCell() const;
        
        Texture2D texture;
        int cellCount;
};