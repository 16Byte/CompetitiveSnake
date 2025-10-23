#include "ModeSelectionScene.hpp"
#include "SceneManager.hpp"
#include "raylib.h"
#include <cmath>

ModeSelectionScene::ModeSelectionScene()
    : Scene("ModeSelection", 1),
      backgroundColor(Color{40, 40, 40, 255}),
      pulseTimer(0.0f),
      selectedOption(0)
{
}

void ModeSelectionScene::OnLoad()
{
    pulseTimer = 0.0f;
    selectedOption = 0;
}

void ModeSelectionScene::Update()
{
    pulseTimer += GetFrameTime();
    
    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedOption = 0;
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedOption = 1;
    }
    
    // Selection
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (selectedOption == 0)
        {
            // PvP Mode - go to GameScene (build index 2)
            SceneManager::GetInstance().LoadScene(2);
        }
        else
        {
            // PvAI Mode - go to AIGameScene (build index 3)
            SceneManager::GetInstance().LoadScene(3);
        }
    }
    
    // ESC to return to main menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        SceneManager::GetInstance().LoadScene(0);
    }
}

void ModeSelectionScene::Draw() const
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Title
    const char* title = "SELECT GAME MODE";
    int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    DrawText(
        title,
        (screenWidth - titleWidth) / 2,
        screenHeight / 4,
        TITLE_FONT_SIZE,
        RAYWHITE
    );
    
    int optionStartY = screenHeight / 2 - 60;
    
    // Option 1: Player vs Player
    const char* pvpText = "PLAYER vs PLAYER";
    int pvpWidth = MeasureText(pvpText, OPTION_FONT_SIZE);
    Color pvpColor = (selectedOption == 0) ? GREEN : LIGHTGRAY;
    
    if (selectedOption == 0)
    {
        // Pulse effect on selected option
        float pulse = 1.0f + 0.05f * sinf(pulseTimer * 5.0f);
        int pulsedSize = static_cast<int>(OPTION_FONT_SIZE * pulse);
        int pulsedWidth = MeasureText(pvpText, pulsedSize);
        DrawText(pvpText, (screenWidth - pulsedWidth) / 2, optionStartY, pulsedSize, pvpColor);
    }
    else
    {
        DrawText(pvpText, (screenWidth - pvpWidth) / 2, optionStartY, OPTION_FONT_SIZE, pvpColor);
    }
    
    // Option 2: Player vs AI
    const char* pvaText = "PLAYER vs AI";
    int pvaWidth = MeasureText(pvaText, OPTION_FONT_SIZE);
    Color pvaColor = (selectedOption == 1) ? GREEN : LIGHTGRAY;
    
    if (selectedOption == 1)
    {
        // Pulse effect on selected option
        float pulse = 1.0f + 0.05f * sinf(pulseTimer * 5.0f);
        int pulsedSize = static_cast<int>(OPTION_FONT_SIZE * pulse);
        int pulsedWidth = MeasureText(pvaText, pulsedSize);
        DrawText(pvaText, (screenWidth - pulsedWidth) / 2, optionStartY + 80, pulsedSize, pvaColor);
    }
    else
    {
        DrawText(pvaText, (screenWidth - pvaWidth) / 2, optionStartY + 80, OPTION_FONT_SIZE, pvaColor);
    }
    
    // Instructions
    const char* instruction = "Use W/S or UP/DOWN to select, ENTER to confirm";
    int instrWidth = MeasureText(instruction, 20);
    DrawText(
        instruction,
        (screenWidth - instrWidth) / 2,
        screenHeight - 100,
        20,
        GRAY
    );
    
    EndDrawing();
}

void ModeSelectionScene::OnUnload()
{
    // Clean up if needed
}
