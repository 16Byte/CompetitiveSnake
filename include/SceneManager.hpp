#pragma once
#include "Scene.hpp"
#include "raylib.h"
#include <memory>
#include <unordered_map>
#include <string>

// Singleton SceneManager (Unity-style)
class SceneManager
{
    public:
        // Singleton access
        static SceneManager& GetInstance();

        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;

        // Scene registration (should be called at initialization)
        void RegisterScene(std::unique_ptr<Scene> scene);
        void LoadScene(int buildIndex);
        void LoadScene(const std::string& sceneName);
        
        // Scene lifecycle management
        void Update();
        void Draw() const;
        
        // Getters
        Scene* GetActiveScene() const { return currentScene; }
        int GetActiveSceneBuildIndex() const;
        std::string GetActiveSceneName() const;
        
        // Data passing between scenes
        void SetPlayer1InitialDirection(Vector2 direction) { player1InitialDir = direction; }
        void SetPlayer2InitialDirection(Vector2 direction) { player2InitialDir = direction; }
        Vector2 GetPlayer1InitialDirection() const { return player1InitialDir; }
        Vector2 GetPlayer2InitialDirection() const { return player2InitialDir; }
        
    private:
        SceneManager() = default;
        ~SceneManager() = default;
        
        Scene* currentScene = nullptr;
        std::unordered_map<int, std::unique_ptr<Scene>> scenesByIndex;
        std::unordered_map<std::string, Scene*> scenesByName;
        
        // Data for passing between scenes
        Vector2 player1InitialDir = {1, 0};
        Vector2 player2InitialDir = {-1, 0};
        
        void TransitionToScene(Scene* scene);
};
