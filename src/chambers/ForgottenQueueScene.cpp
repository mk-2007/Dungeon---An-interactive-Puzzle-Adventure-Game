#include "ForgottenQueueScene.h"

ForgottenQueueScene::ForgottenQueueScene()
    : difficulty(Difficulty::EASY),
      belt(nullptr),
      nextScene(SceneId::FORGOTTEN_QUEUE),
      feedbackTimer(0.0f)
{
    belt = new SpiritConveyerBelt(difficulty);
}

ForgottenQueueScene::~ForgottenQueueScene()
{
    delete belt;
}

void ForgottenQueueScene::Enter()
{
    nextScene = SceneId::FORGOTTEN_QUEUE;
    resetLevel();
}

void ForgottenQueueScene::resetLevel()
{
    delete belt;
    belt = new SpiritConveyerBelt(difficulty);
    feedbackTimer = 0.0f;
    feedbackMessage = "";
}

void ForgottenQueueScene::setFeedback(const std::string& msg, bool isError)
{
    feedbackMessage = msg;
    feedbackTimer = 3.0f;
}

bool ForgottenQueueScene::isSpiritOnBelt(const std::string& name) const
{
    if (!belt) return false;
    for (QNode<Spirit>* cur = belt->conveyerBelt.front; cur != nullptr; cur = cur->next)
    {
        if (cur->data.name == name) return true;
    }
    return false;
}

Color ForgottenQueueScene::getSpiritColor(const std::string& name) const
{
    if (name == "Water") return (Color){ 56, 189, 248, 255 };
    if (name == "Grass") return (Color){ 52, 211, 153, 255 };
    if (name == "Fire")  return (Color){ 239, 68, 68, 255 };
    if (name == "Earth") return (Color){ 217, 119, 6, 255 };
    if (name == "Light") return (Color){ 250, 204, 21, 255 };
    if (name == "Dark")  return (Color){ 168, 85, 247, 255 };
    if (name == "Ice")   return (Color){ 147, 197, 253, 255 };
    return UI::TEXT_LIGHT;
}

void ForgottenQueueScene::Update(float dt)
{
    if (feedbackTimer > 0.0f)
    {
        feedbackTimer -= dt;
    }
}

void ForgottenQueueScene::Draw()
{
    ClearBackground(UI::BG_DARK);

    // Header
    std::string sub = std::string("Chamber 2 · ") + (difficulty == Difficulty::EASY ? "EASY" : "HARD") + " MODE";
    UI::Header("THE FORGOTTEN QUEUE", sub.c_str());

    // Navigation and Control buttons
    if (UI::Button({ 40, 25, 120, 36 }, "< Hub", 16, UI::PANEL_BG, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
    {
        nextScene = SceneId::HUB;
    }

    if (UI::Button({ 840, 25, 120, 36 }, "Reset", 16, UI::PANEL_BG, UI::ACCENT_RED, UI::TEXT_LIGHT))
    {
        resetLevel();
        setFeedback("Queue conveyor cleared.", false);
    }

    // Difficulty buttons
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

    // Rules card
    Rectangle rulesCard = { 60, 135, 880, difficulty == Difficulty::EASY ? 65.0f : 85.0f };
    UI::Panel(rulesCard, UI::PANEL_BG, UI::PANEL_BORDER, 0.15f);

    UI::Text("QUEUE DESIRE RULES: Spirits must board the FIFO conveyor belt in harmony:", 75, 143, 13, UI::ACCENT_GOLD);
    if (difficulty == Difficulty::EASY)
    {
        UI::Text("1. Grass must touch Water.   2. Water cannot touch Fire.   3. Grass cannot touch Fire.", 75, 165, 13, UI::TEXT_LIGHT);
        UI::Text("Spirits: Water, Grass, Earth, Fire (4 total). Enqueue in valid order to solve!", 75, 183, 12, UI::TEXT_MUTED);
    }
    else
    {
        UI::Text("Must touch: Grass+Water, Grass+Light. Cannot touch: Water+Fire, Grass+Fire, Ice+Fire,", 75, 163, 12, UI::TEXT_LIGHT);
        UI::Text("Dark+Fire, Light+Dark, Earth+Ice, Earth+Water. Spirits: 7 total. Order matters!", 75, 180, 12, UI::TEXT_LIGHT);
        UI::Text("Undo uses queue rotation (dequeue and re-enqueue all but last item)!", 75, 198, 11, UI::TEXT_MUTED);
    }

    // Conveyor Belt Track
    float beltY = difficulty == Difficulty::EASY ? 230 : 245;
    Rectangle trackRect = { 60, beltY, 880, 145 };
    UI::Panel(trackRect, (Color){ 20, 25, 38, 255 }, UI::PANEL_BORDER, 0.1f);

    // Track markings / rollers
    for (int rx = 80; rx < 920; rx += 40)
    {
        DrawLine(rx, (int)beltY + 20, rx, (int)beltY + 125, (Color){ 35, 42, 60, 255 });
    }

    // Front (Head) and Rear (Tail) Indicators
    UI::Text("<- FRONT (EXIT)", 75, (int)beltY + 10, 12, UI::ACCENT_CYAN);
    UI::Text("REAR (ENTRY) <-", 815, (int)beltY + 10, 12, UI::ACCENT_GOLD);

    // Render spirits currently on the belt (FIFO order: front at index 0 on left)
    int count = belt->conveyerBelt.getSize();
    int capacity = belt->beltSize;

    float slotWidth = (880.0f - 80.0f) / capacity;
    int idx = 0;
    for (QNode<Spirit>* cur = belt->conveyerBelt.front; cur != nullptr; cur = cur->next, idx++)
    {
        float sx = 80.0f + idx * slotWidth;
        float sy = beltY + 35;
        Rectangle spiritCard = { sx + 5, sy, slotWidth - 10, 75 };

        Color sCol = getSpiritColor(cur->data.name);
        DrawRectangleRounded(spiritCard, 0.2f, 4, (Color){ 28, 34, 52, 255 });
        DrawRectangleRoundedLines(spiritCard, 0.2f, 4, 2.0f, sCol);

        // Position order badge
        std::string posBadge = "#" + std::to_string(idx + 1);
        UI::Text(posBadge.c_str(), (int)(spiritCard.x + 8), (int)(spiritCard.y + 8), 11, UI::TEXT_MUTED);

        // Spirit Name
        int nw = UI::MeasureText(cur->data.name.c_str(), 16);
        UI::Text(cur->data.name.c_str(), (int)(spiritCard.x + (spiritCard.width - nw) / 2), (int)(spiritCard.y + 30), 16, sCol);
    }

    // Empty slot placeholders
    for (int e = count; e < capacity; e++)
    {
        float sx = 80.0f + e * slotWidth;
        float sy = beltY + 35;
        Rectangle slotCard = { sx + 5, sy, slotWidth - 10, 75 };
        DrawRectangleRoundedLines(slotCard, 0.2f, 4, 1.0f, (Color){ 45, 52, 75, 255 });
        const char* emptyLbl = "[Empty]";
        int ew = UI::MeasureText(emptyLbl, 12);
        UI::Text(emptyLbl, (int)(slotCard.x + (slotCard.width - ew) / 2), (int)(slotCard.y + 30), 12, (Color){ 60, 70, 95, 255 });
    }

    // Queue status text
    std::string qStatus = "Conveyor Belt: " + std::to_string(count) + " / " + std::to_string(capacity) + " spirits queued";
    UI::Text(qStatus.c_str(), 75, (int)beltY + 125, 12, UI::TEXT_MUTED);

    // Controls: Undo Last & Clear Queue
    Rectangle undoBtn = { 720, beltY + 115, 105, 25 };
    Rectangle clearBtn = { 830, beltY + 115, 100, 25 };

    if (UI::Button(undoBtn, "Undo Last", 12, UI::PANEL_BG, UI::ACCENT_GOLD, UI::TEXT_LIGHT))
    {
        if (count > 0)
        {
            belt->undo();
            setFeedback("Removed last queued spirit via FIFO rotation.", false);
        }
        else
        {
            setFeedback("Conveyor belt is already empty!");
        }
    }

    if (UI::Button(clearBtn, "Clear Queue", 12, UI::PANEL_BG, UI::ACCENT_RED, UI::TEXT_LIGHT))
    {
        belt->conveyerBelt.clear();
        belt->solutionFound = false;
        belt->lastAttemptWrong = false;
        setFeedback("Cleared conveyor belt.", false);
    }

    // Available Roster Panel
    float rosterY = beltY + 160;
    Rectangle rosterRect = { 60, rosterY, 880, 115 };
    UI::Panel(rosterRect, UI::PANEL_BG, UI::PANEL_BORDER, 0.1f);

    UI::Text("AVAILABLE SPIRITS (Click to Enqueue at Rear):", 75, (int)rosterY + 12, 13, UI::ACCENT_CYAN);

    std::vector<std::string> roster;
    if (difficulty == Difficulty::EASY)
    {
        roster = { "Water", "Grass", "Earth", "Fire" };
    }
    else
    {
        roster = { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" };
    }

    float rCardWidth = (880.0f - 40.0f) / roster.size();
    for (size_t i = 0; i < roster.size(); i++)
    {
        const std::string& sName = roster[i];
        bool onBelt = isSpiritOnBelt(sName);
        float rx = 75.0f + i * rCardWidth;
        float ry = rosterY + 38;
        Rectangle btnRect = { rx + 4, ry, rCardWidth - 8, 55 };

        Color sCol = getSpiritColor(sName);
        if (!onBelt && !belt->isSolved())
        {
            if (UI::Button(btnRect, sName.c_str(), 15, (Color){ 24, 30, 46, 255 }, sCol, sCol))
            {
                if (belt->insert(Spirit(sName)))
                {
                    setFeedback("Enqueued " + sName + " at rear of conveyor.", false);
                }
                else
                {
                    setFeedback("Conveyor belt is full or spirit already added!");
                }
            }
        }
        else
        {
            DrawRectangleRounded(btnRect, 0.2f, 4, (Color){ 18, 22, 32, 255 });
            DrawRectangleRoundedLines(btnRect, 0.2f, 4, 1.0f, (Color){ 35, 42, 58, 255 });
            int tw = UI::MeasureText(sName.c_str(), 14);
            UI::Text(sName.c_str(), (int)(btnRect.x + (btnRect.width - tw) / 2), (int)(btnRect.y + 12), 14, (Color){ 80, 90, 115, 255 });
            const char* inQueueLbl = onBelt ? "[In Queue]" : "[Locked]";
            int iw = UI::MeasureText(inQueueLbl, 11);
            UI::Text(inQueueLbl, (int)(btnRect.x + (btnRect.width - iw) / 2), (int)(btnRect.y + 32), 11, (Color){ 80, 90, 115, 255 });
        }
    }

    // Win or Status Banner
    if (belt->isSolved())
    {
        Rectangle winOverlay = { 200, 210, 600, 180 };
        DrawRectangleRounded(winOverlay, 0.15f, 4, (Color){ 12, 14, 20, 245 });
        DrawRectangleRoundedLines(winOverlay, 0.15f, 4, 2.5f, UI::ACCENT_GOLD);

        const char* winMsg = "QUEUE HARMONIZED! CHAMBER SOLVED!";
        int wm = UI::MeasureText(winMsg, 24);
        UI::Text(winMsg, (UI::VIRTUAL_WIDTH - wm) / 2.0f, 240, 24, UI::ACCENT_GOLD);

        const char* descMsg = "All spirits are aligned on the conveyor belt in peace.";
        int dm = UI::MeasureText(descMsg, 16);
        UI::Text(descMsg, (UI::VIRTUAL_WIDTH - dm) / 2.0f, 280, 16, UI::TEXT_LIGHT);

        if (UI::Button({ 410, 320, 180, 42 }, "Proceed to Hub", 16, UI::ACCENT_GREEN, UI::ACCENT_CYAN, UI::BG_DARK))
        {
            nextScene = SceneId::HUB;
        }
    }
    else if (belt->wasLastAttemptWrong())
    {
        int fw = UI::MeasureText("The spirits are unhappy with this ordering! Use Undo or Clear to rearrange.", 14);
        Rectangle toastRect = { (UI::VIRTUAL_WIDTH - fw - 40) / 2.0f, 615, (float)(fw + 40), 28 };
        DrawRectangleRounded(toastRect, 0.3f, 4, (Color){ 180, 40, 40, 230 });
        UI::Text("The spirits are unhappy with this ordering! Use Undo or Clear to rearrange.", (int)(toastRect.x + 20), (int)(toastRect.y + 7), 14, UI::TEXT_LIGHT);
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

bool ForgottenQueueScene::IsComplete() const
{
    return belt != nullptr && belt->isSolved();
}

Scene* createForgottenQueueScene()
{
    return new ForgottenQueueScene();
}
