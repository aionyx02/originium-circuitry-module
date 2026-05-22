# 開發計劃（Plan）

> 唯一的進度與決策來源。**只有 Phase 1 是詳細版**，其餘階段先列大綱，等做到再展開。

---

## 0. 概念（Concept）

做一個 native Windows `.exe`：玩家啟動後輸入關卡檔路徑，遊玩《明日方舟：終末地》的「源石電路修復」解謎遊戲。**單一執行檔 + assets/ 資料夾**就能在乾淨的教室電腦跑。

- **核心邏輯**：C++17，含 `Part` / `Board` class（老師明確要求）
- **GUI / 動畫 / 音效**：raylib 5.5
- **關卡檔格式**：純文字（教師格式，[io/input-file-format.md](io/input-file-format.md)） — *原 LOGa-trival.md（已併入 [LOG.md](LOG.md)「技術棧決策」entry）裡的 JSON 決策已推翻，理由：用 JSON 會讀不到助教 demo 測資，損失 2%+2%*
- **建置**：CMake + `FetchContent` 拉 raylib，不需在乾淨機器上裝任何 runtime
- **Windows 打包**：MinGW-w64 + `-static`，產出單一 exe

---

## 1. 評分策略（拿分順序）

目標：**程式 65% + 口頭 60%，超過 100 以 100 計**。先把該拿的分鎖死，再美化。

每個 phase 的目標都是「**做完這一段，比上一段多拿 X 分**」，而不是「做完一個模組」。

| 評分項目 | 配分 | 主要在哪個 Phase 拿到 |
|---|---:|---|
| 基本項目（Part / Board class + 讀單色檔） | 5% | Phase 1 |
| 遊戲流程（wasd/r/Esc/Enter + 勝利判定） | 15% | Phase 1 |
| 進階功能（重置、新遊戲、主畫面、雙色等） | 10% | Phase 3 |
| 自動解題（解 + hint + 30s 計時） | 10% | Phase 4 |
| 關卡設計器（編輯 + 匯出 + 試玩） | 10% | Phase 5 |
| 圖形介面（GUI + 動畫 + 滑鼠 + 圖 + 音） | 15% | Phase 2 |
| **程式實作小計** | **65%** | |
| 口頭：流暢度 + 熟悉度 | 30% | Phase 7（練習） |
| 口頭：AI 使用紀錄 | 30% | **每個 Phase 同步更新** [LOG.md](LOG.md) |

**AI 使用紀錄是 30% 大頭，且要從現在開始累積，不能最後一週才補。**

---

## 2. 階段總覽（Phase 0 ~ 7）

兩週時程，獨自開發前期、中後期有另一位夥伴加入（夥伴接 Solver 與 Editor 比較好切）。

| Phase | 內容 | 預期累積分數 | 估時 | 夥伴可接? |
|---|---|---:|---|---|
| 0 | 基礎設施：CMake + raylib + 目錄結構 + Hello-world 跑通 | — | 0.5 天 | — |
| 1 | **MVP：單色關卡可玩**（raylib 最小版 + parser + game loop + 勝利） | +20% | 3 天 | 否 |
| 2 | 視覺打磨：滑鼠拖曳、旋轉動畫、lerp、音效、素材 | +15% | 2–3 天 | 否（同步動 UI 易衝突） |
| 3 | 進階功能 + 雙色：重置、新遊戲、主畫面、雙色 parser/顯示 | +12% | 2 天 | ✅ 主畫面/關卡選擇 |
| 4 | 自動解題：backtracking solver + hint + 30s 計時 | +10% | 2 天 | ✅ 整段（與 UI 解耦） |
| 5 | 關卡設計器：編輯 + 匯出純文字 + 直接遊玩 | +10% | 2–3 天 | ✅ 整段 |
| 6 | Windows 打包 + 乾淨機器 dry run（中文檔名、高 DPI） | 鎖分用 | 1 天 | 否 |
| 7 | 美化 + 口頭報告準備（AI doc 收尾、流程練習） | 口頭 60% | 剩餘 | 一起練 |

> 「累積分數」是程式實作那 65% 內的進度；口頭 60% 不算在內。

**最壞情況**：Phase 0–4 跑完就能拿 ≥57% 程式分，再加口頭就有 80+。Phase 5–7 是加分與保險。

---

## 3. 跨階段恆定的約定（不重複寫）

- **不要 over-engineering**：solo 階段就用簡單的函數呼叫，不抽 `IRenderer` / `IInput` 介面（structure-design.md 提的抽象在團隊大時才划算）
- **保留教師 skeleton 的 class 名稱與欄位**：`Part` / `Board` 沿用 [Framework/Part.h](Framework/Part.h) [Framework/Board.h](Framework/Board.h) 的命名，避免老師看不出對應
- **`main.cpp` 必須讓使用者在執行時輸入路徑**：先用 `argv[1]`，沒給就互動式 `std::cin` 問。違反 = 整個讀檔分（≥6%）歸零
- **每完成一個分數項目立刻在 LOG.md 補一段**：寫 prompt、寫產出、寫你後來怎麼調整。不要堆到最後
- **每個 Phase 結束時用對應的 `docs/io/ExampleN.txt` 跑一次當驗收**

---

## 4. 待確認 / 待更新的事

- [x] ~~**LOG.md 需更新**~~：曾改名為 LOGa-trival.md，後續已 merge 進本檔 [LOG.md](LOG.md) 的「技術棧決策」entry（同日刪除原檔）；JSON 決策已在本檔 §0 註明推翻。
- [x] ~~**CLAUDE.md 還沒存在**~~：草稿在根目錄 `x.md`，待 stable 再 rename 為 `CLAUDE.md`。
- [ ] **夥伴何時加入** 還沒定。若在 Phase 3 前加入，要先給他讀本檔 + [LOG.md](LOG.md) + [scoring.md](scoring.md)
- [ ] **乾淨 Windows VM/實機** 要先借到。Phase 6 才開始找會太晚

---

## 5. Phase 1 詳細計劃 ⭐ （正在執行的就是這段）

### 目標
做完後，跑 `./game docs/io/Example1.txt`：
- 視窗開起來，顯示 3×3 盤面 + 兩個零件在 tray 區
- 鍵盤 `wasd` 移動目前選中的零件、`r` 旋轉、`Esc` 拔掉、`Enter` 放置
- 放到不能放的格子會印錯誤訊息（不 crash）
- 三個零件都擺好且每列每欄填滿數正確 → 跳「You Win」

### 預期分數（累積 20%）
- 基本項目 5%：Part class 1%、Board class 1%、讀單色檔 + 顯示 2%、能讀路徑 1%（其餘 1% 在助教 demo 測資）
- 遊戲流程 15%：移動 5%、旋轉 5%、Esc 拔掉 1%、Enter 放置 1%、勝利判定 1.5%、放置失敗訊息 1.5%

**先不做**（往後留）：動畫、滑鼠、音效、雙色、主畫面、選關、reset

### 目錄結構（Phase 1 結束時的樣子）

```
/
├── CMakeLists.txt          ← 取代 Framework/CMakeLists.txt
├── src/
│   ├── main.cpp            ← argv/stdin 拿路徑、開 raylib window
│   ├── core/
│   │   ├── Part.h          ← 從 Framework/Part.h 演進
│   │   ├── Part.cpp
│   │   ├── Board.h         ← 從 Framework/Board.h 演進
│   │   ├── Board.cpp
│   │   ├── Game.h          ← 主迴圈狀態：board、parts、selectedIdx
│   │   ├── Game.cpp
│   │   ├── Parser.h        ← 讀純文字格式
│   │   ├── Parser.cpp
│   │   ├── WinChecker.h
│   │   └── WinChecker.cpp
│   └── ui/
│       ├── Renderer.h      ← raylib 畫盤 + 零件 + tray + hints
│       ├── Renderer.cpp
│       ├── Input.h         ← 鍵盤事件 → Game action
│       └── Input.cpp
├── Framework/              ← 保留，給老師看「我們有用 skeleton」
└── docs/                   ← 所有文件（plan / STATUS / LOG / learning-notes / DEV_GUIDE / scoring / io / my-note 等）
```

> 為什麼還要 `ui/` 不直接寫在 main.cpp？因為 Phase 2 要加動畫與滑鼠，把 UI 集中後改起來不會碰到 core。但只開兩個檔（Renderer / Input），不抽 interface。

### 每個檔的 contract（簡述，code 留到實作時寫）

| 檔 | 負責什麼 | 公開介面要點 |
|---|---|---|
| `Part` | 一個零件的形狀、顏色、旋轉、位置 | `rotatedCells()` 回傳當前旋轉下被占據的格子 offsets |
| `Board` | 盤面狀態、每色每列/欄的需求數、固定格、不可放置格 | `canPlace(Part, row, col)`、`place(Part, row, col)`、`remove(Part)` |
| `Parser` | 純文字檔 → `Board + vector<Part>` | `static GameData parse(std::istream&)`，丟例外或回 `std::optional` |
| `WinChecker` | 判斷目前盤面是否勝利 | `static bool isWon(const Board&)`，純函數 |
| `Game` | 持有 board、parts、selectedIdx；接 Input、餵 Renderer | `update(Action)`、`render(Renderer&)` |
| `Renderer` | raylib 畫圖：盤格 / 零件 / tray / hint 數字 / win 訊息 | `void draw(const Game&)` |
| `Input` | poll raylib 鍵盤事件，輸出抽象 `Action`（MoveUp、Rotate、Place...） | `Action poll()` |
| `main` | 拿路徑、開 parser、開視窗、run loop | — |

### 執行順序（建議在 3 天內這樣切）

**Day 1**
1. Phase 0 收尾：CMake + raylib FetchContent + 空 main 跑出黑視窗
2. `Parser` 寫好 + 單元測試（手動丟 Example1.txt 看 stdout）
3. `Board` 與 `Part` 補齊 helpers（`rotatedCells`、`canPlace`、`place`、`remove`）

**Day 2**
4. `Game` + `Renderer` 雛形：能畫出盤面 + 零件在 tray，**還不能互動**
5. `Input` + Game.update：wasd 切換 selected、r 旋轉、零件在盤面格子間跳格
6. Enter 放置、Esc 拔掉

**Day 3**
7. 放置失敗訊息（盤外、重疊、不可放置格、固定格）
8. `WinChecker` + 顯示 You Win
9. 用 Example1.txt 走完整流程驗收
10. **補 LOG.md** 第一段（這三天用 AI 做了什麼）

### Phase 1 驗收清單（做完逐項打勾）

- [ ] `./game docs/io/Example1.txt` 能開、顯示 3×3 盤面 + 2 零件
- [ ] 沒給路徑時會問 `Enter level file path: ` 並讀檔
- [ ] 給錯路徑時印錯誤訊息且不 crash
- [ ] wasd 能在盤面與 tray 之間移動 selection
- [ ] r 旋轉零件 4 個方向會循環
- [ ] Enter 放置 / Esc 拔掉
- [ ] 放到重疊位置 → 印錯誤訊息，零件留在原地
- [ ] 全部擺對 → 跳 You Win
- [ ] **LOG.md 補完這個 Phase 的紀錄**
- [ ] `git commit` 一次（Phase 1 milestone）

### 不在 Phase 1 做的事（提前列出避免分心）

- 動畫（lerp 旋轉、跟游標）→ Phase 2
- 滑鼠拖曳 → Phase 2
- 音效、圖片素材 → Phase 2
- 雙色 → Phase 3
- 主畫面 / 選關 → Phase 3
- 重置 / 新遊戲 → Phase 3
- Auto solver → Phase 4
- Editor → Phase 5

---

## 6. 其他階段（只列標題，等做到再展開）

- Phase 2：raylib 視覺打磨（細節等 Phase 1 做完再寫）
- Phase 3：進階功能 + 雙色
- Phase 4：Auto Solver
- Phase 5：關卡編輯器
- Phase 6：Windows 打包 + dry run
- Phase 7：美化 + 口頭報告

---

## 7. 進度紀錄方式

- **每個 Phase 結束**：在這份檔最下方加一行 `## Phase N done — YYYY-MM-DD`，列實際拿到的分數估計與遇到的卡點
- **AI 使用紀錄**：去 [LOG.md](LOG.md) 寫
- **臨時想到的小事**：去 [my-note.md](my-note.md) 寫，不要污染這份檔

---

## 進度時間軸

<!-- 完成一個 Phase 就在這裡加一條 -->

## Phase 0 done — 2026-05-22

- 完成項目：根 [CMakeLists.txt](CMakeLists.txt)（`find_package(raylib 5.5)` + FetchContent 後備）、[src/main.cpp](src/main.cpp) 空 window、[src/core/](src/core/) + [src/ui/](src/ui/) 空目錄、[.gitignore](.gitignore) 補強、[LOG.md](LOG.md) 第一筆。
- 拿到分數：0（Phase 0 是基礎建設）。
- 卡點：開發機沒裝 cmake，brew 補裝 cmake 4.3.2；raylib 5.5 已預先 brew 裝過，`find_package` 直接命中。
- 驗證：`cmake --build build` 通；`./build/game` 在 Apple M3 開出視窗，raylib log 顯示 MSAA x4 + HighDPI + OpenGL 4.1 Metal。
- 下一步：Phase 1 Day 1 step 2 — Parser 寫 + Example1.txt 跑 stdout 驗證。

## Phase 1 done — 2026-05-22

- 完成項目（一次寫完，三天計劃合併到一個 session）：
  - [src/core/](src/core/)：Part / Board / Parser / WinChecker / Game 全套
  - [src/ui/](src/ui/)：Input（鍵盤 → Action）、Renderer（盤面 + tray + 半透明預覽 + 勝利 banner）
  - [src/main.cpp](src/main.cpp)：argv[1] 優先 / stdin 後備、`SetExitKey(KEY_NULL)` 讓 Esc 給遊戲用
  - [CMakeLists.txt](CMakeLists.txt)：加入 7 個新 cpp
- 自動驗證通過（已測）：
  - build 0 warning 0 error
  - `./build/game docs/io/Example1.txt` 視窗開、raylib log 正常
  - bad path 印錯誤訊息、exit 1、不 crash
  - stdin 模式 prompt + 讀檔成功
- **手動驗證待辦（使用者親自跑）**：plan §5 checklist 第 4~8 項：wasd / r 旋轉循環 / Enter 放置 / Esc 拔掉 / 重疊錯誤 / You Win banner
- 卡點：無。skeleton 的 `enum BoardInfo` 加 `: int` 是唯一對 Framework 的改動，是為了讓 `OCCUPIED + partIndex`、`CANNOT_MOVE - color` 等任意 int 值在 C++17 下合法
- 預估拿到分數：+20%（基本 5% + 遊戲流程 15%），等手動驗證後確認
- 下一步：使用者跑一輪 Example1.txt，OK 就 commit + 進 Phase 2
