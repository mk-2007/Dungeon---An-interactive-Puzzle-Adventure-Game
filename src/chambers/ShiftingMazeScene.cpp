#include "ShiftingMazeScene.h"
#include <iostream>

namespace
{
    const int easyVals[] = { 50, 30, 70, 20, 40, 60, 80 };
    const int hardVals[] = { 50, 30, 70, 20, 40, 60, 80, 10, 45, 65, 90 };
}

ShiftingMazeScene::ShiftingMazeScene()
    : isHard(false),
      nextScene(SceneId::SHIFTING_MAZE),
      feedbackTimer(0.0f)
{
    resetLevel();
}

void ShiftingMazeScene::Enter()
{
    nextScene = SceneId::SHIFTING_MAZE;
    resetLevel();
}

void ShiftingMazeScene::resetLevel()
{
    if (!isHard)
    {
        maze.startLevel(easyVals, 7, 60, 30, 35);
    }
    else
    {
        maze.startLevel(hardVals, 11, 45, 70, 75);
    }
    feedbackTimer = 0.0f;
    feedbackMessage = "";
}

void ShiftingMazeScene::setFeedback(const std::string& msg, bool isError)
{
    feedbackMessage = msg;
    feedbackTimer = 3.0f;
}

void ShiftingMazeScene::Update(float dt)
{
    if (feedbackTimer > 0.0f)
    {
        feedbackTimer -= dt;
    }
}

void ShiftingMazeScene::drawTree(const BST::Node* node, float x, float y, float dx, float dy)
{
    if (node == nullptr) return;

    int targetKey = !isHard ? 60 : 45;
    int brokenValue = !isHard ? 30 : 70;
    int fixedValue = !isHard ? 35 : 75;

    // Draw branch lines to children first
    if (node->left != nullptr)
    {
        float childX = x - dx;
        float childY = y + dy;
        DrawLineEx({ x, y }, { childX, childY }, 2.0f, UI::PANEL_BORDER);
        drawTree(node->left, childX, childY, dx * 0.52f, dy);
    }

    if (node->right != nullptr)
    {
        float childX = x + dx;
        float childY = y + dy;
        DrawLineEx({ x, y }, { childX, childY }, 2.0f, UI::PANEL_BORDER);
        drawTree(node->right, childX, childY, dx * 0.52f, dy);
    }

    // Determine node attributes
    bool isCurrent = (maze.currentNode() != nullptr && maze.currentNode()->data == node->data);
    bool isTarget = (node->data == targetKey);
    bool isBroken = (node->data == brokenValue && !maze.hasRepairedNode());
    bool isRepaired = (node->data == fixedValue && maze.hasRepairedNode());

    float radius = isHard ? 22.0f : 26.0f;

    Color nodeBg = (Color){ 24, 30, 46, 255 };
    Color borderCol = UI::PANEL_BORDER;
    float borderThick = 2.0f;

    if (isCurrent)
    {
        nodeBg = (Color){ 30, 70, 110, 255 };
        borderCol = UI::ACCENT_CYAN;
        borderThick = 3.5f;
    }
    else if (isTarget)
    {
        nodeBg = (Color){ 70, 55, 20, 255 };
        borderCol = UI::ACCENT_GOLD;
        borderThick = 2.5f;
    }
    else if (isBroken)
    {
        nodeBg = (Color){ 80, 25, 25, 255 };
        borderCol = UI::ACCENT_RED;
        borderThick = 2.5f;
    }
    else if (isRepaired)
    {
        nodeBg = (Color){ 20, 70, 45, 255 };
        borderCol = UI::ACCENT_GREEN;
        borderThick = 2.5f;
    }

    // Outer glow for current room
    if (isCurrent)
    {
        DrawCircleLines((int)x, (int)y, radius + 5, UI::ACCENT_CYAN);
    }

    DrawCircle((int)x, (int)y, radius, nodeBg);
    DrawCircleLines((int)x, (int)y, radius, borderCol);

    // Node Room Number
    std::string valStr = std::to_string(node->data);
    int fontSize = isHard ? 14 : 16;
    int tw = UI::MeasureText(valStr.c_str(), fontSize);
    UI::Text(valStr.c_str(), (int)(x - tw / 2), (int)(y - fontSize / 2), fontSize, UI::TEXT_LIGHT);

    // Tag below node
    if (isCurrent)
    {
        UI::Text("[YOU]", (int)(x - 16), (int)(y + radius + 3), 10, UI::ACCENT_CYAN);
    }
    else if (isTarget)
    {
        UI::Text("[KEY]", (int)(x - 16), (int)(y + radius + 3), 10, UI::ACCENT_GOLD);
    }
    else if (isBroken)
    {
        UI::Text("[CORRUPT]", (int)(x - 26), (int)(y + radius + 3), 9, UI::ACCENT_RED);
    }
}

void ShiftingMazeScene::Draw()
{
    ClearBackground(UI::BG_DARK);

    int targetKey = !isHard ? 60 : 45;
    int brokenValue = !isHard ? 30 : 70;
    int fixedValue = !isHard ? 35 : 75;

    // Header
    std::string sub = std::string("Chamber 3 · ") + (isHard ? "HARD" : "EASY") + " MODE";
    UI::Header("THE SHIFTING MAZE", sub.c_str());

    // Navigation and Control buttons
    if (UI::Button({ 40, 25, 120, 36 }, "< Hub", 16, UI::PANEL_BG, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
    {
        nextScene = SceneId::HUB;
    }

    if (UI::Button({ 840, 25, 120, 36 }, "Reset", 16, UI::PANEL_BG, UI::ACCENT_RED, UI::TEXT_LIGHT))
    {
        resetLevel();
        setFeedback("Maze reset to entrance.", false);
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

    // Objectives HUD Bar
    Rectangle hudBar = { 60, 135, 880, 48 };
    UI::Panel(hudBar, UI::PANEL_BG, UI::PANEL_BORDER, 0.15f);

    bool targetReached = (maze.currentNode() != nullptr && maze.currentNode()->data == targetKey);

    // Objective 1: Target Key
    std::string obj1 = "Objective 1: Find Key in Room " + std::to_string(targetKey) + " " + (targetReached ? "[FOUND]" : "[SEARCHING]");
    Color obj1Col = targetReached ? UI::ACCENT_GREEN : UI::ACCENT_GOLD;
    UI::Text(obj1.c_str(), 80, 145, 13, obj1Col);

    // Objective 2: Repair Misplaced Node
    std::string obj2 = "Objective 2: Repair Room " + std::to_string(brokenValue) + " -> " + std::to_string(fixedValue) + " " + (maze.hasRepairedNode() ? "[REPAIRED]" : "[PENDING]");
    Color obj2Col = maze.hasRepairedNode() ? UI::ACCENT_GREEN : UI::ACCENT_CYAN;
    UI::Text(obj2.c_str(), 80, 163, 13, obj2Col);

    // Mistakes Counter
    int remaining = 3 - maze.incorrectMoveCount();
    std::string mistStr = "Mistakes: " + std::to_string(maze.incorrectMoveCount()) + " / 3";
    Color mistCol = remaining == 3 ? UI::ACCENT_GREEN : (remaining == 2 ? UI::ACCENT_GOLD : UI::ACCENT_RED);
    UI::Text(mistStr.c_str(), 760, 153, 14, mistCol);

    // Draw the Interactive Binary Search Tree
    float rootX = 500.0f;
    float rootY = isHard ? 210.0f : 225.0f;
    float initDx = isHard ? 190.0f : 200.0f;
    float initDy = isHard ? 65.0f : 75.0f;

    drawTree(maze.getTree().root(), rootX, rootY, initDx, initDy);

    // Bottom Navigation Controls
    const BST::Node* cur = maze.currentNode();
    float navY = 525.0f;

    if (cur != nullptr && !maze.isComplete())
    {
        // Left Door Button
        std::string leftLabel = cur->left != nullptr ? ("<- Left: Room " + std::to_string(cur->left->data)) : "<- Left Door: [WALL]";
        Rectangle leftBtn = { 120, navY, 220, 42 };

        if (cur->left != nullptr)
        {
            if (UI::Button(leftBtn, leftLabel.c_str(), 14, UI::PANEL_BG, UI::ACCENT_CYAN, UI::TEXT_LIGHT))
            {
                if (maze.chooseDirection(ShiftingMaze::Direction::left))
                {
                    setFeedback("Advanced through Left door towards key.", false);
                }
                else
                {
                    setFeedback("Wrong way! Target key is not down that branch.");
                }
            }
        }
        else
        {
            UI::Button(leftBtn, leftLabel.c_str(), 14, (Color){ 25, 28, 38, 255 }, (Color){ 25, 28, 38, 255 }, (Color){ 70, 75, 90, 255 });
        }

        // Current Room Center Card
        Rectangle curCard = { 380, navY - 8, 240, 58 };
        UI::Panel(curCard, (Color){ 20, 26, 40, 255 }, UI::ACCENT_CYAN, 0.2f);
        std::string curText = "Current: Room " + std::to_string(cur->data);
        int ctw = UI::MeasureText(curText.c_str(), 16);
        UI::Text(curText.c_str(), (int)(curCard.x + (curCard.width - ctw) / 2), (int)(curCard.y + 10), 16, UI::ACCENT_GOLD);

        std::string hintText = cur->data > targetKey ? "Target is smaller -> Go Left" : (cur->data < targetKey ? "Target is larger -> Go Right" : "Key Found in this Room!");
        int htw = UI::MeasureText(hintText.c_str(), 12);
        UI::Text(hintText.c_str(), (int)(curCard.x + (curCard.width - htw) / 2), (int)(curCard.y + 34), 12, UI::TEXT_LIGHT);

        // Right Door Button
        std::string rightLabel = cur->right != nullptr ? ("Right: Room " + std::to_string(cur->right->data) + " ->") : "[WALL] :Right Door ->";
        Rectangle rightBtn = { 660, navY, 220, 42 };

        if (cur->right != nullptr)
        {
            if (UI::Button(rightBtn, rightLabel.c_str(), 14, UI::PANEL_BG, UI::ACCENT_CYAN, UI::TEXT_LIGHT))
            {
                if (maze.chooseDirection(ShiftingMaze::Direction::right))
                {
                    setFeedback("Advanced through Right door towards key.", false);
                }
                else
                {
                    setFeedback("Wrong way! Target key is not down that branch.");
                }
            }
        }
        else
        {
            UI::Button(rightBtn, rightLabel.c_str(), 14, (Color){ 25, 28, 38, 255 }, (Color){ 25, 28, 38, 255 }, (Color){ 70, 75, 90, 255 });
        }

        // Repair Room Action Button
        if (!maze.hasRepairedNode())
        {
            Rectangle repairBtn = { 320, navY + 60, 360, 34 };
            std::string repairPrompt = "Repair Room " + std::to_string(brokenValue) + " -> " + std::to_string(fixedValue) + " (Calls BST Remove & Insert)";
            if (UI::Button(repairBtn, repairPrompt.c_str(), 12, (Color){ 60, 30, 30, 255 }, UI::ACCENT_RED, UI::TEXT_LIGHT))
            {
                if (maze.repairMisplacedNode(brokenValue, fixedValue))
                {
                    setFeedback("BST repaired! Live tree structure updated.", false);
                }
                else
                {
                    setFeedback("Failed to repair room!");
                }
            }
        }
    }

    // Win / Loss Overlay
    if (maze.status() == ShiftingMaze::Status::won)
    {
        Rectangle winOverlay = { 200, 210, 600, 180 };
        DrawRectangleRounded(winOverlay, 0.15f, 4, (Color){ 12, 14, 20, 245 });
        DrawRectangleRoundedLines(winOverlay, 0.15f, 4, 2.5f, UI::ACCENT_GOLD);

        const char* winMsg = "MAZE CONQUERED! CHAMBER SOLVED!";
        int wm = UI::MeasureText(winMsg, 24);
        UI::Text(winMsg, (UI::VIRTUAL_WIDTH - wm) / 2.0f, 240, 24, UI::ACCENT_GOLD);

        const char* descMsg = "You located the Key and repaired the corrupted BST node.";
        int dm = UI::MeasureText(descMsg, 16);
        UI::Text(descMsg, (UI::VIRTUAL_WIDTH - dm) / 2.0f, 280, 16, UI::TEXT_LIGHT);

        if (UI::Button({ 410, 320, 180, 42 }, "Proceed to Hub", 16, UI::ACCENT_GREEN, UI::ACCENT_CYAN, UI::BG_DARK))
        {
            nextScene = SceneId::HUB;
        }
    }
    else if (maze.status() == ShiftingMaze::Status::lost)
    {
        Rectangle loseOverlay = { 200, 210, 600, 180 };
        DrawRectangleRounded(loseOverlay, 0.15f, 4, (Color){ 12, 14, 20, 245 });
        DrawRectangleRoundedLines(loseOverlay, 0.15f, 4, 2.5f, UI::ACCENT_RED);

        const char* loseMsg = "LOST IN THE SHIFTING MAZE!";
        int lm = UI::MeasureText(loseMsg, 24);
        UI::Text(loseMsg, (UI::VIRTUAL_WIDTH - lm) / 2.0f, 240, 24, UI::ACCENT_RED);

        const char* descMsg = "Three wrong decisions were made. The doors slam shut!";
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

bool ShiftingMazeScene::IsComplete() const
{
    return maze.status() == ShiftingMaze::Status::won;
}

Scene* createShiftingMazeScene()
{
    return new ShiftingMazeScene();
}
