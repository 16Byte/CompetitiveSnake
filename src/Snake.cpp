#include "Snake.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace std;

Snake::Snake()
    : body{{Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}}},
      direction{1, 0},
      addSegment{false},
      initialPosition{6, 9},
      initialDirection{1, 0}
{
}

Snake::Snake(Vector2 startPos, Vector2 startDirection)
    : body{{startPos, Vector2{startPos.x - startDirection.x, startPos.y - startDirection.y}, 
            Vector2{startPos.x - 2 * startDirection.x, startPos.y - 2 * startDirection.y}}},
      direction{startDirection},
      addSegment{false},
      initialPosition{startPos},
      initialDirection{startDirection}
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
    
    body = {Vector2{initialPosition.x, initialPosition.y}, 
            Vector2{initialPosition.x - initialDirection.x, initialPosition.y - initialDirection.y}, 
            Vector2{initialPosition.x - 2 * initialDirection.x, initialPosition.y - 2 * initialDirection.y}};
    direction = initialDirection;
}

void Snake::ResetWithPosition(Music& music, Vector2 startPos, Vector2 startDirection)
{
    SeekMusicStream(music, 0.0f);
    
    initialPosition = startPos;
    initialDirection = startDirection;
    
    body = {Vector2{startPos.x, startPos.y}, 
            Vector2{startPos.x - startDirection.x, startPos.y - startDirection.y}, 
            Vector2{startPos.x - 2 * startDirection.x, startPos.y - 2 * startDirection.y}};
    direction = startDirection;
}

Vector2 Snake::GetAIDirection(Vector2 foodPos, const Snake& opponent) const
{
    Vector2 head = body[0];
    
    // Calculate Manhattan distance for each direction
    auto getManhattanDistance = [](Vector2 from, Vector2 to) -> float {
        return abs(from.x - to.x) + abs(from.y - to.y);
    };
    
    // Check if position is valid (not hitting walls or snakes)
    auto isValidPosition = [&](Vector2 pos) -> bool {
        // Check walls (assuming 25x25 grid)
        if (pos.x < 0 || pos.x >= 25 || pos.y < 0 || pos.y >= 25)
            return false;
        
        // Check collision with self
        for (const auto& segment : body)
            if (Vector2Equals(segment, pos))
                return false;
        
        // Check collision with opponent
        for (const auto& segment : opponent.body)
            if (Vector2Equals(segment, pos))
                return false;
        
        return true;
    };
    
    // Possible directions
    Vector2 directions[] = {
        {0, -1},  // Up
        {0, 1},   // Down
        {-1, 0},  // Left
        {1, 0}    // Right
    };
    
    Vector2 bestDirection = {0, 0};
    float bestDistance = 1000000.0f;
    
    for (const auto& dir : directions)
    {
        // Don't reverse direction
        if (dir.x == -direction.x && dir.y == -direction.y)
            continue;
        
        Vector2 newPos = Vector2Add(head, dir);
        
        if (isValidPosition(newPos))
        {
            float distance = getManhattanDistance(newPos, foodPos);
            if (distance < bestDistance)
            {
                bestDistance = distance;
                bestDirection = dir;
            }
        }
    }
    
    return bestDirection;
}
