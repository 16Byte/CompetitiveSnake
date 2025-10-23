#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "raylib.h"
#include <cmath>

namespace
{
    constexpr double GAME_UPDATE_INTERVAL = 0.2;
    constexpr int BORDER_PADDING = 5;
    constexpr int TITLE_FONT_SIZE = 40;
    constexpr int TITLE_Y_POSITION = 20;
}

GameScene::GameScene()
    : Scene("Game", 1),
      gameUpdateInterval(GAME_UPDATE_INTERVAL),
      waitingForPlayers(true),
      readyPulseTimer(0.0f),
      player1NextDirection{0, 0},
      player2NextDirection{0, 0},
      player1DirectionChanged(false),
      player2DirectionChanged(false)
{
}

void GameScene::OnLoad()
{
    game = std::make_unique<Game>();
    global = std::make_unique<Global>();
    
    // Don't start the game immediately - wait for players
    game->running = false;
    waitingForPlayers = true;
    readyPulseTimer = 0.0f;
    
    player1NextDirection = {0, 0};
    player2NextDirection = {0, 0};
    player1DirectionChanged = false;
    player2DirectionChanged = false;
}

void GameScene::Update()
{
    readyPulseTimer += GetFrameTime();
    
    // If waiting for players, check for ready input
    if (waitingForPlayers)
    {
        CheckReadyInput();
        return;
    }
    
    // Update music
    if (game->running)
    {
        UpdateMusicStream(Global::easyAndNormalModeMusic);
    }
    
    // Handle input
    HandleInput();
    
    // Update game logic at fixed interval
    if (Global::EventTriggered(gameUpdateInterval))
    {
        // Apply buffered direction changes before updating
        if (player1DirectionChanged)
        {
            game->player1.direction = player1NextDirection;
            player1DirectionChanged = false;
        }
        if (player2DirectionChanged)
        {
            game->player2.direction = player2NextDirection;
            player2DirectionChanged = false;
        }
        
        game->Update();
    }
    
    // ESC to return to main menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        SceneManager::GetInstance().LoadScene(0); // Main menu
    }
}

void GameScene::Draw() const
{
    BeginDrawing();
    ClearBackground(global->backgroundColor);
    
    if (waitingForPlayers)
    {
        DrawReadyScreen();
    }
    else
    {
        DrawUI();
        game->Draw();
    }
    
    EndDrawing();
}

void GameScene::DrawUI() const
{
    const float borderX = static_cast<float>(Game::borderSize - BORDER_PADDING);
    const float borderY = static_cast<float>(Game::borderSize - BORDER_PADDING);
    const float borderWidth = static_cast<float>(Game::cellSize * Game::cellCount + 2 * BORDER_PADDING);
    const float borderHeight = static_cast<float>(Game::cellSize * Game::cellCount + 2 * BORDER_PADDING);
    
    DrawRectangleLinesEx(
        Rectangle{borderX, borderY, borderWidth, borderHeight}, 
        BORDER_PADDING, 
        Global::snakeColor
    );
    
    DrawText(
        "Snake Clone by Navi", 
        Game::borderSize - BORDER_PADDING, 
        TITLE_Y_POSITION, 
        TITLE_FONT_SIZE, 
        global->snakeColor
    );
    
    const int scoreY = Game::borderSize + Game::cellSize * Game::cellCount + BORDER_PADDING * 2;
    DrawText(
        TextFormat("P1 Score: %i", game->score), 
        Game::borderSize - BORDER_PADDING, 
        scoreY, 
        TITLE_FONT_SIZE, 
        global->snakeColor
    );
    
    DrawText(
        TextFormat("P2 Score: %i", game->score2), 
        Game::borderSize + 320, 
        scoreY, 
        TITLE_FONT_SIZE, 
        SKYBLUE
    );
}

void GameScene::HandleInput()
{
    // Get the actual current direction from body position (head vs second segment)
    Vector2 player1ActualDirection = game->player1.direction;
    Vector2 player2ActualDirection = game->player2.direction;
    
    if (game->player1.body.size() >= 2)
    {
        Vector2 head = game->player1.body[0];
        Vector2 neck = game->player1.body[1];
        player1ActualDirection = {head.x - neck.x, head.y - neck.y};
    }
    
    if (game->player2.body.size() >= 2)
    {
        Vector2 head = game->player2.body[0];
        Vector2 neck = game->player2.body[1];
        player2ActualDirection = {head.x - neck.x, head.y - neck.y};
    }
    
    // Player 1 - WASD controls (only allow one direction change per update)
    if (!player1DirectionChanged)
    {
        if (IsKeyPressed(KEY_W) && player1ActualDirection.y != 1)
        {
            game->running = true;
            player1NextDirection = {0, -1};
            player1DirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_S) && player1ActualDirection.y != -1)
        {
            game->running = true;
            player1NextDirection = {0, 1};
            player1DirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_A) && player1ActualDirection.x != 1)
        {
            game->running = true;
            player1NextDirection = {-1, 0};
            player1DirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_D) && player1ActualDirection.x != -1)
        {
            game->running = true;
            player1NextDirection = {1, 0};
            player1DirectionChanged = true;
        }
    }
    
    // Player 2 - Arrow key controls (only allow one direction change per update)
    if (!player2DirectionChanged)
    {
        if (IsKeyPressed(KEY_UP) && player2ActualDirection.y != 1)
        {
            game->running = true;
            player2NextDirection = {0, -1};
            player2DirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_DOWN) && player2ActualDirection.y != -1)
        {
            game->running = true;
            player2NextDirection = {0, 1};
            player2DirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_LEFT) && player2ActualDirection.x != 1)
        {
            game->running = true;
            player2NextDirection = {-1, 0};
            player2DirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_RIGHT) && player2ActualDirection.x != -1)
        {
            game->running = true;
            player2NextDirection = {1, 0};
            player2DirectionChanged = true;
        }
    }
}

void GameScene::OnUnload()
{
    // Clean up game and global instances
    game.reset();
    global.reset();
}

void GameScene::DrawReadyScreen() const
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Title
    const char* title = "PLAYER vs PLAYER";
    int titleWidth = MeasureText(title, 50);
    DrawText(
        title,
        (screenWidth - titleWidth) / 2,
        screenHeight / 3,
        50,
        RAYWHITE
    );
    
    // Player 1 controls
    DrawText(
        "PLAYER 1: W A S D",
        screenWidth / 2 - 300,
        screenHeight / 2,
        30,
        RAYWHITE
    );
    
    // Player 2 controls
    DrawText(
        "PLAYER 2: ARROW KEYS",
        screenWidth / 2 + 50,
        screenHeight / 2,
        30,
        SKYBLUE
    );
    
    // Ready instruction (pulsing)
    const char* readyText = "BOTH PLAYERS PRESS ANY KEY TO START";
    int readyWidth = MeasureText(readyText, 28);
    int alpha = static_cast<int>(200 + 55 * sinf(readyPulseTimer * 4.0f));
    Color readyColor = Color{255, 255, 255, static_cast<unsigned char>(alpha)};
    
    DrawText(
        readyText,
        (screenWidth - readyWidth) / 2,
        screenHeight - 200,
        28,
        readyColor
    );
}

void GameScene::CheckReadyInput()
{
    // Detect which direction each player is pressing
    Vector2 player1Dir = {0, 0};
    Vector2 player2Dir = {0, 0};
    
    // Player 1 input detection
    if (IsKeyDown(KEY_W)) player1Dir = {0, -1};
    else if (IsKeyDown(KEY_S)) player1Dir = {0, 1};
    else if (IsKeyDown(KEY_A)) player1Dir = {-1, 0};
    else if (IsKeyDown(KEY_D)) player1Dir = {1, 0};
    
    // Player 2 input detection
    if (IsKeyDown(KEY_UP)) player2Dir = {0, -1};
    else if (IsKeyDown(KEY_DOWN)) player2Dir = {0, 1};
    else if (IsKeyDown(KEY_LEFT)) player2Dir = {-1, 0};
    else if (IsKeyDown(KEY_RIGHT)) player2Dir = {1, 0};
    
    // Both players must press a key simultaneously to start
    bool player1Ready = (player1Dir.x != 0 || player1Dir.y != 0);
    bool player2Ready = (player2Dir.x != 0 || player2Dir.y != 0);
    
    if (player1Ready && player2Ready)
    {
        // Set initial directions
        game->player1.direction = player1Dir;
        game->player2.direction = player2Dir;
        
        // Start the game
        game->running = true;
        waitingForPlayers = false;
    }
}
