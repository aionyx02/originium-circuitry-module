# STATUS.md — 進度與配分追蹤

> Last updated: 2026-06-08（自動解改逐步飛入 + 30 秒提示一次只亮一個零件，code 完成待實機驗收；分數 58.0%）
> 路由：[index.md](index.md) ｜ 規劃：[plan.md](plan.md) ｜ 配分：[scoring.md](scoring.md) ｜ 工作規範：[CLAUDE.md](../CLAUDE.md) / [docs/CLAUDE.md](CLAUDE.md) ｜ 協作紀錄：[docs/log/](log/)（舊：[Log.md](Log.md)）

> **這份檔只回答「現在到哪、勾了哪些分」**。決策原則去 CLAUDE.md，階段規劃去 plan.md。

---

## 目前分數

**程式實作：58.0%/65%**  
口頭報告：尚未開始評估 / 60%

> 每完成一個 Phase 更新一次。「目前分數」= 已穩穩拿到的分（demo 不會被扣回去的）。

---

## 目前階段

**Phase 5 完成 — 關卡編輯器已驗收；程式面只剩 demo day 加分**

Phase 0–5 完成。Phase 4 的 `F` 自動解與 30 秒 hint 已 GUI 驗收。Phase 5 編輯器也已 GUI 驗收：可改大小/顏色數、畫零件、BLOCK/FIX/ERASE、微調列欄數字；`EXPORT` 可寫 `assets/levels/<自訂檔名>.txt`（支援 editor 內直接改名）；`PLAY` 走 `Editor::buildPlayableSnapshot(...)`，會先把暫放的解答零件收回 tray 再進遊戲。讀檔硬限制也已補強：支援 `argv[1]`、無參數 `stdin` prompt、`--menu`、以及 menu / game 拖放 `.txt` 載入。詳見 [plan.md §10](plan.md)。

各里程碑 commit 與一句話：[plan.md 進度時間軸](plan.md#進度時間軸)。完整過程：[docs/log/](log/) + 封存的 [Log.md](Log.md)。

**下一步**：Phase 6 打包 / 乾淨機器 dry run，以及 demo day 的基本 / 雙色測資加分驗證。

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

- [x] 自動解題並顯示一個解 — 1%（`F` 鍵一鍵自動解填盤；headless 驗證 Example1–6 全解，2026-06-01 實機截圖驗收通過）
- [ ] 解開單色測資 — (+2%)
- [ ] 解開雙色測資 — (+2%)
- [x] 依解答在盤面上顯示半透明提示 — 2.5%（剩餘解答格畫半透明綠 overlay；headless 驗 hintCells 幾何正確，2026-06-01 實機截圖驗收通過）
- [x] 30 秒沒解開才顯示提示 — 2.5%（main loop idle 計時 ≥30s 才 `setHintsVisible`，放置/拔除即重置；2026-06-01 實機截圖驗收通過）

### 關卡設計器（共 10%）

- [x] 編輯功能：盤面大小、顏色數、新增任意零件、不可放置格、固定零件、列/欄數字 — **5%（六項合計）**（GUI 腳本已跑過 stepper / PART DESIGNER / BLOCK / FIX / ERASE / 數字框）
- [x] 匯出設定檔（純文字格式） — 2.5%（`LevelWriter` 可由 editor 直接寫出 `assets/levels/<自訂檔名>.txt`；GUI 匯出 + round-trip / solve 全綠）
- [x] 直接遊玩設計好的關卡 — 2.5%（`P` → `buildPlayableSnapshot(...)` → `Game::init(...)`；GUI 確認零件回 tray，`F` 可解自製關卡）

### 圖形介面（共 15%）

- [x] 實作 GUI — 2.5%（Phase 2 Day 2a：圓角板面/tray/win banner + 背景漸層 + drop shadow 兩 pass）
- [x] 音效 — 1%（Phase 2 Day 2c：assets/sfx/ 4 個音 pickup/place/spin/victory，main loop frame diff 觸發、優先序 win > place > pickup > spin；2026-05-24 驗收通過）
- [x] 圖片或材質顯示零件 — 1.5%（Phase 2 Day 2a：每 cell 上方高光帶 +70/ch + 下方 bevel ÷2 程序材質，跟 cell 一起旋轉）
- [x] 旋轉動畫 — 5%（Phase 2 Day 1：currentAngle lerp 到 `rotateCount * 90°`，frame-rate 獨立的 `1 - exp(-dt*k)` factor）
- [x] 零件跟游標連續移動 — 5%（Phase 2 Day 1：每 frame 對 currentCenterX/Y lerp 到 cursor pivot 目標）

---

## 已完成

已落地的功能與配分見上方 **配分 Checklist**。里程碑日期 / commit / 一句話見 [plan.md 進度時間軸](plan.md#進度時間軸)；完整過程見 [docs/log/](log/) 與封存的 [Log.md](Log.md)。

## 進行中

- 自動解逐步飛入 + 30 秒提示一次只亮一個零件（`fix/stepwise-solve-and-single-hint`）：code 完成、build 通過，待實機 GUI 驗收（[log/2026-06-08.md](log/2026-06-08.md)）。體驗強化，判分邏輯不變。
- 同分支：修掉「編輯器可匯出無解關卡」bug（`deriveConstraints` 只算盤面、卻匯出全部零件）→ EXPORT/PLAY 在零件未全擺上盤面時擋下並提示；Solver 加 O(格數) 開場可行性預檢，無解關卡（如 varyHard）由 1.4 秒凍結變 0.017ms 瞬回。headless 驗證 Example1–6 仍全解，待 GUI 驗收。
- 同分支：Solver 熱路徑優化——預算每零件每旋轉的格子偏移（loop-invariant hoisting）+ 增量計數/「無法達標」剪枝，inline canPlace/place/remove。可解硬關卡（varyhard 12×12/19 零件）**7.1s→107ms（~66×）**，Example1–6 全更快、結果不變。不動 `Part`/`Board` 介面。
- 同分支：tray 改滾輪可捲動（零件過多時下方被切掉）——捲動量烘進 `Layout::trayY`、scissor 裁切視口、捲軸指示、滾輪輸入＋鍵盤自動捲到可見、載入歸零。純 UI 層，待 GUI 驗收（[log/2026-06-08.md](log/2026-06-08.md)）。
- 視覺 polish pass（`feat/visual-polish`）代碼完成、待 GUI 驗收（[log/2026-06-01.md](log/2026-06-01.md)）
- 助教 demo 基本 / 雙色測資 +2% / +2% 留到 demo day：當場測資能透過 argv 或拖放直接載入並遊玩後再勾
- Phase 6：靜態連結 exe / `assets/` 打包與乾淨 Windows 機器 dry run
- 讀檔硬限制補強已完成：`argv[1]`、無參數 `stdin` prompt、`--menu`，以及 menu / game 拖放 `.txt` 載入
- 材質策略：純程序材質（不引入 png 資產）
- 音效策略：使用者已用 jsfxr 生 pickup / place / spin 3 個 mp3 + 沿用 victory.mp3 當 win 音

## 已知 Bug

（尚無；待手動驗證後可能會浮出）

- 助教 demo 雙色測資要等 demo day 才能驗收與入帳。

---

## 更新規則

- 每完成一個 checklist 項目並驗證 → 打勾、更新「目前分數」
- 每個 Phase 結束 → 對應 plan.md 進度時間軸新增一條
- 已知 Bug 不要藏，立刻寫上來；修好了從清單刪掉並記到 [docs/log/](log/) 當日檔
- 修動本檔記得改最上方的 `Last updated`
