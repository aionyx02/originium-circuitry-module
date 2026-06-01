# DEV_GUIDE.md

> 開發環境、build、執行、操作、測試的指令彙整。
> 規劃 / 配分 / 進度去 [plan.md](plan.md) ｜ [scoring.md](scoring.md) ｜ [STATUS.md](STATUS.md)。

---

## Requirements

- C++17 compiler
  - macOS：Apple clang（內建即可）
  - Windows：MinGW-w64（demo 機器目標）或 MSVC
- CMake ≥ 3.16
- raylib 5.5
  - 已裝（`find_package(raylib 5.5)` 命中）→ 直接連結
  - 沒裝 → CMake 透過 `FetchContent` 拉 5.5 source 自動 build，**第一次 configure 會花幾分鐘**

---

## Project Structure

```text
src/
  main.cpp                ← argv[1] / stdin 讀關卡路徑、開 raylib 視窗、main loop
  core/                   ← 純邏輯，不 include raylib
    Part.{h,cpp}          ← 零件形狀 / 顏色 / 旋轉 / 位置
    Board.{h,cpp}         ← 盤面狀態 / 每色每列每欄需求 / 固定格 / 不可放置格
    Parser.{h,cpp}        ← 關卡檔 → GameData（Board + vector<Part>）
    WinChecker.{h,cpp}    ← 勝利判定（純函數）
    Game.{h,cpp}          ← 持有 board / parts / selectedIdx，接 Action 餵 Renderer
  ui/                     ← raylib 相關，core/ 不依賴此目錄
    Input.{h,cpp}         ← raylib 鍵盤事件 → Action
    Renderer.{h,cpp}      ← 畫盤 / tray / 半透明預覽 / 勝利 banner
docs/
  io/                     ← 關卡測資 + 輸入輸出範例
    Example1.txt ... Example6.txt
    input-file-format.md
    sample-input-output.md
  plan.md / STATUS.md / LOG.md / scoring.md / DEV_GUIDE.md / ...
Framework/                ← 老師 skeleton，保留作對照
CMakeLists.txt
```

> 邊界規則：`core/` **不准** include `raylib.h`。新功能要動到繪圖/輸入就放 `ui/`，要動到規則/狀態就放 `core/`。

---

## Build Instructions

### macOS / Linux

```bash
cmake -S . -B build
cmake --build build
```

產出：`build/game`。

### Windows（demo target：MinGW-w64 + static）

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

CMake 在 `WIN32 AND MINGW` 下會自動加 `-static -static-libgcc -static-libstdc++`，產出單一 `game.exe`，乾淨教室機器不需要裝任何 runtime。

### 清乾淨重 build

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

CMake 設定（`CMakeLists.txt` 本身）改了才需要清；只改 .cpp/.h 用 `cmake --build build` 即可。

---

## Run Instructions

兩種讀取關卡路徑的方式（已在 `src/main.cpp` 實作）：

```bash
./build/game docs/io/Example1.txt          # argv[1]
./build/game                                # 互動式：印 "Enter level file path (leave blank for level menu): " 後讀 stdin
./build/game --menu                         # 直接進主選單（跳過 prompt）
```

錯誤行為（都不 crash）：

- 路徑不存在 → 印 `Cannot open file: <path>`，exit 1
- 檔案格式錯 → 印 `Parse error: ...`，exit 1

補充：

- 無參數 prompt 時直接按 Enter（空字串）→ 進主選單，不 exit
- 主選單 / 遊戲中可直接把 `.txt` 關卡拖進視窗載入

---

## Controls

| 鍵 | 動作 |
|---|---|
| `W / A / S / D` 或方向鍵 | 移動選中的零件 |
| `R` | 旋轉零件 |
| `Enter` / `Space` | 放置 |
| `Esc` | 拔起 |
| `Backspace` 或畫面右上 `RESTART` 鈕 | 重置盤面為初始狀態 |
| `F` | 自動解題（解出並填滿盤面） |
| `N` 或畫面右上 `MENU` 鈕 | 回主畫面 / 開新遊戲 |
| 視窗 X 鈕 | 關閉程式 |

> `Esc` 已被改寫成「拔起零件」（`SetExitKey(KEY_NULL)`），所以不會關視窗。遊戲內畫面右上常駐 `MENU` / `RESTART` 兩個按鈕；勝利 banner 也提示 `N`/`Backspace`。
> 主選單：在關卡列上**按滑鼠右鍵**會跳出選單（`Play` / `Delete`）。`Delete` 直接刪該關卡檔（`std::filesystem::remove`）；點選單外或按 `Esc` 關閉選單。左鍵 / Enter 仍是直接開始。

### 關卡編輯器（主選單按 `E` 進入，**滑鼠操作 / 排解答自動生成數字**）

設計概念：**把零件擺到盤面上排出一個解答，列/欄數字就自動算出來**；若想微調，也可直接點數字框增減。

| 操作 | 動作 |
|---|---|
| 側欄 ROWS/COLS/COLORS 的 `-`/`+` | 改盤面大小、顏色數 |
| MAKE A PIECE 網格左鍵 + `ADD` | 畫任意形狀 → 加入 PIECES 清單（自動選取） |
| PIECES 清單點任一零件 | 選取要擺的零件；`ROTATE` 旋轉、`DEL LAST` 刪最後加入的零件 |
| BOARD TOOL = `PLACE` + 點盤面 | 把選取的零件擺上盤面（綠/紅 ghost 預覽合不合法） |
| BOARD TOOL = `BLOCK`/`FIX`/`ERASE` + 點盤面 | 設不可放置格 / 固定零件 / 清除 |
| 盤面格子右鍵 | 清除該格（擺上的零件整個收回） |
| 盤面四周數字 | 依目前解答**自動顯示**；左鍵 +1、右鍵 -1 微調當前顏色的列/欄需求 |
| `EXPORT` / `PLAY` / `MENU`（或 `Esc`） | 匯出 `assets/levels/custom-N.txt` / 試玩 / 回選單 |
| `E` / `P` | 匯出 / 直接試玩快捷鍵 |

> 匯出時零件回到 tray（未放置），數字 = 你排的解答 → 保證可解。編輯 6 子項全到位。

---

## Test / Manual Check Instructions

目前沒有自動測試框架，全部用 `docs/io/Example*.txt` 手動驗證。每加完一個功能至少跑一次：

```bash
./build/game docs/io/Example1.txt
```

要勾的項目見 [STATUS.md](STATUS.md) 的 Checklist 與 [plan.md §5](plan.md) 的驗收清單。Phase 1 必驗：

- 視窗開、3×3 盤面 + tray 顯示
- 沒給路徑時 prompt 正常
- 錯誤路徑印訊息且不 crash
- wasd 切換 selection
- `R` 旋轉四方向循環
- `Enter` 放置 / `Esc` 拔起
- 重疊放置 → 印錯誤訊息、零件留在原地
- 全部擺對 → 出現 You Win banner

---

## Level File Format

完整定義見 [io/input-file-format.md](io/input-file-format.md)。摘要：

```text
C M N                          # 顏色數 / 列數 / 欄數
（以下重複 C 次，每個顏色一段）
  <M 個列需求數>
  <N 個欄需求數>
  X1                           # 該顏色的固定零件數
  <X1 行 "row col">
X2                             # 不可擺放格子數
<X2 行 "row col">
（以下重複直到 EOF）
  ColorIndex M2 N2
  <M2 行 0/1 陣列>             # 1 = 零件佔據，0 = 空
```

座標左上角為 `0 0`。最小範例：[io/Example1.txt](io/Example1.txt)（3×3 單色 + 兩零件）。

---

## Important Entry Points

| 檔 | 看什麼 |
|---|---|
| `src/main.cpp` | argv/stdin 讀路徑、raylib 視窗初始化、main loop |
| `src/core/Game.cpp` | `update(Action)`：把 Input 翻成盤面狀態變化 |
| `src/core/Parser.cpp` | 關卡檔解析；錯誤丟 `Parser::Error` |
| `src/core/Board.cpp` | `canPlace` / `place` / `remove` 規則 |
| `src/core/WinChecker.cpp` | 勝利條件（純函數，不持狀態） |
| `src/ui/Input.cpp` | raylib 鍵盤 → `Action` 對照表 |
| `src/ui/Renderer.cpp` | 盤面 / tray / 半透明預覽 / 勝利 banner 畫法 |

---

## Common Commands

```bash
# 開發迴圈：改 code → rebuild → 跑 Example1
cmake --build build && ./build/game docs/io/Example1.txt

# 換測資
./build/game docs/io/Example2.txt

# stdin 互動式（demo 模式）
./build/game

# 清乾淨重 build（只在改 CMakeLists.txt 時需要）
rm -rf build && cmake -S . -B build && cmake --build build
```

---

## Docs Tooling（dev-only，與遊戲 build 無關）

`scripts/docs-*.mjs` + `package.json` 是純文件檢查工具，**只在開發機跑**，零相依（只用 Node 內建，不需 `npm install`、不產 `node_modules`）。**遊戲 build 與 demo `.exe` 完全不需要 Node**（Hard Constraint #3）。

```bash
npm run docs:refresh          # 跑全部 guard（size / links / placeholders / narrative）
npm run docs:guard-size       # 單跑：現況小檔不得超過大小上限
npm run docs:new-log-day      # 開當日 log，如 docs/log/2026-06-01.md
```

文件拓撲與寫作規範見 [index.md](index.md) 與 [docs/CLAUDE.md](CLAUDE.md)。改完 `docs/` 後跑一次 `docs:refresh` 確認沒把長敘事塞進現況小檔、連結沒斷。
