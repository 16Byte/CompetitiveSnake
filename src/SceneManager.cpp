#include "SceneManager.hpp"
#include <iostream>
#include <stdexcept>

SceneManager& SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::RegisterScene(std::unique_ptr<Scene> scene)
{
    if (!scene)
    {
        std::cerr << "Error: Attempted to register null scene" << std::endl;
        return;
    }
    
    int buildIndex = scene->GetBuildIndex();
    std::string sceneName = scene->GetName();
    
    Scene* scenePtr = scene.get();
    
    scenesByIndex[buildIndex] = std::move(scene);
    scenesByName[sceneName] = scenePtr;
    
    std::cout << "Registered scene: " << sceneName << " (Build Index: " << buildIndex << ")" << std::endl;
}

void SceneManager::LoadScene(int buildIndex)
{
    auto it = scenesByIndex.find(buildIndex);
    if (it == scenesByIndex.end())
    {
        std::cerr << "Error: Scene with build index " << buildIndex << " not found!" << std::endl;
        return;
    }
    
    TransitionToScene(it->second.get());
}

void SceneManager::LoadScene(const std::string& sceneName)
{
    auto it = scenesByName.find(sceneName);
    if (it == scenesByName.end())
    {
        std::cerr << "Error: Scene '" << sceneName << "' not found!" << std::endl;
        return;
    }
    
    TransitionToScene(it->second);
}

void SceneManager::TransitionToScene(Scene* scene)
{
    if (!scene)
    {
        std::cerr << "Error: Attempted to transition to null scene" << std::endl;
        return;
    }
    
    if (currentScene)
    {
        std::cout << "Unloading scene: " << currentScene->GetName() << std::endl;
        currentScene->OnUnload();
    }
    
    currentScene = scene;
    
    std::cout << "Loading scene: " << scene->GetName() << std::endl;
    scene->OnLoad();
}

void SceneManager::Update()
{
    if (currentScene)
    {
        currentScene->Update();
    }
}

void SceneManager::Draw() const
{
    if (currentScene)
    {
        currentScene->Draw();
    }
}

int SceneManager::GetActiveSceneBuildIndex() const
{
    return currentScene ? currentScene->GetBuildIndex() : -1;
}

std::string SceneManager::GetActiveSceneName() const
{
    return currentScene ? currentScene->GetName() : "";
}
