#pragma once
#include "raylib.h"
#include <deque>

class Global
{
    public:
        inline static Color snakeColor = RAYWHITE;
        inline static Color foodColor = RED;
        inline static Color backgroundColor = Color{40, 40, 40, 255};
        inline static Music easyAndNormalModeMusic = LoadMusicStream("Assets/Sounds/Music/Breaking News by SAKUMAMATATA.mp3");
        inline static double lastUpdateTime = 0;
        
        static bool EventTriggered(double interval);
        static bool ElementInDeque(Vector2 element, const std::deque<Vector2>& deque);
};