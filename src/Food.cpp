#include "Food.hpp"
#include "Global.hpp"
#include "raylib.h"
#include <deque>

using namespace std;

Food::Food(const deque<Vector2>& snakeBody, int cellCount) : cellCount(cellCount)
{
    Image image = LoadImage("Assets/Graphics/Sprites/Food_Cherry.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = GenerateRandomPos(snakeBody);
}

Food::~Food()
{
    UnloadTexture(texture);
}

void Food::Draw(int cellSize, int borderSize) const
{
    DrawTexture(texture, borderSize + position.x * cellSize, borderSize + position.y * cellSize, WHITE);
}

Vector2 Food::GenerateRandomCell() const
{
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{x, y};
}

Vector2 Food::GenerateRandomPos(const deque<Vector2>& snakeBody)
{
    Vector2 newPosition = GenerateRandomCell();
    
    while (Global::ElementInDeque(newPosition, snakeBody))
    {
        newPosition = GenerateRandomCell();
    }
    
    return newPosition;
}