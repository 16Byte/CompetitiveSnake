#include "MainMenuScene.hpp"
#include "SceneManager.hpp"
#include "Global.hpp"
#include "raylib.h"
#include <cmath>

MainMenuScene::MainMenuScene()
    : Scene("MainMenu", 0),
      backgroundColor(Color{40, 40, 40, 255}),
      titleColor(RAYWHITE),
      titlePulseTimer(0.0f),
      selectedOption(0),
      gameUpdateInterval(0.2)
{
}

void MainMenuScene::OnLoad()
{
    titlePulseTimer = 0.0f;
    selectedOption = 0;
    
    // Initialize background AI battle (with sounds disabled)
    backgroundGame = std::make_unique<Game>(false);
    backgroundGlobal = std::make_unique<Global>();
    
    // Start the background game immediately
    backgroundGame->running = true;
    backgroundGame->player1.direction = {1, 0};  // Start moving right
    backgroundGame->player2.direction = {-1, 0}; // Start moving left
}

void MainMenuScene::Update()
{
    titlePulseTimer += GetFrameTime();
    
    // If game stopped (someone died), restart it (silent mode)
    if (!backgroundGame->running)
    {
        backgroundGame = std::make_unique<Game>(false);
        backgroundGame->running = true;
        backgroundGame->player1.direction = {1, 0};
        backgroundGame->player2.direction = {-1, 0};
    }
    
    // Update background AI battle
    UpdateBackgroundAI();
    
    // Update game logic at fixed interval
    if (Global::EventTriggered(gameUpdateInterval))
    {
        backgroundGame->Update();
    }
    
    // Navigation
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
    {
        selectedOption--;
        if (selectedOption < 0) selectedOption = 4;
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
    {
        selectedOption++;
        if (selectedOption > 4) selectedOption = 0;
    }
    
    // Selection
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        switch (selectedOption)
        {
            case 0: // Player vs Player
                SceneManager::GetInstance().LoadScene(1);
                break;
            case 1: // Player vs AI
                SceneManager::GetInstance().LoadScene(2);
                break;
            case 2: // AI vs AI
                SceneManager::GetInstance().LoadScene(3);
                break;
            case 3: // Options (placeholder)
                // TODO: Implement options scene
                break;
            case 4: // Quit
                // Close the window - the game loop will exit
                CloseWindow();
                break;
        }
    }
}

void MainMenuScene::Draw() const
{
    BeginDrawing();
    
    // Draw background game with darkening overlay
    ClearBackground(backgroundGlobal->backgroundColor);
    backgroundGame->Draw();
    
    // Dark semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 160});
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Title with pulse effect
    const char* title = "COMPETITIVE SNAKE";
    float pulse = 1.0f + 0.1f * sinf(titlePulseTimer * 3.0f);
    int pulsedFontSize = static_cast<int>(TITLE_FONT_SIZE * pulse);
    int pulsedTitleWidth = MeasureText(title, pulsedFontSize);
    
    DrawText(
        title,
        (screenWidth - pulsedTitleWidth) / 2,
        screenHeight / 5,
        pulsedFontSize,
        titleColor
    );
    
    // Menu options
    const char* options[] = {
        "PLAYER vs PLAYER",
        "PLAYER vs AI",
        "AI vs AI",
        "OPTIONS",
        "QUIT GAME"
    };
    
    int startY = screenHeight / 2 - 80;
    int spacing = 50;
    
    for (int i = 0; i < 5; i++)
    {
        Color optionColor = (i == selectedOption) ? GREEN : LIGHTGRAY;
        int fontSize = OPTION_FONT_SIZE;
        
        // Pulse effect on selected option
        if (i == selectedOption)
        {
            float optionPulse = 1.0f + 0.05f * sinf(titlePulseTimer * 5.0f);
            fontSize = static_cast<int>(OPTION_FONT_SIZE * optionPulse);
        }
        
        int textWidth = MeasureText(options[i], fontSize);
        DrawText(
            options[i],
            (screenWidth - textWidth) / 2,
            startY + (i * spacing),
            fontSize,
            optionColor
        );
    }
    
    // Instructions
    const char* instruction = "Use W/S or UP/DOWN to select, ENTER to confirm";
    int instrWidth = MeasureText(instruction, 20);
    DrawText(
        instruction,
        (screenWidth - instrWidth) / 2,
        screenHeight - 80,
        20,
        GRAY
    );
    
    // Footer
    const char* footer = "Snake Clone by Navi";
    int footerWidth = MeasureText(footer, 18);
    DrawText(
        footer,
        (screenWidth - footerWidth) / 2,
        screenHeight - 40,
        18,
        DARKGRAY
    );
    
    EndDrawing();
}

void MainMenuScene::OnUnload()
{
    // Clean up background game
    backgroundGame.reset();
    backgroundGlobal.reset();
}

void MainMenuScene::UpdateBackgroundAI()
{
    if (!backgroundGame->running) return;
    
    // Update AI for player 1
    Vector2 ai1Direction = backgroundGame->player1.GetAIDirection(
        backgroundGame->food.position, 
        backgroundGame->player2
    );
    
    if (ai1Direction.x != 0 || ai1Direction.y != 0)
    {
        if ((ai1Direction.y == -1 && backgroundGame->player1.direction.y != 1) ||
            (ai1Direction.y == 1 && backgroundGame->player1.direction.y != -1) ||
            (ai1Direction.x == -1 && backgroundGame->player1.direction.x != 1) ||
            (ai1Direction.x == 1 && backgroundGame->player1.direction.x != -1))
        {
            backgroundGame->player1.direction = ai1Direction;
        }
    }
    
    // Update AI for player 2
    Vector2 ai2Direction = backgroundGame->player2.GetAIDirection(
        backgroundGame->food.position, 
        backgroundGame->player1
    );
    
    if (ai2Direction.x != 0 || ai2Direction.y != 0)
    {
        if ((ai2Direction.y == -1 && backgroundGame->player2.direction.y != 1) ||
            (ai2Direction.y == 1 && backgroundGame->player2.direction.y != -1) ||
            (ai2Direction.x == -1 && backgroundGame->player2.direction.x != 1) ||
            (ai2Direction.x == 1 && backgroundGame->player2.direction.x != -1))
        {
            backgroundGame->player2.direction = ai2Direction;
        }
    }
}
