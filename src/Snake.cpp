#include "Snake.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace std;

Snake::Snake()
    : body{{Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}}},
      direction{1, 0},
      addSegment{false}
{
}

void Snake::Draw(int cellSize, int borderSize, Color snakeColor) const
{
    for (unsigned int i = 0; i < body.size(); i++)
    {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{
            (borderSize + x * cellSize), 
            (borderSize + y * cellSize), 
            (float)cellSize, 
            (float)cellSize
        };
        DrawRectangleRounded(segment, 0.5, 6, snakeColor);
    }
}

void Snake::Update()
{
    body.push_front(Vector2Add(body[0], direction));
    
    if (addSegment)
    {
        addSegment = false;
    }
    else
    {
        body.pop_back();
    }
}

void Snake::Reset(Music& music)
{
    SeekMusicStream(music, 0.0f);
    
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
}
