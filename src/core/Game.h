#pragma once
#include "Scene.h"

class Game
{
public:
    Game();
    ~Game();

    void run(SceneId startScene = SceneId::HUB);

private:
    Scene* scenes[5];
    Scene* activeScene;
    SceneId activeSceneId;

    void changeScene(SceneId newId);
};
