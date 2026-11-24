#include "HubScene.h"
#include <iostream>

HubScene::HubScene()
    : practiceMode(false),
      nextScene(SceneId::HUB),
      toastTimer(0.0f)
{
}

void HubScene::Enter()
{
    nextScene = SceneId::HUB;
    toastTimer = 0.0f;
    toastMessage = "";
}

void HubScene::Update(float dt)
{
    if (toastTimer > 0.0f)
    {
        toastTimer -= dt;
    }
}

void HubScene::triggerToast(const std::string& msg)
{
    toastMessage = msg;
    toastTimer = 2.5f;
}

void HubScene::Draw()
{
    ClearBackground(UI::BG_DARK);

    // Header
    UI::Header("DUNGEON: PUZZLE-ADVENTURE", "Data Structure Chambers · FAST-NUCES");

    // Mode Toggle Bar
    Rectangle modeBar = { 300, 95, 400, 38 };
    UI::Panel(modeBar, UI::PANEL_BG, UI::PANEL_BORDER, 0.3f);

    Rectangle storyBtn = { 305, 99, 190, 30 };
    Rectangle practiceBtn = { 505, 99, 190, 30 };

    Color activeColor = UI::ACCENT_GOLD;
    Color inactiveColor = UI::PANEL_BG;

    if (UI::Button(storyBtn, "Story Mode", 16, !practiceMode ? activeColor : inactiveColor, UI::ACCENT_CYAN, !practiceMode ? UI::BG_DARK : UI::TEXT_LIGHT))
    {
        practiceMode = false;
    }
    if (UI::Button(practiceBtn, "Practice Mode", 16, practiceMode ? activeColor : inactiveColor, UI::ACCENT_CYAN, practiceMode ? UI::BG_DARK : UI::TEXT_LIGHT))
    {
        practiceMode = true;
    }

    // 4 Chamber Door Cards
    struct ChamberCardInfo {
        const char* title;
        const char* ds;
        const char* desc;
        SceneId scene;
    };

    ChamberCardInfo cards[4] = {
        { "Stack Vault", "STACK (LIFO)", "Retrieve the hidden key using alternating moves.", SceneId::STACK_VAULT },
        { "Forgotten Queue", "QUEUE (FIFO)", "Align spirits on the conveyor satisfying desire rules.", SceneId::FORGOTTEN_QUEUE },
        { "The Shifting Maze", "BINARY SEARCH TREE", "Navigate rooms by value and repair the broken node.", SceneId::SHIFTING_MAZE },
        { "The Bridge Builder", "GRAPH (BFS)", "Build bridges between islands to find a route to the goal.", SceneId::BRIDGE_BUILDER }
    };

    float cardWidth = 210;
    float cardHeight = 340;
    float startX = (UI::VIRTUAL_WIDTH - (4 * cardWidth + 3 * 20)) / 2.0f;
    float cardY = 160;

    for (int i = 0; i < 4; i++)
    {
        Rectangle cardRect = { startX + i * (cardWidth + 20), cardY, cardWidth, cardHeight };
        bool isUnlocked = practiceMode || progress.isUnlocked(i);
        bool isCompleted = progress.isCompleted(i);

        Color cardBorder = isCompleted ? UI::ACCENT_GREEN : (isUnlocked ? UI::ACCENT_CYAN : UI::PANEL_BORDER);
        UI::Panel(cardRect, UI::PANEL_BG, cardBorder, 0.08f);

        // Door visual arch / icon
        Rectangle archRect = { cardRect.x + 20, cardRect.y + 20, cardRect.width - 40, 110 };
        Color archBg = isCompleted ? (Color){ 20, 60, 45, 255 } : (isUnlocked ? (Color){ 25, 45, 70, 255 } : (Color){ 20, 22, 30, 255 });
        DrawRectangleRounded(archRect, 0.3f, 4, archBg);
        DrawRectangleRoundedLines(archRect, 0.3f, 4, 1.5f, cardBorder);

        // Door number
        std::string doorNum = "DOOR " + std::to_string(i + 1);
        int numW = UI::MeasureText(doorNum.c_str(), 18);
        UI::Text(doorNum.c_str(), (int)(archRect.x + (archRect.width - numW) / 2), (int)(archRect.y + 40), 18, isUnlocked ? UI::ACCENT_GOLD : UI::TEXT_MUTED);

        // Status Badge
        Rectangle badgeRect = { cardRect.x + 25, cardRect.y + 140, cardRect.width - 50, 24 };
        if (isCompleted)
        {
            UI::Badge(badgeRect, "COMPLETED", UI::ACCENT_GREEN, UI::BG_DARK, 13);
        }
        else if (isUnlocked)
        {
            UI::Badge(badgeRect, "OPEN", UI::ACCENT_CYAN, UI::BG_DARK, 13);
        }
        else
        {
            UI::Badge(badgeRect, "LOCKED", UI::PANEL_BORDER, UI::TEXT_MUTED, 13);
        }

        // Title and Data Structure
        int titleW = UI::MeasureText(cards[i].title, 16);
        UI::Text(cards[i].title, (int)(cardRect.x + (cardRect.width - titleW) / 2), (int)(cardRect.y + 175), 16, UI::TEXT_LIGHT);

        int dsW = UI::MeasureText(cards[i].ds, 12);
        UI::Text(cards[i].ds, (int)(cardRect.x + (cardRect.width - dsW) / 2), (int)(cardRect.y + 200), 12, UI::ACCENT_GOLD);

        // Description text
        UI::Text(cards[i].desc, (int)(cardRect.x + 15), (int)(cardRect.y + 225), 11, UI::TEXT_MUTED);

        // Enter Button
        Rectangle enterBtn = { cardRect.x + 20, cardRect.y + 285, cardRect.width - 40, 36 };
        if (isUnlocked)
        {
            if (UI::Button(enterBtn, "Enter Chamber", 14, UI::PANEL_BG, UI::ACCENT_CYAN, UI::TEXT_LIGHT))
            {
                nextScene = cards[i].scene;
            }
        }
        else
        {
            UI::Button(enterBtn, "Locked", 14, (Color){ 25, 28, 38, 255 }, (Color){ 25, 28, 38, 255 }, UI::TEXT_MUTED);
            Vector2 mouse = UI::GetVirtualMouse();
            if (CheckCollisionPointRec(mouse, enterBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                triggerToast("Complete previous chambers in Story Mode to unlock!");
            }
        }
    }

    // Bottom Bar / Win Banner
    if (progress.allCompleted())
    {
        Rectangle winBar = { 150, 520, 700, 75 };
        UI::Panel(winBar, (Color){ 20, 50, 35, 240 }, UI::ACCENT_GREEN, 0.2f);
        const char* winText = "*** ALL CHAMBERS SOLVED! YOU ESCAPED THE DUNGEON! ***";
        int winW = UI::MeasureText(winText, 20);
        UI::Text(winText, (UI::VIRTUAL_WIDTH - winW) / 2.0f, 535, 20, UI::ACCENT_GOLD);
        const char* subText = "You have proven mastery of Stack, Queue, Binary Search Tree, and Graph.";
        int subW = UI::MeasureText(subText, 14);
        UI::Text(subText, (UI::VIRTUAL_WIDTH - subW) / 2.0f, 565, 14, UI::TEXT_LIGHT);
    }
    else
    {
        std::string progStr = "Dungeon Progress: " + std::to_string(progress.completedCount()) + " / 4 Chambers Cleared";
        int progW = UI::MeasureText(progStr.c_str(), 16);
        UI::Text(progStr.c_str(), (UI::VIRTUAL_WIDTH - progW) / 2.0f, 545, 16, UI::ACCENT_GOLD);
    }

    // Toast popup
    if (toastTimer > 0.0f)
    {
        int msgW = UI::MeasureText(toastMessage.c_str(), 16);
        Rectangle toastRect = { (UI::VIRTUAL_WIDTH - msgW - 40) / 2.0f, 600, (float)(msgW + 40), 32 };
        DrawRectangleRounded(toastRect, 0.4f, 4, (Color){ 220, 50, 50, 230 });
        UI::Text(toastMessage.c_str(), (int)(toastRect.x + 20), (int)(toastRect.y + 8), 16, UI::TEXT_LIGHT);
    }
}

bool HubScene::IsComplete() const
{
    return progress.allCompleted();
}

Scene* createHubScene()
{
    return new HubScene();
}
