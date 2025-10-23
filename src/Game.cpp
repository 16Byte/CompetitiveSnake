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
      running(true),
      soundsEnabled(true),
      winner(0)
{
    if (!IsAudioDeviceReady())
    {
        InitAudioDevice();
    }
    
    Global::easyAndNormalModeMusic = LoadMusicStream("Assets/Sounds/Music/Breaking News by SAKUMAMATATA.mp3");
    PlayMusicStream(Global::easyAndNormalModeMusic);
    SetMusicVolume(Global::easyAndNormalModeMusic, 0.25f);
    
    consumptionSound = LoadSound("Assets/Sounds/SFX/Consumption 1.wav");
    deathSound = LoadSound("Assets/Sounds/SFX/Death (from Galaga).wav");
}

Game::Game(bool enableSounds) 
    : player1(),
      player2(Vector2{18, 15}, Vector2{-1, 0}),
      food(player1.body, cellCount),
      score(0),
      score2(0),
      running(true),
      soundsEnabled(enableSounds),
      winner(0)
{
    if (soundsEnabled)
    {
        if (!IsAudioDeviceReady())
        {
            InitAudioDevice();
        }
        
        Global::easyAndNormalModeMusic = LoadMusicStream("Assets/Sounds/Music/Breaking News by SAKUMAMATATA.mp3");
        PlayMusicStream(Global::easyAndNormalModeMusic);
        SetMusicVolume(Global::easyAndNormalModeMusic, 0.25f);
        
        consumptionSound = LoadSound("Assets/Sounds/SFX/Consumption 1.wav");
        deathSound = LoadSound("Assets/Sounds/SFX/Death (from Galaga).wav");
    }
}

Game::~Game()
{
    if (soundsEnabled)
    {
        UnloadSound(consumptionSound);
        UnloadSound(deathSound);
        UnloadMusicStream(Global::easyAndNormalModeMusic);
        // Don't close audio device - it may be reused by another Game instance
    }
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
        if (soundsEnabled)
        {
            PlaySound(consumptionSound);
        }
    }
    
    if (Vector2Equals(player2.body[0], food.position))
    {
        food.position = food.GenerateRandomPos(player2.body);
        player2.addSegment = true;
        score2++;
        if (soundsEnabled)
        {
            PlaySound(consumptionSound);
        }
    }
}

void Game::CheckCollisionWithEdges()
{
    bool p1HitWall = (player1.body[0].x == cellCount || player1.body[0].x == -1 || 
                      player1.body[0].y == cellCount || player1.body[0].y == -1);
    bool p2HitWall = (player2.body[0].x == cellCount || player2.body[0].x == -1 || 
                      player2.body[0].y == cellCount || player2.body[0].y == -1);
    
    if (p1HitWall && p2HitWall)
    {
        // Both hit walls - decide by score
        DeclareWinner(score > score2 ? 1 : (score2 > score ? 2 : 3));
    }
    else if (p1HitWall)
    {
        // Player 1 hit wall - Player 2 wins
        DeclareWinner(2);
    }
    else if (p2HitWall)
    {
        // Player 2 hit wall - Player 1 wins
        DeclareWinner(1);
    }
}

void Game::CheckCollisionWithTail()
{
    deque<Vector2> headlessBody1 = player1.body;
    headlessBody1.pop_front();
    
    deque<Vector2> headlessBody2 = player2.body;
    headlessBody2.pop_front();
    
    // Check if player1 hits its own tail
    bool p1HitSelf = Global::ElementInDeque(player1.body[0], headlessBody1);
    
    // Check if player2 hits its own tail
    bool p2HitSelf = Global::ElementInDeque(player2.body[0], headlessBody2);
    
    // Check head-to-head collision
    bool headToHead = Vector2Equals(player1.body[0], player2.body[0]);
    
    // Check if player1 hits player2's body (not head)
    bool p1HitP2Body = Global::ElementInDeque(player1.body[0], headlessBody2);
    
    // Check if player2 hits player1's body (not head)
    bool p2HitP1Body = Global::ElementInDeque(player2.body[0], headlessBody1);
    
    // Determine winner based on collision type
    if (headToHead)
    {
        // Head-to-head collision - decide by score
        DeclareWinner(score > score2 ? 1 : (score2 > score ? 2 : 3));
    }
    else if (p1HitSelf && p2HitSelf)
    {
        // Both ate themselves - decide by score
        DeclareWinner(score > score2 ? 1 : (score2 > score ? 2 : 3));
    }
    else if (p1HitP2Body && p2HitP1Body)
    {
        // Both hit each other's bodies - decide by score
        DeclareWinner(score > score2 ? 1 : (score2 > score ? 2 : 3));
    }
    else if (p1HitSelf || p1HitP2Body)
    {
        // Player 1 at fault - Player 2 wins
        DeclareWinner(2);
    }
    else if (p2HitSelf || p2HitP1Body)
    {
        // Player 2 at fault - Player 1 wins
        DeclareWinner(1);
    }
}

void Game::DeclareWinner(int winnerNum)
{
    winner = winnerNum;
    GameOver();
}

void Game::GameOver()
{
    if (soundsEnabled)
    {
        player1.Reset(Global::easyAndNormalModeMusic);
        player2.Reset(Global::easyAndNormalModeMusic);
    }
    else
    {
        // For silent mode, just reset positions without music
        player1.body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        player1.direction = {1, 0};
        player2.body = {Vector2{18, 15}, Vector2{19, 15}, Vector2{20, 15}};
        player2.direction = {-1, 0};
    }
    
    food.position = food.GenerateRandomPos(player1.body);
    running = false;
    score = 0;
    score2 = 0;
    
    if (soundsEnabled)
    {
        PlaySound(deathSound);
    }
}