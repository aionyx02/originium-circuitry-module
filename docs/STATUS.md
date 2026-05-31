# STATUS.md — 進度與配分追蹤

> Last updated: 2026-05-31（Phase 3 issue 04/05 手動驗收通過，+4% 入帳）
> 規劃 / 策略：[plan.md](plan.md) ｜ 配分細節：[scoring.md](scoring.md) ｜ 工作規範：[CLAUDE.md](../CLAUDE.md) ｜ 協作紀錄：[LOG.md](LOG.md)

> **這份檔只回答「現在到哪、勾了哪些分」**。決策原則去 CLAUDE.md，階段規劃去 plan.md。

---

## 目前分數

**程式實作：44.5% / 65%**  
口頭報告：尚未開始評估 / 60%

> 每完成一個 Phase 更新一次。「目前分數」= 已穩穩拿到的分（demo 不會被扣回去的）。

---

## 目前階段

**Phase 3 進行中 — issue 04/05 手動驗收通過**

Phase 0 / 1 完成；Phase 2 Day 1（動畫骨架 + 旋轉/移動 pivot）commit `0fcfcd4`；Day 2a（圓角 + 漸層 + 程序材質 + drop shadow）commit `6f7abcb`；Day 2b（滑鼠 hover/pixel-perfect drag/左鍵放置/右鍵 cancel）commit `59401b4`；Day 2c（音效：InitAudioDevice + 4 個 LoadSound + frame diff 觸發、CMake post-build copy assets/）commit `2ea7726`、2026-05-24 驗收通過；auto-select bugfix（Day 2c 驗收暴露的 Phase 1 殘留）commit `c33d562`。Phase 2 圖形分項 **15 / 15 全到位**。Phase 3 issue [01-reset](issues/phase-3/01-reset.md) 驗收通過（`Action::Reset` + `Game::initialBoard/Parts` 快照 + `Game::resetToInitial()` + Backspace 鍵改綁 Reset），**+1% 入帳**。Phase 3 issue [02-new-game](issues/phase-3/02-new-game.md) 已 commit `7240b43` 並手動驗收通過，**+1% 入帳**。Phase 3 issue [03-main-menu](issues/phase-3/03-main-menu.md) 驗收通過：`assets/levels/Example1-6.txt` 到位、menu 正式從 `assets/levels` 掃關卡、支援 hover highlight / keyboard selection / Exo 2 / 圓角漸層，**+1% 入帳**。Phase 3 issue [05-dual-color](issues/phase-3/05-dual-color.md) 程式碼完成並於 2026-05-31 手動驗收通過：Parser 依 `C` 讀多色 constraints / fixed cells / part color，constraint hints 依 `board.colors` 多色顯示，零件/固定格/hints 顏色對齊，Example5/6 可開、可玩、可勝利，**+2% 入帳**。Phase 3 issue [04-row-col-hints](issues/phase-3/04-row-col-hints.md) 程式碼完成並於 2026-05-31 手動驗收通過：`Board::currentFilledForColor(...)` 共用 row/column current 計算，`WinChecker` 與 Renderer 共用同一套 current source of truth，Renderer 將 `current/need` 改成條狀 hints；不足時保留色框、滿足亮綠、超出時只有多出的 segments 變紅，放置/拔除即時更新，雙色 column hints 左右分 lane、row hints 上下分 lane，**+2% 入帳**。助教 demo 雙色測資 +2% 保留到 demo day。

---

## 配分 Checklist

> 每項後面是 scoring.md 標的配分。打勾代表「已完成且驗證過」，未打勾代表還沒做或還沒驗證。  
> 助教 demo 測資相關項目用 `(+2%)` 標示，因為要 demo 當天才能驗。

### 基本項目（共 5%）

- [x] Part class — 1%
- [x] Board class — 1%
- [x] 讀單色設定檔 + 正常顯示零件與盤面 — 2%（兩件事合算）
- [ ] 能讀取助教 demo 測資（基本） — (+2%)

### 遊戲流程（共 15%，**全部必拿**）

- [x] wasd / 滑鼠拖移選中零件 — 5%（鍵盤 + 滑鼠 pixel-perfect drag 均驗收通過）
- [x] r 旋轉零件 — 5%（4 方向循環已驗；mouse drag 中按 R 也保留旋轉動畫）
- [x] Esc 拔掉零件 — 1%（鍵盤 Esc 持有→還原 / 盤面→拔起 + 滑鼠右鍵 = Remove 雙語意都驗過）
- [x] Enter / 滑鼠左鍵放置 — 1%（鍵盤 + 滑鼠均驗收通過）
- [x] 勝利判定 — 1.5%（You Win banner 驗過）
- [x] 放置失敗印錯誤訊息 — 1.5%（重疊紅框 + 狀態列訊息驗過）

### 進階功能（共 10%）

- [x] 重置盤面為初始狀態 — 1%（Backspace 重來同關，driver = `Game::resetToInitial()` snapshot deep-copy；issue [01-reset](issues/phase-3/01-reset.md) 2026-05-24 驗收通過）
- [x] 不關程式開新遊戲 — 1%（`N` 回 menu、選關後重新 parse + `Game::init()`；2026-05-29 手動驗收通過）
- [x] 精美主畫面 / 關卡選擇 — 1%（`assets/levels` 動態掃關卡、Exo 2 + 圓角漸層 + hover highlight；2026-05-29 手動驗收通過）
- [x] 某列/欄滿足或超出需求時周圍提示 — 1%（條狀 hints 不足/滿足/超出狀態，2026-05-31 手動驗收通過）
- [x] 周圍顯示每列每欄目前填滿格數 — 1%（條狀 segments 等價呈現 current/need，2026-05-31 手動驗收通過）
- [x] 旋轉/移動中心不為空 — 1%（Phase 2 Day 1.5/1.6：`Part::computeCenterCell` 選離形心最近占據格；中心格亮色覆蓋；cursor=pivot 語意使旋轉時 pivot 螢幕位置不動）
- [x] 雙色設定檔載入與遊玩 — 2%（Example5/6 雙色 hints、零件顏色、可玩與可勝利，2026-05-31 手動驗收通過）
- [ ] 能讀取助教 demo 測資（雙色） — (+2%)

### 自動解題（共 10%）

- [ ] 自動解題並顯示一個解 — 1%
- [ ] 解開單色測資 — (+2%)
- [ ] 解開雙色測資 — (+2%)
- [ ] 依解答在盤面上顯示半透明提示 — 2.5%
- [ ] 30 秒沒解開才顯示提示 — 2.5%

### 關卡設計器（共 10%）

- [ ] 編輯功能：盤面大小、顏色數、新增任意零件、不可放置格、固定零件、列/欄數字 — **5%（六項合計）**
- [ ] 匯出設定檔（純文字格式） — 2.5%
- [ ] 直接遊玩設計好的關卡 — 2.5%

### 圖形介面（共 15%）

- [x] 實作 GUI — 2.5%（Phase 2 Day 2a：圓角板面/tray/win banner + 背景漸層 + drop shadow 兩 pass）
- [x] 音效 — 1%（Phase 2 Day 2c：assets/sfx/ 4 個音 pickup/place/spin/victory，main loop frame diff 觸發、優先序 win > place > pickup > spin；2026-05-24 驗收通過）
- [x] 圖片或材質顯示零件 — 1.5%（Phase 2 Day 2a：每 cell 上方高光帶 +70/ch + 下方 bevel ÷2 程序材質，跟 cell 一起旋轉）
- [x] 旋轉動畫 — 5%（Phase 2 Day 1：currentAngle lerp 到 `rotateCount * 90°`，frame-rate 獨立的 `1 - exp(-dt*k)` factor）
- [x] 零件跟游標連續移動 — 5%（Phase 2 Day 1：每 frame 對 currentCenterX/Y lerp 到 cursor pivot 目標）

---

## 已完成

- **Phase 0**（2026-05-22）：CMake + raylib FetchContent + src/{core,ui}/ skeleton + .gitignore；build 0 warning，視窗 OK。詳見 [LOG.md Phase 0](LOG.md#2026-05-22--phase-0-完成基礎設施)
- **Phase 1**（2026-05-22）：MVP 單色可玩。Part / Board / Parser / WinChecker / Game / Input / Renderer / main.cpp 共 9 檔；Example1.txt 手動互動驗證 8 項全綠（wasd / r 4循環 / Enter / Esc 雙語意 / 重疊紅框錯誤 / You Win）。詳見 [LOG.md Phase 1](LOG.md#2026-05-22--phase-1-完成mvp-單色關卡可玩)
- **Phase 2 Day 1**（2026-05-22，commit `0fcfcd4`）：動畫骨架（per-part `currentCenterX/Y/Angle/Scale` lerp，`1 - exp(-dt*k)` frame-rate 獨立）+ 旋轉/移動 pivot（`Part::computeCenterCell` 取離形心最近占據格、cursor 改成 pivot target、中心格亮色 +120/ch）。+11%。
- **Phase 2 Day 2a**（2026-05-22，commit `6f7abcb`）：視覺 polish + 程序材質。圓角板面 / tray / win banner、背景垂直漸層、每 cell 上方高光帶（+70/ch）+ 下方 bevel（÷2），drop shadow 兩 pass（offset 4/5px）。使用者手動驗收 A–H 全綠。+4%。
- **Phase 2 Day 2b**（2026-05-22 代碼 + 2026-05-24 驗收，commit `59401b4`）：滑鼠。Layout struct 從 Renderer.cpp anon-ns 提到 Renderer.h 公開；Game 加 `setCursor(row, col, isTray)` cursor 原語 + `bool mouseControlling` UI hint flag；Input 加 `pollMouse(Layout, Game&)` 做板面/tray hit-test + 左鍵 = Place / 右鍵 = Remove。drag follow 三輪迭代：cell-quantized（卡頓）→ pixel-perfect on board（緊跟手）→ pixel-perfect 從 tray 撿起起算（點 tray 即跟手）。視覺與 cursor 解耦：cursor 仍 cell-quantized 給紅綠框 + canPlace、視覺 = mouse pixel + snap 不 lerp。**不加分**（rubric 完整性而已）。
- **Phase 2 Day 2c**（2026-05-24，commit `2ea7726`）：音效。CMakeLists.txt 加 post-build `copy_directory ${CMAKE_SOURCE_DIR}/assets`；main.cpp 加 `InitAudioDevice` + 4 個 `LoadSound`（assets/sfx/{pickup,place,spin,victory}.mp3）+ 每 frame snapshot `heldIdx / won / placedCount / rotateSum` 做 diff、依優先序 `win > place > pickup > spin` 最多放一個音 + `IsSoundValid` 守缺檔。core / Renderer / Input 完全不碰。+1%。
- **auto-select bugfix**（2026-05-24，commit `c33d562`）：拔掉 Phase 1 keyboard-era 的 `Game::autoSelectNextUnplaced()` — 兩處呼叫（init 末 + handlePlace 放置成功後）+ 函式定義 + 宣告全刪。修掉滑鼠模式下「開檔零件自動跟手 + 放完自動接下一個」的 bug（Day 2c 驗收時暴露）。鍵盤路徑相容（cursor 初始在 TRAY_COL row 0，按 Enter 一樣撿第一件）。
- **Phase 3 issue 01-reset**（2026-05-24，驗收通過；commit `a22ea8b`）：`Action::Reset` + `Game::initialBoard / initialParts` 快照 + `Game::resetToInitial()` + Backspace 改綁 Reset（Esc 保留 Remove 語意）。`init()` 在 `std::move` 後從成員 deep-copy 一份 snapshot；`update()` 在 `if (won) return;` 之前處理 Reset 讓勝利後也能重來。Manual tests 6 條 + Regression 3 條全綠。+1%。
- **Phase 3 issue 02-new-game + 03-main-menu**（2026-05-29，驗收通過 / 待 commit）：`AppState { Menu, InGame }` + `N` 回 menu + 選關重新 parse/init；`assets/levels/Example1-6.txt` 正式關卡來源；主畫面 Exo 2、圓角面板、全屏漸層、hover highlight、鍵盤/滑鼠選關。Manual flow 驗收通過。+2%。
- **Phase 3 issue 04-row-col-hints + 05-dual-color**（2026-05-31，手動驗收通過）：雙色 Example5/6 可開、可玩、可勝利；零件/固定格/hints 顏色對齊；條狀 row/column hints 會隨放置/拔除即時更新，不足保留色框、滿足亮綠、超出多出的 segments 變紅。+4%。
- **文件結構**：CLAUDE / plan / STATUS / LOG / learning-notes / DEV_GUIDE / my-note 7 檔到位

## 進行中

- 助教 demo 雙色測資 +2% 留到 demo day：當場測資能透過 argv 直接載入並遊玩後再勾
- 材質策略：純程序材質（不引入 png 資產）
- 音效策略：使用者已用 jsfxr 生 pickup / place / spin 3 個 mp3 + 沿用 victory.mp3 當 win 音

## 已知 Bug

（尚無；待手動驗證後可能會浮出）

待確認 / 驗收風險：

- 助教 demo 雙色測資要等 demo day 才能驗收與入帳。

---

## 更新規則

- 每完成一個 checklist 項目並驗證 → 打勾、更新「目前分數」、補一句到「已完成」
- 每個 Phase 結束 → 對應 plan.md 進度時間軸新增一條
- 已知 Bug 不要藏，立刻寫上來；修好了從清單刪掉並記到 LOG.md
- 修動本檔記得改最上方的 `Last updated`
