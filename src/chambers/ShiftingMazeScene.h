#pragma once
#include "../core/Scene.h"
#include "../core/UI.h"
#include "ShiftingMaze.h"
#include <string>
#include <map>

class ShiftingMazeScene : public Scene
{
public:
    ShiftingMazeScene();
    void Enter() override;
    void Update(float dt) override;
    void Draw() override;
    bool IsComplete() const override;
    SceneId NextScene() const override { return nextScene; }
    void SetNextScene(SceneId id) override { nextScene = id; }

private:
    bool isHard;
    ShiftingMaze maze;
    SceneId nextScene;
    std::string feedbackMessage;
    float feedbackTimer;

    void resetLevel();
    void setFeedback(const std::string& msg, bool isError = true);

    void drawTree(const BST::Node* node, float x, float y, float dx, float dy);
};
