#include "StackVaultScene.h"
#include <cmath>

StackVaultScene::StackVaultScene()
    : difficulty(Difficulty::EASY),
      chamber(nullptr),
      nextScene(SceneId::STACK_VAULT),
      selectedStack(-1),
      feedbackTimer(0.0f)
{
    chamber = new StackChamber(difficulty);
}

void StackVaultScene::Enter()
{
    nextScene = SceneId::STACK_VAULT;
    resetLevel();
}

void StackVaultScene::resetLevel()
{
    delete chamber;
    chamber = new StackChamber(difficulty);
    selectedStack = -1;
    feedbackTimer = 0.0f;
    feedbackMessage = "";
}

void StackVaultScene::setFeedback(const std::string& msg, bool isError)
{
    feedbackMessage = msg;
    feedbackTimer = 3.0f;
}

void StackVaultScene::Update(float dt)
{
    if (feedbackTimer > 0.0f)
    {
        feedbackTimer -= dt;
    }
}

void StackVaultScene::Draw()
{
    ClearBackground(UI::BG_DARK);

    // Header
    std::string sub = std::string("Chamber 1 · ") + (difficulty == Difficulty::EASY ? "EASY" : "HARD") + " MODE";
    UI::Header("STACK VAULT", sub.c_str());

    // Navigation and Control buttons
    if (UI::Button({ 40, 25, 120, 36 }, "< Hub", 16, UI::PANEL_BG, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
    {
        nextScene = SceneId::HUB;
    }

    if (UI::Button({ 840, 25, 120, 36 }, "Reset", 16, UI::PANEL_BG, UI::ACCENT_RED, UI::TEXT_LIGHT))
    {
        resetLevel();
        setFeedback("Vault reset to initial state.", false);
    }

    // Difficulty switch buttons
    Rectangle diffEasy = { 400, 95, 95, 28 };
    Rectangle diffHard = { 505, 95, 95, 28 };
    if (UI::Button(diffEasy, "Easy", 14, difficulty == Difficulty::EASY ? UI::ACCENT_CYAN : UI::PANEL_BG, UI::ACCENT_CYAN, difficulty == Difficulty::EASY ? UI::BG_DARK : UI::TEXT_LIGHT))
    {
        if (difficulty != Difficulty::EASY)
        {
            difficulty = Difficulty::EASY;
            resetLevel();
        }
    }
    if (UI::Button(diffHard, "Hard", 14, difficulty == Difficulty::HARD ? UI::ACCENT_CYAN : UI::PANEL_BG, UI::ACCENT_CYAN, difficulty == Difficulty::HARD ? UI::BG_DARK : UI::TEXT_LIGHT))
    {
        if (difficulty != Difficulty::HARD)
        {
            difficulty = Difficulty::HARD;
            resetLevel();
        }
    }

    // Rules summary card
    Rectangle rulesCard = { 80, 135, 840, 48 };
    UI::Panel(rulesCard, UI::PANEL_BG, UI::PANEL_BORDER, 0.2f);
    UI::Text("RULES: Reveal the Key [1000]! Push/Pop only topmost elements. To Helper: signs must alternate (+/-).", 95, 143, 14, UI::TEXT_LIGHT);
    if (difficulty == Difficulty::HARD)
    {
        UI::Text("HARD MODE: Parity must ALSO alternate (Even / Odd) on the Helper stack!", 95, 163, 13, UI::ACCENT_GOLD);
    }
    else
    {
        UI::Text("Click stack top to move to Helper. Click Helper button to move back to stack.", 95, 163, 13, UI::TEXT_MUTED);
    }

    // 5 Stacks rendering: 4 normal stacks + 1 Helper stack
    float colWidth = 130;
    float colHeight = 310;
    float gap = 30;
    float startX = (UI::VIRTUAL_WIDTH - (5 * colWidth + 4 * gap)) / 2.0f;
    float baseY = 530;

    for (int col = 0; col < 5; col++)
    {
        float x = startX + col * (colWidth + gap);
        float y = baseY - colHeight;
        Rectangle colRect = { x, y, colWidth, colHeight };

        bool isHelper = (col == 4);
        Color borderColor = isHelper ? UI::ACCENT_GOLD : UI::PANEL_BORDER;
        UI::Panel(colRect, (Color){ 22, 27, 40, 255 }, borderColor, 0.05f);

        // Stack title
        std::string title = isHelper ? "HELPER" : ("STACK " + std::to_string(col + 1));
        int tw = UI::MeasureText(title.c_str(), 14);
        UI::Text(title.c_str(), (int)(x + (colWidth - tw) / 2), (int)(y - 25), 14, isHelper ? UI::ACCENT_GOLD : UI::TEXT_LIGHT);

        // Render Crates
        const Stack<int>& s = isHelper ? chamber->helper : chamber->s[col];
        int count = s.size();

        float crateHeight = 38;
        float crateMargin = 6;

        for (int row = 0; row < count; row++)
        {
            int val = s.Arr[row];
            float crateY = baseY - (row + 1) * (crateHeight + crateMargin);
            Rectangle crateRect = { x + 8, crateY, colWidth - 16, crateHeight };

            bool isTop = (row == count - 1);
            bool isKey = (val == 1000);

            Color crateBg;
            Color borderCol = isTop ? UI::ACCENT_CYAN : UI::PANEL_BORDER;

            if (isKey)
            {
                crateBg = (Color){ 245, 185, 66, 255 };
                borderCol = (Color){ 255, 230, 120, 255 };
            }
            else if (val > 0)
            {
                crateBg = (Color){ 30, 80, 110, 255 };
            }
            else
            {
                crateBg = (Color){ 120, 50, 40, 255 };
            }

            DrawRectangleRounded(crateRect, 0.2f, 4, crateBg);
            DrawRectangleRoundedLines(crateRect, 0.2f, 4, isTop ? 2.0f : 1.0f, borderCol);

            // Crate text
            std::string label;
            if (isKey)
            {
                label = "KEY 1000";
            }
            else
            {
                label = (val > 0 ? "+" : "") + std::to_string(val);
                if (difficulty == Difficulty::HARD)
                {
                    label += (abs(val) % 2 == 0 ? " (E)" : " (O)");
                }
            }
            int lw = UI::MeasureText(label.c_str(), 14);
            UI::Text(label.c_str(), (int)(crateRect.x + (crateRect.width - lw) / 2), (int)(crateRect.y + (crateHeight - 14) / 2), 14, isKey ? UI::BG_DARK : UI::TEXT_LIGHT);
        }

        // Action Buttons under stack
        if (!isHelper)
        {
            Rectangle toHelperBtn = { x, baseY + 12, colWidth, 28 };
            Rectangle fromHelperBtn = { x, baseY + 45, colWidth, 28 };

            if (UI::Button(toHelperBtn, "-> Helper", 12, UI::PANEL_BG, UI::ACCENT_CYAN, UI::TEXT_LIGHT))
            {
                if (chamber->canMovetohelper(col))
                {
                    chamber->moveStackToHelper(col);
                    setFeedback("Moved crate to Helper.", false);
                }
                else
                {
                    if (chamber->s[col].isEmpty())
                    {
                        setFeedback("Stack is empty!");
                    }
                    else
                    {
                        setFeedback("Illegal move! Signs must alternate (and parity in Hard mode).");
                    }
                }
            }

            if (UI::Button(fromHelperBtn, "<- Helper", 12, UI::PANEL_BG, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
            {
                if (chamber->canMovetoStack(col))
                {
                    chamber->moveHelperToStack(col);
                    setFeedback("Returned crate from Helper.", false);
                }
                else
                {
                    setFeedback("Helper stack is empty!");
                }
            }
        }
        else
        {
            // Indicator under Helper
            Rectangle infoBox = { x, baseY + 12, colWidth, 61 };
            UI::Panel(infoBox, (Color){ 20, 24, 34, 255 }, UI::PANEL_BORDER, 0.15f);
            UI::Text("LIFO Helper", (int)(x + 18), (int)(baseY + 22), 12, UI::ACCENT_GOLD);
            std::string itemsCount = std::to_string(chamber->helper.size()) + " crates";
            UI::Text(itemsCount.c_str(), (int)(x + 28), (int)(baseY + 44), 12, UI::TEXT_MUTED);
        }
    }

    // Feedback or Win Notification
    if (chamber->isWin())
    {
        Rectangle winOverlay = { 200, 230, 600, 180 };
        DrawRectangleRounded(winOverlay, 0.15f, 4, (Color){ 12, 14, 20, 245 });
        DrawRectangleRoundedLines(winOverlay, 0.15f, 4, 2.5f, UI::ACCENT_GOLD);

        const char* winMsg = "CHAMBER SOLVED!";
        int wm = UI::MeasureText(winMsg, 28);
        UI::Text(winMsg, (UI::VIRTUAL_WIDTH - wm) / 2.0f, 260, 28, UI::ACCENT_GOLD);

        const char* descMsg = "You uncovered the Golden Key (1000)! Stack Vault Complete.";
        int dm = UI::MeasureText(descMsg, 16);
        UI::Text(descMsg, (UI::VIRTUAL_WIDTH - dm) / 2.0f, 305, 16, UI::TEXT_LIGHT);

        if (UI::Button({ 410, 345, 180, 42 }, "Proceed to Hub", 16, UI::ACCENT_GREEN, UI::ACCENT_CYAN, UI::BG_DARK))
        {
            nextScene = SceneId::HUB;
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

bool StackVaultScene::IsComplete() const
{
    return chamber != nullptr && chamber->isWin();
}

Scene* createStackVaultScene()
{
    return new StackVaultScene();
}
