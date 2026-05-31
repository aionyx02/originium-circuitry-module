# 開發計劃（Plan）

> 唯一的進度與決策來源。**Phase 1（§5）、Phase 2（§6）、Phase 4（§9）是詳細版**，其餘階段先列大綱，等做到再展開。

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
- [x] 滑鼠 hover / drag（pixel-perfect、從 tray 撿起即跟手）/ 左鍵放置 / 右鍵 cancel（Day 2b，commit `59401b4`、2026-05-24 目視驗收通過）
- [x] 4 個音效（Day 2c，2026-05-24 代碼完成 + smoke 4 個 sound 全 load successfully + 手動驗收通過）
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

- **每個 Phase / 里程碑結束**：在下方「進度時間軸」表加一列（日期 / commit / 分數 / 一句話）。詳細過程不寫這裡。
- **AI 使用紀錄 & 詳細過程**：去 [docs/log/](log/) 當月檔 append（寫法見 [CLAUDE.md §Log 寫法](CLAUDE.md#log-寫法)）。2026-05-31 前的歷史在已封存的 [Log.md](Log.md)。
- **現況（到哪、勾哪些分）**：去 [STATUS.md](STATUS.md)。
- **臨時想到的小事**：去 [my-note.md](my-note.md)，不要污染這份檔。

---

## 9. Phase 4 詳細計劃（自動解題，進行中）

### 目標與配分（共 10%）

| 項 | 配分 | 增量 |
|---|---:|---|
| 自動解題並顯示一個解 | 1%（[scoring.md:67](scoring.md#L67)） | 1 |
| 解開單色測資（demo day） | +2%（:68） | 1 |
| 解開雙色測資（demo day） | +2%（:69） | 1 |
| 依解答顯示半透明提示 | 2.5%（:70） | 2 |
| 30 秒未解才顯示提示 | 2.5%（:71） | 2 |

### 設計

- **核心 `src/core/Solver.{h,cpp}`（純 core，不碰 raylib，夥伴可接）**：`solve(Board, vector<Part>, nodeBudget)` 傳值（不動 live game），回 `optional<vector<PartLocation>>`（對齊 parts index）。
- **演算法**：backtracking — 最大件優先、去重旋轉、放置後「任一列/欄某色超出需求即剪枝」、node budget 防卡死、leaf 用 `WinChecker::isWon` 精確驗收。完全重用 `Board::canPlace/place/remove`、`currentFilledForColor`、`WinChecker::isWon`，不重寫規則。
- **勝利條件**：所有 part placed 且每色每列/欄填滿數剛好等於 `_constraints[color]`（前 M 列、後 N 欄）；固定格（CANNOT_MOVE）計入該色並占格。

### 增量切分

- **增量 1（已完成代碼，待 GUI 驗收）**：核心 Solver + `Action::Solve` + `Game::solveAndApply()`（從 initial 快照解、套回 live board）+ Input 綁 `F` + CMake。鎖 1%，並讓 demo 能解 TA 測資（+2%/+2%）。headless 已驗 Example1–6 全解。
- **增量 2（未開工）**：半透明提示 overlay（依 Solver 解標出待放格，2.5%）+ 30 秒 idle 計時才顯示提示（2.5%）。主要動 Renderer + main loop 計時，重用同一 Solver。

### 增量 1 驗收清單

- [ ] Example1–6 進遊戲按 `F` → 盤面被正確填滿、顏色/旋轉正確、跳 You Win
- [ ] 無解的盤面按 `F` → 顯示 "No solution found." 不 crash
- [ ] 解完按 Backspace（reset）/ `N`（新遊戲）行為正常、零退化
- [ ] 通過後在 [STATUS.md](STATUS.md) 勾「自動解題並顯示一個解 1%」

### 不在增量 1 做

- 半透明 hint overlay、30 秒計時 → 增量 2
- solver 動畫（逐步演示解題過程）→ 不做（非配分項）

---

## 進度時間軸

<!-- 完成一個里程碑就加一列。每列只放：日期 / commit / 分數 / 一句話。詳細過程去 docs/log/（或封存的 Log.md）。 -->

| 日期 | 里程碑 | commit | 分數 | 一句話 |
|---|---|---|---|---|
| 2026-05-22 | Phase 0 | — | +0% | CMake + raylib FetchContent + src/{core,ui} skeleton 跑通 |
| 2026-05-22 | Phase 1 | — | +20% → 21.5% | MVP 單色可玩：Part/Board/Parser/WinChecker/Game/Input/Renderer/main 9 檔，手動 8 項全綠 |
| 2026-05-22 | Phase 2 Day 1 | `0fcfcd4` | +11% → 32.5% | 動畫骨架（per-part lerp）+ 旋轉/移動 pivot（中心格亮、pivot 不動） |
| 2026-05-22 | Phase 2 Day 2a | `6f7abcb` | +4% → 36.5% | 圓角面板 + 漸層底 + 程序材質（高光帶/bevel）+ drop shadow |
| 2026-05-22/24 | Phase 2 Day 2b | `59401b4` | +0% → 36.5% | 滑鼠 hover / pixel-perfect drag / 左鍵放置 / 右鍵 cancel（rubric 完整性） |
| 2026-05-24 | Phase 2 Day 2c | `2ea7726` | +1% → 37.5% | 音效（4 sfx + frame diff 觸發），圖形分項 15/15 全到位 |
| 2026-05-24 | auto-select bugfix | `c33d562` | — | 拔掉 Phase 1 殘留 `autoSelectNextUnplaced`（滑鼠模式跟手 bug） |
| 2026-05-24 | Phase 3 / 01-reset | `a22ea8b` | +1% → 38.5% | `Action::Reset` + deep-copy 快照 + Backspace 重來同關 |
| 2026-05-29 | Phase 3 / 02-new-game | `7240b43` | +1% → 39.5% | `AppState{Menu,InGame}` + `N` 回 menu + 選關重新 parse/init |
| 2026-05-29 | Phase 3 / 03-main-menu | （待 commit） | +1% → 40.5% | `assets/levels` 掃關卡 + Exo 2 / 圓角漸層 / hover / 鍵盤選關 |
| 2026-05-31 | Phase 3 / 05-dual-color | — | +2% → 42.5% | 雙色 Example5/6 可開可玩可勝利，零件/固定格/hints 顏色對齊 |
| 2026-05-31 | Phase 3 / 04-row-col-hints | — | +2% → 44.5% | 條狀 row/column hints：不足色框 / 滿足亮綠 / 超出多出段變紅，即時更新 |
| 2026-05-31 | Phase 4 / 增量1 solver | （待 commit） | (+1% 待 GUI 驗收) | 核心 backtracking Solver + F 鍵一鍵自動解填盤；headless 驗證 Example1–6 全解 |

> 各里程碑的完整過程、設計演進與驗收細節：見 [docs/log/](log/) 與封存的 [Log.md](Log.md)；現況分數見 [STATUS.md](STATUS.md)。

