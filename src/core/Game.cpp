#include "Game.h"
#include "SceneFactories.h"
#include "RaylibCompat.h"
#include "UI.h"

Game::Game()
    : activeScene(nullptr),
      activeSceneId(SceneId::HUB)
{
    scenes[0] = createHubScene();
    scenes[1] = createStackVaultScene();
    scenes[2] = createForgottenQueueScene();
    scenes[3] = createShiftingMazeScene();
    scenes[4] = createBridgeBuilderScene();

    activeScene = scenes[0];
}

Game::~Game()
{
    for (int i = 0; i < 5; i++)
    {
        delete scenes[i];
    }
}

void Game::changeScene(SceneId newId)
{
    if (newId == activeSceneId) return;

    if (activeScene != nullptr)
    {
        // Check if exiting a completed chamber
        if (activeSceneId == SceneId::STACK_VAULT && scenes[1]->IsComplete())
        {
            scenes[0]->MarkChamberCompleted(0);
        }
        else if (activeSceneId == SceneId::FORGOTTEN_QUEUE && scenes[2]->IsComplete())
        {
            scenes[0]->MarkChamberCompleted(1);
        }
        else if (activeSceneId == SceneId::SHIFTING_MAZE && scenes[3]->IsComplete())
        {
            scenes[0]->MarkChamberCompleted(2);
        }
        else if (activeSceneId == SceneId::BRIDGE_BUILDER && scenes[4]->IsComplete())
        {
            scenes[0]->MarkChamberCompleted(3);
        }

        activeScene->Exit();
    }

    int sceneIndex = 0;
    switch (newId)
    {
    case SceneId::HUB:
        sceneIndex = 0;
        break;
    case SceneId::STACK_VAULT:
        sceneIndex = 1;
        break;
    case SceneId::FORGOTTEN_QUEUE:
        sceneIndex = 2;
        break;
    case SceneId::SHIFTING_MAZE:
        sceneIndex = 3;
        break;
    case SceneId::BRIDGE_BUILDER:
        sceneIndex = 4;
        break;
    default:
        sceneIndex = 0;
        newId = SceneId::HUB;
        break;
    }

    activeSceneId = newId;
    activeScene = scenes[sceneIndex];
    activeScene->SetNextScene(newId);
    activeScene->Enter();
}

void Game::run(SceneId startScene)
{
    const int baseWidth = 1000;
    const int baseHeight = 650;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(baseWidth, baseHeight, "Dungeon: A Data Structure Adventure");
    SetWindowMinSize(800, 520);
    SetTargetFPS(60);

    UI::Init();

    if (startScene != SceneId::HUB)
    {
        changeScene(startScene);
    }
    else
    {
        activeScene->Enter();
    }

    while (!WindowShouldClose())
    {
        // Fullscreen toggle: F11 or Alt+Enter
        if (IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)))
        {
            ToggleFullscreen();
        }

        // F12 Screenshot
        if (IsKeyPressed(KEY_F12))
        {
            TakeScreenshot("screenshot.png");
        }

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        float scale = fminf((float)screenW / (float)baseWidth, (float)screenH / (float)baseHeight);
        float offsetX = (screenW - (baseWidth * scale)) * 0.5f;
        float offsetY = (screenH - (baseHeight * scale)) * 0.5f;

        Camera2D camera = { 0 };
        camera.offset = { offsetX, offsetY };
        camera.target = { 0.0f, 0.0f };
        camera.rotation = 0.0f;
        camera.zoom = scale;

        UI::UpdateMouse(camera);

        float dt = GetFrameTime();

        // Update
        activeScene->Update(dt);

        // Check Scene Transition
        SceneId next = activeScene->NextScene();
        if (next != activeSceneId)
        {
            changeScene(next);
        }

        // Draw Directly to Window Framebuffer (No RenderTexture Blur)
        BeginDrawing();
        ClearBackground(UI::BG_DARK);

        BeginMode2D(camera);
        activeScene->Draw();
        EndMode2D();

        EndDrawing();
    }

    UI::Cleanup();
    CloseWindow();
}
