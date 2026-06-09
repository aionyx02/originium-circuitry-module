#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "raylib.h"

#include "core/Editor.h"
#include "core/Game.h"
#include "core/LevelWriter.h"
#include "core/Parser.h"
#include "ui/Input.h"
#include "ui/Renderer.h"
#include "ui/Theme.h"

namespace {

struct LevelEntry {
    std::filesystem::path path;
    std::string name;
};

enum class AppState {
    Menu,
    InGame,
    Editor,
};

std::string promptPath(int argc, char** argv) {
    if (argc > 1 && argv[1] != nullptr) {
        const std::string arg = argv[1];
        if (arg == "--menu" || arg == "-m") return {};
        return arg;
    }
    std::cout << "Enter level file path (leave blank for level menu): " << std::flush;
    std::string path;
    if (!std::getline(std::cin, path)) return {};
    return path;
}

bool loadGame(const std::string& path, Game& game, std::string& errorOut) {
    std::ifstream in(path);
    if (!in) {
        errorOut = "Cannot open file: " + path;
        return false;
    }
    try {
        GameData data = Parser::parse(in);
        game.init(std::move(data.board), std::move(data.parts));
        return true;
    } catch (const Parser::Error& e) {
        errorOut = std::string("Parse error: ") + e.what();
        return false;
    } catch (const std::exception& e) {
        errorOut = std::string("Unexpected error: ") + e.what();
        return false;
    }
}

bool tryLoadDroppedLevel(Game& game, std::string& loadedPath, std::string& errorOut) {
    if (!IsFileDropped()) return false;

    FilePathList dropped = LoadDroppedFiles();
    const auto unload = [&]() { UnloadDroppedFiles(dropped); };

    for (unsigned i = 0; i < dropped.count; ++i) {
        const std::filesystem::path path = dropped.paths[i];
        if (path.extension() != ".txt") continue;
        if (loadGame(path.string(), game, errorOut)) {
            loadedPath = path.string();
            unload();
            return true;
        }
        unload();
        return false;
    }

    errorOut = "Drop a .txt level file.";
    unload();
    return false;
}

constexpr int kMenuPanelW = 600;
constexpr int kMenuPanelH = 520;
constexpr int kMenuListOffsetX = 44;
constexpr int kMenuListOffsetY = 168;
constexpr int kMenuRowH = 42;
constexpr int kMenuVisibleRows = 6;

int menuVisibleStart(int selected, int levelCount) {
    if (levelCount <= kMenuVisibleRows) return 0;
    if (selected < 0) return 0;
    int start = selected - kMenuVisibleRows / 2;
    start = std::max(0, start);
    start = std::min(start, levelCount - kMenuVisibleRows);
    return start;
}

std::vector<LevelEntry> findLevels() {
    namespace fs = std::filesystem;

    std::vector<LevelEntry> levels;
    const fs::path root = "assets/levels";
    if (!fs::exists(root) || !fs::is_directory(root)) return levels;

    for (const auto& entry : fs::directory_iterator(root)) {
        if (!entry.is_regular_file()) continue;
        const fs::path path = entry.path();
        if (path.extension() != ".txt") continue;
        levels.push_back(LevelEntry{path, path.filename().string()});
    }

    std::sort(levels.begin(), levels.end(), [](const LevelEntry& a, const LevelEntry& b) {
        return a.name < b.name;
    });
    return levels;
}

// Display-only prettifier: "Example1.txt" -> "Example 1", "custom-3.txt" ->
// "Custom 3". Never used for file IO — LevelEntry::name keeps the raw filename.
std::string prettyLevelName(const std::string& filename) {
    std::string s = filename;
    const std::string ext = ".txt";
    if (s.size() > ext.size() && s.compare(s.size() - ext.size(), ext.size(), ext) == 0)
        s.erase(s.size() - ext.size());
    const std::string pre = "custom-";
    if (s.compare(0, pre.size(), pre) == 0)
        return "Custom " + s.substr(pre.size());
    // Space before a trailing digit run: "Example1" -> "Example 1".
    std::size_t i = s.size();
    while (i > 0 && std::isdigit(static_cast<unsigned char>(s[i - 1]))) --i;
    if (i > 0 && i < s.size() && !std::isspace(static_cast<unsigned char>(s[i - 1])))
        s.insert(i, " ");
    if (!s.empty()) s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
    return s;
}

int menuSelectionFromMouse(const std::vector<LevelEntry>& levels,
                           int selected,
                           int screenW,
                           int screenH) {
    if (levels.empty()) return -1;

    const Vector2 m = GetMousePosition();
    const int panelX = (screenW - kMenuPanelW) / 2;
    const int panelY = (screenH - kMenuPanelH) / 2;
    const int listX = panelX + kMenuListOffsetX;
    const int listY = panelY + kMenuListOffsetY;
    const int listW = kMenuPanelW - 2 * kMenuListOffsetX;
    const int maxVisible = std::min(static_cast<int>(levels.size()), kMenuVisibleRows);

    if (m.x < listX || m.x >= listX + listW) return -1;
    if (m.y < listY || m.y >= listY + maxVisible * kMenuRowH) return -1;
    const int localIdx = static_cast<int>(m.y - listY) / kMenuRowH;
    const int idx = menuVisibleStart(selected, static_cast<int>(levels.size())) + localIdx;
    return idx >= 0 && idx < static_cast<int>(levels.size()) ? idx : -1;
}

void drawMenu(const std::vector<LevelEntry>& levels,
              int selected,
              int hover,
              const std::string& message,
              Font font,
              int screenW,
              int screenH) {
    DrawRectangleGradientV(0, 0, screenW, screenH,
                           theme::kBgGradTop, theme::kBgGradBottom);

    const int panelX = (screenW - kMenuPanelW) / 2;
    const int panelY = (screenH - kMenuPanelH) / 2;
    Rectangle panel = {
        static_cast<float>(panelX),
        static_cast<float>(panelY),
        static_cast<float>(kMenuPanelW),
        static_cast<float>(kMenuPanelH),
    };
    DrawRectangleRounded(panel, 0.06f, 8, Color{18, 22, 32, 230});
    DrawRectangleGradientV(panelX + 1, panelY + 1, kMenuPanelW - 2, kMenuPanelH - 2,
                           Color{35, 43, 56, 120}, Color{16, 20, 30, 35});
    DrawRectangleRoundedLinesEx(panel, 0.06f, 8, 1.0f, theme::kPanelBorder);

    DrawTextEx(font, "ORIGINIUM", Vector2{static_cast<float>(panelX + 42), static_cast<float>(panelY + 34)},
               32.0f, 1.5f, theme::kTextBright);
    DrawTextEx(font, "CIRCUIT REPAIR", Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + 70)},
               18.0f, 1.2f, theme::kAccent);
    DrawRectangle(panelX + 44, panelY + 103, 88, 2, Color{109, 236, 218, 190});
    DrawTextEx(font, "SELECT LEVEL", Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + 132)},
               18.0f, 1.4f, theme::kTextMuted);

    if (levels.empty()) {
        DrawTextEx(font, "No playable .txt levels found.",
                   Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + 180)},
                   20.0f, 1.0f, Color{220, 120, 120, 255});
    } else {
        const int listX = panelX + kMenuListOffsetX;
        const int listY = panelY + kMenuListOffsetY;
        const int listW = kMenuPanelW - 2 * kMenuListOffsetX;
        const int levelCount = static_cast<int>(levels.size());
        const int start = menuVisibleStart(selected, levelCount);
        const int maxVisible = std::min(levelCount, kMenuVisibleRows);
        for (int localIdx = 0; localIdx < maxVisible; ++localIdx) {
            const int i = start + localIdx;
            Rectangle row = {
                static_cast<float>(listX),
                static_cast<float>(listY + localIdx * kMenuRowH),
                static_cast<float>(listW),
                static_cast<float>(kMenuRowH - 8),
            };
            const bool isSelected = (i == selected);
            const bool isHover = (i == hover);
            DrawRectangleRounded(row, 0.10f, 6,
                                 isSelected ? Color{36, 55, 61, 255}
                                 : isHover ? Color{36, 44, 56, 245}
                                            : Color{31, 37, 48, 230});
            DrawRectangleRoundedLinesEx(row, 0.10f, 6, isSelected ? 2.0f : 1.0f,
                                        isSelected ? theme::kAccent
                                        : isHover ? Color{104, 130, 150, 245}
                                                   : Color{75, 88, 104, 255});
            DrawTextEx(font, prettyLevelName(levels[i].name).c_str(),
                       Vector2{static_cast<float>(listX + 18),
                               static_cast<float>(listY + localIdx * kMenuRowH + 7)},
                       20.0f, 1.0f, theme::kTextBright);
        }
        if (levelCount > maxVisible) {
            // Scroll affordance: line chevrons when there's more above / below
            // (drawn as lines so we don't depend on Unicode glyphs in the font).
            const float cx = static_cast<float>(listX + listW - 10);
            if (start > 0) {
                const float cy = static_cast<float>(listY - 12);
                DrawLineEx({cx - 6, cy + 4}, {cx, cy - 4}, 2.0f, theme::kAccent);
                DrawLineEx({cx, cy - 4}, {cx + 6, cy + 4}, 2.0f, theme::kAccent);
            }
            if (start + maxVisible < levelCount) {
                const float cy = static_cast<float>(listY + maxVisible * kMenuRowH + 2);
                DrawLineEx({cx - 6, cy - 4}, {cx, cy + 4}, 2.0f, theme::kAccent);
                DrawLineEx({cx, cy + 4}, {cx + 6, cy - 4}, 2.0f, theme::kAccent);
            }
            char range[64];
            std::snprintf(range, sizeof(range), "Showing %d-%d of %d",
                          start + 1, start + maxVisible, levelCount);
            DrawTextEx(font, range,
                       Vector2{static_cast<float>(listX),
                               static_cast<float>(listY + maxVisible * kMenuRowH + 10)},
                       14.0f, 1.0f, Color{114, 128, 139, 255});
        }
    }

    DrawTextEx(font, "Enter/Click Play | Up/Down Select | Right/Right-click Options | E Create",
               Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + kMenuPanelH - 54)},
               15.0f, 1.0f, Color{170, 184, 196, 255});
    if (!message.empty()) {
        DrawTextEx(font, message.c_str(),
                   Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + kMenuPanelH - 28)},
                   15.0f, 1.0f, Color{220, 120, 120, 255});
    }
}

} // namespace

namespace {

int countPlaced(const Game& g) {
    int n = 0;
    for (const auto& p : g.parts) if (p.location.placed) ++n;
    return n;
}

int sumRotateCount(const Game& g) {
    int s = 0;
    for (const auto& p : g.parts) s += p.rotateCount;
    return s;
}

// --- Level editor (Phase 5) ---

// Part-designer paint grid dimension (NxN canvas; trimmed to bbox on add).
constexpr int kPaintN = 5;
constexpr int kMaxExportStemLen = 24;

void editorCenteredText(Font font, const char* s, float cx, float cy, float fs, Color col) {
    const Vector2 sz = MeasureTextEx(font, s, fs, 1.0f);
    DrawTextEx(font, s, Vector2{cx - sz.x / 2.0f, cy - sz.y / 2.0f}, fs, 1.0f, col);
}

std::string trimSpaces(std::string s) {
    auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
    const auto first = std::find_if(s.begin(), s.end(), notSpace);
    const auto last = std::find_if(s.rbegin(), s.rend(), notSpace).base();
    if (first >= last) return {};
    return std::string(first, last);
}

bool isExportNameChar(int codepoint) {
    return (codepoint >= '0' && codepoint <= '9')
        || (codepoint >= 'A' && codepoint <= 'Z')
        || (codepoint >= 'a' && codepoint <= 'z')
        || codepoint == '-' || codepoint == '_' || codepoint == ' ';
}

std::string normalizeExportStem(std::string stem) {
    stem = trimSpaces(std::move(stem));
    if (stem.size() >= 4) {
        std::string ext = stem.substr(stem.size() - 4);
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        if (ext == ".txt") stem.erase(stem.size() - 4);
    }
    return trimSpaces(std::move(stem));
}

std::string nextDefaultExportStem() {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::create_directories("assets/levels", ec);
    for (int n = 1; n < 10000; ++n) {
        const fs::path path = fs::path("assets/levels") / ("custom-" + std::to_string(n) + ".txt");
        if (!fs::exists(path)) return "custom-" + std::to_string(n);
    }
    return "custom-level";
}

// Number of designed parts not yet placed on the board. The editor derives the
// row/column requirements only from placed pieces (Editor::deriveConstraints),
// so exporting/playing with any piece still in the tray yields constraints that
// under-count that piece's color — an unsolvable level. EXPORT and PLAY are both
// gated on this being 0 so the placed layout is always a valid solution.
int editorUnplacedCount(const Editor& ed) {
    int n = 0;
    for (const auto& p : ed.parts) if (!p.location.placed) ++n;
    return n;
}

// Write the level to assets/levels/<name>.txt. Returns the path written, or
// "" on failure. Existing files are protected, except that re-exporting the
// same file again in the current editor session is allowed.
std::string exportEditorLevel(const Editor& ed, const std::string& rawStem,
                              const std::string& lastExportPath, std::string& errorOut) {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::create_directories("assets/levels", ec);

    if (const int unplaced = editorUnplacedCount(ed); unplaced > 0) {
        errorOut = "Place all pieces on the board first ("
                 + std::to_string(unplaced) + " still in tray).";
        return {};
    }

    const std::string stem = normalizeExportStem(rawStem);
    if (stem.empty()) {
        errorOut = "Enter a file name first.";
        return {};
    }

    const fs::path path = fs::path("assets/levels") / (stem + ".txt");
    if (fs::exists(path) && path.string() != lastExportPath) {
        errorOut = "That file already exists. Pick another name.";
        return {};
    }

    std::ofstream out(path);
    if (!out) {
        errorOut = "Export failed.";
        return {};
    }
    Board board;
    std::vector<Part> parts;
    ed.buildPlayableSnapshot(board, parts);
    out << LevelWriter::write(board, parts);
    if (!out) {
        errorOut = "Export failed.";
        return {};
    }
    return path.string();
}

enum class EditorResult { Stay, Play, Menu };

// Immediate-mode editor: draws and handles mouse input in one pass. Click a
// board cell to apply the active tool (erase / block / fix); click the number
// boxes around the board to fine-tune the current color's row/column
// requirement (left = +1, right = -1); paint a shape in the designer and ADD
// it as a part.
EditorResult runEditor(Editor& ed, std::vector<std::vector<bool>>& paint, int& tool,
                       int& selPiece, std::string& msg, std::string& exportStem,
                       std::string& lastExportPath, bool& exportNameFocused,
                       Font font, int sw, int sh, bool& hoverClickable) {
    const Vector2 mp = GetMousePosition();
    const bool lc = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool rc = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    const Color cyan = theme::kAccent;
    const Color text = theme::kTextBright;
    const Color cur  = theme::colorBadge(ed.currentColor);
    if (IsKeyPressed(KEY_F2)) exportNameFocused = !exportNameFocused;
    const bool editingExportName = exportNameFocused;

    auto hit = [&](Rectangle r) { return CheckCollisionPointRec(mp, r); };
    auto button = [&](Rectangle r, const char* label, bool active, float fs = 16.0f) -> bool {
        const bool over = hit(r);
        if (over) hoverClickable = true;
        DrawRectangleRounded(r, 0.22f, 5,
            active ? Color{36, 55, 61, 255} : over ? Color{40, 48, 60, 255} : Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(r, 0.22f, 5, active ? 2.0f : 1.0f,
            active ? cyan : over ? Color{120, 150, 170, 255} : Color{72, 94, 112, 255});
        editorCenteredText(font, label, r.x + r.width / 2, r.y + r.height / 2, fs, text);
        return over && lc;
    };

    EditorResult result = EditorResult::Stay;
    // PLAY only when every piece is on the board — otherwise the derived
    // constraints don't match the part list and the level can't be won.
    auto requestPlay = [&]() {
        if (const int unplaced = editorUnplacedCount(ed); unplaced > 0)
            msg = "Place all pieces on the board first ("
                + std::to_string(unplaced) + " still in tray).";
        else
            result = EditorResult::Play;
    };
    if (!editingExportName && IsKeyPressed(KEY_ESCAPE)) result = EditorResult::Menu;
    if (!editingExportName && IsKeyPressed(KEY_P)) requestPlay();
    if (!editingExportName && IsKeyPressed(KEY_R)) ed.rotatePart(selPiece);
    if (!editingExportName && IsKeyPressed(KEY_E)) {
        std::string err;
        const std::string path = exportEditorLevel(ed, exportStem, lastExportPath, err);
        if (path.empty()) msg = err;
        else {
            lastExportPath = path;
            msg = "Exported: " + path;
        }
    }
    if (editingExportName) {
        int ch = GetCharPressed();
        while (ch > 0) {
            if (isExportNameChar(ch) && exportStem.size() < kMaxExportStemLen)
                exportStem.push_back(static_cast<char>(ch));
            ch = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !exportStem.empty()) exportStem.pop_back();
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_ESCAPE))
            exportNameFocused = false;
    }

    DrawRectangleGradientV(0, 0, sw, sh, theme::kBgGradTop, theme::kBgGradBottom);

    // ---- Sidebar ----
    Rectangle panel = {24.0f, 20.0f, 320.0f, (float)sh - 40};
    DrawRectangleRounded(panel, 0.04f, 8, Color{18, 22, 32, 235});
    DrawRectangleRoundedLinesEx(panel, 0.04f, 8, 1.0f, theme::kPanelBorder);
    DrawTextEx(font, "LEVEL EDITOR", Vector2{44, 34}, 24.0f, 1.2f, text);
    DrawTextEx(font, "CIRCUIT DESIGNER", Vector2{45, 62}, 13.0f, 1.4f, cyan);
    DrawRectangle(45, 84, 88, 2, Color{109, 236, 218, 190});  // accent underline (matches game/menu)

    float y = 92;
    auto stepper = [&](const char* label, int value, int lo, int hi, std::function<void(int)> apply) {
        DrawTextEx(font, label, Vector2{44, y + 5}, 15.0f, 1.0f, Color{174, 191, 203, 255});
        if (button({168, y, 26, 26}, "-", false)) apply(std::max(lo, value - 1));
        char b[8]; std::snprintf(b, sizeof(b), "%d", value);
        editorCenteredText(font, b, 224, y + 13, 18.0f, text);
        if (button({250, y, 26, 26}, "+", false)) apply(std::min(hi, value + 1));
        y += 34;
    };
    stepper("ROWS",   (int)ed.board.rows, 1, Editor::kMaxDim, [&](int v){ ed.setSize(v, ed.board.cols); });
    stepper("COLS",   (int)ed.board.cols, 1, Editor::kMaxDim, [&](int v){ ed.setSize(ed.board.rows, v); });
    stepper("COLORS", (int)ed.board.colors, 1, Editor::kMaxColors, [&](int v){ ed.setColorCount(v); });

    // Color picker.
    y += 4;
    DrawTextEx(font, "COLOR", Vector2{44, y}, 14.0f, 1.2f, Color{150, 166, 180, 255});
    for (int i = 0; i < (int)ed.board.colors; ++i) {
        Rectangle sw_ = {110.0f + i * 40, y - 4, 32, 22};
        DrawRectangleRounded(sw_, 0.3f, 5, theme::colorBadge(i));
        if ((int)ed.currentColor == i)
            DrawRectangleRoundedLinesEx(sw_, 0.3f, 5, 2.5f, cyan);
        if (hit(sw_)) { hoverClickable = true; if (lc) ed.currentColor = (unsigned)i; }
    }
    y += 34;

    // Board tool: PLACE pieces, or paint BLOCK / FIX / ERASE cells.
    DrawTextEx(font, "BOARD TOOL", Vector2{44, y}, 14.0f, 1.2f, Color{150, 166, 180, 255});
    y += 20;
    if (button({44, y, 70, 28},  "PLACE", tool == 0)) tool = 0;
    if (button({118, y, 70, 28}, "BLOCK", tool == 1)) tool = 1;
    if (button({192, y, 70, 28}, "FIX",   tool == 2)) tool = 2;
    if (button({266, y, 70, 28}, "ERASE", tool == 3)) tool = 3;
    y += 32;
    const char* toolHelp = tool == 0 ? "PLACE: pick a piece below, click board to lay it"
                         : tool == 1 ? "BLOCK: click a cell = no-placement (X)"
                         : tool == 2 ? "FIX: click a cell = fixed piece (current color)"
                                     : "ERASE: click a cell to clear it";
    DrawTextEx(font, toolHelp, Vector2{44, y}, 13.0f, 1.0f, Color{130, 146, 160, 255});
    y += 26;

    // Piece palette — every piece you've made; click one to select for PLACE.
    if (selPiece >= (int)ed.parts.size()) selPiece = (int)ed.parts.size() - 1;
    DrawTextEx(font, "PIECES", Vector2{44, y}, 14.0f, 1.2f, Color{150, 166, 180, 255});
    if (button({200, y - 4, 64, 22}, "ROTATE", false)) ed.rotatePart(selPiece);
    if (button({258, y - 4, 78, 22}, "DEL LAST", false)) {
        if (selPiece >= 0 && selPiece == (int)ed.parts.size() - 1) ed.removeLastPart();
        selPiece = (int)ed.parts.size() - 1;
    }
    y += 22;
    const int tile = 42;
    const int perRow = 6;
    for (int i = 0; i < (int)ed.parts.size(); ++i) {
        const int tx = 44 + (i % perRow) * (tile + 4);
        const int ty = (int)y + (i / perRow) * (tile + 4);
        Rectangle box = {(float)tx, (float)ty, (float)tile, (float)tile};
        const bool sel = (i == selPiece);
        DrawRectangleRounded(box, 0.14f, 4, Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(box, 0.14f, 4, sel ? 2.5f : 1.0f,
                                    sel ? cyan : Color{72, 94, 112, 255});
        // mini shape preview (current rotation), dimmed if already placed.
        const auto cells = ed.parts[i].rotatedCells();
        int mx = 0, my = 0;
        for (auto [dr, dc] : cells) { mx = std::max(mx, dc); my = std::max(my, dr); }
        const int span = std::max(1, std::max(mx, my) + 1);
        const float cs = (tile - 10.0f) / span;
        Color pcCol = theme::colorBadge(ed.parts[i].colorIndex);
        if (ed.parts[i].location.placed) pcCol = Color{(unsigned char)(pcCol.r/2),(unsigned char)(pcCol.g/2),(unsigned char)(pcCol.b/2),255};
        for (auto [dr, dc] : cells)
            DrawRectangleRounded(Rectangle{tx + 5 + dc * cs, (float)ty + 5 + dr * cs, cs - 1, cs - 1}, 0.2f, 3, pcCol);
        if (hit(box)) { hoverClickable = true; if (lc) selPiece = i; }
    }
    const int rows_ = (ed.parts.empty() ? 1 : static_cast<int>((ed.parts.size() + perRow - 1) / perRow));
    y += rows_ * (tile + 4) + 6;
    if (ed.parts.empty())
        DrawTextEx(font, "(make a piece below)", Vector2{44, y - tile + 6}, 14.0f, 1.0f,
                   Color{120, 134, 148, 255});

    // Make a piece — draw a shape, ADD it to the palette.
    DrawTextEx(font, "MAKE A PIECE", Vector2{44, y}, 14.0f, 1.2f, Color{174, 191, 203, 255});
    DrawRectangleRounded(Rectangle{186, y, 30, 15}, 0.6f, 6, cur);
    y += 20;
    const int pcs = 20, pgx = 56, pgy = static_cast<int>(y);
    for (int r = 0; r < kPaintN; ++r) {
        for (int c = 0; c < kPaintN; ++c) {
            Rectangle pc = {static_cast<float>(pgx + c * pcs), static_cast<float>(pgy + r * pcs), static_cast<float>(pcs - 3), (float)(pcs - 3)};
            const bool on = paint[r][c];
            DrawRectangleRounded(pc, 0.18f, 4, on ? cur : Color{30, 36, 46, 255});
            DrawRectangleRoundedLinesEx(pc, 0.18f, 4, 1.0f, on ? cyan : Color{70, 82, 98, 255});
            if (hit(pc)) { hoverClickable = true; if (lc) paint[r][c] = !paint[r][c]; }
        }
    }
    if (button({static_cast<float>(pgx + kPaintN * pcs + 12),  static_cast<float>(pgy) + 8, 92, 30}, "ADD", false)) {
        const std::size_t before = ed.parts.size();
        ed.addPart(paint, ed.currentColor);
        if (ed.parts.size() > before) {
            for (auto& row : paint) std::fill(row.begin(), row.end(), false);
            selPiece = static_cast<int>(ed.parts.size()) - 1;
            msg = "Piece added — pick PLACE and click the board.";
        } else {
            msg = "Draw a shape in the grid first.";
        }
    }
    y = pgy + kPaintN * pcs + 8;

    // Export destination + file name.
    const float ay = panel.y + panel.height - 44;
    const float exportBoxY = ay - 48.0f;
    DrawTextEx(font, "EXPORT NAME  (F2, saved to assets/levels/)", Vector2{44, exportBoxY - 18.0f},
               12.0f, 1.0f, Color{150, 166, 180, 255});
    const Rectangle exportBox = {44.0f, exportBoxY, 210.0f, 28.0f};
    const bool exportBoxOver = hit(exportBox);
    if (lc && exportBoxOver) exportNameFocused = true;
    else if (lc && !exportBoxOver) exportNameFocused = false;
    if (exportBoxOver) hoverClickable = true;
    DrawRectangleRounded(exportBox, 0.18f, 5, Color{24, 29, 39, 255});
    DrawRectangleRoundedLinesEx(exportBox, 0.18f, 5, exportNameFocused ? 2.0f : 1.0f,
                                exportNameFocused ? cyan
                                                  : exportBoxOver ? Color{120, 150, 170, 255}
                                                                  : Color{72, 94, 112, 255});
    const std::string shownStem = exportStem.empty() ? std::string("<name>") : exportStem;
    const Color stemColor = exportStem.empty() ? Color{110, 124, 138, 255} : text;
    DrawTextEx(font, shownStem.c_str(), Vector2{52.0f, exportBoxY + 5.0f}, 16.0f, 1.0f, stemColor);
    DrawTextEx(font, ".txt", Vector2{261.0f, exportBoxY + 5.0f}, 15.0f, 1.0f, Color{170, 184, 196, 255});

    // Action buttons (bottom of sidebar).
    if (button({44, ay, 84, 30}, "EXPORT", false)) {
        std::string err;
        const std::string path = exportEditorLevel(ed, exportStem, lastExportPath, err);
        if (path.empty()) msg = err;
        else {
            lastExportPath = path;
            msg = "Exported: " + path;
        }
    }
    if (button({134, ay, 84, 30}, "PLAY", false)) requestPlay();
    if (button({224, ay, 84, 30}, "MENU", false)) result = EditorResult::Menu;

    // ---- Board + clickable number boxes ----
    // Read dims AFTER the steppers/color count above (they may have resized the
    // board this frame) so the loops below never index a stale, larger grid.
    const int M = static_cast<int>(ed.board.rows);
    const int N = static_cast<int>(ed.board.cols);
    const int availW = sw - 360 - 40;
    const int availH = sh - 170;
    int cell = std::min(availW / std::max(1, N), availH / std::max(1, M));
    cell = std::max(26, std::min(cell, 60));
    const int boardX = 372 + (availW - N * cell) / 2;
    const int boardY = 120 + (availH - M * cell) / 2;
    const int nb = std::min(cell - 2, 30);  // number-box size

    // Board header + how-to.
    DrawTextEx(font, "BOARD", Vector2{static_cast<float>(boardX - nb - 6), 46.0f}, 22.0f, 1.2f, text);
    DrawTextEx(font, "Lay your pieces out to form a solution — the row/column",
               Vector2{static_cast<float>(boardX - nb - 6), 74.0f}, 14.0f, 1.0f, Color{150, 166, 180, 255});
    DrawTextEx(font, "numbers auto-fill; click a box to fine-tune. Right-click a cell to clear it.",
               Vector2{static_cast<float>(boardX - nb - 6), 94.0f}, 14.0f, 1.0f, Color{150, 166, 180, 255});

    // Hovered board cell.
    int hovR = -1, hovC = -1;
    if (mp.x >= boardX && mp.y >= boardY) {
        const int c = (static_cast<int>(mp.x) - boardX) / cell;
        const int r = (static_cast<int>(mp.y) - boardY) / cell;
        if (r >= 0 && r < M && c >= 0 && c < N) { hovR = r; hovC = c; }
    }

    // Auto-derived row/column numbers. Left-click = +1, right-click = -1 for
    // the current color, so the editor still supports explicit number-setting.
    auto numberBox = [&](const int x, const int yy, const int idx, const bool isRow) {
        const Rectangle r = {static_cast<float>(x), static_cast<float>(yy), static_cast<float>(nb), static_cast<float>(nb)};
        const bool over = hit(r);
        if (over) {
            hoverClickable = true;
            if (lc) ed.adjustConstraint(ed.currentColor, idx, isRow, +1);
            if (rc) ed.adjustConstraint(ed.currentColor, idx, isRow, -1);
        }
        DrawRectangleRounded(r, 0.25f, 5, Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(r, 0.25f, 5, over ? 2.0f : 1.0f, cur);
        char b[8]; std::snprintf(b, sizeof(b), "%u", ed.constraintAt(ed.currentColor, idx, isRow));
        editorCenteredText(font, b, x + nb / 2.0f, yy + nb / 2.0f, 17.0f, text);
    };
    for (int r = 0; r < M; ++r)
        numberBox(boardX - nb - 6, boardY + r * cell + (cell - nb) / 2, r, true);
    for (int c = 0; c < N; ++c)
        numberBox(boardX + c * cell + (cell - nb) / 2, boardY - nb - 6, c, false);

    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            Rectangle cellRect = {static_cast<float>(boardX + c * cell), static_cast<float>(boardY + r * cell),
                                  static_cast<float>(cell - 4), static_cast<float>(cell - 4)};
            const int v = ed.board._boardInfo[r][c];
            if (v == Board::CANNOT_PLACE) {
                DrawRectangleRounded(cellRect, 0.15f, 6, Color{28, 30, 38, 255});
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.0f, Color{200, 80, 80, 255});
                editorCenteredText(font, "X", cellRect.x + cellRect.width / 2,
                                   cellRect.y + cellRect.height / 2, 22.0f, Color{200, 80, 80, 255});
            } else if (Board::isCannotMove(v)) {
                const Color cc = theme::colorBadge(static_cast<unsigned>(Board::cannotMoveColor(v)));
                DrawRectangleRounded(cellRect, 0.15f, 6,
                    Color{static_cast<unsigned char>(cc.r / 3), static_cast<unsigned char>(cc.g / 3), static_cast<unsigned char>(cc.b / 3), 255});
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.5f, cc);
                editorCenteredText(font, "=", cellRect.x + cellRect.width / 2,
                                   cellRect.y + cellRect.height / 2, 22.0f, cc);
            } else if (Board::isOccupied(v)) {
                const int pi = Board::occupiedPartIndex(v);
                const Color cc = (pi >= 0 && pi < static_cast<int>(ed.parts.size()))
                               ? theme::colorBadge(ed.parts[pi].colorIndex) : Color{120,120,120,255};
                DrawRectangleRounded(cellRect, 0.15f, 6, cc);
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.0f, Color{240,248,250,160});
            } else {
                DrawRectangleRounded(cellRect, 0.15f, 6, Color{45, 50, 62, 255});
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.0f, Color{80, 90, 110, 255});
            }
        }
    }

    // Ghost preview of the selected piece under the cursor (PLACE tool).
    if (tool == 0 && hovR >= 0 && selPiece >= 0 && selPiece < static_cast<int>(ed.parts.size())
        && !ed.parts[selPiece].location.placed) {
        const bool ok = ed.board.canPlace(ed.parts[selPiece], hovR, hovC);
        const Color g = ok ? Color{80, 220, 100, 110} : Color{220, 80, 80, 110};
        for (auto [dr, dc] : ed.parts[selPiece].rotatedCells()) {
            const int rr = hovR + dr, ccol = hovC + dc;
            if (rr < 0 || ccol < 0 || rr >= M || ccol >= N) continue;
            DrawRectangleRounded(Rectangle{static_cast<float>(boardX + ccol * cell), static_cast<float>(boardY + rr * cell),
                                           static_cast<float>(cell - 4), static_cast<float>(cell - 4)}, 0.15f, 6, g);
        }
    } else if (hovR >= 0) {
        DrawRectangleRoundedLinesEx(Rectangle{static_cast<float>(boardX + hovC * cell), (float)(boardY + hovR * cell),
                                              static_cast<float>(cell - 4), (float)(cell - 4)}, 0.15f, 6, 2.0f, cyan);
    }

    // Clicks on the board.
    if (hovR >= 0) {
        hoverClickable = true;
        if (lc) {
            if (tool == 0) {
                if (selPiece < 0 || selPiece >= static_cast<int>(ed.parts.size())) msg = "Make/select a piece first.";
                else if (!ed.placePart(selPiece, hovR, hovC)) msg = "Can't place there.";
            } else if (tool == 1) ed.setBlocked(hovR, hovC);
            else if (tool == 2) ed.setFixed(hovR, hovC, ed.currentColor);
            else ed.clearCell(hovR, hovC);
        }
        if (rc) ed.clearCell(hovR, hovC);
    }

    if (!msg.empty())
        DrawTextEx(font, msg.c_str(), Vector2{static_cast<float>(boardX), (float)(boardY + M * cell + 16)},
                   16.0f, 1.0f, Color{255, 203, 126, 255});

    return result;
}

} // namespace

int main(int argc, char** argv) {
    Game game;
    std::string err;
    AppState appState = AppState::Menu;
    const std::string path = promptPath(argc, argv);
    if (!path.empty()) {
        if (!loadGame(path, game, err)) {
            std::cerr << err << std::endl;
            return 1;
        }
        appState = AppState::InGame;
    }

    constexpr int kScreenW = 960;
    constexpr int kScreenH = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(kScreenW, kScreenH, "Originium Circuit Repair");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    InitAudioDevice();
    Sound sndPickup = LoadSound("assets/sfx/pickup.mp3");
    Sound sndPlace  = LoadSound("assets/sfx/place.mp3");
    Sound sndSpin   = LoadSound("assets/sfx/spin.mp3");
    Sound sndWin    = LoadSound("assets/sfx/victory.mp3");

    int  prevHeld       = -1;
    bool prevWon        = false;
    int  prevPlacedCnt  = 0;
    int  prevRotateSum  = 0;
    float idleTimer     = 0.0f;
    float trayScroll    = 0.0f;   // tray vertical scroll offset (px); 0 = top
    bool  muted         = false;  // global sfx mute (M); persists across levels
    auto resetSoundBaseline = [&]() {
        prevHeld      = game.heldPartIdx;
        prevWon       = game.won;
        prevPlacedCnt = countPlaced(game);
        prevRotateSum = sumRotateCount(game);
        idleTimer     = 0.0f;
        trayScroll    = 0.0f;     // new level / restart starts scrolled to top
    };
    resetSoundBaseline();

    std::vector<LevelEntry> levels = findLevels();
    int selectedLevel = levels.empty() ? -1 : 0;

    std::vector<std::vector<bool>> paintGrid(kPaintN, std::vector<bool>(kPaintN, false));
    auto clearPaint = [&]() {
        for (auto& row : paintGrid) std::fill(row.begin(), row.end(), false);
    };

    {
        int editorSelPiece = -1;
        std::string editorMessage;
        std::string editorExportStem = nextDefaultExportStem();
        std::string editorLastExportPath;
        bool editorExportNameFocused = false;
        int editorTool = 0;
        Editor editor;
        std::string menuMessage;
        int ctxSel = 0;
        Vector2 ctxPos{};
        int ctxMenuIdx = -1;
        Font menuFont = LoadFontEx("assets/fonts/Exo2-Regular.ttf", 64, nullptr, 0);
        const bool hasMenuFont = IsFontValid(menuFont);
        if (hasMenuFont) {
            SetTextureFilter(menuFont.texture, TEXTURE_FILTER_BILINEAR);
        } else {
            menuFont = GetFontDefault();
        }
        Renderer renderer;
        while (!WindowShouldClose()) {
            const int sw = GetScreenWidth();
            const int sh = GetScreenHeight();

            if (appState != AppState::Editor) {
                std::string droppedPath;
                std::string droppedError;
                if (tryLoadDroppedLevel(game, droppedPath, droppedError)) {
                    appState = AppState::InGame;
                    resetSoundBaseline();
                    menuMessage.clear();
                    if (!droppedPath.empty()) {
                        game.statusMessage = "Loaded: " +
                                             std::filesystem::path(droppedPath).filename().string();
                    }
                } else if (!droppedError.empty()) {
                    if (appState == AppState::Menu) menuMessage = droppedError;
                    else game.statusMessage = droppedError;
                }
            }

            // Set to true by any branch when the cursor is over a clickable
            // element; applied as a pointing-hand cursor before EndDrawing.
            bool hoverClickable = false;

            BeginDrawing();
            if (appState == AppState::Menu) {
                const Vector2 mp = GetMousePosition();
                const bool lclick = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                const bool rclick = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
                const int hover = menuSelectionFromMouse(levels, selectedLevel, sw, sh);
                const Vector2 mouseDelta = GetMouseDelta();
                const bool mouseMoved = (mouseDelta.x != 0.0f) || (mouseDelta.y != 0.0f);
                const bool ctxOpen = (ctxMenuIdx >= 0 && ctxMenuIdx < static_cast<int>(levels.size()));
                if (!ctxOpen && hover >= 0) hoverClickable = true;

                if (!ctxOpen) {
                    if (hover >= 0 && mouseMoved) selectedLevel = hover;

                    if (!levels.empty()) {
                        if (IsKeyPressed(KEY_DOWN))
                            selectedLevel = (selectedLevel + 1) % static_cast<int>(levels.size());
                        if (IsKeyPressed(KEY_UP))
                            selectedLevel = (selectedLevel + static_cast<int>(levels.size()) - 1)
                                          % static_cast<int>(levels.size());
                    }

                    // Right-click → open the context menu for the hovered (or
                    // currently selected) level. Tolerant of clicking slightly
                    // off a row.
                    if (rclick) {
                        const int idx = hover >= 0 ? hover : selectedLevel;
                        if (idx >= 0 && idx < static_cast<int>(levels.size())) {
                            ctxMenuIdx = idx;
                            selectedLevel = idx;
                            ctxSel = 0;
                            ctxPos = mp;
                        }
                    }

                    // Right ARROW key → open the options menu beside the selected row.
                    if (IsKeyPressed(KEY_RIGHT) && !levels.empty() && selectedLevel >= 0) {
                        const int panelX = (sw - kMenuPanelW) / 2;
                        const int panelY = (sh - kMenuPanelH) / 2;
                        const int start = menuVisibleStart(selectedLevel, static_cast<int>(levels.size()));
                        const int localIdx = selectedLevel - start;
                        ctxMenuIdx = selectedLevel;
                        ctxSel = 0;
                        ctxPos = Vector2{static_cast<float>(panelX + kMenuPanelW - 188),
                                         static_cast<float>(panelY + kMenuListOffsetY + localIdx * kMenuRowH)};
                    }

                    const bool chooseByKey = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
                    const bool chooseByMouse = hover >= 0 && lclick;
                    if (chooseByMouse) selectedLevel = hover;
                    if (!levels.empty() && selectedLevel >= 0 && (chooseByKey || chooseByMouse)) {
                        if (loadGame(levels[selectedLevel].path.string(), game, menuMessage)) {
                            appState = AppState::InGame;
                            resetSoundBaseline();
                            menuMessage.clear();
                        }
                    }
                    if (IsKeyPressed(KEY_E)) {
                        editor = Editor();
                        editorTool = 0;
                        editorSelPiece = -1;
                        editorMessage.clear();
                        editorExportStem = nextDefaultExportStem();
                        editorLastExportPath.clear();
                        editorExportNameFocused = false;
                        clearPaint();
                        appState = AppState::Editor;
                    }
                }

                drawMenu(levels, selectedLevel, ctxOpen ? -1 : hover, menuMessage, menuFont, sw, sh);

                // OPTIONS button below the panel — opens the same menu with a
                // left click (works even without a right mouse button).
                if (!ctxOpen && !levels.empty() && selectedLevel >= 0) {
                    const int panelY = (sh - kMenuPanelH) / 2;
                    Rectangle ob = {static_cast<float>(sw / 2 - 86),
                                    static_cast<float>(panelY + kMenuPanelH + 8), 172, 30};
                    const bool over = CheckCollisionPointRec(mp, ob);
                    if (over) hoverClickable = true;
                    DrawRectangleRounded(ob, 0.3f, 6, over ? Color{40, 48, 60, 255}
                                                           : Color{26, 31, 41, 235});
                    DrawRectangleRoundedLinesEx(ob, 0.3f, 6, 1.5f, Color{109, 236, 218, 210});
                    editorCenteredText(menuFont, "OPTIONS",
                                       ob.x + ob.width / 2, ob.y + ob.height / 2, 14.0f,
                                       Color{232, 244, 245, 255});
                    if (over && lclick) {
                        ctxMenuIdx = selectedLevel;
                        ctxSel = 0;
                        ctxPos = Vector2{ob.x, ob.y - 2 * 34.0f - 6};
                    }
                }

                // Options menu (Play / Delete) overlay — mouse or keyboard.
                if (ctxOpen) {
                    constexpr int n = 2;
                    const float iw = 152.0f, ih = 34.0f;
                    float px = ctxPos.x, py = ctxPos.y;
                    if (px + iw > sw) px = sw - iw - 4;
                    if (py + ih * n > sh) py = sh - ih * n - 4;

                    // Keyboard navigation within the menu.
                    if (IsKeyPressed(KEY_DOWN)) ctxSel = (ctxSel + 1) % n;
                    if (IsKeyPressed(KEY_UP))   ctxSel = (ctxSel + n - 1) % n;

                    Rectangle bg = {px, py, iw, ih * n};
                    DrawRectangleRounded(bg, 0.12f, 6, Color{24, 29, 39, 248});
                    DrawRectangleRoundedLinesEx(bg, 0.12f, 6, 1.5f, Color{109, 236, 218, 210});
                    DrawTextEx(menuFont, prettyLevelName(levels[ctxMenuIdx].name).c_str(), Vector2{px + 12, py - 22},
                               14.0f, 1.0f, Color{174, 191, 203, 255});

                    // Run an item then close the menu.
                    bool done = false;
                    auto doItem = [&](int i) {
                        if (i == 0) {
                            if (loadGame(levels[ctxMenuIdx].path.string(), game, menuMessage)) {
                                appState = AppState::InGame;
                                resetSoundBaseline();
                                menuMessage.clear();
                            }
                        } else {
                            std::error_code ec;
                            std::filesystem::remove(levels[ctxMenuIdx].path, ec);
                            levels = findLevels();
                            if (selectedLevel >= static_cast<int>(levels.size()))
                                selectedLevel = levels.empty() ? -1 : static_cast<int>(levels.size()) - 1;
                            menuMessage = ec ? "Delete failed." : "Level deleted.";
                        }
                        done = true;
                    };

                    for (int i = 0; i < n; ++i) {
                        const char* items[] = {"Play", "Delete"};
                        Rectangle it = {px, py + i * ih, iw, ih};
                        const bool over = CheckCollisionPointRec(mp, it);
                        if (over) { ctxSel = i; hoverClickable = true; }  // hover updates keyboard selection
                        if (i == ctxSel) DrawRectangleRounded(it, 0.12f, 6, Color{40, 48, 60, 255});
                        editorCenteredText(menuFont, items[i], it.x + iw / 2, it.y + ih / 2, 17.0f,
                                           i == 1 ? Color{235, 140, 140, 255} : Color{232, 244, 245, 255});
                        if (over && lclick) doItem(i);
                    }
                    if (!done && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) doItem(ctxSel);

                    if (done) ctxMenuIdx = -1;
                    else if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_LEFT)
                             || (lclick && !CheckCollisionPointRec(mp, bg)))
                        ctxMenuIdx = -1;
                }
            } else if (appState == AppState::Editor) {
                const EditorResult res =
                    runEditor(editor, paintGrid, editorTool, editorSelPiece, editorMessage,
                              editorExportStem, editorLastExportPath, editorExportNameFocused,
                              menuFont, sw, sh, hoverClickable);
                if (res == EditorResult::Play) {
                    Board board;
                    std::vector<Part> parts;
                    editor.buildPlayableSnapshot(board, parts);
                    game.init(std::move(board), std::move(parts));
                    appState = AppState::InGame;
                    resetSoundBaseline();
                    editorMessage.clear();
                } else if (res == EditorResult::Menu) {
                    appState = AppState::Menu;
                    levels = findLevels();   // surface any just-exported level
                    if (selectedLevel < 0 && !levels.empty()) selectedLevel = 0;
                    editorMessage.clear();
                    editorExportNameFocused = false;
                }
            } else {
                constexpr float kHintDelaySec = 30.0f;
                // In-game quick buttons (top-right): MENU and RESTART.
                const Vector2 mpg = GetMousePosition();
                const bool lclickg = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                const float quickBtnX = static_cast<float>(sw - kQuickButtonRightMargin
                                                           - kQuickButtonWidth);
                const Rectangle menuBtn = {quickBtnX, static_cast<float>(kQuickButtonTop),
                                           static_cast<float>(kQuickButtonWidth),
                                           static_cast<float>(kQuickButtonHeight)};
                const Rectangle restartBtn = {
                    quickBtnX,
                    static_cast<float>(kQuickButtonTop + kQuickButtonHeight + kQuickButtonGap),
                    static_cast<float>(kQuickButtonWidth),
                    static_cast<float>(kQuickButtonHeight)
                };

                if (IsKeyPressed(KEY_N) || (lclickg && CheckCollisionPointRec(mpg, menuBtn))) {
                    appState = AppState::Menu;
                    levels = findLevels();
                    if (selectedLevel < 0 && !levels.empty()) selectedLevel = 0;
                    if (selectedLevel >= static_cast<int>(levels.size())) {
                        selectedLevel = levels.empty() ? -1 : 0;
                    }
                    menuMessage.clear();
                    drawMenu(levels, selectedLevel, -1, menuMessage, menuFont, sw, sh);
                    EndDrawing();
                    continue;
                }
                const bool restartClicked = lclickg && CheckCollisionPointRec(mpg, restartBtn);
                if (CheckCollisionPointRec(mpg, menuBtn) || CheckCollisionPointRec(mpg, restartBtn))
                    hoverClickable = true;

                // Tray scroll bounds (slot list can be taller than the viewport).
                const int trayViewH = std::max(kTraySlotHeight,
                                               sh - kTrayViewTop - kTrayBottomMargin);
                const int trayContentH = static_cast<int>(game.parts.size()) * kTraySlotHeight;
                const int trayMaxScroll = std::max(0, trayContentH - trayViewH);
                // Mouse wheel scrolls the tray when the pointer is over it.
                const Vector2 wmp = GetMousePosition();
                const bool overTray = wmp.x < static_cast<float>(kTrayViewX + kTrayViewW)
                                   && wmp.y >= static_cast<float>(kTrayViewTop)
                                   && wmp.y < static_cast<float>(kTrayViewTop + trayViewH);
                if (overTray) trayScroll -= GetMouseWheelMove() * kTrayScrollSpeed;
                trayScroll = std::max(0.0f, std::min(trayScroll, static_cast<float>(trayMaxScroll)));

                const Layout L = computeLayout(game, sw, sh, static_cast<int>(trayScroll));

                // M toggles a global sfx mute (handy for demos / oral exam).
                if (IsKeyPressed(KEY_M)) muted = !muted;

                // Mouse wheel rotates the held part when the pointer is over the
                // board (over the tray it scrolls instead, handled above). One
                // notch = one 90° turn, reusing the normal rotate path + sfx.
                if (!overTray && !game.isSolving() && game.heldPartIdx >= 0
                    && wmp.x >= static_cast<float>(L.boardX)
                    && GetMouseWheelMove() != 0.0f) {
                    game.update(Action::Rotate);
                }

                const Action a = Input::poll();
                if (game.isSolving()) {
                    // Auto-solve animation in flight: ignore all player input
                    // except a Reset (Backspace / RESTART), which cancels it.
                    if (a == Action::Reset || restartClicked) game.update(Action::Reset);
                } else {
                    if (a != Action::None) game.update(a);
                    if (restartClicked) game.update(Action::Reset);
                    else Input::pollMouse(L, game);
                }
                // Advance the step-by-step solve before reading placed/won below,
                // so each placement triggers the place sfx + resets the idle timer.
                game.advanceSolve(GetFrameTime());

                // Keep the keyboard-selected tray slot inside the viewport.
                if (game.cursorCol == Game::TRAY_COL && !game.parts.empty()) {
                    const int slotTop = game.cursorRow * kTraySlotHeight;
                    const int slotBot = slotTop + kTraySlotHeight;
                    if (slotTop < static_cast<int>(trayScroll))
                        trayScroll = static_cast<float>(slotTop);
                    else if (slotBot > static_cast<int>(trayScroll) + trayViewH)
                        trayScroll = static_cast<float>(slotBot - trayViewH);
                    trayScroll = std::max(0.0f, std::min(trayScroll, static_cast<float>(trayMaxScroll)));
                }

                const int  heldNow      = game.heldPartIdx;
                const bool wonNow       = game.won;
                const int  placedCntNow = countPlaced(game);
                const int  rotateSumNow = sumRotateCount(game);

                // One sound per frame, priority: win > place > pickup > spin.
                if (!muted) {
                    if (wonNow && !prevWon) {
                        if (IsSoundValid(sndWin)) PlaySound(sndWin);
                    } else if (placedCntNow > prevPlacedCnt) {
                        if (IsSoundValid(sndPlace)) PlaySound(sndPlace);
                    } else if (prevHeld < 0 && heldNow >= 0) {
                        if (IsSoundValid(sndPickup)) PlaySound(sndPickup);
                    } else if (rotateSumNow > prevRotateSum) {
                        if (IsSoundValid(sndSpin)) PlaySound(sndSpin);
                    }
                }

                // Phase 4: reveal the solution hint only after ~30s with no
                // placement change. Any progress (place/remove) or a win resets
                // the timer and hides the hint again.
                if (placedCntNow != prevPlacedCnt || wonNow) idleTimer = 0.0f;
                else idleTimer += GetFrameTime();
                game.setHintsVisible(!wonNow && idleTimer >= kHintDelaySec);

                prevHeld      = heldNow;
                prevWon       = wonNow;
                prevPlacedCnt = placedCntNow;
                prevRotateSum = rotateSumNow;

                renderer.draw(game, sw, sh, GetFrameTime(), static_cast<int>(trayScroll));

                // Draw the quick buttons on top of the game.
                auto qbtn = [&](Rectangle r, const char* label) {
                    const bool over = CheckCollisionPointRec(mpg, r);
                    DrawRectangleRounded(r, 0.25f, 6, over ? Color{40, 48, 60, 255}
                                                          : Color{26, 31, 41, 235});
                    DrawRectangleRoundedLinesEx(r, 0.25f, 6, 1.5f, Color{109, 236, 218, 255});
                    editorCenteredText(menuFont, label, r.x + r.width / 2, r.y + r.height / 2,
                                       14.0f, Color{232, 244, 245, 255});
                };
                qbtn(menuBtn, "MENU (N)");
                qbtn(restartBtn, "RESTART");

                // Mute indicator pill below the quick buttons.
                if (muted) {
                    Rectangle mb = {quickBtnX, restartBtn.y + kQuickButtonHeight + kQuickButtonGap,
                                    static_cast<float>(kQuickButtonWidth),
                                    static_cast<float>(kQuickButtonHeight)};
                    DrawRectangleRounded(mb, 0.25f, 6, Color{48, 33, 25, 235});
                    DrawRectangleRoundedLinesEx(mb, 0.25f, 6, 1.5f, Color{255, 176, 88, 220});
                    editorCenteredText(menuFont, "MUTED  (M)", mb.x + mb.width / 2,
                                       mb.y + mb.height / 2, 16.0f, Color{255, 203, 126, 255});
                }
            }
            SetMouseCursor(hoverClickable ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
            EndDrawing();
        }
        if (hasMenuFont) {
            UnloadFont(menuFont);
        }
    }

    UnloadSound(sndPickup);
    UnloadSound(sndPlace);
    UnloadSound(sndSpin);
    UnloadSound(sndWin);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
