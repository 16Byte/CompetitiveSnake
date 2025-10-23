#include "AIGameScene.hpp"
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

AIGameScene::AIGameScene()
    : Scene("AIGame", 2),
      gameUpdateInterval(GAME_UPDATE_INTERVAL),
      waitingForPlayer(true),
      readyPulseTimer(0.0f),
      playerNextDirection{0, 0},
      playerDirectionChanged(false)
{
}

void AIGameScene::OnLoad()
{
    game = std::make_unique<Game>();
    global = std::make_unique<Global>();
    
    // Don't start the game immediately - wait for player
    game->running = false;
    waitingForPlayer = true;
    readyPulseTimer = 0.0f;
    
    playerNextDirection = {0, 0};
    playerDirectionChanged = false;
}

void AIGameScene::Update()
{
    readyPulseTimer += GetFrameTime();
    
    // If waiting for player, check for ready input
    if (waitingForPlayer)
    {
        CheckReadyInput();
        return;
    }
    
    // If game ended, wait for any key to restart
    if (!game->running && game->winner != 0)
    {
        // Check for any key press to restart
        if (GetKeyPressed() != 0 || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            game->winner = 0;
            waitingForPlayer = true;
            game->running = false;
            readyPulseTimer = 0.0f;
        }
        return;
    }
    
    // Update music
    if (game->running)
    {
        UpdateMusicStream(Global::easyAndNormalModeMusic);
    }
    
    // Handle player input
    HandlePlayerInput();
    
    // Update AI
    UpdateAI();
    
    // Update game logic at fixed interval
    if (Global::EventTriggered(gameUpdateInterval))
    {
        // Apply buffered direction change before updating
        if (playerDirectionChanged)
        {
            game->player1.direction = playerNextDirection;
            playerDirectionChanged = false;
        }
        
        game->Update();
    }
    
    // ESC to return to main menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        SceneManager::GetInstance().LoadScene(0); // Main menu
    }
}

void AIGameScene::Draw() const
{
    BeginDrawing();
    ClearBackground(global->backgroundColor);
    
    if (waitingForPlayer)
    {
        DrawReadyScreen();
    }
    else
    {
        DrawUI();
        game->Draw();
        
        // Draw victory screen if game ended
        if (!game->running && game->winner != 0)
        {
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();
            
            // Semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});
            
            const char* victoryText;
            Color victoryColor;
            
            if (game->winner == 1)
            {
                victoryText = "YOU WIN!";
                victoryColor = GREEN;
            }
            else if (game->winner == 2)
            {
                victoryText = "AI WINS!";
                victoryColor = RED;
            }
            else
            {
                victoryText = "TIE GAME!";
                victoryColor = YELLOW;
            }
            
            int victoryWidth = MeasureText(victoryText, 60);
            DrawText(
                victoryText,
                (screenWidth - victoryWidth) / 2,
                screenHeight / 2 - 50,
                60,
                victoryColor
            );
            
            // Instructions
            const char* instruction = "Press any key to continue";
            int instrWidth = MeasureText(instruction, 25);
            DrawText(
                instruction,
                (screenWidth - instrWidth) / 2,
                screenHeight / 2 + 50,
                25,
                LIGHTGRAY
            );
        }
    }
    
    EndDrawing();
}

void AIGameScene::DrawUI() const
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
        TextFormat("PLAYER: %i", game->score), 
        Game::borderSize - BORDER_PADDING, 
        scoreY, 
        TITLE_FONT_SIZE, 
        global->snakeColor
    );
    
    DrawText(
        TextFormat("AI: %i", game->score2), 
        Game::borderSize + 320, 
        scoreY, 
        TITLE_FONT_SIZE, 
        RED
    );
}

void AIGameScene::HandlePlayerInput()
{
    // Get the actual current direction from body position (head vs second segment)
    Vector2 playerActualDirection = game->player1.direction;
    
    if (game->player1.body.size() >= 2)
    {
        Vector2 head = game->player1.body[0];
        Vector2 neck = game->player1.body[1];
        playerActualDirection = {head.x - neck.x, head.y - neck.y};
    }
    
    // Player 1 - WASD controls only (only allow one direction change per update)
    if (!playerDirectionChanged)
    {
        if (IsKeyPressed(KEY_W) && playerActualDirection.y != 1)
        {
            game->running = true;
            playerNextDirection = {0, -1};
            playerDirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_S) && playerActualDirection.y != -1)
        {
            game->running = true;
            playerNextDirection = {0, 1};
            playerDirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_A) && playerActualDirection.x != 1)
        {
            game->running = true;
            playerNextDirection = {-1, 0};
            playerDirectionChanged = true;
        }
        else if (IsKeyPressed(KEY_D) && playerActualDirection.x != -1)
        {
            game->running = true;
            playerNextDirection = {1, 0};
            playerDirectionChanged = true;
        }
    }
}

void AIGameScene::UpdateAI()
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

void AIGameScene::OnUnload()
{
    // Clean up game and global instances
    game.reset();
    global.reset();
}

void AIGameScene::DrawReadyScreen() const
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Title
    const char* title = "PLAYER vs AI";
    int titleWidth = MeasureText(title, 50);
    DrawText(
        title,
        (screenWidth - titleWidth) / 2,
        screenHeight / 3,
        50,
        RAYWHITE
    );
    
    // Player controls
    DrawText(
        "CONTROLS: W A S D",
        screenWidth / 2 - 150,
        screenHeight / 2,
        30,
        RAYWHITE
    );
    
    // AI indicator
    DrawText(
        "vs SMART AI",
        screenWidth / 2 - 100,
        screenHeight / 2 + 50,
        30,
        RED
    );
    
    // Ready instruction (pulsing)
    const char* readyText = "PRESS ANY KEY TO START";
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

void AIGameScene::CheckReadyInput()
{
    // Detect player input
    Vector2 playerDir = {0, 0};
    
    if (IsKeyDown(KEY_W)) playerDir = {0, -1};
    else if (IsKeyDown(KEY_S)) playerDir = {0, 1};
    else if (IsKeyDown(KEY_A)) playerDir = {-1, 0};
    else if (IsKeyDown(KEY_D)) playerDir = {1, 0};
    
    bool playerReady = (playerDir.x != 0 || playerDir.y != 0);
    
    if (playerReady)
    {
        // Set player initial direction
        game->player1.direction = playerDir;
        
        // Set AI initial direction (start moving right)
        game->player2.direction = {1, 0};
        
        // Start the game
        game->running = true;
        waitingForPlayer = false;
    }
}
