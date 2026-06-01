#ifndef ORIGINIUM_UI_THEME_H
#define ORIGINIUM_UI_THEME_H

#include "raylib.h"

// Single source of truth for the UI palette so the three screens (game,
// menu, editor) stay visually consistent. Pure constants — no behavior.
namespace theme {

// Accent / cyan used for highlights, selection borders, titles.
constexpr Color kAccent     = {109, 236, 218, 255};
// Text shades.
constexpr Color kTextBright = {232, 244, 245, 255};
constexpr Color kTextMuted  = {174, 191, 203, 255};
// Panel chrome.
constexpr Color kPanelBg     = { 18,  22,  32, 255};
constexpr Color kPanelBorder = { 72,  94, 112, 255};
// Background gradient (top → bottom), shared by all screens.
constexpr Color kBgGradTop    = { 22,  29,  43, 255};
constexpr Color kBgGradBottom = {  9,  12,  20, 255};

// 4-colour part/badge palette. Single source for both gameplay and editor.
inline Color colorBadge(unsigned colorIndex) {
    static const Color colors[] = {
        { 76, 175,  80, 255},
        { 33, 150, 243, 255},
        {244,  67,  54, 255},
        {255, 193,   7, 255},
    };
    return colors[colorIndex % (sizeof(colors) / sizeof(colors[0]))];
}

} // namespace theme

#endif
