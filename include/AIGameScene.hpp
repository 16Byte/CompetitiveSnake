#pragma once
#include "Scene.hpp"
#include "Game.hpp"
#include "Global.hpp"
#include <memory>

class AIGameScene : public Scene
{
    public:
        AIGameScene();
        ~AIGameScene() override = default;
        
        void OnLoad() override;
        void Update() override;
        void Draw() const override;
        void OnUnload() override;
        
    private:
        std::unique_ptr<Game> game;
        std::unique_ptr<Global> global;
        
        double gameUpdateInterval;
        bool waitingForPlayer;
        float readyPulseTimer;
        
        void DrawUI() const;
        void DrawReadyScreen() const;
        void HandlePlayerInput();
        void CheckReadyInput();
        void UpdateAI();
};
