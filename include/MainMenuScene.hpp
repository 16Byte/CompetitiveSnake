#pragma once
#include "Scene.hpp"
#include "raylib.h"

class MainMenuScene : public Scene
{
    public:
        MainMenuScene();
        ~MainMenuScene() override = default;
        
        void OnLoad() override;
        void Update() override;
        void Draw() const override;
        void OnUnload() override;
        
    private:
        static const int TITLE_FONT_SIZE = 60;
        static const int OPTION_FONT_SIZE = 30;
        
        Color backgroundColor;
        Color titleColor;
        float titlePulseTimer;
        int selectedOption; // 0=PvP, 1=PvAI, 2=AIvsAI, 3=Options, 4=Quit
};
