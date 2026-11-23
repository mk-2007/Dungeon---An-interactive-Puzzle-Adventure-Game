#include "UI.h"

namespace UI
{
    Font fontBody;
    Font fontTitle;
    bool isFontReady = false;
    Vector2 virtualMousePos = { 0.0f, 0.0f };

    void Init()
    {
        const char* fontPaths[] = {
            "assets/fonts/font.ttf",
            "C:\\Windows\\Fonts\\segoeui.ttf",
            "C:\\Windows\\Fonts\\arial.ttf"
        };

        for (int i = 0; i < 3; i++)
        {
            if (FileExists(fontPaths[i]))
            {
                fontBody = LoadFontEx(fontPaths[i], 40, nullptr, 0);
                fontTitle = LoadFontEx(fontPaths[i], 60, nullptr, 0);

                GenTextureMipmaps(&fontBody.texture);
                GenTextureMipmaps(&fontTitle.texture);

                SetTextureFilter(fontBody.texture, TEXTURE_FILTER_TRILINEAR);
                SetTextureFilter(fontTitle.texture, TEXTURE_FILTER_TRILINEAR);

                isFontReady = true;
                break;
            }
        }
    }

    void Cleanup()
    {
        if (isFontReady)
        {
            UnloadFont(fontBody);
            UnloadFont(fontTitle);
            isFontReady = false;
        }
    }

    void UpdateMouse(Camera2D camera)
    {
        virtualMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    }

    Vector2 GetVirtualMouse()
    {
        return virtualMousePos;
    }

    void Text(const char* text, float x, float y, float size, Color col)
    {
        if (!text) return;
        if (isFontReady)
        {
            Vector2 pos = { x, y };
            Font& f = (size >= 24.0f) ? fontTitle : fontBody;
            DrawTextEx(f, text, pos, size, 0.0f, col);
        }
        else
        {
            DrawText(text, (int)x, (int)y, (int)size, col);
        }
    }

    int MeasureText(const char* text, float size)
    {
        if (!text) return 0;
        if (isFontReady)
        {
            Font& f = (size >= 24.0f) ? fontTitle : fontBody;
            Vector2 v = MeasureTextEx(f, text, size, 0.0f);
            return (int)v.x;
        }
        return ::MeasureText(text, (int)size);
    }

    bool Button(Rectangle rect, const char* text, float fontSize, Color bg, Color hoverBg, Color textCol)
    {
        Vector2 mouse = GetVirtualMouse();
        bool hovered = CheckCollisionPointRec(mouse, rect);
        bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        Color currentBg = hovered ? hoverBg : bg;
        Color currentText = hovered ? (hoverBg.r > 180 && hoverBg.g > 180 ? BG_DARK : TEXT_LIGHT) : textCol;

        DrawRectangleRounded(rect, 0.2f, 4, currentBg);
        DrawRectangleRoundedLines(rect, 0.2f, 4, 1.5f, hovered ? ACCENT_GOLD : PANEL_BORDER);

        int textWidth = MeasureText(text, fontSize);
        float textX = rect.x + (rect.width - textWidth) / 2.0f;
        float textY = rect.y + (rect.height - fontSize) / 2.0f;
        Text(text, textX, textY, fontSize, currentText);

        return clicked;
    }

    void Panel(Rectangle rect, Color bg, Color border, float roundness)
    {
        DrawRectangleRounded(rect, roundness, 4, bg);
        DrawRectangleRoundedLines(rect, roundness, 4, 1.5f, border);
    }

    void Header(const char* title, const char* subtitle)
    {
        int titleWidth = MeasureText(title, 28);
        Text(title, (1000 - titleWidth) / 2.0f, 20, 28, ACCENT_GOLD);
        if (subtitle != nullptr)
        {
            int subWidth = MeasureText(subtitle, 15);
            Text(subtitle, (1000 - subWidth) / 2.0f, 54, 15, TEXT_MUTED);
        }
        DrawLine(50, 80, 950, 80, PANEL_BORDER);
    }

    void Badge(Rectangle rect, const char* text, Color bg, Color textCol, float fontSize)
    {
        DrawRectangleRounded(rect, 0.3f, 4, bg);
        int w = MeasureText(text, fontSize);
        Text(text, rect.x + (rect.width - w) / 2.0f, rect.y + (rect.height - fontSize) / 2.0f, fontSize, textCol);
    }
}
