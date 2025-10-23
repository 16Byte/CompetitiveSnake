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
      selectedOption(0)
{
}

void MainMenuScene::OnLoad()
{
    titlePulseTimer = 0.0f;
    selectedOption = 0;
}

void MainMenuScene::Update()
{
    titlePulseTimer += GetFrameTime();
    
    // Navigation
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
    {
        selectedOption--;
        if (selectedOption < 0) selectedOption = 3;
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
    {
        selectedOption++;
        if (selectedOption > 3) selectedOption = 0;
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
            case 2: // Options (placeholder)
                // TODO: Implement options scene
                break;
            case 3: // Quit
                // Close the window - the game loop will exit
                CloseWindow();
                break;
        }
    }
}

void MainMenuScene::Draw() const
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    
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
        "OPTIONS",
        "QUIT GAME"
    };
    
    int startY = screenHeight / 2 - 60;
    int spacing = 60;
    
    for (int i = 0; i < 4; i++)
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
    // Clean up any resources if needed
}
