#pragma once
#include "Scene.hpp"
#include "Game.hpp"
#include "Global.hpp"
#include <memory>

class AIvsAIScene : public Scene
{
    public:
        AIvsAIScene();
        ~AIvsAIScene() override = default;
        
        void OnLoad() override;
        void Update() override;
        void Draw() const override;
        void OnUnload() override;
        
    private:
        std::unique_ptr<Game> game;
        std::unique_ptr<Global> global;
        
        double gameUpdateInterval;
        bool waitingToStart;
        float startPulseTimer;
        float deathDelayTimer;
        bool inDeathDelay;
        
        void DrawUI() const;
        void DrawStartScreen() const;
        void UpdateAI1();
        void UpdateAI2();
};
