#include "AIvsAIScene.hpp"
#include "SceneManager.hpp"
#include "raylib.h"
#include <cmath>

namespace
{
    constexpr double GAME_UPDATE_INTERVAL = 0.2;
    constexpr int BORDER_PADDING = 5;
    constexpr int TITLE_FONT_SIZE = 40;
    constexpr int TITLE_Y_POSITION = 20;
    constexpr float START_DELAY = 2.0f;
}

AIvsAIScene::AIvsAIScene()
    : Scene("AIvsAI", 3),
      gameUpdateInterval(GAME_UPDATE_INTERVAL),
      waitingToStart(true),
      startPulseTimer(0.0f)
{
}

void AIvsAIScene::OnLoad()
{
    game = std::make_unique<Game>();
    global = std::make_unique<Global>();
    
    // Don't start the game immediately - show countdown
    game->running = false;
    waitingToStart = true;
    startPulseTimer = 0.0f;
}

void AIvsAIScene::Update()
{
    startPulseTimer += GetFrameTime();
    
    // Auto-start after delay
    if (waitingToStart && startPulseTimer >= START_DELAY)
    {
        // Set initial directions for both AI
        game->player1.direction = {1, 0};  // Start moving right
        game->player2.direction = {-1, 0}; // Start moving left
        
        game->running = true;
        waitingToStart = false;
    }
    
    // If still waiting, return early
    if (waitingToStart)
    {
        return;
    }
    
    // Update music
    if (game->running)
    {
        UpdateMusicStream(Global::easyAndNormalModeMusic);
    }
    
    // Update both AIs
    UpdateAI1();
    UpdateAI2();
    
    // Update game logic at fixed interval
    if (Global::EventTriggered(gameUpdateInterval))
    {
        game->Update();
    }
    
    // ESC to return to main menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        SceneManager::GetInstance().LoadScene(0); // Main menu
    }
}

void AIvsAIScene::Draw() const
{
    BeginDrawing();
    ClearBackground(global->backgroundColor);
    
    if (waitingToStart)
    {
        DrawStartScreen();
    }
    else
    {
        DrawUI();
        game->Draw();
    }
    
    EndDrawing();
}

void AIvsAIScene::DrawUI() const
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
        TextFormat("AI 1: %i", game->score), 
        Game::borderSize - BORDER_PADDING, 
        scoreY, 
        TITLE_FONT_SIZE, 
        global->snakeColor
    );
    
    DrawText(
        TextFormat("AI 2: %i", game->score2), 
        Game::borderSize + 320, 
        scoreY, 
        TITLE_FONT_SIZE, 
        RED
    );
}

void AIvsAIScene::UpdateAI1()
{
    if (!game->running) return;
    
    // Get AI direction based on food position
    Vector2 aiDirection = game->player1.GetAIDirection(game->food.position, game->player2);
    
    // Only update if it's a valid move (not reversing)
    if (aiDirection.x != 0 || aiDirection.y != 0)
    {
        if ((aiDirection.y == -1 && game->player1.direction.y != 1) ||
            (aiDirection.y == 1 && game->player1.direction.y != -1) ||
            (aiDirection.x == -1 && game->player1.direction.x != 1) ||
            (aiDirection.x == 1 && game->player1.direction.x != -1))
        {
            game->player1.direction = aiDirection;
        }
    }
}

void AIvsAIScene::UpdateAI2()
{
    if (!game->running) return;
    
    // Get AI direction based on food position
    Vector2 aiDirection = game->player2.GetAIDirection(game->food.position, game->player1);
    
    // Only update if it's a valid move (not reversing)
    if (aiDirection.x != 0 || aiDirection.y != 0)
    {
        if ((aiDirection.y == -1 && game->player2.direction.y != 1) ||
            (aiDirection.y == 1 && game->player2.direction.y != -1) ||
            (aiDirection.x == -1 && game->player2.direction.x != 1) ||
            (aiDirection.x == 1 && game->player2.direction.x != -1))
        {
            game->player2.direction = aiDirection;
        }
    }
}

void AIvsAIScene::OnUnload()
{
    // Clean up game and global instances
    game.reset();
    global.reset();
}

void AIvsAIScene::DrawStartScreen() const
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Title
    const char* title = "AI vs AI";
    int titleWidth = MeasureText(title, 50);
    DrawText(
        title,
        (screenWidth - titleWidth) / 2,
        screenHeight / 3,
        50,
        RAYWHITE
    );
    
    // AI indicators
    DrawText(
        "GREEN AI",
        screenWidth / 2 - 200,
        screenHeight / 2,
        30,
        Global::snakeColor
    );
    
    DrawText(
        "vs",
        screenWidth / 2 - 20,
        screenHeight / 2,
        30,
        RAYWHITE
    );
    
    DrawText(
        "RED AI",
        screenWidth / 2 + 50,
        screenHeight / 2,
        30,
        RED
    );
    
    // Countdown text (pulsing)
    const char* startText = "STARTING...";
    int startWidth = MeasureText(startText, 28);
    int alpha = static_cast<int>(200 + 55 * sinf(startPulseTimer * 4.0f));
    Color startColor = Color{255, 255, 255, static_cast<unsigned char>(alpha)};
    
    DrawText(
        startText,
        (screenWidth - startWidth) / 2,
        screenHeight - 200,
        28,
        startColor
    );
    
    // ESC hint
    DrawText(
        "Press ESC to return to menu",
        screenWidth / 2 - 150,
        screenHeight - 150,
        20,
        GRAY
    );
}
