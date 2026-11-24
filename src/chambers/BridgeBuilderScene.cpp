#include "BridgeBuilderScene.h"

BridgeBuilderScene::BridgeBuilderScene()
    : isHard(false),
      nextScene(SceneId::BRIDGE_BUILDER),
      feedbackTimer(0.0f),
      switchCount(0)
{
    resetLevel();
}

void BridgeBuilderScene::Enter()
{
    nextScene = SceneId::BRIDGE_BUILDER;
    resetLevel();
}

void BridgeBuilderScene::resetLevel()
{
    currentSites.clear();
    if (!isHard)
    {
        switchCount = 1;
        game.startLevel(5, 0, 4, 6, BridgeBuilder::Difficulty::easy);
        game.addSwitch(); // switch 0

        currentSites = {
            { 0, 1, -1 },
            { 1, 2, -1 },
            { 2, 4,  0 }, // locked by switch 0
            { 1, 3, -1 },
            { 3, 4, -1 }
        };
    }
    else
    {
        switchCount = 2;
        game.startLevel(7, 0, 6, 5, BridgeBuilder::Difficulty::hard);
        game.addSwitch(); // switch 0
        game.addSwitch(); // switch 1

        currentSites = {
            { 0, 1, -1 },
            { 1, 2, -1 },
            { 2, 5, -1 },
            { 5, 6,  0 }, // locked by switch 0
            { 0, 3, -1 },
            { 3, 4, -1 },
            { 4, 6,  1 }, // locked by switch 1
            { 1, 3, -1 }
        };
    }

    for (size_t i = 0; i < currentSites.size(); i++)
    {
        game.addBridgeSite(currentSites[i].a, currentSites[i].b, currentSites[i].sw);
    }

    feedbackTimer = 0.0f;
    feedbackMessage = "";
}

void BridgeBuilderScene::setFeedback(const std::string& msg, bool isError)
{
    feedbackMessage = msg;
    feedbackTimer = 3.0f;
}

Vector2 BridgeBuilderScene::getIslandPos(int id) const
{
    if (!isHard)
    {
        switch (id)
        {
        case 0: return { 170, 340 }; // Start
        case 1: return { 360, 240 };
        case 3: return { 360, 440 };
        case 2: return { 580, 240 };
        case 4: return { 770, 340 }; // Goal
        default: return { 500, 340 };
        }
    }
    else
    {
        switch (id)
        {
        case 0: return { 140, 340 }; // Start
        case 1: return { 290, 230 };
        case 3: return { 290, 450 };
        case 2: return { 460, 230 };
        case 4: return { 460, 450 };
        case 5: return { 630, 230 };
        case 6: return { 790, 340 }; // Goal
        default: return { 500, 340 };
        }
    }
}

void BridgeBuilderScene::Update(float dt)
{
    if (feedbackTimer > 0.0f)
    {
        feedbackTimer -= dt;
    }
}

void BridgeBuilderScene::Draw()
{
    ClearBackground(UI::BG_DARK);

    // Header
    std::string sub = std::string("Chamber 4 · ") + (isHard ? "HARD" : "EASY") + " MODE";
    UI::Header("THE BRIDGE BUILDER", sub.c_str());

    // Navigation and Control buttons
    if (UI::Button({ 40, 25, 120, 36 }, "< Hub", 16, UI::PANEL_BG, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
    {
        nextScene = SceneId::HUB;
    }

    if (UI::Button({ 840, 25, 120, 36 }, "Reset", 16, UI::PANEL_BG, UI::ACCENT_RED, UI::TEXT_LIGHT))
    {
        resetLevel();
        setFeedback("Bridge network reset.", false);
    }

    // Difficulty switch buttons
    Rectangle diffEasy = { 400, 95, 95, 28 };
    Rectangle diffHard = { 505, 95, 95, 28 };
    if (UI::Button(diffEasy, "Easy", 14, !isHard ? UI::ACCENT_CYAN : UI::PANEL_BG, UI::ACCENT_CYAN, !isHard ? UI::BG_DARK : UI::TEXT_LIGHT))
    {
        if (isHard)
        {
            isHard = false;
            resetLevel();
        }
    }
    if (UI::Button(diffHard, "Hard", 14, isHard ? UI::ACCENT_CYAN : UI::PANEL_BG, UI::ACCENT_CYAN, isHard ? UI::BG_DARK : UI::TEXT_LIGHT))
    {
        if (!isHard)
        {
            isHard = true;
            resetLevel();
        }
    }

    // HUD Bar
    Rectangle hudBar = { 60, 135, 880, 48 };
    UI::Panel(hudBar, UI::PANEL_BG, UI::PANEL_BORDER, 0.15f);

    // Objective text
    std::string objText = "Objective: Connect Start (0) to Goal (" + std::to_string(game.goalIslandId()) + ") via Graph BFS";
    UI::Text(objText.c_str(), 75, 151, 14, UI::TEXT_LIGHT);

    // Moves Counter
    int left = game.movesLeft();
    std::string movesText = "Moves Left: " + std::to_string(left) + " / " + std::to_string(game.maxMoveCount());
    Color movesCol = left > 2 ? UI::ACCENT_GREEN : (left > 1 ? UI::ACCENT_GOLD : UI::ACCENT_RED);
    UI::Text(movesText.c_str(), 520, 151, 14, movesCol);

    // Route Status
    bool routeFound = game.hasRoute();
    std::string routeText = routeFound ? "Route: CONNECTED!" : "Route: Disconnected";
    Color routeCol = routeFound ? UI::ACCENT_GREEN : UI::TEXT_MUTED;
    UI::Text(routeText.c_str(), 730, 151, 14, routeCol);

    // Render Bridge Sites (lines between island centers)
    for (size_t i = 0; i < currentSites.size(); i++)
    {
        int a = currentSites[i].a;
        int b = currentSites[i].b;
        int sw = currentSites[i].sw;

        bool built = game.isBridgeBuilt(a, b);
        bool blocked = game.isBridgeBlocked(a, b);

        Vector2 pA = getIslandPos(a);
        Vector2 pB = getIslandPos(b);

        Vector2 mid = { (pA.x + pB.x) / 2.0f, (pA.y + pB.y) / 2.0f };
        Rectangle toggleBtn = { mid.x - 28, mid.y - 14, 56, 28 };

        if (built)
        {
            // Built bridge: wide solid bridge line
            Color bCol = routeFound ? UI::ACCENT_GREEN : UI::ACCENT_CYAN;
            DrawLineEx(pA, pB, 6.0f, bCol);

            // Dismantle button
            if (game.status() == BridgeBuilder::Status::playing)
            {
                if (UI::Button(toggleBtn, "Drop", 11, (Color){ 30, 40, 60, 240 }, UI::ACCENT_RED, UI::TEXT_LIGHT))
                {
                    if (game.removeBridge(a, b))
                    {
                        setFeedback("Bridge removed. 1 move spent.", false);
                    }
                    else
                    {
                        setFeedback("Cannot remove bridge!");
                    }
                }
            }
        }
        else if (blocked)
        {
            // Blocked bridge: red line
            DrawLineEx(pA, pB, 2.5f, UI::ACCENT_RED);

            // Padlock badge
            Rectangle lockBadge = { mid.x - 38, mid.y - 12, 76, 24 };
            std::string swLabel = "Sw " + std::to_string(sw);
            UI::Badge(lockBadge, swLabel.c_str(), (Color){ 80, 25, 25, 250 }, UI::ACCENT_RED, 11);
        }
        else
        {
            // Unbuilt bridge site: dashed/faint line
            DrawLineEx(pA, pB, 2.0f, (Color){ 50, 60, 85, 255 });

            // Build button
            if (game.status() == BridgeBuilder::Status::playing)
            {
                if (UI::Button(toggleBtn, "+Build", 11, UI::PANEL_BG, UI::ACCENT_CYAN, UI::TEXT_LIGHT))
                {
                    if (game.buildBridge(a, b))
                    {
                        setFeedback("Bridge constructed! 1 move spent.", false);
                    }
                    else
                    {
                        if (game.lastResult() == BridgeBuilder::Result::bridgeBlocked)
                        {
                            setFeedback("Bridge is blocked! Activate its switch first.");
                        }
                        else if (game.lastResult() == BridgeBuilder::Result::noMovesLeft)
                        {
                            setFeedback("Out of moves!");
                        }
                    }
                }
            }
        }
    }

    // Render Islands (Vertices)
    int islands = game.islandTotal();
    for (int i = 0; i < islands; i++)
    {
        Vector2 pos = getIslandPos(i);
        bool isStart = (i == game.startIslandId());
        bool isGoal = (i == game.goalIslandId());

        float radius = isStart || isGoal ? 32.0f : 26.0f;
        Color islandBg = (Color){ 25, 32, 48, 255 };
        Color islandBorder = UI::PANEL_BORDER;

        if (isStart)
        {
            islandBg = (Color){ 20, 60, 40, 255 };
            islandBorder = UI::ACCENT_GREEN;
        }
        else if (isGoal)
        {
            islandBg = (Color){ 65, 30, 80, 255 };
            islandBorder = UI::ACCENT_PURPLE;
        }

        DrawCircle((int)pos.x, (int)pos.y, radius, islandBg);
        DrawCircleLines((int)pos.x, (int)pos.y, radius, islandBorder);

        std::string num = std::to_string(i);
        int tw = UI::MeasureText(num.c_str(), 18);
        UI::Text(num.c_str(), (int)(pos.x - tw / 2), (int)(pos.y - 9), 18, UI::TEXT_LIGHT);

        if (isStart)
        {
            UI::Text("START", (int)(pos.x - 20), (int)(pos.y + radius + 3), 11, UI::ACCENT_GREEN);
        }
        else if (isGoal)
        {
            UI::Text("GOAL", (int)(pos.x - 17), (int)(pos.y + radius + 3), 11, UI::ACCENT_PURPLE);
        }
    }

    // Remote Switches Panel (Bottom)
    float swPanelY = 535.0f;
    Rectangle swPanel = { 60, swPanelY, 880, 65 };
    UI::Panel(swPanel, UI::PANEL_BG, UI::PANEL_BORDER, 0.15f);

    UI::Text("REMOTE SWITCH CONTROLS (Activate to unlock blocked bridge sites):", 75, (int)swPanelY + 12, 13, UI::ACCENT_CYAN);

    for (int s = 0; s < switchCount; s++)
    {
        bool on = game.isSwitchOn(s);
        Rectangle swBtn = { 75.0f + s * 160.0f, swPanelY + 30.0f, 140.0f, 26.0f };

        std::string sText = "Switch " + std::to_string(s) + ": " + (on ? "ON [Unlocked]" : "OFF [Pull]");
        Color bgCol = on ? (Color){ 20, 65, 45, 255 } : UI::PANEL_BG;
        Color hoverCol = on ? bgCol : UI::ACCENT_GOLD;

        if (game.status() == BridgeBuilder::Status::playing)
        {
            if (UI::Button(swBtn, sText.c_str(), 12, bgCol, hoverCol, on ? UI::ACCENT_GREEN : UI::TEXT_LIGHT))
            {
                if (!on)
                {
                    if (game.activateSwitch(s))
                    {
                        setFeedback("Switch activated! Blocked bridge unlocked.", false);
                    }
                    else if (game.lastResult() == BridgeBuilder::Result::noMovesLeft)
                    {
                        setFeedback("Out of moves!");
                    }
                }
                else
                {
                    setFeedback("Switch is already ON.", false);
                }
            }
        }
        else
        {
            UI::Button(swBtn, sText.c_str(), 12, bgCol, bgCol, UI::TEXT_MUTED);
        }
    }

    // Win / Loss Overlay
    if (game.status() == BridgeBuilder::Status::won)
    {
        Rectangle winOverlay = { 200, 210, 600, 180 };
        DrawRectangleRounded(winOverlay, 0.15f, 4, (Color){ 12, 14, 20, 245 });
        DrawRectangleRoundedLines(winOverlay, 0.15f, 4, 2.5f, UI::ACCENT_GOLD);

        const char* winMsg = "ISLANDS CONNECTED! CHAMBER SOLVED!";
        int wm = UI::MeasureText(winMsg, 24);
        UI::Text(winMsg, (UI::VIRTUAL_WIDTH - wm) / 2.0f, 240, 24, UI::ACCENT_GOLD);

        const char* descMsg = "A continuous bridge route was verified from Start to Goal.";
        int dm = UI::MeasureText(descMsg, 16);
        UI::Text(descMsg, (UI::VIRTUAL_WIDTH - dm) / 2.0f, 280, 16, UI::TEXT_LIGHT);

        if (UI::Button({ 410, 320, 180, 42 }, "Proceed to Hub", 16, UI::ACCENT_GREEN, UI::ACCENT_CYAN, UI::BG_DARK))
        {
            nextScene = SceneId::HUB;
        }
    }
    else if (game.status() == BridgeBuilder::Status::lost)
    {
        Rectangle loseOverlay = { 200, 210, 600, 180 };
        DrawRectangleRounded(loseOverlay, 0.15f, 4, (Color){ 12, 14, 20, 245 });
        DrawRectangleRoundedLines(loseOverlay, 0.15f, 4, 2.5f, UI::ACCENT_RED);

        const char* loseMsg = "OUT OF MOVES!";
        int lm = UI::MeasureText(loseMsg, 24);
        UI::Text(loseMsg, (UI::VIRTUAL_WIDTH - lm) / 2.0f, 240, 24, UI::ACCENT_RED);

        const char* descMsg = "You exhausted all bridge building moves before finding a route.";
        int dm = UI::MeasureText(descMsg, 16);
        UI::Text(descMsg, (UI::VIRTUAL_WIDTH - dm) / 2.0f, 280, 16, UI::TEXT_LIGHT);

        if (UI::Button({ 410, 320, 180, 42 }, "Try Again", 16, UI::ACCENT_RED, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
        {
            resetLevel();
        }
    }
    else if (feedbackTimer > 0.0f)
    {
        int fw = UI::MeasureText(feedbackMessage.c_str(), 14);
        Rectangle toastRect = { (UI::VIRTUAL_WIDTH - fw - 40) / 2.0f, 615, (float)(fw + 40), 28 };
        DrawRectangleRounded(toastRect, 0.3f, 4, (Color){ 20, 24, 36, 230 });
        DrawRectangleRoundedLines(toastRect, 0.3f, 4, 1.0f, UI::ACCENT_CYAN);
        UI::Text(feedbackMessage.c_str(), (int)(toastRect.x + 20), (int)(toastRect.y + 7), 14, UI::TEXT_LIGHT);
    }
}

bool BridgeBuilderScene::IsComplete() const
{
    return game.status() == BridgeBuilder::Status::won;
}

Scene* createBridgeBuilderScene()
{
    return new BridgeBuilderScene();
}
