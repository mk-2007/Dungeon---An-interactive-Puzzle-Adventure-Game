#pragma once
#include "Scene.h"
#include "GameProgress.h"
#include "UI.h"

class HubScene : public Scene
{
public:
    HubScene();
    void Enter() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() const override;
    SceneId NextScene() const override { return nextScene; }
    void SetNextScene(SceneId id) override { nextScene = id; }
    void MarkChamberCompleted(int index) override { progress.completeChamber(index); }

    GameProgress& getProgress() { return progress; }
    bool isPracticeMode() const { return practiceMode; }
    void setPracticeMode(bool practice) { practiceMode = practice; }

private:
    GameProgress progress;
    bool practiceMode;
    SceneId nextScene;
    std::string toastMessage;
    float toastTimer;

    void triggerToast(const std::string& msg);
};
