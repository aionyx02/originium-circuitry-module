# STATUS.md — 進度與配分追蹤

> Last updated: 2026-05-31（Phase 4 Solver 增量 1 代碼完成，待 GUI 驗收；分數暫仍 44.5%）
> 路由：[index.md](index.md) ｜ 規劃：[plan.md](plan.md) ｜ 配分：[scoring.md](scoring.md) ｜ 工作規範：[CLAUDE.md](../CLAUDE.md) / [docs/CLAUDE.md](CLAUDE.md) ｜ 協作紀錄：[docs/log/](log/)（舊：[Log.md](Log.md)）

> **這份檔只回答「現在到哪、勾了哪些分」**。決策原則去 CLAUDE.md，階段規劃去 plan.md。

---

## 目前分數

**程式實作：44.5% / 65%**  
口頭報告：尚未開始評估 / 60%

> 每完成一個 Phase 更新一次。「目前分數」= 已穩穩拿到的分（demo 不會被扣回去的）。

---

## 目前階段

**Phase 5 進行中 — Editor 增量 1 代碼完成（待 GUI 驗收）**

Phase 0–3 完成；Phase 4（自動解題 Solver + 提示 overlay + 30 秒計時）代碼完成、headless 全綠、待 GUI 驗收。Phase 5 關卡編輯器增量 1：`LevelWriter`（反向 Parser）+ `Editor` 資料模型 + 編輯（大小/顏色數/列欄數字）+ **PART DESIGNER（滑鼠畫任意形狀零件、A 加入/D 刪除）** + 匯出 `assets/levels/custom-N.txt` + `P` 直接試玩。Headless 驗 round-trip Example1–6、Editor edits、設計關可解全綠。點格畫不可放置/固定格留增量 2。詳見 [plan.md §9](plan.md)/[§10](plan.md)。

各里程碑 commit 與一句話：[plan.md 進度時間軸](plan.md#進度時間軸)。完整過程：[docs/log/](log/) + 封存的 [Log.md](Log.md)。

**下一步**：GUI 驗收 Phase 4（F 解 / 30 秒提示）+ Phase 5 編輯器（E 進入 / 匯出 / P 試玩）→ Phase 5 增量 2（點格畫不可放置/固定格 + 零件設計器）。

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

- [ ] 自動解題並顯示一個解 — 1%（`F` 鍵一鍵自動解填盤；headless 驗證 Example1–6 全解，**待 GUI 目視驗收**）
- [ ] 解開單色測資 — (+2%)
- [ ] 解開雙色測資 — (+2%)
- [ ] 依解答在盤面上顯示半透明提示 — 2.5%（剩餘解答格畫半透明綠 overlay；headless 驗 hintCells 幾何正確，**待 GUI 目視驗收**）
- [ ] 30 秒沒解開才顯示提示 — 2.5%（main loop idle 計時 ≥30s 才 `setHintsVisible`，放置/拔除即重置；**待 GUI 目視驗收**）

### 關卡設計器（共 10%）

- [ ] 編輯功能：盤面大小、顏色數、新增任意零件、不可放置格、固定零件、列/欄數字 — **5%（六項合計）**（已做 4/6：大小/顏色數/列欄數字/**任意形狀零件**；不可放置/固定格留增量2，**待 GUI 驗收**）
- [ ] 匯出設定檔（純文字格式） — 2.5%（`LevelWriter` 寫 `assets/levels/custom-N.txt`；headless round-trip + 設計關可解全綠，**待 GUI 驗收**）
- [ ] 直接遊玩設計好的關卡 — 2.5%（`P` → `Game::init`，含設計的零件；headless 驗設計關可解，**待 GUI 驗收**）

### 圖形介面（共 15%）

- [x] 實作 GUI — 2.5%（Phase 2 Day 2a：圓角板面/tray/win banner + 背景漸層 + drop shadow 兩 pass）
- [x] 音效 — 1%（Phase 2 Day 2c：assets/sfx/ 4 個音 pickup/place/spin/victory，main loop frame diff 觸發、優先序 win > place > pickup > spin；2026-05-24 驗收通過）
- [x] 圖片或材質顯示零件 — 1.5%（Phase 2 Day 2a：每 cell 上方高光帶 +70/ch + 下方 bevel ÷2 程序材質，跟 cell 一起旋轉）
- [x] 旋轉動畫 — 5%（Phase 2 Day 1：currentAngle lerp 到 `rotateCount * 90°`，frame-rate 獨立的 `1 - exp(-dt*k)` factor）
- [x] 零件跟游標連續移動 — 5%（Phase 2 Day 1：每 frame 對 currentCenterX/Y lerp 到 cursor pivot 目標）

---

## 已完成

已落地的功能與配分見上方 **配分 Checklist**（每項附一句實作摘要）。各里程碑的日期 / commit / 一句話見 [plan.md 進度時間軸](plan.md#進度時間軸)；完整過程見 [docs/log/](log/) 與封存的 [Log.md](Log.md)。

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
- 已知 Bug 不要藏，立刻寫上來；修好了從清單刪掉並記到 [docs/log/](log/) 當月檔
- 修動本檔記得改最上方的 `Last updated`
