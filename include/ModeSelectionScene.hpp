#pragma once
#include "Scene.hpp"
#include "raylib.h"

class ModeSelectionScene : public Scene
{
    public:
        ModeSelectionScene();
        ~ModeSelectionScene() override = default;
        
        void OnLoad() override;
        void Update() override;
        void Draw() const override;
        void OnUnload() override;
        
    private:
        static const int TITLE_FONT_SIZE = 50;
        static const int OPTION_FONT_SIZE = 35;
        
        Color backgroundColor;
        float pulseTimer;
        int selectedOption; // 0 = PvP, 1 = PvAI
};
