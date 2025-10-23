#include "SceneManager.hpp"
#include "MainMenuScene.hpp"
#include "GameScene.hpp"
#include "AIGameScene.hpp"
#include "AIvsAIScene.hpp"
#include "Game.hpp"
#include "raylib.h"
#include <memory>

using namespace std;

namespace
{
    constexpr int TARGET_FPS = 165;
}

void InitializeWindow()
{
    const int windowSize = 2 * Game::borderSize + Game::cellSize * Game::cellCount;
    InitWindow(windowSize, windowSize, "Competitive Snake");
    SetTargetFPS(TARGET_FPS);
}

void RegisterScenes()
{
    SceneManager& sceneManager = SceneManager::GetInstance();
    
    // Register MainMenu scene (Build Index: 0)
    sceneManager.RegisterScene(std::make_unique<MainMenuScene>());
    
    // Register PvP Game scene (Build Index: 1)
    sceneManager.RegisterScene(std::make_unique<GameScene>());
    
    // Register PvAI Game scene (Build Index: 2)
    sceneManager.RegisterScene(std::make_unique<AIGameScene>());
    
    // Register AIvsAI Game scene (Build Index: 3)
    sceneManager.RegisterScene(std::make_unique<AIvsAIScene>());
}

int main() 
{
    InitializeWindow();

    SetExitKey(0);
    
    // Register all scenes with the SceneManager
    RegisterScenes();
    
    // Load the initial scene (MainMenu - Build Index 0)
    SceneManager::GetInstance().LoadScene(0);
    
    // Main game loop
    while (!WindowShouldClose()) 
    {
        SceneManager::GetInstance().Update();
        SceneManager::GetInstance().Draw();
    }
    
    // Clean up audio device before closing
    if (IsAudioDeviceReady())
    {
        CloseAudioDevice();
    }
    
    CloseWindow();
    return 0;
}
