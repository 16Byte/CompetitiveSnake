#pragma once
#include "raylib.h"
#include <deque>

class Snake
{
    public:
        Snake();
        
        void Draw(int cellSize, int borderSize, Color snakeColor) const;
        void Update();
        void Reset(Music& music);
        
        std::deque<Vector2> body;
        Vector2 direction;
        bool addSegment;
};