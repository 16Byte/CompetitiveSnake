#include "Game.hpp"
#include "Global.hpp"
#include "raylib.h"
#include "raymath.h"
#include <deque>

using namespace std;

Game::Game() 
    : snake(),
      food(snake.body, cellCount),
      score(0),
      running(true)
{
    InitAudioDevice();
    
    Global::easyAndNormalModeMusic = LoadMusicStream("Assets/Sounds/Music/Breaking News by SAKUMAMATATA.mp3");
    PlayMusicStream(Global::easyAndNormalModeMusic);
    SetMusicVolume(Global::easyAndNormalModeMusic, 0.25f);
    
    consumptionSound = LoadSound("Assets/Sounds/SFX/Consumption 1.wav");
    deathSound = LoadSound("Assets/Sounds/SFX/Death (from Galaga).wav");
}

Game::~Game()
{
    UnloadSound(consumptionSound);
    UnloadSound(deathSound);
    CloseAudioDevice();
}

void Game::Draw() const
{
    food.Draw(cellSize, borderSize);
    snake.Draw(cellSize, borderSize, Global::snakeColor);
}

void Game::Update()
{
    if (running)
    {
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
    }
}

void Game::CheckCollisionWithFood()
{
    if (Vector2Equals(snake.body[0], food.position))
    {
        food.position = food.GenerateRandomPos(snake.body);
        snake.addSegment = true;
        score++;
        PlaySound(consumptionSound);
    }
}

void Game::CheckCollisionWithEdges()
{
    if (snake.body[0].x == cellCount || snake.body[0].x == -1 || 
        snake.body[0].y == cellCount || snake.body[0].y == -1)
    {
        GameOver();
    }
}

void Game::CheckCollisionWithTail()
{
    deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();
    
    if (Global::ElementInDeque(snake.body[0], headlessBody))
    {
        GameOver();
    }
}

void Game::GameOver()
{
    snake.Reset(Global::easyAndNormalModeMusic);
    food.position = food.GenerateRandomPos(snake.body);
    running = false;
    score = 0;
    PlaySound(deathSound);
}