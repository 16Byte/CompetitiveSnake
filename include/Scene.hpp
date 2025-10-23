#pragma once
#include <string>

// Abstract base class for all scenes (like Unity's Scene system)
class Scene
{
    public:
        Scene(const std::string& name, int buildIndex);
        virtual ~Scene() = default;
        
        // Scene lifecycle methods
        virtual void OnLoad() = 0;      // Called when scene is loaded
        virtual void Update() = 0;       // Called every frame
        virtual void Draw() const = 0;   // Called every frame for rendering
        virtual void OnUnload() = 0;     // Called before scene is unloaded
        
        // Getters
        std::string GetName() const { return sceneName; }
        int GetBuildIndex() const { return buildIndex; }
        
    protected:
        std::string sceneName;
        int buildIndex;
};
