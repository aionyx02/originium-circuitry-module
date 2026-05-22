# learning-notes.md — 主題式技術筆記

> Last updated: 2026-05-22  
> 規劃：[plan.md](plan.md) ｜ 進度：[STATUS.md](STATUS.md) ｜ 過程紀錄：[LOG.md](LOG.md) ｜ 工作規範：[CLAUDE.md](../CLAUDE.md)

> **這份檔回答「這段 code 在做什麼、為什麼這樣做、demo / 口頭報告我得能講出什麼」。**  
> 對應口頭報告的「對程式的熟悉度」15%。  
> 過程紀錄、prompt、AI 怎麼幫請寫在 [LOG.md](LOG.md)，**不要重複寫在這裡**。

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

```
### Feature
功能 / 模組名稱

### What & Key Concepts
- 這個模組在做什麼
- 用到的 C++ / raylib / 演算法 / 資料結構概念
- 若有非平凡演算法，簡述（不貼整段 code，貼關鍵幾行 + 解釋）

### Why This Design
- 為什麼這樣設計？
- 考慮過哪些替代方案？為何不選？
- （**不寫「Claude 建議了什麼」** — 那是 LOG.md 的事）

### What I Should Be Able To Explain
demo / 口頭報告會被問到時，要能講出來什麼？
列 3–5 個 bullet，每個都是「老師可能問的問題 + 我會怎麼答」
範例：
- Q：為什麼旋轉用 lerp 而不是直接設 90°？
  A：直接設會跳變、無動畫；lerp(current, target, 0.18) 每 frame 逼近 target，視覺上是平滑旋轉。
- Q：rotate 4 次回到原狀，這怎麼處理？
  A：用 rotationSteps % 4 紀錄狀態，render 時用 renderAngle 平滑插值到 90 * steps。

### Score Connection
對應 [scoring.md](scoring.md) 哪幾項，含 % 與行號
範例：旋轉動畫 — 5%（scoring.md L93）
```

---

## Notes

---

### Feature
Parser：純文字關卡檔 → `Board + vector<Part>`

### What & Key Concepts
- 檔案格式（[io/input-file-format.md](io/input-file-format.md)）：第一行 `C M N`（顏色數 / 列 / 欄）→ 每色：M 個列需求、N 個欄需求、X1 固定零件位置 → 全色跑完接 X2 不可放置格 → 最後是零件清單 `colorIdx M2 N2` + 0/1 矩陣，直到 EOF。
- 用 `std::istream::operator>>` 逐 token 讀，自動跳空白與換行 → **格式對空白／空行很寬容**，助教手寫的測資也讀得進。
- 例外路徑：丟自訂 `Parser::Error`（C++ 標準例外型別不夠語意明確）。
- 邊界檢查：fixed-part / blocked-cell 座標、顏色索引超界都直接丟例外。

### Why This Design
- 為什麼純文字不用 JSON：助教 demo 當天會給測資，**測資是純文字格式**。用 JSON 等於放棄 4% 分（單色 +2% + 雙色 +2%）。詳見 [LOG.md「技術棧決策」entry](LOG.md#2026-05-22--技術棧決策c17--raylib-55--cmake--mingw-static) Risks/Follow-ups 段。
- 為什麼用 `>>` 而不是逐行讀 + split：`>>` 自動處理空白，省下 80% 的解析錯誤處理代碼；老師格式不規範空行也讀得進。
- 為什麼例外而非 `std::optional`：要把錯誤訊息傳出去（例如「parse error: expected row constraint」），optional 沒有 payload。

### What I Should Be Able To Explain
- Q：為什麼不用 JSON？  
  A：助教 demo 測資是純文字、教師指定格式；用 JSON 會讓我們讀不到測資，失去 4% 配分。早期版本確實選了 JSON、後來在 plan.md 寫作時 catch 到推翻。
- Q：parser 怎麼處理檔尾？  
  A：零件清單 while loop 讀 `colorIdx` 失敗就跳出（EOF 或非數字），不需要顯式 part-count。
- Q：固定零件 vs 不可放置格差別？  
  A：固定零件（CANNOT_MOVE）算入該色的列/欄需求數，不可放置格（CANNOT_PLACE）只是禁止任何零件覆蓋。前者帶顏色 idx 編碼進 cell value。

### Score Connection
- 基本項目 5%：讀單色設定檔 + 正常顯示 — 2%（[scoring.md:40](scoring.md#L40)）
- 助教 demo 測資 +2%（[scoring.md:41](scoring.md#L41)） — Phase 6 demo 當天才能拿

---

### Feature
Board cell 編碼方式

### What & Key Concepts
- `_boardInfo[r][c]` 是 `int`，一個欄位塞四種狀態：
  - `EMPTY = 0`
  - `CANNOT_PLACE = -1`（不可放置格）
  - `CANNOT_MOVE - color`：固定零件，例如 color=0 → cell = -2、color=1 → cell = -3
  - `OCCUPIED + partIndex`：被動零件覆蓋，例如 partIdx=0 → cell = 1、partIdx=1 → cell = 2
- 解碼用 static helpers：`Board::isOccupied(cell)`、`occupiedPartIndex(cell)`、`isCannotMove(cell)`、`cannotMoveColor(cell)`
- skeleton（[Framework/Board.h](../Framework/Board.h)）給的 enum 沒寫 underlying type；C++17 預設 underlying 是某個能 hold 列出的 enumerator 的最小 int 型別 → 不允許存 `OCCUPIED + partIdx` 這種任意 int。所以 **enum 加 `: int` 明確指定**讓 cast 合法（[Phase 1 LOG entry「skeleton 修正」](LOG.md#2026-05-22--phase-1-完成mvp-單色關卡可玩) 段）。

### Why This Design
- 為什麼一個 int 塞四種：教師 skeleton 就這樣設計、不想偏離；負數區用來標固定/禁止、正數區用來標被佔據，0 = 空，乾淨分離。
- 為什麼不用 struct `{ State state; int extra; }`：更乾淨但要動 skeleton 的型別 → 違反 Hard Constraint #4「保留 Part/Board 命名與欄位」。
- enum 加 `: int` 是**一字元變更**，不算改名也不算改欄位，符合 skeleton 精神。

### What I Should Be Able To Explain
- Q：為什麼 cell 是 int 不是 enum？  
  A：教師 skeleton 規定。狀態多到 5 種、且 occupied / cannot_move 還要帶 partIdx 或 colorIdx，所以塞 int 用算術 offset 編碼。
- Q：`OCCUPIED + partIdx` 跟 `CANNOT_MOVE - color` 會撞嗎？  
  A：不會。OCCUPIED=1（正數區）、CANNOT_MOVE=-2（負數區）、CANNOT_PLACE=-1。正負 + 0 三區分開。
- Q：為什麼 enum 要加 `: int`？  
  A：C++17 enum 預設 underlying 是 compiler 挑「能 hold 列出值」的最小 int 型別，可能是 signed char 之類。`static_cast<BoardInfo>(OCCUPIED + partIdx)` 結果若超出該型別範圍是 UB。加 `: int` 強制 underlying 是 `int`，cast 合法。

### Score Connection
- 基本項目 5%：Board class 1% ＋ 讀單色設定檔 2% 都靠這個編碼撐起來

---

### Feature
Cursor 設計演進：Phase 1「= bbox 左上角」→ Phase 2 Day 1.6「= pivot target」

### What & Key Concepts
- **Phase 1 模型**：cursor 是「下一個放置位置的 bbox 左上角」。游標跨 tray + board：`cursorCol == -1` 是 tray、`>= 0` 是 board。clampCursor 在 col=-1 時把 row 上限放寬到 `max(parts.size(), board.rows) - 1`。
- **Phase 2 改動**：cursor 改成「pivot cell 要落的位置」。`syncHeldLocation` 內部用 `rotatedCenterCell` offset 把 bbox top-left 算出來：`location.row = cursorRow - rotatedCr; location.col = cursorCol - rotatedCc`。
- **效果**：旋轉時 cursor 不動 → pivot 螢幕位置不動 → 其他 cells 繞 pivot 轉。`Board::canPlace` API 完全沒動，只是 `Game` layer 餵進不同的 (row, col)。
- **Esc 雙語意**：持有狀態 = 把零件還回 tray；無持有 + cursor 在 OCCUPIED cell = 把那 cell 對應的 part 從盤面拔回 hand。對使用者來說都是「取消當前動作」的單一語意，但 implementation 是 cursor context 決定的。

### Why This Design
- 為什麼不分兩個 cursor（一個 tray、一個 board）：要在 tray 跟 board 間切換時 cursor 狀態同步更麻煩。一個 cursor + col=-1 sentinel 簡單。
- 為什麼 Phase 2 改 cursor 語意而不改 `Board::canPlace`：API 動越少越好。Board 是教師 skeleton 規定要保留的核心 class。Game layer 的語意自由，怎麼改都行。
- 為什麼拔零件時 snap cursor：不 snap 的話、舊 bbox top-left 經 syncHeldLocation 重算後可能跟 cursor 對不上，零件視覺會跳。snap 後算出來的 location 等於拔之前的 location，零件留在原位。

### What I Should Be Able To Explain
- Q：cursor 怎麼跨 tray 跟 board？  
  A：用 `cursorCol == -1` 當 sentinel 表示「在 tray」、`>= 0` 表示「在 board」。clampCursor 在這兩個區域分別 clamp row 上限。
- Q：Phase 2 為什麼改 cursor 語意？  
  A：使用者要求旋轉時 pivot 不動。原本 cursor 指 bbox 左上角，rotate 後 pivot 在 bbox 內位置變了 → 螢幕位置漂；改成 cursor 指 pivot target，rotate 時 cursor 不動 → pivot 不動。
- Q：拔零件為什麼要 snap cursor？  
  A：拔之前 location.row/col 是拔之前的 bbox top-left；拔之後 syncHeldLocation 會用「cursor - rotatedPivot」重算。如果 cursor 不 snap 到原 pivot 位置，新算出來的 bbox top-left 可能跟原本不同，零件會視覺跳一格。
- Q：Esc 為什麼兩種行為？  
  A：對使用者是「取消當前動作」的單一語意；implementation 看 heldPartIdx 與 cursor 位置決定具體做什麼（hand→tray 或 board→hand）。

### Score Connection
- 遊戲流程：wasd 移動 5%（[scoring.md:47](scoring.md#L47)）、Esc 拔掉 1%（[scoring.md:49](scoring.md#L49)）
- 進階：旋轉/移動中心不為空 1%（[scoring.md:61](scoring.md#L61)） — Phase 2 改 cursor 語意是為了滿足這一項的視覺效果

---

### Feature
動畫 lerp 框架（frame-rate independent）

### What & Key Concepts
- 每 frame 把 current 往 target 拉一個比例。比例 = `1 - exp(-dt * k)`。
- `dt` = `GetFrameTime()`，每 frame 不同。`k` = 衰減速率常數（位置 14、角度/scale 12）。
- 公式對 dt 是線性可拆的：兩個 dt 半步 ≡ 一個 dt 全步（指數性質 `e^(-dt*k) * e^(-dt*k) = e^(-2*dt*k)`），所以 60FPS / 144FPS 的視覺速度一致。
- 對比固定 factor `current = lerp(current, target, 0.4)`：60FPS 跑一秒衰減約 60 次、144FPS 跑同一秒衰減 144 次 → 高 frame rate 動畫變兩倍快。

### Why This Design
- 為什麼不直接用固定 factor：demo 機跟開發機 frame rate 可能不同（教室電腦未知）。
- 為什麼指數衰減而不是線性插值：指數衰減 = 「越接近目標越慢」，看起來像彈簧；線性 = 等速 → 終點突然停。
- `k` 選 12–14：肉眼大約 0.3 秒內到位，不會慢到拖沓也不會快到看不見動畫。

### What I Should Be Able To Explain
- Q：為什麼 `1 - exp(-dt*k)`？  
  A：frame-rate 獨立。兩個半 dt 跟一個全 dt 的效果一樣，因為指數的可加性。固定 factor 做不到。
- Q：`k=14` 是什麼意思？  
  A：時間常數倒數。`1/k = 0.071秒` ≈ 動畫衰減到 e⁻¹（≈37%）所需時間。所以 0.3 秒能衰減到 ~1%、肉眼看到位。
- Q：lerp 永遠不會真的到達 target，怎麼辦？  
  A：浮點下會無限趨近、實務上幾 frame 後 diff 小於 1px、肉眼看不出。如果要嚴格 snap 可以加 threshold「diff < 0.5 時直接賦值 target」，但目前沒做。

### Score Connection
- 圖形介面：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)）+ 零件跟游標連續移動 5%（[scoring.md:94](scoring.md#L94)）

---

### Feature
單調累加 `rotateCount` vs `fmod` shortest-arc

### What & Key Concepts
- 旋轉動畫第一版用 `fmod(target - current + 540, 360) - 180` 算最短弧。數學上正確，但浮點實務上脆弱：
  - `current` 隨 lerp 累積、可能比 target 稍微超過或低於整 90°
  - 多次 R 後 `current` 在 360 附近浮動，下次 mod 出來的 diff 不一定是預期方向
  - 使用者實測：4 次 R 後第 4 次反向
- 改用 `Part::rotateCount`（unsigned，每按 R `++`）+ `targetAngle = rotateCount * 90.0f`：
  - target 永遠是「下一個 +90°」，單調遞增
  - `currentAngle` 線性 lerp 永遠是「往大走」 → 視覺一定 CW
  - 沒有 mod、沒有正負判斷
- 代價：`currentAngle` 會無限長大；float 在數千度內精度都夠（每按 R 加 90，按 10000 次才到 900000，float32 精度仍有 16 位有效數字）。Render 出來看起來和 mod 後一樣。

### Why This Design
- 為什麼換掉：第一版被使用者實測 fail，AI debug 出來的算法雖然 case analysis 都對、實際還是看到反向 → 改 robust 而非 clever。
- 為什麼不用 double：raylib API 大多用 float，到處 cast 反而髒。float 精度夠 → 不必動。

### What I Should Be Able To Explain
- Q：為什麼 rotation 不存成 `int rotateCount` 就好、何必還有 `Rotate::CW_0/90/180/270` enum？  
  A：兩個目的不同：`rotateCount` 是視覺動畫累加器（單調 float→angle）；`location.rotate` 是邏輯狀態（0–3）給 `rotatedShape` 用，會 mod 4。兩個都需要、不能合併。
- Q：fmod 那版到底哪裡錯？  
  A：理論上對。實務上 `current` 接近邊界（例如 359.5）時，`target - current + 540` mod 360 後可能很接近 540 邊界，浮點誤差讓結果有時是 ~-179 有時 ~+181，diff 方向不穩。單調累加完全繞過這個。
- Q：rotateCount 無限大不會壞嗎？  
  A：float32 在數千度內仍精準，每秒按一次 R 連按一年才會出問題。不擔心。

### Score Connection
- 圖形介面：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)） — robust 實作

---

### Feature
旋轉/移動 pivot cell 選擇演算法

### What & Key Concepts
- 每個 Part 載入時 `computeCenterCell()`：
  1. 算所有占據格的形心 `(avgR, avgC)`
  2. 在占據格中挑 squared distance 最小的當 pivot
- 例子：
  - P0 L-shape `[[1,1],[1,0]]` → 形心 `(0.33, 0.33)` → 最近占據格 `(0, 0)`
  - P1 1×3 `[[1,1,1]]` → 形心 `(0, 1)` → 最近占據格 `(0, 1)`（中間那格）
- `rotatedCenterCell(out)` 用同一條旋轉公式 case 0~3 把 (centerCellRow, centerCellCol) 套到旋轉後的 bbox 位置。

### Why This Design
- 為什麼不用 bbox 中心：bbox 中心可能落在空格上（L-shape 的 bbox 中心是空的 (0.5, 0.5)）→ 違反 rubric「中心不為空」。
- 為什麼形心 + 最近占據格：保證落在占據格上（直接從占據集挑），且離形心最近、最像「中心」。
- 為什麼 squared distance 而非 abs：避免 sqrt（沒必要）、且偏好直線最近，符合視覺直覺。

### What I Should Be Able To Explain
- Q：怎麼保證 pivot 不會落在空格？  
  A：候選集只取占據格 `if (shape[r][c])`，自動排除空格。
- Q：tie 怎麼辦（兩個占據格距離一樣）？  
  A：top-to-bottom、left-to-right 第一個遇到的勝出（迴圈順序）。確定性。
- Q：旋轉時 pivot 怎麼跟著轉？  
  A：`rotatedCenterCell` 用和 `rotatedShape` 同一條 case 公式把原 pivot 位置變換到旋轉後 bbox 內的位置。然後 Renderer 用這個算螢幕 pivot。
- Q：對 L-shape 來說 pivot 在角上感覺怪？  
  A：是。但形心 = (0.33, 0.33) 確實最近的占據格就是角；要選別的會偏離形心，更不對。Trade-off。

### Score Connection
- 進階：旋轉/移動中心不為空 1%（[scoring.md:61](scoring.md#L61)）

---

### Feature
`DrawRectanglePro` 任意 pivot 旋轉的座標數學

### What & Key Concepts
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

### Why This Design
- 為什麼不畫到 RenderTexture 再 DrawTexturePro：DrawTexturePro 也支援旋轉，但要先 baking 整個 part 進 texture、複雜度高、size 有上限。每 cell 直接 DrawRectanglePro 簡單且 GPU 一樣處理。
- 為什麼 pivot 不用 bbox 中心：要視覺上「繞 pivot cell 轉」而不是「繞 bbox 中心轉」。bbox 中心對 L-shape 來說會繞空格中心，視覺奇怪。

### What I Should Be Able To Explain
- Q：DrawRectanglePro 的 `origin` 是什麼？  
  A：rect 左上角到 pivot 點的位移向量（un-rotated 坐標）。`(rec.w/2, rec.h/2)` = 繞中心轉；`(0, 0)` = 繞左上角轉。
- Q：怎麼讓多個 cell 一起繞共同 pivot 轉？  
  A：所有 cell 共用 `rec.x/y` = pivot 螢幕位置，每 cell 的 origin 是「該 cell 左上角到 pivot 的 vector」。raylib 自動把 cell 繞 pivot 旋轉。
- Q：rotation 是度還是弧度？  
  A：度。raylib 一律用度，內部 cos/sin 轉。
- Q：origin 的座標是「旋轉前」還是「旋轉後」？  
  A：旋轉前（un-rotated）。raylib 拿到後先算 rotated 4 角再 DrawTriangle。

### Score Connection
- 圖形介面：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)） — 數學機制
- 進階：旋轉中心不為空 1%（[scoring.md:61](scoring.md#L61)） — 視覺實作支援
