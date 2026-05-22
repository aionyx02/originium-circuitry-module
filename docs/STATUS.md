# STATUS.md — 進度與配分追蹤

> Last updated: 2026-05-22  
> 規劃 / 策略：[plan.md](plan.md) ｜ 配分細節：[scoring.md](scoring.md) ｜ 工作規範：[CLAUDE.md](../CLAUDE.md) ｜ 協作紀錄：[LOG.md](LOG.md)

> **這份檔只回答「現在到哪、勾了哪些分」**。決策原則去 CLAUDE.md，階段規劃去 plan.md。

---

## 銀行存款

**程式實作：32.5% / 65%**  
口頭報告：尚未開始評估 / 60%

> 每完成一個 Phase 更新一次。「銀行存款」= 已穩穩拿到的分（demo 不會被扣回去的）。

---

## 目前階段

**Phase 2 — 視覺打磨進行中**

Phase 0 / 1 完成；Phase 2 Day 1（動畫骨架 + 旋轉/移動 pivot）已 commit `0fcfcd4`；Day 2a（圓角 + 漸層 + 程序材質 + drop shadow）已實作待手動驗收；Day 2b（滑鼠）/ Day 2c（音效）未開工。

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

- [x] wasd / 滑鼠拖移選中零件 — 5%（鍵盤已驗，滑鼠 Phase 2 補上）
- [x] r 旋轉零件 — 5%（4 方向循環已驗）
- [x] Esc 拔掉零件 — 1%（持有→還原 / 盤面→拔起 兩種語意都驗過）
- [x] Enter / 滑鼠左鍵放置 — 1%（鍵盤已驗，滑鼠 Phase 2 補上）
- [x] 勝利判定 — 1.5%（You Win banner 驗過）
- [x] 放置失敗印錯誤訊息 — 1.5%（重疊紅框 + 狀態列訊息驗過）

### 進階功能（共 10%）

- [ ] 重置盤面為初始狀態 — 1%
- [ ] 不關程式開新遊戲 — 1%
- [ ] 精美主畫面 / 關卡選擇 — 1%
- [ ] 某列/欄滿足或超出需求時周圍提示 — 1%
- [ ] 周圍顯示每列每欄目前填滿格數 — 1%
- [x] 旋轉/移動中心不為空 — 1%（Phase 2 Day 1.5/1.6：`Part::computeCenterCell` 選離形心最近占據格；中心格亮色覆蓋；cursor=pivot 語意使旋轉時 pivot 螢幕位置不動）
- [ ] 雙色設定檔載入與遊玩 — 2%
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

- [x] 實作 GUI — 2.5%（基礎 GUI 已具備；Day 2a 加圓角 + 漸層 + drop shadow，**待手動驗收**）
- [ ] 音效 — 1%（Day 2c 接 4 個 jsfxr 生的 wav）
- [ ] 圖片或材質顯示零件 — 1.5%（Day 2a 加每 cell 上方高光帶 + 下方暗邊 bevel 程序材質，**待手動驗收**）
- [x] 旋轉動畫 — 5%（Phase 2 Day 1：currentAngle lerp 到 `rotateCount * 90°`，frame-rate 獨立的 `1 - exp(-dt*k)` factor）
- [x] 零件跟游標連續移動 — 5%（Phase 2 Day 1：每 frame 對 currentCenterX/Y lerp 到 cursor pivot 目標）

---

## 已完成

- **Phase 0**（2026-05-22）：CMake + raylib FetchContent + src/{core,ui}/ skeleton + .gitignore；build 0 warning，視窗 OK。詳見 [LOG.md Phase 0](LOG.md#2026-05-22--phase-0-完成基礎設施)
- **Phase 1**（2026-05-22）：MVP 單色可玩。Part / Board / Parser / WinChecker / Game / Input / Renderer / main.cpp 共 9 檔；Example1.txt 手動互動驗證 8 項全綠（wasd / r 4循環 / Enter / Esc 雙語意 / 重疊紅框錯誤 / You Win）。詳見 [LOG.md Phase 1](LOG.md#2026-05-22--phase-1-完成mvp-單色關卡可玩)
- **Phase 2 Day 1**（2026-05-22，commit `0fcfcd4`）：動畫骨架（per-part `currentCenterX/Y/Angle/Scale` lerp，`1 - exp(-dt*k)` frame-rate 獨立）+ 旋轉/移動 pivot（`Part::computeCenterCell` 取離形心最近占據格、cursor 改成 pivot target、中心格亮色 +120/ch）。+11%。
- **文件結構**：CLAUDE / plan / STATUS / LOG / learning-notes / DEV_GUIDE / my-note 7 檔到位

## 進行中

- Phase 2 Day 2a 視覺 polish + 程序材質：圓角板面 / tray / win banner、背景漸層、cell 上高光帶 + 下暗邊 bevel、drop shadow 兩 pass — **代碼已寫、smoke OK，等使用者目視驗收**
- 後續：Day 2b 滑鼠（不加分但補 rubric 完整性）→ Day 2c 音效（+1%，使用者已生 sound1.wav 一個）
- 材質策略：純程序材質（不引入 png 資產）
- 音效策略：使用者用 jsfxr 生 4 個 wav（pickup / place / rotate / win）

## 已知 Bug

（尚無；待手動驗證後可能會浮出）

---

## 更新規則

- 每完成一個 checklist 項目並驗證 → 打勾、更新「銀行存款」、補一句到「已完成」
- 每個 Phase 結束 → 對應 plan.md 進度時間軸新增一條
- 已知 Bug 不要藏，立刻寫上來；修好了從清單刪掉並記到 LOG.md
- 修動本檔記得改最上方的 `Last updated`
