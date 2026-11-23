#pragma once
#include "../core/Scene.h"
#include "../core/UI.h"
#include "SpiritConveyerBelt.h"
#include <string>
#include <vector>

class ForgottenQueueScene : public Scene
{
public:
    ForgottenQueueScene();
    ~ForgottenQueueScene();
    void Enter() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() const override;
    SceneId NextScene() const override { return nextScene; }
    void SetNextScene(SceneId id) override { nextScene = id; }

private:
    Difficulty difficulty;
    SpiritConveyerBelt* belt;
    SceneId nextScene;
    std::string feedbackMessage;
    float feedbackTimer;

    void resetLevel();
    void setFeedback(const std::string& msg, bool isError = true);
    bool isSpiritOnBelt(const std::string& name) const;
    Color getSpiritColor(const std::string& name) const;
};
