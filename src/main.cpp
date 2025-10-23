#include "Game.hpp"
#include "Global.hpp"
#include "raylib.h"

using namespace std;

namespace
{
    constexpr double GAME_UPDATE_INTERVAL = 0.2;
    constexpr int TARGET_FPS = 165;
    constexpr int BORDER_PADDING = 5;
    constexpr int TITLE_FONT_SIZE = 40;
    constexpr int TITLE_Y_POSITION = 20;
}

Game Init()
{
    const int windowSize = 2 * Game::borderSize + Game::cellSize * Game::cellCount;
    InitWindow(windowSize, windowSize, "Snake");
    SetTargetFPS(TARGET_FPS);
    
    return Game();
}

void GetInput(Game& game)
{
    // Player 1 - WASD controls
    if (IsKeyPressed(KEY_W) && game.player1.direction.y != 1)
    {
        game.running = true;
        game.player1.direction = {0, -1};
    }
    else if (IsKeyPressed(KEY_S) && game.player1.direction.y != -1)
    {
        game.running = true;
        game.player1.direction = {0, 1};
    }
    else if (IsKeyPressed(KEY_A) && game.player1.direction.x != 1)
    {
        game.running = true;
        game.player1.direction = {-1, 0};
    }
    else if (IsKeyPressed(KEY_D) && game.player1.direction.x != -1)
    {
        game.running = true;
        game.player1.direction = {1, 0};
    }
    
    // Player 2 - Arrow key controls
    if (IsKeyPressed(KEY_UP) && game.player2.direction.y != 1)
    {
        game.running = true;
        game.player2.direction = {0, -1};
    }
    else if (IsKeyPressed(KEY_DOWN) && game.player2.direction.y != -1)
    {
        game.running = true;
        game.player2.direction = {0, 1};
    }
    else if (IsKeyPressed(KEY_LEFT) && game.player2.direction.x != 1)
    {
        game.running = true;
        game.player2.direction = {-1, 0};
    }
    else if (IsKeyPressed(KEY_RIGHT) && game.player2.direction.x != -1)
    {
        game.running = true;
        game.player2.direction = {1, 0};
    }
}

void DrawWindow(const Game& game, const Global& global)
{
    BeginDrawing();
    ClearBackground(global.backgroundColor);
    
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
        global.snakeColor
    );
    
    const int scoreY = Game::borderSize + Game::cellSize * Game::cellCount + BORDER_PADDING * 2;
    DrawText(
        TextFormat("P1 Score: %i", game.score), 
        Game::borderSize - BORDER_PADDING, 
        scoreY, 
        TITLE_FONT_SIZE, 
        global.snakeColor
    );
    
    DrawText(
        TextFormat("P2 Score: %i", game.score2), 
        Game::borderSize + 320, 
        scoreY, 
        TITLE_FONT_SIZE, 
        SKYBLUE
    );
}

void Update(Game& game, Global& global)
{
    DrawWindow(game, global);
    
    if (game.running)
    {
        UpdateMusicStream(global.easyAndNormalModeMusic);
    }
    
    if (global.EventTriggered(GAME_UPDATE_INTERVAL))
    {
        game.Update();
    }
    
    GetInput(game);
    game.Draw();
    EndDrawing();
}

int main() 
{
    Game game = Init();
    Global global = Global();
    
    while (!WindowShouldClose()) 
    {
        Update(game, global);
    }
    
    CloseWindow();
    return 0;
}
