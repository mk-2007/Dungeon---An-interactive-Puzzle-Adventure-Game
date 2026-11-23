#pragma once
#include "../core/Scene.h"
#include "../core/UI.h"
#include "StackChamber.h"
#include <string>

class StackVaultScene : public Scene
{
public:
    StackVaultScene();
    void Enter() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() const override;
    SceneId NextScene() const override { return nextScene; }
    void SetNextScene(SceneId id) override { nextScene = id; }

private:
    Difficulty difficulty;
    StackChamber* chamber;
    SceneId nextScene;
    int selectedStack;
    std::string feedbackMessage;
    float feedbackTimer;

    void setFeedback(const std::string& msg, bool isError = true);
    void resetLevel();
};
