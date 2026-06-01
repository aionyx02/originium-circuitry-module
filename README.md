# Originium Circuit Repair

《明日方舟：終末地》「源石電路修復」小遊戲的 C++ / raylib 實作。

玩家需要把不同形狀、不同顏色的零件放到盤面上，讓每一列、每一欄的顏色數量都符合周圍提示。玩法接近 Nonogram，但只檢查每列/欄的格子數量，不要求同色格子連續。

## Features

- 可從命令列、啟動後輸入路徑、關卡選單，或拖放 `.txt` 檔載入純文字關卡
- 支援完整遊戲流程：選取零件、移動、旋轉、放置、拔起、重玩與過關判定
- 支援單色與雙色關卡，盤面會顯示列/欄需求、目前進度、固定格與不可放置格
- 放置失敗會顯示錯誤訊息與紅框提示
- 可用鍵盤操作，也可直接用滑鼠從 tray 拖曳零件到盤面
- 內建求解器：`F` 可自動填入一組解，30 秒未完成時會顯示半透明提示
- 內建關卡編輯器：可調盤面大小、顏色數、零件、不可放置格、固定格與列/欄數字，並匯出純文字關卡
- 介面包含主畫面、關卡選擇、平滑移動、旋轉動畫、程序材質、陰影與音效回饋
- 使用 CMake 建置；raylib 5.5 可由 CMake 自動下載與編譯

目前進度與配分追蹤請看 [docs/STATUS.md](docs/STATUS.md)。

## Requirements

- C++17 compiler
- CMake 3.16+
- raylib 5.5

如果本機沒有安裝 raylib，CMake 會在第一次 configure 時自動下載並建置 raylib 5.5。

## Build

```bash
cmake -S . -B build
cmake --build build
```

產物：

- macOS / Linux: `build/game`
- Windows: `build/game.exe`

Windows MinGW 環境下會自動使用 static link，方便 demo 時帶著執行檔與 `assets/` 直接跑。

## Run

使用命令列參數載入關卡：

```bash
./build/game assets/levels/Example1.txt
```

或直接啟動，依提示輸入關卡路徑：

```bash
./build/game
```

程式會印出：

```text
Enter level file path (leave blank for level menu):
```

也可以直接進入關卡選單：

```bash
./build/game --menu
```

可用測資位於 [assets/levels](assets/levels)，例如：

```bash
./build/game assets/levels/Example1.txt
./build/game assets/levels/Example2.txt
./build/game assets/levels/Example5.txt
```

## Controls

### Menu

| 操作 | 動作 |
|---|---|
| `Up` / `Down` | 選擇關卡 |
| `Enter` / `Space` / 滑鼠左鍵 | 開始選中的關卡 |
| `E` | 開啟關卡編輯器 |
| `Right` / 對關卡按滑鼠右鍵 | 開啟關卡選項 |
| 拖放 `.txt` 檔到視窗 | 直接載入該關卡 |

### Game

| 操作 | 動作 |
|---|---|
| `W` / `A` / `S` / `D` | 移動目前選取的零件 |
| 方向鍵 | 移動目前選取的零件 |
| `R` | 旋轉零件 |
| `Enter` / `Space` | 放置或選取零件 |
| `Esc` | 拔起零件 / 取消持有 |
| `Backspace` | 重玩目前關卡 |
| `F` | 自動解題並顯示一組解 |
| `N` | 回到關卡選單 |
| 滑鼠左鍵 | 從 tray 選取、在盤面放置或取起零件 |
| 滑鼠右鍵 | 取消持有，或從盤面拔起零件 |
| 拖放 `.txt` 檔到視窗 | 不關程式載入新關卡 |

`Esc` 在遊戲中用來拔起零件，不會關閉視窗。

### Editor

| 操作 | 動作 |
|---|---|
| 滑鼠左鍵 | 使用目前工具編輯盤面或點選按鈕 |
| `R` | 旋轉目前選中的零件 |
| `E` | 匯出到 `assets/levels/<名稱>.txt` |
| `F2` | 編輯匯出檔名 |
| `P` | 直接遊玩目前設計 |
| `Esc` | 回到關卡選單 |

## Level Format

關卡使用純文字格式。完整說明請看 [docs/io/input-file-format.md](docs/io/input-file-format.md)。

格式摘要：

```text
C M N

<color 0 row requirements>
<color 0 column requirements>
X1
<fixed cells for color 0>

... repeat for each color ...

X2
<blocked cells>

ColorIndex M2 N2
<piece shape matrix>

... repeat pieces until EOF ...
```

其中：

- `C` 是顏色數量
- `M N` 是盤面列數與欄數
- row / column requirements 是每個顏色在每列、每欄需要填滿的格數
- fixed cells 是預先放在盤面上的不可移動色塊
- blocked cells 是不能放置任何零件的格子
- piece shape matrix 使用 `1` 表示零件佔據格，`0` 表示空格

## Project Structure

```text
src/
  main.cpp              # 啟動、讀關卡路徑、raylib main loop
  core/                 # 遊戲邏輯，不依賴 raylib
    Board.*             # 盤面狀態、放置規則、需求數字
    Part.*              # 零件形狀、顏色、旋轉與位置
    Parser.*            # 純文字關卡檔解析
    LevelWriter.*       # 編輯器匯出的純文字關卡寫入
    Game.*              # 遊戲狀態與操作更新
    WinChecker.*        # 勝利判定
    Solver.*            # 自動解題與提示資料
    Editor.*            # 關卡編輯器資料模型
  ui/                   # raylib 輸入與繪圖
    Input.*
    Renderer.*
assets/
  fonts/
  levels/               # 內建與編輯器匯出的關卡
  sfx/
docs/
  io/                   # 輸入格式說明
  issues/               # Phase issue drafts
  STATUS.md             # 目前進度
  DEV_GUIDE.md          # 開發指令與驗收方式
```

## Development Notes

常用開發迴圈：

```bash
cmake --build build
./build/game assets/levels/Example1.txt
```

清乾淨重建：

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

更多開發細節、手動測試項目與入口檔案說明請看 [docs/DEV_GUIDE.md](docs/DEV_GUIDE.md)。

## Documentation

- [docs/game-rule.md](docs/game-rule.md): 遊戲規則整理
- [docs/io/input-file-format.md](docs/io/input-file-format.md): 關卡檔格式
- [docs/DEV_GUIDE.md](docs/DEV_GUIDE.md): build、run、測試與開發指南
- [docs/STATUS.md](docs/STATUS.md): 目前完成度與評分 checklist
- [docs/plan.md](docs/plan.md): 階段計劃與評分策略
- [docs/scoring.md](docs/scoring.md): 配分整理
