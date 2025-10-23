#include "Game.hpp"
#include "Global.hpp"
#include "raylib.h"
#include "raymath.h"
#include <deque>

using namespace std;

Game::Game() 
    : player1(),
      player2(Vector2{18, 15}, Vector2{-1, 0}),
      food(player1.body, cellCount),
      score(0),
      score2(0),
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
    player1.Draw(cellSize, borderSize, Global::snakeColor);
    player2.Draw(cellSize, borderSize, SKYBLUE);
}

void Game::Update()
{
    if (running)
    {
        player1.Update();
        player2.Update();
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
    }
}

void Game::CheckCollisionWithFood()
{
    if (Vector2Equals(player1.body[0], food.position))
    {
        food.position = food.GenerateRandomPos(player1.body);
        player1.addSegment = true;
        score++;
        PlaySound(consumptionSound);
    }
    
    if (Vector2Equals(player2.body[0], food.position))
    {
        food.position = food.GenerateRandomPos(player2.body);
        player2.addSegment = true;
        score2++;
        PlaySound(consumptionSound);
    }
}

void Game::CheckCollisionWithEdges()
{
    if (player1.body[0].x == cellCount || player1.body[0].x == -1 || 
        player1.body[0].y == cellCount || player1.body[0].y == -1)
    {
        GameOver();
    }
    
    if (player2.body[0].x == cellCount || player2.body[0].x == -1 || 
        player2.body[0].y == cellCount || player2.body[0].y == -1)
    {
        GameOver();
    }
}

void Game::CheckCollisionWithTail()
{
    deque<Vector2> headlessBody = player1.body;
    headlessBody.pop_front();
    
    // Check if player1 hits its own tail
    if (Global::ElementInDeque(player1.body[0], headlessBody))
    {
        GameOver();
    }
    
    // Check if player1 hits player2's body
    if (Global::ElementInDeque(player1.body[0], player2.body))
    {
        GameOver();
    }
    
    deque<Vector2> headlessBody2 = player2.body;
    headlessBody2.pop_front();
    
    // Check if player2 hits its own tail
    if (Global::ElementInDeque(player2.body[0], headlessBody2))
    {
        GameOver();
    }
    
    // Check if player2 hits player1's body
    if (Global::ElementInDeque(player2.body[0], player1.body))
    {
        GameOver();
    }
}

void Game::GameOver()
{
    player1.Reset(Global::easyAndNormalModeMusic);
    player2.Reset(Global::easyAndNormalModeMusic);
    food.position = food.GenerateRandomPos(player1.body);
    running = false;
    score = 0;
    score2 = 0;
    PlaySound(deathSound);
}