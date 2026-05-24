# 開發計劃（Plan）

> 唯一的進度與決策來源。**Phase 1（§5）與 Phase 2（§6）是詳細版**，其餘階段先列大綱，等做到再展開。

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

## 6. Phase 2 詳細計劃 ⭐ （正在執行）

### 目標

把 Phase 1 keyboard-only 可玩的單色版本，加上：
- **旋轉動畫**（5%）+ **跟游標連續移動**（5%）→ 圖形介面大頭兩項
- **GUI 細節**（2.5%）：圓角、漸層、陰影
- **程序材質**（1.5%）：cell 上方高光帶 + 下方暗邊
- **音效**（1%）：4 個 jsfxr 生的 wav
- **進階：旋轉/移動中心不為空**（1%）：每零件有 pivot cell
- 鍵盤路徑全保留；補上**滑鼠拖移 + 左鍵放置**（rubric 完整性，不重複加分）

### 預期分數（累積 +15%）

| 項 | 配分 | Day |
|---|---:|---|
| 旋轉動畫 | 5% | Day 1 |
| 跟游標連續移動 | 5% | Day 1 |
| 進階：旋轉/移動中心 | 1% | Day 1 |
| GUI 細節 | 2.5% | Day 2a |
| 程序材質 | 1.5% | Day 2a |
| 音效 | 1% | Day 2c |

> Day 2b（滑鼠）不直接加分，但 [scoring.md:47](scoring.md#L47) 與 [:50](scoring.md#L50) 列出「或滑鼠拖移 / 左鍵放置」，做了完整性更高。

### 策略決定

- **材質**：純程序材質（不引入 png 資產）— 避免 Windows 中文路徑 + 打包風險。報告講「procedural texture 也是『特別材質』」。
- **音效**：使用者用 [sfxr.me](https://sfxr.me/) 生 4 個短 wav 放 `assets/sfx/`。
- **動畫狀態**：加在 `Part` 中（5 個 float + 1 個 bool 旋轉計數），lerp 邏輯在 Renderer。
- **Cursor 語意改變**（Day 1.6）：從「= bbox 左上角」改成「= pivot cell 目標」，這樣旋轉時 pivot 螢幕位置不動。canPlace API 保留，Game::syncHeldLocation 在內部換算 bbox top-left。

### Day 切分

**Day 1** — 動畫骨架（commit `0fcfcd4`，已完成）
1. Part 加 animation fields + `centerCellRow/Col` + `rotateCount`
2. Game::update Action::Rotate 順便 `++rotateCount`
3. Game::syncHeldLocation 用 pivot offset 算 bbox top-left
4. Game::handlePlace 從盤面拔零件時把 cursor snap 到 pivot
5. Renderer::animateParts 每 frame lerp（`1 - exp(-dt*k)`）
6. Renderer::drawPartAnimated 用 `DrawRectanglePro` + 自訂 origin 繞 pivot 旋轉、中心格亮色

**Day 2a** — 視覺 polish + 程序材質（代碼已寫、待驗收）
1. Renderer 加 `drawRoundedCell` / `drawRoundedRect` helper
2. 板面空格 / tray slot / win banner 全部改圓角
3. 全屏背景漸層
4. drawPartAnimated 每 cell 加上方高光帶（+70/ch）+ 下方 bevel（÷2）
5. drawPartShadow 兩 pass

**Day 2b** — 滑鼠（待開工）
- Input 擴成「鍵盤 Action + 滑鼠事件」
- Tray hover 偵測 + 左鍵撿起
- Drag follow（持有時 currentCenterX/Y 跟滑鼠位置 lerp）
- 左鍵放置 / 右鍵 cancel

**Day 2c** — 音效（待開工）
- 使用者生 4 個 wav 放 `assets/sfx/`
- `LoadSound` + 4 個觸發點（pickup / place / rotate / win）
- `CMakeLists.txt` 加 install 規則把 assets/ 跟 exe 一起出貨

### 驗收清單

- [x] 移動有平滑動畫（不是瞬移）
- [x] 旋轉「轉過去」（按 R 4 次方向一致 CW）
- [x] Pivot cell 是亮色、旋轉時螢幕位置不動
- [x] 圓角 + 漸層 + 程序材質視覺（Day 2a，commit `6f7abcb`）
- [x] Drop shadow（Day 2a，commit `6f7abcb`）
- [x] 滑鼠 hover / drag（pixel-perfect、從 tray 撿起即跟手）/ 左鍵放置 / 右鍵 cancel（Day 2b，2026-05-24 目視驗收通過）
- [ ] 4 個音效（Day 2c）
- [x] Phase 1 鍵盤流程零退化

### 不在 Phase 2 做的事

- 雙色 → Phase 3
- 主畫面 / 選關 / reset → Phase 3
- Auto solver → Phase 4
- Editor → Phase 5
- Windows 打包 → Phase 6

---

## 7. 其他階段（只列標題，等做到再展開）

- Phase 3：進階功能 + 雙色
- Phase 4：Auto Solver
- Phase 5：關卡編輯器
- Phase 6：Windows 打包 + dry run
- Phase 7：美化 + 口頭報告

---

## 8. 進度紀錄方式

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
- **手動驗證通過（2026-05-22 補測）**：wasd 移動 / R 4 方向循環 / Enter 放置 / Esc 雙語意（持有→還原、盤面→拔起）/ 重疊紅框 + 錯誤訊息 / 全擺對跳 You Win banner — 全部 8 項 OK
- 卡點：無。skeleton 的 `enum BoardInfo` 加 `: int` 是唯一對 Framework 的改動，是為了讓 `OCCUPIED + partIndex`、`CANNOT_MOVE - color` 等任意 int 值在 C++17 下合法
- 拿到分數：**+20%**（基本 5% + 遊戲流程 15%），銀行存款 21.5% / 65%
- 下一步：Phase 2 視覺打磨（動畫 + 滑鼠 + 程序材質 + 音效，預期 +15%）

## Phase 2 Day 1 done — 2026-05-22

- 完成項目（commit `0fcfcd4`）：
  - [src/core/Part.h](src/core/Part.h)：加 `centerCellRow/Col`、`rotateCount`、5 個 visual fields（`currentCenterX/Y/Angle/Scale` + `visualInitialized`）+ `computeCenterCell` / `rotatedCenterCell` 方法
  - [src/core/Part.cpp](src/core/Part.cpp)：實作 `computeCenterCell`（離形心最近占據格）與 `rotatedCenterCell`（套用旋轉公式）
  - [src/core/Parser.cpp](src/core/Parser.cpp)：parser 載入後 call `p.computeCenterCell()`
  - [src/core/Game.cpp](src/core/Game.cpp)：(a) `Action::Rotate` 順便 `++rotateCount`（單調累加 CW，繞過 shortest-arc 邊角 bug）(b) `syncHeldLocation` 改用 pivot offset 算 bbox top-left（cursor 語意 = pivot target）(c) `handlePlace` 從盤面拔零件時 snap cursor 到 pivot 避免視覺跳動
  - [src/ui/Renderer.h](src/ui/Renderer.h) / [Renderer.cpp](src/ui/Renderer.cpp)：簽名加 `dt`；新增 `animateParts`（每 frame `1 - exp(-dt*k)` lerp）、`computeTarget`（依 placed/held/tray 算目標）、`drawPartAnimated`（`DrawRectanglePro` 以 pivot cell 為樞紐 + 中心格 +120/ch 亮色）
  - [src/main.cpp](src/main.cpp)：傳 `GetFrameTime()` 進 renderer
- 拿到分數：**+11%**
  - 旋轉動畫 5%（[scoring.md:93](scoring.md#L93)）
  - 零件跟游標連續移動 5%（[scoring.md:94](scoring.md#L94)）
  - 進階：旋轉/移動中心不為空 1%（[scoring.md:61](scoring.md#L61)）
- 銀行存款：21.5 → **32.5%** / 65%
- 卡點與設計演進：
  - Day 1 第一版用 `std::fmod` 做 shortest-arc 角度 lerp，使用者驗證時 D 項（4 次旋轉方向一致）報失敗 → 換成 `rotateCount` 單調累加，避免 mod 邊角
  - Day 1.5 加白色圓點當 pivot marker，使用者要求改成「中心格亮色覆蓋」+ 旋轉時 pivot 應該完全不動 → Day 1.6 把 cursor 語意從 bbox top-left 改成 pivot target，內部用 `rotatedCenterCell` offset 換算
- 驗證：8 項使用者目視驗收全綠（中心格亮、旋轉 pivot 不動、4 方向 CW、拔起不跳、Phase 1 流程零退化）
- 下一步：Day 2a 視覺 polish + 程序材質（圓角 + 漸層 + 高光帶 + drop shadow） — 代碼已寫、待使用者目視驗收

## Phase 2 Day 2a done — 2026-05-22

- 完成項目（[src/ui/Renderer.cpp](src/ui/Renderer.cpp) 單檔重寫部分函式 +98 行）：
  - 新 helper `drawRoundedCell` / `drawRoundedRect`（用 `DrawRectangleRounded` + `DrawRectangleRoundedLinesEx`）
  - 板面空格（EMPTY/OCCUPIED 底）、CANNOT_PLACE 的 X 格、CANNOT_MOVE 固定零件格、tray slot 背景、Win banner 全部改圓角
  - `Renderer::draw` 加 `DrawRectangleGradientV` 全屏垂直漸層底
  - 新 `drawPartShadow`（offset 4/5px、alpha 110、純黑），`drawParts` 改兩 pass — 先掃所有 placed/held-on-board 畫 shadow、再掃畫所有 part 本體，shadow 不會疊到別的 part
  - `drawPartAnimated` 加：每 cell 上方高光帶（30% 高、左右內縮 6%、+70/ch 色 alpha 170）+ 下方 bevel（10% 高、純色 ÷2 alpha 220）— 都用 `DrawRectanglePro` 跟 cell 一起旋轉
- 拿到分數：**+4%**
  - 實作 GUI 2.5%（[scoring.md:90](scoring.md#L90)）
  - 使用圖片或特別材質 1.5%（[scoring.md:92](scoring.md#L92)）— 程序材質：高光帶 + bevel 構成 cell 的「鏡面/金屬」質感
- 銀行存款：32.5 → **36.5%** / 65%
- 驗證：使用者跑 `./build/game docs/io/Example1.txt` 手動驗收 8 項全綠 — 背景漸層、板面圓角、tray 圓角、cell 程序材質、drop shadow、中心格仍亮、Win banner 圓角、Phase 1 流程零退化
- 卡點：無。一次寫一次過、smoke OK、目視驗收通過
- 下一步：Day 2b 滑鼠（tray hover/拖曳/左鍵放置/右鍵 cancel） → Day 2c 音效（4 個 wav 接 raylib LoadSound）

## Phase 2 Day 2b done — 2026-05-22（代碼）+ 2026-05-24（目視驗收通過 + pixel-perfect 二/三輪 polish）

- 完成項目：
  - [src/ui/Renderer.h](src/ui/Renderer.h) 把 `Layout` struct + `computeLayout` + `kTraySlotHeight/Width` 從 anon namespace 提到 public header（讓 Input 也能用）
  - [src/ui/Renderer.cpp](src/ui/Renderer.cpp) 移除重複定義、`computeLayout` 改為 public 自由函式
  - [src/core/Game.h](src/core/Game.h) + [Game.cpp](src/core/Game.cpp)：新 `setCursor(int row, int col, bool isTray)` cursor 原語 — 設位置 + clampCursor + syncHeldLocation
  - [src/ui/Input.h](src/ui/Input.h) + [Input.cpp](src/ui/Input.cpp)：新 `pollMouse(const Layout& L, Game& g)` — 板面 hit-test、tray hit-test、持有時 cursor 跟滑鼠 cell（drag follow）、左鍵走 `Action::Place`（pick/place 一鍵搞定）、右鍵走 `Action::Remove`（cancel / 拔起）
  - [src/main.cpp](src/main.cpp)：每 frame 算 Layout、`Input::poll()` 後立刻 `Input::pollMouse(L, game)`
- 設計選擇（C 方案，使用者於 plan 階段拍板）：
  - 不擴 `Action` enum、不破 Input→Action 模型
  - Game 提供 cursor 原語（單一 `setCursor`），Input 翻譯滑鼠座標 → setCursor + 既有 Action 流程
  - 板面 hit-test 用 `(mouseX - boardX) / cellSize` 整除；tray 用 `(mouseY - trayY) / kTraySlotHeight`
  - 持有時左鍵點 tray：no-op（plan 中使用者選的方案 — 不要意外丟回 tray）
  - 板面已放零件被左鍵點到：未持有時撿起、持有時嘗試放置（既有 `handlePlace` 邏輯）
  - drag follow 三輪迭代：
    1. 第一版 cell-quantized — 使用者覺得卡頓
    2. 第二版 pixel-perfect on board（Game 加 `mouseControlling` flag、Renderer 在 held + on board + mouseControlling 時 target = `GetMousePosition()` + currentCenterX/Y 直接 snap 不 lerp；放開時 part 變 placed、走既有 lerp 從 mouse pixel 滑到 cell center「落框感」）
    3. 第三版（2026-05-24）pixel-perfect 從 tray 撿起起算 — 把 mouseDrag 條件從「on board + held」放寬成「held」，點 tray slot 那一 frame 就立刻跟手 + scale 1.0
  - cursor 仍是 cell-quantized — 紅綠預覽框 + canPlace 仍用 cell；視覺與 cursor 解耦：「框框瞄準、視覺跟手」
- 拿到分數：**+0%**（rubric 完整性而已，wasd/Enter 已涵蓋滑鼠分項）
- 銀行存款：36.5% / 65%（不變）
- 卡點：無。build 0 warning 0 error、smoke OK
- **使用者目視驗收（2026-05-24 通過）**：
  - [x] 滑鼠移到 tray 第 N 個 slot → 左鍵 → 撿起該 part（且**從 tray 撿起那一 frame 就跟手** — 第三版補的）
  - [x] 撿起後移到板面 → 持有零件**緊貼游標**（pixel-perfect、無卡頓）
  - [x] 持有時左鍵點空格（合法）→ 放置成功 + 從 mouse pixel 平滑滑落 cell center；點不合法格 → 紅框 + 錯誤訊息
  - [x] 持有時右鍵 → 退回 tray
  - [x] 未持有時左鍵點板面上已放零件 → 撿起來
  - [x] 未持有時右鍵點板面上已放零件 → 拔到 tray
  - [x] 持有時左鍵點 tray slot → 無動作（不會意外丟）
  - [x] mouse drag 中按 R → 零件繼續跟手、旋轉動畫保留
  - [x] mouse drag 中按 W → 切回 keyboard mode、零件即刻 lerp 到 cursor cell
  - [x] 鍵盤路徑（wasd/R/Enter/Esc）跟之前一樣可用、零退化
- 下一步：Day 2c 音效（使用者已有 sound1.wav + victory.mp3、需備齊 pickup / place / rotate / win 4 個 wav）
