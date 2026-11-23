#pragma once
#include <string>
#include "RaylibCompat.h"

namespace UI
{
    const int VIRTUAL_WIDTH    = 1000;
    const int VIRTUAL_HEIGHT   = 650;

    const Color BG_DARK        = { 18, 20, 29, 255 };
    const Color BG_OVERLAY     = { 12, 14, 20, 220 };
    const Color PANEL_BG       = { 28, 34, 50, 255 };
    const Color PANEL_BORDER   = { 52, 64, 94, 255 };
    const Color ACCENT_GOLD    = { 245, 185, 66, 255 };
    const Color ACCENT_CYAN    = { 56, 189, 248, 255 };
    const Color ACCENT_GREEN   = { 52, 211, 153, 255 };
    const Color ACCENT_RED     = { 239, 68, 68, 255 };
    const Color ACCENT_PURPLE  = { 168, 85, 247, 255 };
    const Color ACCENT_ORANGE  = { 251, 146, 60, 255 };
    const Color TEXT_LIGHT     = { 241, 245, 249, 255 };
    const Color TEXT_MUTED     = { 148, 163, 184, 255 };

    extern Font fontBody;
    extern Font fontTitle;
    extern bool isFontReady;
    extern Vector2 virtualMousePos;

    void Init();
    void Cleanup();

    void UpdateMouse(Camera2D camera);
    Vector2 GetVirtualMouse();

    void Text(const char* text, float x, float y, float size, Color col);
    int MeasureText(const char* text, float size);

    bool Button(Rectangle rect, const char* text, float fontSize = 16.0f, Color bg = PANEL_BG, Color hoverBg = ACCENT_CYAN, Color textCol = TEXT_LIGHT);
    void Panel(Rectangle rect, Color bg = PANEL_BG, Color border = PANEL_BORDER, float roundness = 0.15f);
    void Header(const char* title, const char* subtitle = nullptr);
    void Badge(Rectangle rect, const char* text, Color bg, Color textCol = TEXT_LIGHT, float fontSize = 13.0f);
}
