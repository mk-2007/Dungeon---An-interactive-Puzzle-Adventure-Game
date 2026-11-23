#pragma once
#include "../core/Scene.h"
#include "../core/UI.h"
#include "BridgeBuilder.h"
#include <string>
#include <vector>

class BridgeBuilderScene : public Scene
{
public:
    BridgeBuilderScene();
    void Enter() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() const override;
    SceneId NextScene() const override { return nextScene; }
    void SetNextScene(SceneId id) override { nextScene = id; }

private:
    struct SiteDef
    {
        int a;
        int b;
        int sw;
    };

    bool isHard;
    BridgeBuilder game;
    SceneId nextScene;
    std::string feedbackMessage;
    float feedbackTimer;
    std::vector<SiteDef> currentSites;
    int switchCount;

    void resetLevel();
    void setFeedback(const std::string& msg, bool isError = true);
    Vector2 getIslandPos(int id) const;
};
