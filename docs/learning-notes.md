# learning-notes.md — 主題式技術筆記

> Last updated: 2026-05-24（Phase 2 Day 2b pixel-perfect 二輪 polish）  
> 規劃：[plan.md](plan.md) ｜ 進度：[STATUS.md](STATUS.md) ｜ 過程紀錄：[LOG.md](LOG.md) ｜ 工作規範：[CLAUDE.md](../CLAUDE.md)

> **這份檔回答「這段 code 在做什麼、為什麼這樣做、demo / 口頭報告我得能講出什麼」。**  
> 對應口頭報告的「對程式的熟悉度」15%。  
> 過程紀錄、prompt、AI 怎麼幫請寫在 [LOG.md](LOG.md)，**不要重複寫在這裡**。

---

## 索引（按 commit 切分，最新在上）

> **新增 entry 時：(1) append 到下方 Notes 段，(2) 在這個索引對應的 commit 分組下加一行。**  
> 連結若無法直接點開，用 Cmd+F 搜 Feature 名稱。  
> 用「Phase X · commit `<short>`」標注每個 entry 在哪段 code 改動產生，方便對照原始 commit 學習。

### Phase 4 增量 1 — `<待 commit>` (2026-05-31)
自動解題 Solver（backtracking + 剪枝）— 一鍵解出並填盤

- [Solver：backtracking + 剪枝 + 重用既有規則](#solverbacktracking--剪枝--重用既有規則)

### Phase 3 issue 01-reset — `<待 commit>` (2026-05-24)
重置盤面（Reset Board）— 重來同關不必關程式

- [Game 狀態 snapshot：value-type deep copy 還原初始盤面](#game-狀態-snapshotvalue-type-deep-copy-還原初始盤面)

### Phase 2 Day 2b — `<待 commit>` (2026-05-22)
滑鼠 hover / drag / 左鍵放置 / 右鍵 cancel

- [Cursor 原語：用單一 setCursor 把多 input source 接到同條 Action 尾](#cursor-原語用單一-setcursor-把多-input-source-接到同條-action-尾)

### Phase 2 Day 2a — `6f7abcb` (2026-05-22)
視覺 polish + 程序材質 + drop shadow

- [raylib 圓角與漸層 API（axis-aligned UI 元素用）](#raylib-圓角與漸層-apiaxis-aligned-ui-元素用)
- [程序材質：高光帶 + bevel 在旋轉 cell 上的座標數學](#程序材質高光帶--bevel-在旋轉-cell-上的座標數學)
- [Drop shadow 為什麼要兩 pass 渲染](#drop-shadow-為什麼要兩-pass-渲染)

### Phase 2 Day 1 — `0fcfcd4` (2026-05-22)
動畫骨架 + 旋轉/移動 pivot 中心

- [動畫 lerp 框架（frame-rate independent）](#動畫-lerp-框架frame-rate-independent)
- [單調累加 `rotateCount` vs `fmod` shortest-arc](#單調累加-rotatecount-vs-fmod-shortest-arc)
- [旋轉/移動 pivot cell 選擇演算法](#旋轉移動-pivot-cell-選擇演算法)
- [`DrawRectanglePro` 任意 pivot 旋轉的座標數學](#drawrectanglepro-任意-pivot-旋轉的座標數學)
- [Cursor 設計演進：Phase 1「= bbox 左上角」→ Phase 2 Day 1.6「= pivot target」](#cursor-設計演進phase-1-bbox-左上角--phase-2-day-16-pivot-target)（跨 commit：Phase 1 模型 + Phase 2 改動同篇）

### Phase 0–1 — `970644c` (2026-05-22)
MVP 單色關卡可玩

- [Parser：純文字關卡檔 → `Board + vector<Part>`](#parser純文字關卡檔--board--vectorpart)
- [Board cell 編碼方式](#board-cell-編碼方式)

---

## 寫什麼、不寫什麼

**判準：有沒有非平凡概念可教**（跟事件大小、LOG.md 的大事/小事**無關**）。  
一個 bug fix 若讓你學到 raylib event loop 的某個性質 → 寫；改個 padding 顏色 → 不寫。

**要寫**：

- 新模組（Parser / WinChecker / Solver / Editor / Renderer 等）
- 引入非平凡概念（backtracking、lerp、deep copy、`std::filesystem` 掃目錄、座標轉換等）
- 跟 raylib 對接的關鍵 API（`DrawTexturePro` 的 origin、rotate、`SetTargetFPS` 等）
- **任何 size 的變更**，只要過程中學到「以後會想再用 / 老師可能問」的概念
- 決策的**最終結果**（不寫過程，過程在 LOG.md）

**不寫**：

- 純常數調整（顏色、padding、字體大小）
- 改錯字、rename 變數、補 trivial helper
- 「Claude 怎麼幫」、「我 review 了 diff」這類**過程紀錄**（過程去 LOG.md）

> 寧可多寫一篇，也不要漏掉一個可講的概念。「對程式的熟悉度」15% 是真的會被老師逐一問的。

---

## 語言

中文或英文皆可，technical term 用原文較自然。一個 entry 內混用 OK；跨 entry 不強制一致。

---

## Entry 模板

````markdown
### {Feature name}
*Phase X · commit `<short-sha>`*

#### What & Key Concepts
- 這個模組在做什麼
- 用到的 C++ / raylib / 演算法 / 資料結構概念
- 若有非平凡演算法，簡述（不貼整段 code，貼關鍵幾行 + 解釋）

#### Why This Design
- 為什麼這樣設計？
- 考慮過哪些替代方案？為何不選？
- （**不寫「Claude 建議了什麼」** — 那是 LOG.md 的事）

#### What I Should Be Able To Explain
demo / 口頭報告會被問到時，要能講出來什麼？  
列 3–5 個 bullet，每個都是「老師可能問的問題 + 我會怎麼答」  
範例：
- Q：為什麼旋轉用 lerp 而不是直接設 90°？  
  A：直接設會跳變、無動畫；lerp(current, target, 0.18) 每 frame 逼近 target，視覺上是平滑旋轉。

#### Score Connection
對應 [scoring.md](scoring.md) 哪幾項，含 % 與行號  
範例：旋轉動畫 — 5%（[scoring.md:93](scoring.md#L93)）
````

---

## Notes

> 條目時間正序（Phase 1 在最上、新的往下加）。索引在最上面、按 commit 分組，最新 commit 在上。

---

### Parser：純文字關卡檔 → `Board + vector<Part>`
*Phase 0–1 · commit `970644c`*

#### What & Key Concepts
- 檔案格式（[io/input-file-format.md](io/input-file-format.md)）：第一行 `C M N`（顏色數 / 列 / 欄）→ 每色：M 個列需求、N 個欄需求、X1 固定零件位置 → 全色跑完接 X2 不可放置格 → 最後是零件清單 `colorIdx M2 N2` + 0/1 矩陣，直到 EOF。
- 用 `std::istream::operator>>` 逐 token 讀，自動跳空白與換行 → **格式對空白／空行很寬容**，助教手寫的測資也讀得進。
- 例外路徑：丟自訂 `Parser::Error`（C++ 標準例外型別不夠語意明確）。
- 邊界檢查：fixed-part / blocked-cell 座標、顏色索引超界都直接丟例外。

#### Why This Design
- 為什麼純文字不用 JSON：助教 demo 當天會給測資，**測資是純文字格式**。用 JSON 等於放棄 4% 分（單色 +2% + 雙色 +2%）。詳見 [LOG.md「技術棧決策」entry](LOG.md#2026-05-22--技術棧決策c17--raylib-55--cmake--mingw-static) Risks/Follow-ups 段。
- 為什麼用 `>>` 而不是逐行讀 + split：`>>` 自動處理空白，省下 80% 的解析錯誤處理代碼；老師格式不規範空行也讀得進。
- 為什麼例外而非 `std::optional`：要把錯誤訊息傳出去（例如「parse error: expected row constraint」），optional 沒有 payload。

#### What I Should Be Able To Explain
- Q：為什麼不用 JSON？  
  A：助教 demo 測資是純文字、教師指定格式；用 JSON 會讓我們讀不到測資，失去 4% 配分。早期版本確實選了 JSON、後來在 plan.md 寫作時 catch 到推翻。
- Q：parser 怎麼處理檔尾？  
  A：零件清單 while loop 讀 `colorIdx` 失敗就跳出（EOF 或非數字），不需要顯式 part-count。
- Q：固定零件 vs 不可放置格差別？  
  A：固定零件（CANNOT_MOVE）算入該色的列/欄需求數，不可放置格（CANNOT_PLACE）只是禁止任何零件覆蓋。前者帶顏色 idx 編碼進 cell value。

#### Score Connection
- 基本項目 5%：讀單色設定檔 + 正常顯示 — 2%（[scoring.md:40](scoring.md#L40)）
- 助教 demo 測資 +2%（[scoring.md:41](scoring.md#L41)） — Phase 6 demo 當天才能拿

---

### Board cell 編碼方式
*Phase 0–1 · commit `970644c`*

#### What & Key Concepts
- `_boardInfo[r][c]` 是 `int`，一個欄位塞四種狀態：
  - `EMPTY = 0`
  - `CANNOT_PLACE = -1`（不可放置格）
  - `CANNOT_MOVE - color`：固定零件，例如 color=0 → cell = -2、color=1 → cell = -3
  - `OCCUPIED + partIndex`：被動零件覆蓋，例如 partIdx=0 → cell = 1、partIdx=1 → cell = 2
- 解碼用 static helpers：`Board::isOccupied(cell)`、`occupiedPartIndex(cell)`、`isCannotMove(cell)`、`cannotMoveColor(cell)`
- skeleton（[Framework/Board.h](../Framework/Board.h)）給的 enum 沒寫 underlying type；C++17 預設 underlying 是某個能 hold 列出的 enumerator 的最小 int 型別 → 不允許存 `OCCUPIED + partIdx` 這種任意 int。所以 **enum 加 `: int` 明確指定**讓 cast 合法（[Phase 1 LOG entry「skeleton 修正」](LOG.md#2026-05-22--phase-1-完成mvp-單色關卡可玩) 段）。

#### Why This Design
- 為什麼一個 int 塞四種：教師 skeleton 就這樣設計、不想偏離；負數區用來標固定/禁止、正數區用來標被佔據，0 = 空，乾淨分離。
- 為什麼不用 struct `{ State state; int extra; }`：更乾淨但要動 skeleton 的型別 → 違反 Hard Constraint #4「保留 Part/Board 命名與欄位」。
- enum 加 `: int` 是**一字元變更**，不算改名也不算改欄位，符合 skeleton 精神。

#### What I Should Be Able To Explain
- Q：為什麼 cell 是 int 不是 enum？  
  A：教師 skeleton 規定。狀態多到 5 種、且 occupied / cannot_move 還要帶 partIdx 或 colorIdx，所以塞 int 用算術 offset 編碼。
- Q：`OCCUPIED + partIdx` 跟 `CANNOT_MOVE - color` 會撞嗎？  
  A：不會。OCCUPIED=1（正數區）、CANNOT_MOVE=-2（負數區）、CANNOT_PLACE=-1。正負 + 0 三區分開。
- Q：為什麼 enum 要加 `: int`？  
  A：C++17 enum 預設 underlying 是 compiler 挑「能 hold 列出值」的最小 int 型別，可能是 signed char 之類。`static_cast<BoardInfo>(OCCUPIED + partIdx)` 結果若超出該型別範圍是 UB。加 `: int` 強制 underlying 是 `int`，cast 合法。

#### Score Connection
- 基本項目 5%：Board class 1% ＋ 讀單色設定檔 2% 都靠這個編碼撐起來

---

### Cursor 設計演進：Phase 1「= bbox 左上角」→ Phase 2 Day 1.6「= pivot target」
*Phase 1（初代模型） · commit `970644c`* / *Phase 2 Day 1.6（演進） · commit `0fcfcd4`*

#### What & Key Concepts
- **Phase 1 模型**：cursor 是「下一個放置位置的 bbox 左上角」。游標跨 tray + board：`cursorCol == -1` 是 tray、`>= 0` 是 board。clampCursor 在 col=-1 時把 row 上限放寬到 `max(parts.size(), board.rows) - 1`。
- **Phase 2 改動**：cursor 改成「pivot cell 要落的位置」。`syncHeldLocation` 內部用 `rotatedCenterCell` offset 把 bbox top-left 算出來：`location.row = cursorRow - rotatedCr; location.col = cursorCol - rotatedCc`。
- **效果**：旋轉時 cursor 不動 → pivot 螢幕位置不動 → 其他 cells 繞 pivot 轉。`Board::canPlace` API 完全沒動，只是 `Game` layer 餵進不同的 (row, col)。
- **Esc 雙語意**：持有狀態 = 把零件還回 tray；無持有 + cursor 在 OCCUPIED cell = 把那 cell 對應的 part 從盤面拔回 hand。對使用者來說都是「取消當前動作」的單一語意，但 implementation 是 cursor context 決定的。

#### Why This Design
- 為什麼不分兩個 cursor（一個 tray、一個 board）：要在 tray 跟 board 間切換時 cursor 狀態同步更麻煩。一個 cursor + col=-1 sentinel 簡單。
- 為什麼 Phase 2 改 cursor 語意而不改 `Board::canPlace`：API 動越少越好。Board 是教師 skeleton 規定要保留的核心 class。Game layer 的語意自由，怎麼改都行。
- 為什麼拔零件時 snap cursor：不 snap 的話、舊 bbox top-left 經 syncHeldLocation 重算後可能跟 cursor 對不上，零件視覺會跳。snap 後算出來的 location 等於拔之前的 location，零件留在原位。

#### What I Should Be Able To Explain
- Q：cursor 怎麼跨 tray 跟 board？  
  A：用 `cursorCol == -1` 當 sentinel 表示「在 tray」、`>= 0` 表示「在 board」。clampCursor 在這兩個區域分別 clamp row 上限。
- Q：Phase 2 為什麼改 cursor 語意？  
  A：使用者要求旋轉時 pivot 不動。原本 cursor 指 bbox 左上角，rotate 後 pivot 在 bbox 內位置變了 → 螢幕位置漂；改成 cursor 指 pivot target，rotate 時 cursor 不動 → pivot 不動。
- Q：拔零件為什麼要 snap cursor？  
  A：拔之前 location.row/col 是拔之前的 bbox top-left；拔之後 syncHeldLocation 會用「cursor - rotatedPivot」重算。如果 cursor 不 snap 到原 pivot 位置，新算出來的 bbox top-left 可能跟原本不同，零件會視覺跳一格。
- Q：Esc 為什麼兩種行為？  
  A：對使用者是「取消當前動作」的單一語意；implementation 看 heldPartIdx 與 cursor 位置決定具體做什麼（hand→tray 或 board→hand）。

#### Score Connection
- 遊戲流程：wasd 移動 5%（[scoring.md:47](scoring.md#L47)）、Esc 拔掉 1%（[scoring.md:49](scoring.md#L49)）
- 進階：旋轉/移動中心不為空 1%（[scoring.md:61](scoring.md#L61)） — Phase 2 改 cursor 語意是為了滿足這一項的視覺效果

---

### 動畫 lerp 框架（frame-rate independent）
*Phase 2 Day 1 · commit `0fcfcd4`*

#### What & Key Concepts
- 每 frame 把 current 往 target 拉一個比例。比例 = `1 - exp(-dt * k)`。
- `dt` = `GetFrameTime()`，每 frame 不同。`k` = 衰減速率常數（位置 14、角度/scale 12）。
- 公式對 dt 是線性可拆的：兩個 dt 半步 ≡ 一個 dt 全步（指數性質 `e^(-dt*k) * e^(-dt*k) = e^(-2*dt*k)`），所以 60FPS / 144FPS 的視覺速度一致。
- 對比固定 factor `current = lerp(current, target, 0.4)`：60FPS 跑一秒衰減約 60 次、144FPS 跑同一秒衰減 144 次 → 高 frame rate 動畫變兩倍快。

#### Why This Design
- 為什麼不直接用固定 factor：demo 機跟開發機 frame rate 可能不同（教室電腦未知）。
- 為什麼指數衰減而不是線性插值：指數衰減 = 「越接近目標越慢」，看起來像彈簧；線性 = 等速 → 終點突然停。
- `k` 選 12–14：肉眼大約 0.3 秒內到位，不會慢到拖沓也不會快到看不見動畫。

#### What I Should Be Able To Explain
- Q：為什麼 `1 - exp(-dt*k)`？  
  A：frame-rate 獨立。兩個半 dt 跟一個全 dt 的效果一樣，因為指數的可加性。固定 factor 做不到。
- Q：`k=14` 是什麼意思？  
  A：時間常數倒數。`1/k = 0.071秒` ≈ 動畫衰減到 e⁻¹（≈37%）所需時間。所以 0.3 秒能衰減到 ~1%、肉眼看到位。
- Q：lerp 永遠不會真的到達 target，怎麼辦？  
  A：浮點下會無限趨近、實務上幾 frame 後 diff 小於 1px、肉眼看不出。如果要嚴格 snap 可以加 threshold「diff < 0.5 時直接賦值 target」，但目前沒做。

#### Score Connection
- 圖形介面：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)）+ 零件跟游標連續移動 5%（[scoring.md:94](scoring.md#L94)）

---

### 單調累加 `rotateCount` vs `fmod` shortest-arc
*Phase 2 Day 1.5 · commit `0fcfcd4`*

#### What & Key Concepts
- 旋轉動畫第一版用 `fmod(target - current + 540, 360) - 180` 算最短弧。數學上正確，但浮點實務上脆弱：
  - `current` 隨 lerp 累積、可能比 target 稍微超過或低於整 90°
  - 多次 R 後 `current` 在 360 附近浮動，下次 mod 出來的 diff 不一定是預期方向
  - 使用者實測：4 次 R 後第 4 次反向
- 改用 `Part::rotateCount`（unsigned，每按 R `++`）+ `targetAngle = rotateCount * 90.0f`：
  - target 永遠是「下一個 +90°」，單調遞增
  - `currentAngle` 線性 lerp 永遠是「往大走」 → 視覺一定 CW
  - 沒有 mod、沒有正負判斷
- 代價：`currentAngle` 會無限長大；float 在數千度內精度都夠（每按 R 加 90，按 10000 次才到 900000，float32 精度仍有 16 位有效數字）。Render 出來看起來和 mod 後一樣。

#### Why This Design
- 為什麼換掉：第一版被使用者實測 fail，AI debug 出來的算法雖然 case analysis 都對、實際還是看到反向 → 改 robust 而非 clever。
- 為什麼不用 double：raylib API 大多用 float，到處 cast 反而髒。float 精度夠 → 不必動。

#### What I Should Be Able To Explain
- Q：為什麼 rotation 不存成 `int rotateCount` 就好、何必還有 `Rotate::CW_0/90/180/270` enum？  
  A：兩個目的不同：`rotateCount` 是視覺動畫累加器（單調 float→angle）；`location.rotate` 是邏輯狀態（0–3）給 `rotatedShape` 用，會 mod 4。兩個都需要、不能合併。
- Q：fmod 那版到底哪裡錯？  
  A：理論上對。實務上 `current` 接近邊界（例如 359.5）時，`target - current + 540` mod 360 後可能很接近 540 邊界，浮點誤差讓結果有時是 ~-179 有時 ~+181，diff 方向不穩。單調累加完全繞過這個。
- Q：rotateCount 無限大不會壞嗎？  
  A：float32 在數千度內仍精準，每秒按一次 R 連按一年才會出問題。不擔心。

#### Score Connection
- 圖形介面：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)） — robust 實作

---

### 旋轉/移動 pivot cell 選擇演算法
*Phase 2 Day 1.5 · commit `0fcfcd4`*

#### What & Key Concepts
- 每個 Part 載入時 `computeCenterCell()`：
  1. 算所有占據格的形心 `(avgR, avgC)`
  2. 在占據格中挑 squared distance 最小的當 pivot
- 例子：
  - P0 L-shape `[[1,1],[1,0]]` → 形心 `(0.33, 0.33)` → 最近占據格 `(0, 0)`
  - P1 1×3 `[[1,1,1]]` → 形心 `(0, 1)` → 最近占據格 `(0, 1)`（中間那格）
- `rotatedCenterCell(out)` 用同一條旋轉公式 case 0~3 把 (centerCellRow, centerCellCol) 套到旋轉後的 bbox 位置。

#### Why This Design
- 為什麼不用 bbox 中心：bbox 中心可能落在空格上（L-shape 的 bbox 中心是空的 (0.5, 0.5)）→ 違反 rubric「中心不為空」。
- 為什麼形心 + 最近占據格：保證落在占據格上（直接從占據集挑），且離形心最近、最像「中心」。
- 為什麼 squared distance 而非 abs：避免 sqrt（沒必要）、且偏好直線最近，符合視覺直覺。

#### What I Should Be Able To Explain
- Q：怎麼保證 pivot 不會落在空格？  
  A：候選集只取占據格 `if (shape[r][c])`，自動排除空格。
- Q：tie 怎麼辦（兩個占據格距離一樣）？  
  A：top-to-bottom、left-to-right 第一個遇到的勝出（迴圈順序）。確定性。
- Q：旋轉時 pivot 怎麼跟著轉？  
  A：`rotatedCenterCell` 用和 `rotatedShape` 同一條 case 公式把原 pivot 位置變換到旋轉後 bbox 內的位置。然後 Renderer 用這個算螢幕 pivot。
- Q：對 L-shape 來說 pivot 在角上感覺怪？  
  A：是。但形心 = (0.33, 0.33) 確實最近的占據格就是角；要選別的會偏離形心，更不對。Trade-off。

#### Score Connection
- 進階：旋轉/移動中心不為空 1%（[scoring.md:61](scoring.md#L61)）

---

### `DrawRectanglePro` 任意 pivot 旋轉的座標數學
*Phase 2 Day 1 · commit `0fcfcd4`*

#### What & Key Concepts
- raylib API：`DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color)`
  - `rec.x, rec.y` = pivot 點的螢幕位置
  - `origin` = 從矩形左上角到 pivot 的 offset（**在 un-rotated 坐標系裡**）
  - 等價於：「先把矩形畫在 (rec.x - origin.x, rec.y - origin.y) 到 (...+rec.w, ...+rec.h)、然後繞 (rec.x, rec.y) 旋轉 rotation 度」
- 用來畫繞共同 pivot 的多 cell 部件：所有 cells 共用 `rec.x/y = pivot 螢幕位置`，每 cell 有自己的 `origin`：
  ```
  origin.x = (centerCellCol - col + 0.5) * scs
  origin.y = (centerCellRow - row + 0.5) * scs
  ```
- 驗算：rotation=0 時，cell (r, c) top-left = `(pivot.x - origin.x, pivot.y - origin.y) = (pivot.x - (cc - c + 0.5)*scs, pivot.y - (cr - r + 0.5)*scs)`。當 c=cc、r=cr 時 = `(pivot.x - 0.5*scs, pivot.y - 0.5*scs)` → cell 中心剛好在 pivot，正確。

#### Why This Design
- 為什麼不畫到 RenderTexture 再 DrawTexturePro：DrawTexturePro 也支援旋轉，但要先 baking 整個 part 進 texture、複雜度高、size 有上限。每 cell 直接 DrawRectanglePro 簡單且 GPU 一樣處理。
- 為什麼 pivot 不用 bbox 中心：要視覺上「繞 pivot cell 轉」而不是「繞 bbox 中心轉」。bbox 中心對 L-shape 來說會繞空格中心，視覺奇怪。

#### What I Should Be Able To Explain
- Q：DrawRectanglePro 的 `origin` 是什麼？  
  A：rect 左上角到 pivot 點的位移向量（un-rotated 坐標）。`(rec.w/2, rec.h/2)` = 繞中心轉；`(0, 0)` = 繞左上角轉。
- Q：怎麼讓多個 cell 一起繞共同 pivot 轉？  
  A：所有 cell 共用 `rec.x/y` = pivot 螢幕位置，每 cell 的 origin 是「該 cell 左上角到 pivot 的 vector」。raylib 自動把 cell 繞 pivot 旋轉。
- Q：rotation 是度還是弧度？  
  A：度。raylib 一律用度，內部 cos/sin 轉。
- Q：origin 的座標是「旋轉前」還是「旋轉後」？  
  A：旋轉前（un-rotated）。raylib 拿到後先算 rotated 4 角再 DrawTriangle。

#### Score Connection
- 圖形介面：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)） — 數學機制
- 進階：旋轉中心不為空 1%（[scoring.md:61](scoring.md#L61)） — 視覺實作支援

---

### raylib 圓角與漸層 API（axis-aligned UI 元素用）
*Phase 2 Day 2a · commit `<待 commit>`*

#### What & Key Concepts
- raylib 5.5 提供的圓角 API：
  - `DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color)` — 填色圓角矩形
  - `DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color)` — 圓角邊框（可設粗細）
  - `roundness` 0.0–1.0：0 = 直角、1 = 圓邊到底；本專案用 0.18 給 cell、0.24 給 Win banner
  - `segments` = 每個圓角的多邊形分段數；6–8 足夠看起來平滑
- 漸層 API：`DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom)` — 垂直漸層（也有 H 水平、Ex 自訂四角）
- **限制**：圓角 API 只支援 axis-aligned 矩形、沒有 rotation 參數。旋轉中的 cell 不能用圓角 API、要繼續用 `DrawRectanglePro`。
- 本專案分工：
  - 板面空格、CANNOT_PLACE 格、CANNOT_MOVE 固定零件格、tray slot、Win banner → 圓角 API（不旋轉）
  - Part cells、shadow、高光帶、bevel → `DrawRectanglePro`（會旋轉、無圓角）

#### Why This Design
- 為什麼背景漸層用 `GradientV` 一行而不是自己畫多條：raylib API 直接內建一行解決、GPU 也是一個三角形 batch、沒理由自己造輪。
- 為什麼板面 cell roundness 0.18 而不是更大：3×3 cell 已經不大、太圓會看起來像獨立按鈕、失去「盤面格」感。0.18 是「明顯不是直角但仍像格」的甜蜜點。
- 為什麼 segments 用 6：6 段對 cell 大小（cs 在 32–96 之間）肉眼已看不出多邊形邊；更多會浪費 GPU。

#### What I Should Be Able To Explain
- Q：raylib 圓角 API 能不能旋轉？  
  A：不能。`DrawRectangleRounded` 沒有 rotation 參數。需要旋轉時用 `DrawRectanglePro`（無圓角）或先 baking 到 RenderTexture 再 `DrawTexturePro`（複雜）。本專案 part cells 接受無圓角、UI 框架元素用圓角 API。
- Q：roundness 怎麼選？  
  A：`0.0` 直角、`1.0` 圓邊到底（圓角佔短邊 50%）。Cell 用 0.18 看起來是「有圓角的格子」；banner 用 0.24 看起來像「有圓角的卡片」。常數來自視覺實驗。
- Q：segments 越多越平滑？  
  A：對，但對小元素邊際效益很快歸零。Cell 大小 ~80px 時 6 段就看不出多邊形邊；要 100+px 才需要 8+。我用 6（圓角 helper）跟 8（其他）。

#### Score Connection
- 圖形介面：實作 GUI 2.5%（[scoring.md:90](scoring.md#L90)）— 圓角 + 漸層是 GUI polish 的具體實作

---

### 程序材質：高光帶 + bevel 在旋轉 cell 上的座標數學
*Phase 2 Day 2a · commit `<待 commit>`*

#### What & Key Concepts
- 每個 part cell 由三層 `DrawRectanglePro` 疊出來、**全部共用同一個 pivot 與 rotation**，所以旋轉時三層一起轉：
  1. **Base fill** — 整 cell（scs × scs），純色（中心格用 `+120/ch` 亮色，其他用原色）
  2. **高光帶** — cell 頂部，寬 `scs × 0.88` 高 `scs × 0.32`（左右各內縮 6%），色 `+70/ch` alpha 170
  3. **Bevel** — cell 底部，寬 `scs` 高 `scs × 0.10`，色原色 `÷2` alpha 220
- 座標計算（用 `inset = 0.06 * scs`、`bandH = 0.32 * scs`、`edgeH = 0.10 * scs`）：
  ```
  // base cell
  origin = ((cc - col + 0.5) * scs,
            (cr - r   + 0.5) * scs)

  // 高光帶（在 cell 頂部、左右內縮）
  bandRec.w = scs - 2 * inset
  bandRec.h = bandH
  bandOrigin.x = (cc - col + 0.5) * scs - inset   // 把 rect 從 cell 左緣往右挪 inset
  bandOrigin.y = (cr - r   + 0.5) * scs           // 跟 cell 頂部對齊

  // bevel（在 cell 底部）
  edgeRec.w = scs
  edgeRec.h = edgeH
  edgeOrigin.x = (cc - col + 0.5) * scs
  edgeOrigin.y = (cr - r   + 0.5) * scs - (scs - edgeH)   // 從頂往下挪 (scs - edgeH) 到底部
  ```
- 因為三層 `DrawRectanglePro` 都用 cell 的 pivot screen pos 跟 rotation 當參數，raylib 把它們一起轉、視覺上看起來是同一塊有材質的 cell。

#### Why This Design
- 為什麼算「特別材質」：rubric「圖片或特別材質」的「或」字明示 procedural 也算。高光帶 + bevel 模仿金屬鏡面/塑膠光澤效果、視覺上跟單色塊有明顯區別。
- 為什麼不畫到 RenderTexture 一張紋理：要做但要 baking 邏輯 + 紋理 size 對齊 + 每個 part color 一張 → 複雜度高、儲存量大。每 cell 三 `DrawRectanglePro` GPU cost 一樣低（都是 1 個矩形 = 2 個三角形）。
- 為什麼高光在頂部、bevel 在底部：模擬光源從上方來的真實感（高光對應反光、bevel 對應陰影）。倒過來就像光從地板照、不自然。
- 為什麼高光帶左右內縮：純頂部矩形跟 cell 同寬會看起來像「被劈成兩半」；左右內縮就像「鏡面反光只在中央區」、看起來更像金屬。

#### What I Should Be Able To Explain
- Q：高光帶為什麼要左右 inset？  
  A：跟 cell 同寬會看起來像被切兩半；inset 後高光集中在中央、像金屬反光、視覺更自然。
- Q：三層 rect 怎麼確保旋轉時一起轉？  
  A：`DrawRectanglePro` 的 `rec.x/y` 跟 `rotation` 三層都用同樣的值（pivot 螢幕位置 + currentAngle）；只有 `rec.w/h` 跟 `origin` 不同。raylib 把每個 rect 各自繞共同 pivot 旋轉相同角度。
- Q：為什麼 +70 / ÷2 而不是 hardcode 顏色？  
  A：每個 part 顏色不同（綠/藍/紅/黃...）。`+70/channel` 對所有色都會變亮、`÷2` 對所有色都會變暗。alpha 控制柔和度。比 hardcode 一張高光顏色表簡單且可遷移。
- Q：origin 的 y 為什麼 base cell 跟高光帶相同？  
  A：兩者的 rect 都從 cell 頂部開始畫；origin.y 對應到「rect 左上角」到 pivot 的垂直距離 = (cr - r + 0.5)*scs。Bevel 的 rect 從 cell 底部開始畫、所以 origin.y 要再減去 `(scs - edgeH)`。

#### Score Connection
- 圖形介面：使用圖片或特別材質 1.5%（[scoring.md:92](scoring.md#L92)） — 高光帶 + bevel 是本專案對「特別材質」的實作

---

### Drop shadow 為什麼要兩 pass 渲染
*Phase 2 Day 2a · commit `<待 commit>`*

#### What & Key Concepts
- 對盤面上的零件（placed 或 held-on-board）畫陰影：純黑 alpha 110、offset (4, 5) px 朝右下。
- **天真做法**：對每個 part 先畫 shadow、再畫 body
- **問題**：若 partA 跟 partB 在繪製順序上 partA 先、partB 後，partA 的 shadow 畫完接著畫 partA body 沒問題；但接下來畫 partB shadow 時、會疊到 partA body 上（partB shadow 朝右下、可能覆蓋 partA body 的右上角）。視覺：partB shadow 看起來像「打在 partA 身上」、不真實。
- **兩 pass 修法**：
  ```
  Pass 1: 對所有 part 畫 shadow（不畫 body）
  Pass 2: 對所有 part 畫 body
  ```
  Pass 2 的 body 必定畫在所有 shadow 之上（因為 Pass 2 整段比 Pass 1 後執行）。Shadow 永遠在底層。

#### Why This Design
- 為什麼不用 depth buffer：2D 遊戲 raylib 預設沒 depth、用 painter's algorithm（後畫的在上）。Two-pass 是 painter's algorithm 下標準的「分離層」做法。
- 為什麼不只給最後一個 part 畫 shadow：所有放在盤面上的 part 都應該有 shadow（同一個光源）、缺一個會看起來不一致。
- 為什麼 shadow 不畫 tray idle 部件：tray slot 在「淺凹槽」隱喻、部件已經在槽裡、沒立體感、shadow 反而會把 slot 邊緣弄髒。實作上 `drawParts` 判斷 `onBoard = location.placed || (held && cursorCol >= 0)` 才畫 shadow。

#### What I Should Be Able To Explain
- Q：為什麼一定要 two-pass？  
  A：一 pass 順序裡晚畫的 part 的 shadow 會疊到早畫的 part body 上、視覺上 shadow 像「打在身上」。Two-pass：先全部 shadow、再全部 body、確保 shadow 永遠在最底層。
- Q：你的「painter's algorithm」是什麼？  
  A：2D 渲染常用：後畫的覆蓋先畫的。raylib 預設沒 depth buffer、靠繪製順序決定誰在上。
- Q：tray 部件為什麼沒陰影？  
  A：tray slot 是「凹槽」隱喻、零件在槽裡沒立體感；加 shadow 會疊到 slot 邊緣、視覺髒。實作判斷 `onBoard` 才畫。
- Q：shadow alpha 為什麼 110 而不是 255 或 50？  
  A：110 對黑色 = ~43% 不透明、肉眼看是淡灰；視覺實驗結果。255 太黑像描邊、50 太淡看不出立體感。

#### Score Connection
- 圖形介面：實作 GUI 2.5%（[scoring.md:90](scoring.md#L90)） — drop shadow 是 GUI polish 細節之一

---

### Cursor 原語：用單一 setCursor 把多 input source 接到同條 Action 尾
*Phase 2 Day 2b · commit `<待 commit>`*

#### What & Key Concepts
- Phase 1 的 input flow：
  ```
  Input::poll() (鍵盤)  →  Action  →  Game::update(Action)  →  改 cursor + handlePlace/handleRemove
  ```
- Phase 2 Day 2b 加滑鼠時、有兩種設計選擇：
  - **方案 A**：擴 `Action` 變 `std::variant`、每個 mouse action 帶 row/col；core 全部要動。
  - **方案 C（採用）**：保留 keyboard flow 不動；Game 加一個 cursor 原語 `setCursor(row, col, isTray)`；Input 加 `pollMouse(Layout, Game&)` 翻譯滑鼠座標 → setCursor、再呼叫既有 `Action::Place / Remove`。
- 結果：keyboard 跟 mouse 共用同一條尾巴：
  ```
                                                ┌──────────────┐
  Input::poll()       →  Action::Move/Place/... ┤              │
                                                │ Game::update ├──→ handlePlace/Remove
  Input::pollMouse()  →  setCursor + update(    │              │
                          Action::Place/Remove) └──────────────┘
  ```

#### Why This Design
- **不擴 Action 的成本**：方案 A 看起來最 OO，但 `std::variant<MoveUp, ..., PlaceAt{row,col}, ...>` 要把所有 `update()` 的 switch 改寫成 `std::visit`、keyboard branch 也要適配 — 為了一個「滑鼠多帶 row/col」的小問題改動 core API 一大塊。
- **單一 cursor 原語的好處**：`setCursor` 內部只是 `cursorRow/Col = ...` + `clampCursor()` + `syncHeldLocation()`，跟既有 keyboard `Action::MoveX` 用同一個 clamp + sync 尾巴 — invariant 不會走兩條不同的路。
- **handlePlace / handleRemove 已經是三分支設計**：
  - handlePlace：持有+板面 → 放置 / 持有+TRAY → 提示 / 未持有+板面 OCCUPIED → 撿起 / 未持有+TRAY → 撿起 tray 該 part
  - handleRemove：持有 → 退回 tray / cursorCol==TRAY_COL → no-op / OCCUPIED → 拔起
  - 對任何 cursor 位置都成立。滑鼠只需要把 cursor 設過去、再走 Action 就對。
- **未來重用**：Phase 5 editor 也會有「點哪一格」需求 — `setCursor` 直接重用。
- **代價（已知）**：cursor 同時被鍵盤跟滑鼠驅動 — 滑鼠 hover 會蓋掉 keyboard 剛改的 cursor。**只有持有時** pollMouse 才會主動跟 cursor；不持有時 keyboard 路徑零干擾。

#### 設計演進：cell-quantized → pixel-perfect drag follow
第一版（PR 提交前）寫 cell-quantized：cursor 跟到 cell、Renderer 既有 lerp 到 cell center 自動產生 drag-follow 視覺；好處是 Renderer 維持「只看 Game state」的純函式。使用者目視驗收前回報想要「緊跟滑鼠、不要卡頓感、瞄準交給紅綠框、放開時落到框框那」、改成 pixel-perfect。

**關鍵洞察**：cursor 跟視覺其實是兩件事 —
- **cursor**（cell-quantized）= game logic 的「使用者目前指哪一格」、紅綠預覽框 / canPlace / 最終落點 都用 cell
- **part visual**（mouse pixel）= UI 的「手上拿著的零件目前長在螢幕哪裡」、純美學

把這兩件事用 `bool Game::mouseControlling` 解耦：
- 任何 mouse activity（移動、左/右鍵）→ `mouseControlling = true`
- 任何鍵盤 `Action::Move*` → `mouseControlling = false`
- Renderer 在「held + mouseControlling」分支 target = `GetMousePosition()` + scale 1.0；其它分支照舊（涵蓋 tray cursor，因此**從 tray 點擊撿起的那一 frame 就已跟手**、不等 mouse 移到板面）
- animateParts 加 `mouseDrag = held && mouseControlling`：snap `currentCenterX/Y`（不 lerp），只 lerp angle/scale

**手感結果**：
- mouse mode：part 緊跟滑鼠像素、零延遲；紅綠框落在 cursor cell；點下時 handlePlace 用 cursor cell（不是 mouse pixel）驗證 + 放置 — 視覺上「框內合法 = 點下 = 放這格」
- keyboard mode：跟 Phase 2 Day 1 一樣，視覺 = cursor cell center、所有東西 cell-quantized
- 放開時：`heldPartIdx = -1` → 那 part 變 placed → computeTarget 走 placed 分支（cell center）→ 既有 lerp 從 mouse pixel 滑到 cell center 產生「落框感」

**反思**：第一版設計時、我把「視覺跟 cursor 綁在一起」當作 Renderer 純函式的保證、結果手感差。**Renderer 直接讀 `GetMousePosition()` 確實打破了純函式性**（每 frame 不同 mouse → 不同 target），但這個耦合只發生在「held + mouseControlling」這個窄分支、其它分支仍是 Game state 的純函式。**邊界該打破時就要打破、不要為了純度抹掉手感**。

#### Implementation Notes
- [Game::setCursor](../src/core/Game.cpp) 三行：
  ```cpp
  void Game::setCursor(int row, int col, bool isTray) {
      cursorRow = row;
      cursorCol = isTray ? TRAY_COL : col;
      clampCursor();
      syncHeldLocation();
  }
  ```
- [Input::pollMouse](../src/ui/Input.cpp) 結構：
  1. `GetMouseDelta` / 點擊偵測 → 算 `mouseActive`；mouseActive → `g.mouseControlling = true`
  2. 板面 hit-test：`(mouseX - boardX) / cellSize` 整除 → (boardRow, boardCol) 或 -1
  3. tray hit-test：`(mouseY - trayY) / kTraySlotHeight` 整除 → trayRow 或 -1
  4. 左鍵：在板面 → `setCursor` + `update(Action::Place)`；在 tray 且未持有 → `setCursor` + `update(Action::Place)`
  5. 右鍵：在板面就先 `setCursor`、無論在哪 → `update(Action::Remove)`
  6. **末尾**：若 `mouseControlling && held && over board` → `setCursor` 跟著 mouse cell（每 frame、不依賴 mouseMoved）— 這樣 click-place 自動選下一個 part 後、新 part 也立刻跟上滑鼠
- Layout 從 [Renderer.cpp](../src/ui/Renderer.cpp) anon namespace 提到 [Renderer.h](../src/ui/Renderer.h) public — Input.cpp include Renderer.h 後共用 `Layout` / `computeLayout` / `kTraySlotHeight/Width`。沒形成循環（Renderer.h → Game.h；Input.h → Renderer.h → Game.h；Game.h 不 include 任何 UI）。

#### What I Should Be Able To Explain
- Q：為什麼不擴 Action enum 變 variant？  
  A：成本不成比例 — 只為了「滑鼠多帶 row/col」就要把 update() 的 switch 全部改寫、keyboard branch 也要適配。方案 C 用 cursor 原語分兩步、新增 1 個小函式即可。設計時優先考慮改動量、不要為了純度做大重構。
- Q：keyboard 跟 mouse 為什麼能共用同一條尾巴？  
  A：因為 cursor 是抽象的「使用者目前指的格子」、不限定誰改它的。`setCursor` 內部做 clamp + sync held、跟既有 `Action::MoveX` 共用尾巴。`handlePlace / handleRemove` 已經是「對任何 cursor 位置都成立」的三分支設計、不需要寫滑鼠版的 place / remove。
- Q：為什麼最後選 pixel-perfect drag follow？跟最初 cell-quantized 的設計演進是？  
  A：第一版選 cell-quantized（Renderer 維持純函式、視覺=cursor 簡單）、使用者試之前回報「希望緊跟滑鼠、不要卡頓」。改成 pixel-perfect：拆開 cursor 跟視覺、cursor 仍 cell-quantized（紅綠框、canPlace、落點）、視覺 = mouse pixel 且 snap 不 lerp。Renderer 確實在這個窄分支裡讀 `GetMousePosition()`、打破純函式性、但其它分支不受影響。學到：邊界該打破時就要打破、不要為了純度抹掉手感。
- Q：mouseControlling 怎麼切換？keyboard 跟 mouse 衝突時誰贏？  
  A：任何 mouse activity（移動、點擊）→ `mouseControlling = true`；任何鍵盤 `Action::Move*` → `mouseControlling = false`。Place/Remove/Rotate 不變動 flag。所以 mouse drag 中按 R 旋轉 → 維持 mouse mode；mouse drag 中按 W 移動 → 切回 keyboard mode、cursor 跟 part 立刻跳到 cell。
- Q：Layout 為什麼能從 anon namespace 提到 public header？  
  A：Layout 是 UI 內部物（不該 leak 進 Game/Parser/Solver），但需要被 Input 跟 Renderer 兩個 UI 模組共用 — 「public to UI、private to core」就是 Renderer.h 的位置。邊界不是非黑即白、要看 caller 是誰。
- Q：cursor 同時被鍵盤跟滑鼠驅動會打架嗎？  
  A：只有持有時 pollMouse 才會主動跟 cursor、不持有時 keyboard 路徑零干擾。持有時就是要 drag follow — 玩家按 W 然後 mouse hover 別處、cursor 跟著 mouse 是預期行為。

#### Score Connection
- 遊戲流程：wasd/滑鼠移動 5%（[scoring.md:47](scoring.md#L47)）+ Enter/左鍵放置 1%（[scoring.md:50](scoring.md#L50)） — 不增加分數，但補上「或滑鼠拖移 / 左鍵放置」的 rubric 完整性與 demo 體驗


### Game 狀態 snapshot：value-type deep copy 還原初始盤面

*Phase 3 · commit `<待 commit>`*

#### What & Key Concepts

`Game::resetToInitial()` 要把整盤遊戲狀態回到 `init()` 剛讀完檔的樣子。實作上不靠記檔案路徑、不靠 re-parse：而是 `init()` 末尾就把 `board` 與 `parts` 各 copy 一份到 private 成員 `initialBoard` / `initialParts`，reset 時 `board = initialBoard; parts = initialParts;` 一行還原。

**關鍵在 C++ 的 value semantics**：`Board` 跟 `Part` 全是 value-type 成員（`std::vector<std::vector<...>>`、`int`、`enum`、`float`），整個物件 `operator=` 預設就是 deep copy — 不必自己寫 copy constructor 或 deep-copy helper。`std::vector` 的 `operator=` 會配新記憶體並逐 element copy，巢狀 vector 同理遞迴下去。

**Snapshot 的時序很關鍵**：`init(Board b, std::vector<Part> p)` 的 parameter 是 by-value（已經是 caller 給的 copy），主體用 `std::move(b/p)` 把資料搬到成員。搬完 `b` / `p` 就空了，要從**成員** copy 而不是從 parameter copy：

```cpp
void Game::init(Board b, std::vector<Part> p) {
    board = std::move(b);       // b 變空殼
    parts = std::move(p);       // p 變空殼
    initialBoard = board;       // 從成員 copy，是 deep copy
    initialParts = parts;       // 同理
    // ... reset cursor / held / won ...
}
```

「reset 時要不要重置 Part 視覺欄位（`currentCenterX/Y/Angle`、`visualInitialized`、`rotateCount`）」直接被 snapshot 解掉了：snapshot 是在 `init()` 末尾、玩家還沒互動之前拍的，那時所有 Part 視覺欄位都是 default 值（`visualInitialized=false`、`rotateCount=0`、`currentAngle=0`、`currentScale=1`）。reset 時整個 copy 過去，視覺欄位自動歸 default — 不必逐欄位手動重置。

#### Why This Design

- **不依賴 file system**：若改用「reset = 把檔案路徑塞進 Game、reset 時 re-parse」，檔案中途被改 / 刪 / mv → reset 失敗。snapshot 在記憶體裡，不受外界影響。
- **不必擴 Game 的 public API**：不用把 `std::string levelPath` 變成 Game 的成員、不用把 Parser 變成 Game 的依賴。reset 只看自己擁有的兩個成員。
- **記憶體成本可接受**：Example1 約 10 個 part、6×6 board，snapshot 大概幾 KB，遠小於 re-parse 帶來的 parser code path 複雜度。
- **避免逐欄位手動重置的 bug**：手寫「reset 時要清哪些欄位」會跟 Phase 2 視覺欄位賽跑 — 每加一個視覺欄位就要記得來 reset 補一筆，很容易漏。整盤 copy initial 是天然防呆。

#### What I Should Be Able To Explain

- Q：為什麼 deep copy 不必自己寫？  
  A：`Board` / `Part` 的所有成員都是 value-type（`std::vector`、enum、POD），C++ 預設的 `operator=` 對 class 做 member-wise copy；對 `std::vector` 來說 member-wise copy 就是配新記憶體 + 逐 element copy（深層）。所以 `initialBoard = board;` 一行就完成整盤拷貝。如果欄位裡有 raw pointer，就必須自己寫 copy constructor 處理；本專案剛好沒有 raw pointer。
- Q：為什麼從成員 copy 而不是從 parameter copy？  
  A：parameter `b` / `p` 已經被 `std::move` 搬空了，再 copy 過去只能拿到 valid-but-unspecified 的空殼。從成員 copy（已經是搬完的目的地）才是完整資料。
- Q：為什麼不用 re-parse 檔案的方式 reset？  
  A：(1) 不依賴外部檔案 — 中途檔案被改 / 刪不影響；(2) 不必把 file path / Parser 塞進 `Game` 的依賴；(3) 記憶體成本小於 parser code path；(4) reset 速度比 re-parse 快（一個 memcpy vs 一段 parsing 邏輯）。
- Q：reset 為什麼要在 `if (won) return` 之前處理？  
  A：勝利後玩家也要能重新開始；如果 Reset 走一般 action 流程就會被 win-gate 擋住，所以必須早早攔截。
- Q：Phase 2 視覺欄位（`currentCenterX/Y/Angle/Scale`、`visualInitialized`、`rotateCount`）reset 時要怎麼處理？  
  A：snapshot 是 `init()` 末尾拍的、玩家還沒互動前 — 那時視覺欄位都是 default 值。reset 時整個 copy 過去就自然回到 default，不必逐欄位手動重置，也不會漏掉新加的視覺欄位。

#### Score Connection
- 進階功能：重置盤面為初始狀態 1%（[scoring.md:56](scoring.md#L56)）

---

### AppState state machine：同一個 process 內切換新關卡

*Phase 3 · commit `<待 commit>`*

#### What & Key Concepts

Issue 02-new-game 的核心不是「把同一關 reset」，而是讓程式在不關閉視窗、不重啟 process 的情況下，離開當前關卡、選另一個關卡、重新 parse 並呼叫 `Game::init()`。

這次在 [main.cpp](../src/main.cpp) 加了一個很小的 state machine：

```cpp
enum class AppState {
    Menu,
    InGame,
};
```

主迴圈每 frame 依狀態分流：

```cpp
if (appState == AppState::Menu) {
    // keyboard / mouse 選關，選到後 loadGame(path, game, err)
} else {
    // 原本的 game update / mouse / sound / render
    // KEY_N 會切回 Menu
}
```

`loadGame(path, game, err)` 仍是單一入口：開檔、`Parser::parse`、`game.init`。這讓「argv 啟動」、「menu 選關」、「之後 editor 匯出後試玩」都能共用同一條載入路徑。

#### Why This Design

- **保留 argv 直接進遊戲**：`argc > 1` 時仍先 `loadGame(argv[1])`，成功才開視窗進 InGame；壞路徑仍印錯誤並 exit 1。這保住讀檔 hard constraint。
- **menu 只放在 main.cpp**：這輪先服務 #4，不新增 `Menu.cpp` / `Menu.h`。#6 要做完整漂亮主畫面時，可以再把 menu code 抽出去。
- **`Game::init()` 是切關重置點**：#3 reset 已經讓 `init()` 會清 cursor / held / won / status / mouseControlling，切關不需要額外手動清 transient state。
- **sound lifecycle 不跟關卡綁定**：`InitAudioDevice` 和 `LoadSound` 仍只在 process 啟動時跑一次。切關後只重設 `prevHeld / prevWon / prevPlacedCnt / prevRotateSum` baseline，避免新關第一 frame 誤判成事件。
- **level 掃描 fallback**：正式設計是 `assets/levels/*.txt`，但目前 repo 的 Example1–6 在 `docs/io/`。`findLevels()` 先找 `assets/levels`，沒有就 fallback 到 `docs/io`，讓現在可以直接驗收。

#### What I Should Be Able To Explain

- Q：Reset 跟 New Game 差在哪？  
  A：Reset 是同一個已載入關卡回到初始狀態，不重新 parse、不離開 InGame；New Game 是回 menu 選關，重新 `Parser::parse` 並 `Game::init`，可以換不同檔案。
- Q：為什麼 `KEY_N` 不放進 `Action` enum？  
  A：`Action` 是遊戲內的抽象操作（Move / Rotate / Place / Remove / Reset），New Game 是 app-level flow，會切 `AppState`，不應該塞進 core `Game::update()`。所以 `main.cpp` 直接偵測 `IsKeyPressed(KEY_N)`。
- Q：勝利後為什麼也能按 N？  
  A：`N` 在 main loop 的 InGame branch 先處理，沒有走 `Game::update()`，因此不會被 `if (won) return;` 擋住。
- Q：為什麼切關後音效不會亂叫？  
  A：切關成功後呼叫 `resetSoundBaseline()`，把上一 frame snapshot 更新成新關初始狀態。下一 frame diff 不會把「新關初始值」誤認成 pickup/place/win。
- Q：為什麼現在 menu 是 minimal，不直接完整做 #6？  
  A：#4 的分數重點是 flow；#6 的分數重點是「精美主畫面」。先用 minimal menu 驗證 state machine 和切關，再在穩定基底上做視覺 polish，風險比較低。

#### Score Connection

- 進階功能：遊戲進行中或遊戲結束後可直接開啟新遊戲 1%（[scoring.md:57](scoring.md#L57)）— 目前程式碼完成，待手動驗收後入帳。

---

### Menu level source：assets/levels 作為打包後關卡入口

*Phase 3 · commit `<待 commit>`*

#### What & Key Concepts

主畫面選關不能依賴 `docs/io`，因為 demo 打包時要出貨的是 exe + `assets/` folder，而不是整個 repo 文件目錄。因此 #6 把 Example1–6 複製到 `assets/levels/`，並讓 `findLevels()` 只掃：

```cpp
const fs::path root = "assets/levels";
```

這件事把「開發測資」和「遊戲可選關卡」分開：

- `docs/io`：保留為格式文件與助教範例來源，方便閱讀與比對
- `assets/levels`：runtime menu 的正式關卡來源，會被 CMake post-build copy 到 `build/assets/levels`

#### Why This Design

- **打包一致性**：CMake 已經會 copy `assets/` 到 executable 旁。menu 掃 `assets/levels`，Windows demo 時只要帶 exe + assets 就能選關。
- **符合 issue 行為**：#6 指定主畫面要從 `assets/levels/*.txt` 動態掃，而且顯示 filename only、按 filename 排序。
- **missing / empty graceful**：若 `assets/levels` 不存在或沒有 txt，`findLevels()` 回空 vector；menu 顯示 no playable levels，不 crash。
- **hover 與 keyboard 解耦**：滑鼠 hover 可以有高亮，但只有滑鼠真的移動時才更新 selected；這避免滑鼠停在某格時，Up/Down 每 frame 被拉回 hover 格。

#### What I Should Be Able To Explain

- Q：為什麼不繼續 fallback 到 `docs/io`？  
  A：`docs/io` 是開發/文件資料，不是 runtime asset。正式 demo 打包不保證有 docs folder；menu 應依賴會跟 exe 一起出貨的 `assets/levels`。
- Q：為什麼 assets/levels 裡要複製 Example1–6，而不是移動？  
  A：`docs/io` 仍是輸入格式文件與測資參考位置；`assets/levels` 是遊戲可選關卡清單。兩者用途不同，複製可以避免文件連結與既有測試路徑失效。
- Q：如果關卡超過 6 個怎麼辦？  
  A：menu 每次顯示 6 列，`menuVisibleStart(selected, levelCount)` 讓列表圍繞目前 selected 捲動；Up/Down 可以走完整清單。
- Q：hover 跟鍵盤選取如何避免衝突？  
  A：hover row 仍可視覺高亮；但 selected 只有在 mouse delta 非 0 時才被 hover 更新。滑鼠靜止時，鍵盤 Up/Down 可以自由切換；左鍵點擊會直接選 hover 的關卡。

#### Score Connection

- 進階功能：精美主畫面 / 關卡選擇 1%（[scoring.md:58](scoring.md#L58)）— 目前程式碼完成，待手動驗收後入帳。

---

### Dual-color renderer：constraints 從單色 hardcode 改成 C 色 layout

*Phase 3 · commit `<待 commit>`*

#### What & Key Concepts

雙色關卡的 core 其實早就大致準備好：`Parser` 會依 `C` 讀每色 constraints，`WinChecker` 也會依 `board.colors` 逐色檢查。真正卡住的是 Renderer 仍有 Phase 1 留下的單色假設：

```cpp
const int color = 0; // Phase 1: single color
```

這次把 constraint hints 改成 loop over `board.colors`：

- column hint：每個顏色一排，往 board 上方堆疊。
- row hint：每個顏色一欄，往 board 左側堆疊。
- `computeLayout` 依色數增加 top / left reserve，避免雙色 hints 壓到 sidebar。
- tray 色條與零件本體改用 `Part::colorIndex`，讓零件顏色、固定格顏色、hint 顏色使用同一套 `colorBadge(...)`。

#### Why This Design

- **不動 core**：多色資料已經在 `Board::_constraints[color][...]`、`Part::colorIndex`、`Board::cannotMoveColor(...)` 裡；UI 只需要正確呈現，不應重寫 parser 或 win rules。
- **每色一排/一欄最直觀**：比把兩個數字塞同一格更容易看懂，也方便下一張 issue 04-row-col-hints 加 `current/need` 和 overfilled 狀態。
- **單色不退化**：`board.colors == 1` 時 offset 算出來接近原本 `boardY - 30` / `boardX - 30` 的位置，不會突然多出雙色空白。
- **顏色語意一致**：原本 `partColor(partIndex)` 讓每個零件用不同顏色，雙色時會和 constraints 的 color index 斷開。改成 `colorBadge(colorIndex)` 後，同色零件會和同色 hint / fixed cells 對應。

#### What I Should Be Able To Explain

- Q：為什麼雙色不需要大改 Parser？
  A：Parser 從 Phase 1 就用 `for (int color = 0; color < C; ++color)` 讀 constraints 和 fixed cells，part 也有 `colorIndex`。雙色缺的是 Renderer 把這些資料畫出來。
- Q：row / column constraints 的 index 怎麼存？
  A：`_constraints[color][0..rows-1]` 是每列需求，`_constraints[color][rows..rows+cols-1]` 是每欄需求。所以 column hint 取 `b._constraints[color][L.rows + c]`，row hint 取 `b._constraints[color][r]`。
- Q：為什麼零件本體要用 `colorIndex`，不是 `partIndex`？
  A：`partIndex` 是「第幾個零件」，只適合當 unique id；`colorIndex` 才是 puzzle 規則裡的顏色。WinChecker 也用 `parts[partIdx].colorIndex` 計數，所以 UI 應跟規則一致。
- Q：助教 demo 雙色測資怎麼拿分？
  A：保持 runtime path 載入：`./build/game path/to/ta-dual-color.txt`。只要格式符合純文字規格，Parser 讀 `C=2`，Renderer 依 `board.colors` 畫兩色 hints，WinChecker 依兩色 constraints 判勝。

#### Score Connection

- 進階功能：雙色設定檔載入與遊玩 2%（[scoring.md:62](scoring.md#L62)）— 程式碼完成，待 Example5/6 手動遊玩與勝利驗收後入帳。
- 助教 demo 雙色測資 2%（[scoring.md:63](scoring.md#L63)）— demo day 才能用助教檔案實測。

---

### Row/column current hints：把計數邏輯放回 Board

*Phase 3 · commit `<待 commit>`*

#### What & Key Concepts

Issue 04 要讓每個 row / column hint 顯示 `current/need`，並依狀態變色：

- `current < need`：灰色，代表還不足。
- `current == need`：綠色，代表剛好滿足。
- `current > need`：紅色，代表超出。

關鍵不是 Renderer 怎麼排字，而是「current 怎麼算」必須和勝利判定一致。因此這次把原本藏在 `WinChecker.cpp` anonymous namespace 的 `cellCountsForColor(...)` 邏輯搬到 `Board.cpp`，並提供：

```cpp
unsigned Board::currentFilledForColor(int color, int idx, bool isRow,
                                      const std::vector<Part>& parts) const;
```

Renderer 和 WinChecker 都呼叫這個 helper，避免兩邊計數規則分裂。

#### Why This Design

- **Board 是盤面狀態查詢的自然位置**：`canPlace` 已經在 Board；「這一列目前某色有幾格」也是 Board 的 query。
- **WinChecker 和 Renderer 共用同一套計數**：如果固定格、已放零件、雙色規則未來有調整，只要改 Board helper，hint 與勝利判定一起更新。
- **不把 hint 狀態變成 placement rule**：`current > need` 只變紅，不禁止玩家放置。這符合 issue non-goal，也避免把 UI feedback 混進 core placement validation。
- **雙色自然支援**：helper 接 `color`，Renderer 外層已經 loop `board.colors`，所以每色各算各的 current。

#### What I Should Be Able To Explain

- Q：為什麼 `currentFilledForColor` 需要 `parts` 參數？
  A：`Board::_boardInfo` 對已放零件只存 `OCCUPIED + partIndex`，真正的顏色存在 `parts[partIdx].colorIndex`。所以只看 Board cell 值不知道 occupied cell 屬於哪個 color，必須帶 parts 查回去。
- Q：固定格怎麼算進 current？
  A：固定格用 `CANNOT_MOVE - color` 編碼，`Board::isCannotMove(cell)` 後用 `Board::cannotMoveColor(cell)` 取回顏色。若等於要查的 color，就算一格 current。
- Q：為什麼 WinChecker 也要改？
  A：以前 WinChecker 自己在 anonymous namespace 算 current；Renderer 如果再寫一份就會重複邏輯。改成 WinChecker 呼叫 Board helper 後，勝利判定和 UI hint 的「目前格數」完全一致。
- Q：為什麼超出不直接禁止放置？
  A：issue 明確說 hint 只是提示，不改變 placement rule。玩家可能暫時超出，之後移除或重新擺；紅色提示足夠。

#### Score Connection

- 進階功能：某列/欄滿足或超出需求時周圍提示 1%（[scoring.md:59](scoring.md#L59)）— 程式碼完成，待手動驗收後入帳。
- 進階功能：周圍顯示每列每欄目前填滿格數 1%（[scoring.md:60](scoring.md#L60)）— 程式碼完成，待手動驗收後入帳。

---

## Solver：backtracking + 剪枝 + 重用既有規則

*Phase 4 增量 1 · commit `<待 commit>`*

### 它在做什麼

`src/core/Solver.{h,cpp}`。輸入一個盤面 `Board`（只有固定格 + 不可放置格 + 空格）和一堆未放置的零件 `vector<Part>`，找出「每個零件要放哪一格、轉幾度」使得**每個顏色的每一列、每一欄填滿的格數剛好等於需求**（就是遊戲的勝利條件）。回傳一組解（多解只給一個），無解回 `std::nullopt`。

API 故意設計成**純函數**：`Solver::solve(Board, vector<Part>, nodeBudget)` 吃**傳值**（copy），不動到正在玩的 `Game`。Game 按 `F` 時呼叫它、拿到解、再套用到實際盤面。所以 Solver 完全不依賴 raylib / Game / UI，可獨立測試、也可交接給夥伴。

### 演算法（為什麼選 backtracking）

零件擺放是典型的**約束滿足 / 組合搜尋**問題，沒有公式解，最直觀且口試好解釋的就是 **backtracking（回溯）**：

1. 依序處理每個零件。
2. 對當前零件，枚舉「每個不同旋轉 × 盤面每個 (row, col)」。
3. 若 `Board::canPlace` 過 → `place`（暫時放上去）→ 遞迴處理下一個零件。
4. 下一層失敗就 `remove`（撤回），換下一個位置 / 旋轉。
5. 所有零件都放完 → 用 `WinChecker::isWon` 做最終精確檢查；過了就是一組解。

### 三個讓它夠快、夠穩的關鍵

- **去重旋轉**：正方形或對稱零件轉 90°/180° 可能長一樣，先把重複的旋轉濾掉，少跑無用分支。
- **超量剪枝**：每次放完就檢查——只要**任何一列或一欄某個顏色的填滿數已經超過需求**，這條路就死了（之後只會加格、不會減），直接回溯。這把搜尋樹砍掉非常多。
- **最大件優先**：先放格子最多的零件（合法位置最少），讓搜尋樹在上層就變窄。
- **node budget**：設一個展開節點上限（預設 200 萬），超過就放棄回 `nullopt`，保證 demo 當天不會卡死當機。

### 為什麼「重用既有規則」很重要

Solver **沒有自己重寫任何遊戲規則**：能不能放用 `Board::canPlace`、放/撤用 `Board::place`/`remove`、某列某色填滿數用 `Board::currentFilledForColor`、勝利判定用 `WinChecker::isWon`。這樣 solver 對「什麼叫解開」的定義跟玩家手動玩的定義**保證一致**，不可能出現「solver 說解開了但遊戲不認」的 bug。這也是專案 core/UI「規則只寫一份」原則的體現。

### 套用解到實際盤面（`Game::solveAndApply`）

從 `initialBoard`/`initialParts`（乾淨快照）去解，避免半完成的盤面干擾搜尋；解出後 reset 到初始再逐件 `place`。有個小細節：每件的 `rotateCount` 要設成跟解出的 `rotate` 對齊（renderer 的旋轉動畫是 lerp 到 `rotateCount * 90°`），否則會出現「邏輯上轉了但畫面沒轉」的視覺錯位。

### 口試我要能講出什麼

- backtracking 的「放→遞迴→撤回」骨架，以及它為什麼適合這種沒有公式解的擺放問題。
- 三個剪枝/加速手段各自砍掉什麼（尤其「超量即剪枝」的正確性論證：填滿數單調遞增）。
- 為什麼 solver 傳值、純函數、重用 Board/WinChecker——測試性、交接性、規則一致性。
- node budget 是防呆，不是演算法的一部分。

### Score Connection

- 自動解題：自動解題並顯示一個解 1%（[scoring.md:67](scoring.md#L67)）— 代碼完成、headless 驗 Example1–6 全解，待 GUI 目視驗收後入帳。
- 自動解題：解開單色 +2%（[scoring.md:68](scoring.md#L68)）/ 雙色 +2%（[scoring.md:69](scoring.md#L69)）— demo day 用 TA 測資實測。
