#pragma once
#include "raylib.h"
#include <deque>

class Snake
{
    public:
        Snake();
        Snake(Vector2 startPos, Vector2 startDirection);
        
        void Draw(int cellSize, int borderSize, Color snakeColor) const;
        void Update();
        void Reset(Music& music);
        void ResetWithPosition(Music& music, Vector2 startPos, Vector2 startDirection);
        Vector2 GetAIDirection(Vector2 foodPos, const Snake& opponent) const;
        
        std::deque<Vector2> body;
        Vector2 direction;
        bool addSegment;
        
    private:
        Vector2 initialPosition;
        Vector2 initialDirection;
};