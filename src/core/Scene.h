#pragma once

enum class SceneId
{
    HUB,
    STACK_VAULT,
    FORGOTTEN_QUEUE,
    SHIFTING_MAZE,
    BRIDGE_BUILDER,
    VICTORY,
    QUIT
};

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void Enter() = 0;            // reset / prepare state when scene opens
    virtual void Update(float dt) = 0;   // handle input + puzzle logic
    virtual void Draw() = 0;             // render visuals (no game logic)
    virtual bool IsComplete() const = 0; // true when the player has won
    virtual void Exit() {}               // cleanup
    virtual SceneId NextScene() const { return SceneId::HUB; }
    virtual void SetNextScene(SceneId id) {}
    virtual void MarkChamberCompleted(int index) {}
};
