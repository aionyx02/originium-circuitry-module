# LOG.md

這份文件用來記錄本專案的開發過程與 AI 協作過程。

用途：
- 追蹤目前做了什麼
- 記錄 AI 協助了哪些部分
- 記錄使用者在 prompt 或討論過程中的判斷、要求與修正
- 記錄驗證方式、測試結果與後續風險
- 先把開發過程中的重要資訊都放進來，後續可以再統整成報告內容
- 作為口頭報告「如何用 AI 實作程式」的素材

---

# 索引（最新在上）

> **新增 entry 時：(1) 把實際 entry append 到本檔最下方（時間正序），(2) 在這個索引「最上面」加一行。**  
> 標注格式：`日期` · `Phase` · `Type` · 一句話描述。連結若無法直接點開，用 Cmd+F 搜日期或標題。

- **2026-05-22** — [文件 rename 與 reference 統一](#2026-05-22--文件-rename-與-reference-統一small) · `Phase 0` · `small` · Log/LEARNING_NOTES/mynote → LOG/learning-notes/my-note + 7 檔 reference 同步
- **2026-05-22** — [文件結構與工作流程整理](#2026-05-22--文件結構與工作流程整理) · `Phase 0` · `Discussion + Refactor` · CLAUDE / STATUS / LOG / learning-notes / plan 重整、Phase 0/1 entry 遷移、路徑大清理
- **2026-05-22** — [Phase 1 完成（MVP 單色關卡可玩）](#2026-05-22--phase-1-完成mvp-單色關卡可玩) · `Phase 1` · `Implementation` · 預期 +20%（基本 5% + 流程 15%），待手動互動驗證
- **2026-05-22** — [Phase 0 完成（基礎設施）](#2026-05-22--phase-0-完成基礎設施) · `Phase 0` · `Implementation` · CMake + raylib + Hello-world 跑通
- **2026-05-22** — [技術棧決策（C++17 / raylib 5.5 / CMake / MinGW static）](#2026-05-22--技術棧決策c17--raylib-55--cmake--mingw-static) · `Phase 0` · `Discussion` · 拍板技術棧、淘汰 Python/Web UI、POC 證明 raylib 視覺潛力（JSON 後續推翻）
- **2026-05-22** — [設計與 Claude Code 協作的工作流程](#2026-05-22--設計與-claude-code-協作的工作流程) · — · `Discussion` · 文件結構與協作流程奠基

---

# 寫法說明

## 大事 vs 小事

**大事**（用下方完整 10 欄 template）：
- Phase milestone（一個 Phase 開始或結束）
- 新功能 / 新模組
- 架構決策、技術選型、決策被推翻
- 較大重構

**小事**（一行 one-liner 即可，不用填 template）：
- Bug fix
- UI 常數微調（顏色、間距、字體大小）
- 補 helper function
- 小改錯字、改 doc 文字

小事格式：

```
## YYYY-MM-DD — 一句話描述（small）
Phase: (if have)  
Commit: (if have)
```

只要可以讓未來自己（或夥伴）一眼看出做了什麼即可。

---

## Entry Template

## YYYY-MM-DD — 摘要

Type: Discussion / Implementation / Debug / Refactor  
Phase:  (if have)
Feature:  (if have)
Commit:  (if have)

### Context

### AI Contribution

### Human Decision / Review

### Details

### Verification / Tests Performed

### Result

### Risks / Follow-ups

### Other Notes



## 各 section 建議寫法

### Context
寫這次紀錄的背景：  
為什麼要做這件事？這件事在專案中處於哪個階段？跟配分或後續實作有什麼關係？
### AI Contribution
寫 AI 協助了什麼。  
例如：整理需求、分析規則、提出架構、比較方案、產生程式碼、修改 bug、建議測試方式等。
### Human Decision / Review
寫使用者在 prompt 或過程中有討論到、決定、修正、要求的內容。
可以包含：
- 使用者一開始怎麼描述需求
- 使用者要求 AI 採用或避免哪些做法
- 使用者選擇了哪個方案
- 使用者修正了 AI 的方向
- 使用者提出的限制條件
- 使用者如何 review AI 的結果
- 使用者確認或否定了哪些設計
### Details
寫更完整的過程紀錄。  
可以把重要資訊先放進來，不一定要一次整理得很漂亮，之後報告前再統整。
### Verification / Tests Performed
寫驗證或測試方式。  
如果還沒寫 code，可以寫「尚未進行程式測試」，並說明是用需求比對、配分比對或人工 review 來確認。
### Result
寫這次工作的結果。  
例如完成了某份文件、確定了某個技術選型、實作了某個功能、修好了某個 bug。
### Risks / Follow-ups
寫還沒解決的風險、後續要補的事情、可能需要再確認的地方。
### Other Notes
寫額外備註。  
例如：這段未來可以放進報告、這個決策可能會被老師問、這裡需要自己再理解一次。


--- 

# Log:


## 2026-05-22 — 設計與 Claude Code 協作的工作流程
Type: Discussion  
### Context
本專案口頭報告需要說明如何用 AI 實作程式，因此在正式實作前，需要先設計一套可控、可追蹤、可解釋的 AI 協作流程。
### AI Contribution
AI 協助設計與 Claude Code 合作的方式，包含：
- Claude Code 作為主力工程師
- 使用者作為 project owner、reviewer 與 final decision maker
- 每個功能先由 AI 讀檔與規劃，再進行實作
- 實作後整理 changed files、測試方式、風險與配分對應
- 每次重要變更後更新文件紀錄
AI 也協助設計專案文件：
- `CLAUDE.md`
- `STATUS.md`
- `DEV_GUIDE.md`
- `LOG.md`
- `learning-notes.md`
### Human Decision / Review
使用者在討論過程中決定：
- Claude Code 的角色是主力工程師
- 使用者負責審查、測試、理解與最後決策
- 專案以配分優先，但仍保持合理架構
- 使用 C++ / raylib 作為技術棧
- 其他 library 可以由 AI 提議，但不能自行加入或安裝
- `CLAUDE.md` 不要寫太長，只放長期規則
- `SCORING_CHECKLIST.md` 與 `PROJECT_STATUS.md` 可以合併成 `STATUS.md`
- `AI_USAGE_LOG.md` 改名為 `LOG.md`
- `LOG.md` 主要用中文記錄
- `Documentation` 和 `Planning` 都歸入 `Discussion`
- `learning-notes.md` 用來讓 AI 在實作後補充教學，說明它做了什麼、為什麼這樣做
### Details
建立的協作流程如下：
1. AI 讀取相關檔案。
2. AI 提出小範圍 implementation plan。
3. 若變更較大，先等使用者確認。
4. AI 實作功能。
5. 使用者檢查 diff 並測試。
6. AI 更新 `STATUS.md`、`LOG.md`、`learning-notes.md`。
7. 每次記錄 AI 做了什麼、使用者怎麼檢查、結果與後續風險。
### Verification / Tests Performed
尚未進行程式測試。
這一步主要驗證這套流程是否能支援：
- 跨 session 接續任務
- 記錄 AI 使用方式
- 追蹤配分進度
- 幫助使用者理解程式
- 準備口頭報告
### Result
完成初步 AI 協作流程設計。  
後續實作將依照這套流程進行。
### Risks / Follow-ups
- 文件太多時可能需要維護成本。
- `STATUS.md` 和 `DEV_GUIDE.md` 必須保持更新，否則 AI 下次可能讀到過時資訊。
- 需要避免 AI 一次修改太多功能，導致難以 review。
### Other Notes
這段是報告中很重要的素材，可以用來說明：  
本專案不是直接讓 AI 生成完整程式，而是先設計一套人類主導、AI 協助的開發流程。


## 2026-05-22 — 技術棧決策（C++17 / raylib 5.5 / CMake / MinGW static）
Type: Discussion  
Phase: 0  
Feature: 技術棧、第三方套件、Windows 打包策略

### Context
在開始任何實作前，先把技術棧定下來。受兩個剛性限制驅動：

1. **核心邏輯必須用 C++**（老師規定）
2. **demo 在教室乾淨的 Windows 電腦執行** — 不能假設裝了任何 runtime（Python / Node / VC++ Redistributable / 瀏覽器以外的東西）

加上 [scoring.md](scoring.md) 兩條重點：
- 第 25 行：「讀入檔案的功能必須能在程式執行時輸入檔案路徑」— 違反就讀檔分全失
- 圖形介面 15%：旋轉動畫 5% + 零件跟游標連續移動 5% 是大頭

### AI Contribution
1. **方案探索與比較**：列舉並評估 5 個 C++ GUI 框架（raylib / SFML / SDL2 / Qt / Dear ImGui）以及 4 個 Web UI 路線（WASM / 本機 server / WebView2 / Electron），提出比較表與淘汰理由。
2. **POC 實作**：寫了 [demo/main.cpp](../demo/main.cpp)（約 280 行）證明 raylib 能達到的視覺水準。
3. **整理為決策文件**（即原 LOGa-trival.md）：紀錄淘汰理由、選擇理由、第三方套件清單、已知風險與後續決策待辦。

### Human Decision / Review
**使用者主導 / 拍板的判斷**（從外部限制或 review 過程可追到）：

- **淘汰 Python + Pygame**：使用者根據老師「核心邏輯必須用 C++」的規定拍板。即使 pybind11 能滿足，仍因「乾淨 Windows 不能假設有 Python runtime」+「跨語言邊界增加 demo 風險」雙重否決。
- **淘汰整套 Web UI 路線**（WASM / 本機 server / WebView2 / Electron）：因「讀檔剛性要求」（純前端只能用 `<input type=file>`，沒有「輸入路徑」這個概念，可能不算數）與 demo 環境不可控（瀏覽器版本 / 防火牆 / WebView2 runtime 任一出包就掛）。
- **選 raylib over SFML / SDL2 / Qt / ImGui**：使用者接受 AI 提出的 5 點理由（單一 .exe 打包、API 量小語意清楚、內建音效、MIT 授權、GUI 動畫對應 raylib 強項）。
- **MinGW-w64 + static linking** 作為 Windows 打包方案：使用者拍板。
- **關卡檔格式選 JSON + nlohmann/json**：使用者當時拍板。**此決定後續被推翻** — 詳見下方 Risks / Follow-ups。

### Details
**最終技術棧定案**：
- **核心邏輯**：C++17
- **GUI / 動畫 / 音效**：raylib 5.5
- **建置系統**：CMake
- **Windows 打包**：MinGW-w64 靜態連結（`-static -static-libgcc -static-libstdc++`），產出單一 `.exe` + `assets/` 資料夾
- **關卡檔格式**：~~JSON（nlohmann/json）~~ → **後續推翻為純文字（教師格式）**

**C++ GUI 框架比較**：

| 框架 | 動畫易寫度 | 單 .exe 打包 | 學習曲線 | 教室電腦相容性 |
|---|---|---|---|---|
| **raylib** ✅ | 易（`DrawTextureEx` 帶角度一行解決） | 靜態連結容易 | 平緩 | 最高 |
| SFML | 易（`sf::Sprite`） | 要帶 4–5 個 DLL | 中等 | 中（需 MSVC runtime） |
| SDL2 | 中（要自己畫旋轉） | 要帶 DLL | 較低階 | 中 |
| Qt | 要學 QPropertyAnimation | 靜態連結很難（授權 + 體積） | 陡 | 低 |
| Dear ImGui | 不擅長遊戲動畫 | 易 | 中 | 高 |

**為什麼選 raylib**：
1. **單一 `.exe` 打包**：用 MinGW-w64 + `-static` 把所有相依靜態連結，乾淨 Windows 直接跑
2. **API 量小、語意清楚**：`DrawTextureEx(tex, pos, angle, scale, tint)` 一行畫旋轉零件，配合 `Lerp` 做動畫超直觀
3. **內建音效**：`LoadSound` / `PlaySound`，不必再接 OpenAL
4. **MIT 授權**：無 SFML 那種附 LICENSE 的尷尬
5. **GUI 動畫 10% 的兩項剛好對到 raylib 強項**

**Web UI 方案比較（已淘汰）**：

| 架構 | C++ 怎麼接 | 主要問題 |
|---|---|---|
| A. WASM（Emscripten） | Emscripten | 通常需要本機 HTTP server |
| B. C++ 本機 server + 瀏覽器 | cpp-httplib / Crow | Windows 防火牆會跳警告，demo 場面尷尬 |
| C. C++ + WebView2 | Win32 + WebView2 API | 需要 WebView2 Runtime（不保證有） |
| D. Electron + N-API | C++ addon | 80–150 MB 包，部署笨重 |

**為什麼不走 Web UI**（除了上表的個別問題之外的通用問題）：
- **讀檔剛性要求**：純前端 WASM 只能用 `<input type=file>`，沒有「輸入路徑」這個概念，可能不算數
- **跨語言邊界**：每個 web 方案都要對付 C++ ↔ JS 兩套 build，學生專案投資報酬率低
- **demo 環境不可控**：教室電腦的瀏覽器版本、防火牆、WebView2 runtime 任一條出包就掛
- Web UI 唯一明顯贏的點是「CSS 排版方便」，但這對遊戲類專案影響有限 — 遊戲畫面主要是 canvas/sprites，HTML 元素優勢用不上

**第三方相依套件**：

| 用途 | 套件 | 為什麼 |
|---|---|---|
| GUI / 動畫 / 音效 | raylib 5.5 | 見上 |
| ~~JSON 解析~~ | ~~nlohmann/json~~ | **推翻**：教師格式是純文字，引入 JSON 等於放棄助教測資分 |
| 建置 | CMake | 跨平台、IDE 都支援 |
| Windows 編譯器 | MinGW-w64（或 MSVC `/MT`） | 靜態連結最容易 |

**不會用**：第三方 GUI widget 函式庫（raylib 內建 raygui 已夠用）、ECS 框架（過度設計）、跨平台音訊函式庫（raylib 已含）

**POC（[demo/main.cpp](../demo/main.cpp)）已實作的視覺效果**：
- 零件跟游標連續移動（`Lerp` factor 0.40）— 對應 scoring 5%
- 旋轉動畫（`Lerp` 角度逼近 targetAngle）— 對應 scoring 5%
- Hover lift（滑鼠靠近時零件浮起、放大、影子加大）— 進階質感
- 拖曳目標格高亮（盤面格子變色）— 對應進階 1%
- 圓角格子 + 漸層背景 + 上方高光帶 + 暗邊框 — 對應圖形介面 2.5%
- 列/欄需求數字 — 對應進階 1%
- MSAA 4x + HighDPI — 解決字邊鋸齒和高解析度螢幕

POC 編譯結果：在 Apple Silicon Mac 上 `clang++ -std=c++17 -O2` 編譯，產出 36 KB arm64 執行檔，零警告。

### Verification / Tests Performed
- POC 編譯與執行成功（macOS clang++、36 KB 執行檔、零警告）。
- 視覺效果（旋轉動畫、跟游標連續移動、hover lift、漸層背景等）以目視確認運作。
- **尚未驗證**：Windows VM 靜態連結最終產出在乾淨環境執行（留給 Phase 6）。

### Result
- 技術棧拍板：C++17 + raylib 5.5 + CMake + MinGW-w64 static。
- POC 證明 raylib 能滿足 GUI 15% 的兩個大項（旋轉動畫 5% + 跟游標連續移動 5%）。
- 後續 Phase 0 / 1 直接以此技術棧為前提，未再回頭比較其他框架。

### Risks / Follow-ups
**已處理**：
- ~~`CMakeLists.txt` 把 raylib 串起來~~：Phase 0 完成（用 `find_package` + `FetchContent` 雙保險）
- ~~`core/part.h` 與 `core/board.h` 公開介面設計~~：Phase 1 完成
- ~~[CLAUDE.md](../CLAUDE.md) 的 Architecture 段從 Python 模組改 C++~~：在「文件結構整理」session 一併處理

**已推翻**：
- **關卡檔格式 JSON 決定**：discovered during plan.md 寫作時 — JSON 會讓我們讀不到助教 demo 測資（純文字格式），損失 2% + 2%。已改為純文字格式（教師格式）；不再引入 nlohmann/json。推翻紀錄見 [plan.md](plan.md) §0 與 CLAUDE.md Hard Constraint #2。

**待處理**：
- Windows VM 上靜態連結產出於乾淨環境執行驗證（Phase 6）
- 中文檔名讀檔在 Windows 上是否需要 wide-char API + `std::filesystem::path` 處理（Phase 6）
- Solver 雙色測資爆炸：C++ 演算法配 30 秒 `std::chrono` timeout（Phase 4 處理）
- 高 DPI UI 變小：`SetConfigFlags(FLAG_WINDOW_HIGHDPI)`（demo 已加，正式專案 Phase 0 沿用）
- AI 使用紀錄忘記寫（口頭報告 30%）：每完成一個功能立刻補 [LOG.md](LOG.md)（已在 CLAUDE.md Hard Constraint #5 落實）

### Other Notes
**口頭報告素材**：

- **「為什麼不用 Web UI」是會被問的問題**：答有四點 — 讀檔剛性要求、跨語言邊界成本、demo 環境不可控、Electron 體積。Web UI 唯一明顯贏的點是 CSS 排版方便，但遊戲畫面是 canvas / sprites，HTML 用不上。
- **「為什麼 raylib 不是 SFML」**：核心差異是「單一 .exe 打包」 — SFML 要帶 4–5 個 DLL，乾淨 Windows demo 場面尷尬。raylib 用 MinGW + `-static` 一條命令搞定。
- **「JSON 被推翻」是個誠實面對錯誤的好案例**：早期決定當時看起來合理（nlohmann/json 是 header-only、好用），但忽略了「助教 demo 當天會丟測資 → 測資是純文字格式」這個高權重事實。後來在 plan.md 寫作時 catch 到並推翻。可以講「決策不是一錘定音，會根據新發現的事實調整」。
- **POC 先做的好處**：[demo/main.cpp](../demo/main.cpp) 證明 raylib 視覺水準後，後面 Phase 2 的視覺打磨工作有明確的目標可以複製。

**本 entry 的歷史來源**：從原 LOGa-trival.md（曾名為 /LOG.md）整理而來。原檔已於 2026-05-22 刪除；內容完整保留在本 entry。


## 2026-05-22 — Phase 0 完成（基礎設施）
Type: Implementation  
Phase: 0  
Feature: CMake + raylib + Hello-world

### Context
Phase 0 目標：把建置環境打通，確保 cmake / raylib / 目錄結構就緒，下一步能直接進 Phase 1 寫遊戲邏輯。對應 [plan.md](plan.md) §2 Phase 0。

### AI Contribution
1. 檢視 [plan.md](plan.md)、[Framework/](../Framework/)、LOGa-trival.md，確認限制與既有 skeleton。
2. 產出 [CMakeLists.txt](../CMakeLists.txt)：
   - `find_package(raylib 5.5 QUIET)` 先用系統裝好的，失敗才退回 `FetchContent` 抓 raylib 5.5 source。
   - `MSVC` 加 `/utf-8`，`MINGW` 加 `-static -static-libgcc -static-libstdc++` 為了未來 Windows 單檔打包。
   - C++17，禁用 GNU extensions。
3. 產出 [src/main.cpp](../src/main.cpp)：最小 raylib window，960×720、MSAA 4x、HighDPI、印 "Phase 0 OK"。
4. 補 [.gitignore](../.gitignore)：忽略 `build/`、`.DS_Store`、IDE 設定、`*.exe/o/obj`。
5. 建 `src/core/`、`src/ui/` 空目錄，等 Phase 1 填內容。
6. 驗證流程：`cmake -S . -B build` → `cmake --build build` → 跑 `./build/game`，raylib log 顯示 OpenGL 4.1 Metal 跑起來，視窗 2 秒測試後手動 kill。

### Human Decision / Review
- **使用者拒絕了 AI 一開始建議的 macOS 框架手動 link 區塊**（`-framework Cocoa` 等）。修正理由：raylib 的 CMake target 已透過 `INTERFACE_LINK_LIBRARIES` 帶進來，手動加會重複。

### Details
工具：Claude Code（Opus 4.7）；Homebrew 安裝 cmake 4.3.2；系統先前已裝 raylib 5.5。

**設計選擇**（AI 提出、使用者未要求修正）：
- `find_package(raylib 5.5 QUIET)` 優先、`FetchContent` 後備：開發機（mac）跳過下載省時間，乾淨 Windows demo 機沒裝 raylib 也能自動拉 source 編。
- Phase 0 不動 `Framework/` 裡的檔：[plan.md](plan.md) §3 明定「保留教師 skeleton 給老師看」，等 Phase 1 才把 `Part.h`/`Board.h` 演進到 `src/core/`。

### Verification / Tests Performed
- `cmake --build build`：0 warning 0 error。
- `./build/game`：開啟視窗、raylib 5.5 初始化成功（log 確認 Apple M3 + MSAA x4 + HighDPI）。

### Result
Phase 0 完成。下一步 Phase 1。

### Risks / Follow-ups
- Windows 中文檔名讀檔行為未驗證，留給 Phase 6。
- 教室電腦的靜態連結最終驗證留給 Phase 6。

### Other Notes
此 entry 可作報告素材：cross-platform build 用 `find_package` + `FetchContent` 雙保險的設計。


## 2026-05-22 — Phase 1 完成（MVP 單色關卡可玩）
Type: Implementation  
Phase: 1  
Feature: Parser + Part/Board/WinChecker + Game + Input + Renderer + main 互動式路徑
Commit: 

### Context
Phase 1 目標：MVP 可跑單色關卡。對應 [plan.md](plan.md) §5 詳細計劃，預期銀行存款 +20%（基本 5% + 流程 15%）。

### AI Contribution
1. **讀規格**：把 [docs/io/input-file-format.md](io/input-file-format.md)、[docs/io/sample-input-output.md](io/sample-input-output.md)、[docs/io/Example1.txt](io/Example1.txt) 全讀過，確認：
   - cell 編碼：`EMPTY=0`、`CANNOT_PLACE=-1`、`CANNOT_MOVE=-2-color`、`OCCUPIED=1+partIdx`
   - constraints 結構：`_constraints[color][0..M-1]` = 列需求、`[M..M+N-1]` = 欄需求
   - parser 結構：`C M N` → 每色 (M 列需求, N 欄需求, X1 固定零件) × C → X2 不可放格子 → 每個零件 `colorIdx M2 N2` + 0/1 矩陣 直到 EOF
2. **保留教師 skeleton 命名**：`Part` / `Board` 沿用 [Framework/](../Framework/) 裡欄位名（Hard Constraint #4），只給 enum 補上 `: int` 讓存 partIndex+1 / -2-color 等任意 int 值合法。
3. **產出 9 個檔**：
   - [src/core/Part.h](../src/core/Part.h) / [Part.cpp](../src/core/Part.cpp) — `rotatedShape()` 與 `rotatedCells()`，旋轉公式 case 0~3 用 (M,N) ↔ (N,M) 軸交換
   - [src/core/Board.h](../src/core/Board.h) / [Board.cpp](../src/core/Board.cpp) — `canPlace` / `place` / `remove` + 靜態 `isOccupied`/`isCannotMove` 解碼器
   - [src/core/Parser.h](../src/core/Parser.h) / [Parser.cpp](../src/core/Parser.cpp) — `Parser::Error` 例外、用 `>>` 跳空白、邊界與顏色索引檢查
   - [src/core/WinChecker.h](../src/core/WinChecker.h) / [WinChecker.cpp](../src/core/WinChecker.cpp) — 純函數，要求 (a) 所有零件 placed (b) 每色每列每欄計數等於 constraint
   - [src/core/Game.h](../src/core/Game.h) / [Game.cpp](../src/core/Game.cpp) — cursor (row, col with col=-1 為 tray)、heldPartIdx、`update(Action)`、Esc/Enter 雙重語意、放置成功自動 select 下一個未放
   - [src/ui/Input.h](../src/ui/Input.h) / [Input.cpp](../src/ui/Input.cpp) — `IsKeyPressed` 一次性觸發，WASD/方向鍵/R/Enter/Space/Esc/Backspace
   - [src/ui/Renderer.h](../src/ui/Renderer.h) / [Renderer.cpp](../src/ui/Renderer.cpp) — 自適應 cell size、constraints 標籤、半透明預覽（canPlace 失敗變紅）、Tray 縮圖、勝利 banner
   - [src/main.cpp](../src/main.cpp) — `argv[1]` 優先，空就 `std::cin` 問；`SetExitKey(KEY_NULL)` 讓 Esc 拿來「拔零件」而不關視窗
4. **更新 [CMakeLists.txt](../CMakeLists.txt)** 加上 7 個新 cpp。

### Human Decision / Review
使用者未在 Phase 1 實作過程中介入；所有決策由 AI 自行做出，詳見 Details 設計選擇段。

**Process gap（需改善）**：後續 Phase 應在 (1) plan 提案、(2) 實作中、(3) 實作後 各 review 一次，才能讓本欄有實質內容。Phase 2 開始實施。

### Details
工具：Claude Code（Opus 4.7）、raylib 5.5（系統 brew 已裝）、CMake 4.3.2。

**設計選擇**（AI 自行決定，使用者未參與）：
- **enum BoardInfo 補 `: int`**：skeleton 沒寫 underlying type，但註解明說 `OCCUPIED + partIndex`，加 `: int` 讓 cast 合法。一字元變更，不會影響老師對 skeleton 的辨識。
- **cursor 採「同一個 cursor 跨 tray + board」模型**（不分兩個 cursor）：`cursorCol=-1` = tray、`>=0` = board。`clampCursor` 在 col=-1 時把 row 上限放寬到 `max(parts.size(), board.rows)-1`，離開 tray 時再縮回 `board.rows-1`。對應 plan「wasd 能在盤面與 tray 之間移動 selection」。
- **`Esc` 雙重語意**：握著零件按 Esc = 放回 tray；沒握零件且 cursor 在已放置零件上 = 把它拔回 tray。對使用者來說都是「取消當前動作」的單一語意。
- **放置成功後 auto-select 下一個未放**：plan 沒寫但 UX 上幾乎必要，否則玩家每次都要手動回 tray。**超出 plan 的設計擴充。**
- **canPlace 預覽用紅框**：plan 寫「放到不能放的格子會印錯誤訊息（不 crash）」— AI 多做視覺回饋（預覽紅色 = 立即知道不行）。**超出 plan 的設計擴充。**
- **不寫單元測試**：plan 沒列，且 raylib 視窗目視即可驗 8 個 checklist 項目；正式 test 留到 Phase 4 solver 才做。
- **延後處理 Windows 中文檔名**：按 [plan.md](plan.md) §2 留到 Phase 6 一併處理；Phase 1 用 ASCII 路徑（Example1.txt）。

### Verification / Tests Performed
- Build：0 warning 0 error，8 個 .cpp 全部 compile。
- `./build/game docs/io/Example1.txt`：raylib 視窗開，3 秒目視 OK 後手動 kill。
- `./build/game /tmp/nonexistent.txt`：印 "Cannot open file:..."，exit code 1，沒 crash。
- `echo docs/io/Example1.txt | ./build/game`：stdin 模式，prompt "Enter level file path: " 出現後讀檔成功。
- **未驗證（需手動互動）**：wasd 移動、r 旋轉循環、Enter 放置、Esc 拔掉、重疊錯誤訊息、全擺對的 You Win banner。Phase 1 checklist 第 4~8 項要使用者親手測。

### Result
Phase 1 程式碼完成。等使用者手動互動測試後可勾掉 [STATUS.md](STATUS.md) 的「遊戲流程」項目（共 15%）。

### Risks / Follow-ups
- 使用者親手跑一輪 Example1.txt 驗 checklist 4~8 項。
- 若 OK，commit Phase 1 milestone，進 Phase 2 視覺打磨。

### Other Notes
- 「Esc 雙重語意」是設計點，老師可能會問為何同一個 key 兩種行為 — 答：是 cursor 上下文驅動的（hold 或 free），對使用者來說是「取消當前動作」的單一語意。
- 「cursor 跨 tray + board 模型」也是會被問的設計點，把 tray 視為盤面外的虛擬欄（col=-1），避免維護兩套 cursor 狀態。


## 2026-05-22 — 文件結構與工作流程整理
Type: Discussion + Refactor  
Phase: 0  
Feature: 文件結構、CLAUDE.md、STATUS.md、learning-notes.md、LOG.md、plan.md

### Context
這次 session 是進 Phase 0/1 程式實作之外，**獨立的一段「文件規劃 + 結構重整」對話**。動機：原本散落的文件互相矛盾（LOGa-trival.md 寫 JSON 但教師格式是純文字、CLAUDE.md 引用不存在的 STATUS.md / DEV_GUIDE.md / learning-notes.md、AI_usage_note.md 與 LOG.md 用途重疊），需要把 Phase 規劃、評分 checklist、AI 紀錄方式、文件分工拍板，才能讓後續 Phase 有穩定地基。

對應 [scoring.md](scoring.md)：口頭報告 30%（AI 使用紀錄）+ 15%（程式熟悉度）兩個大頭的素材，都來自這次整理出來的 doc 結構。

### AI Contribution
1. **Phase 規劃**：讀完 [scoring.md](scoring.md)、[structure-design.md](structure-design.md)、[game-rule.md](game-rule.md)、[io/](io/) 範例、[Framework/](../Framework/) skeleton、[demo/main.cpp](../demo/main.cpp)、舊 LOG.md，提出 Phase 0–7 拿分策略（MVP-first → 銀行存款分數 → 美化），寫進 [plan.md](plan.md)。
2. **發現重大矛盾並 flag**：舊 LOG.md 寫「關卡檔用 JSON」但教師格式是純文字 → 用 JSON = 失去助教 demo 測資 4%。標為推翻決策。
3. **CLAUDE.md 升級**（從舊版重寫成新版 [x.md](../x.md)）：
   - 加 5 條 **Hard Constraints**（runtime path / plain text / clean Windows / Part/Board 命名 / AI 紀錄 30%）
   - **Large vs Small workflow** 分流（7 個 large trigger）
   - **Source of Truth** 指向 plan.md
   - Tech Stack 寫到版本（C++17 / raylib 5.5 / MinGW static）
4. **CLAUDE.md 瘦身**：~150 行 → ~115 行，砍 Project Goal 重複段、Coding Rules generic baseline、Refactoring Rule 跟 large trigger 重疊段。
5. **STATUS.md 重寫**：英文 → 中文、每項加配分、修正分數誤導（Load+Display 是同一個 2%、編輯器 6 子項合計 5%）、加 `Last updated` 戳記、加銀行存款累積區、刪掉重複 plan.md 的 Decisions / Next Priorities 段。
6. **learning-notes.md 重寫**：合併冗餘欄位、刪首篇 AI-meta entry、加「寫/不寫」規則（後續修正為「以非平凡概念為判準，跟事件大小無關」）、語言彈性（中英文皆可）。
7. **LOG.md 加大事/小事分流**：完整 10 欄 template 給大事、one-liner 給小事。
8. **LOG.md 加完整索引**：最新在上、annotated（`Phase` · `Type` · 一句話）、附維護規則（append 在最下、索引在最上加一行）。
9. **Phase 0/1 entry 遷移**：把使用者另一個 session 寫到 [AI_usage_note.md](AI_usage_note.md) 的 Phase 0/1 紀錄，用 10 欄 template 重新整成兩個 entry 放進 [LOG.md](LOG.md)，再刪掉 AI_usage_note.md。
10. **路徑大清理**：用 grep 找出所有 stale ref，逐一修正：
    - 所有 `AI_usage_note.md` → `LOG.md`（plan.md 8 處、x.md 多處）
    - `../plan.md` → `plan.md`（STATUS.md / learning-notes.md，因為都在 docs/）
    - `[plan.md](plan.md)` → `[plan.md](docs/plan.md)`（x.md 5 處，因為它在根目錄）
    - `[my-note.md](my-note.md)` → `[my-note.md](docs/my-note.md)`（x.md）
    - bare `LOG.md` → `LOGa-trival.md`（plan.md / x.md）
    - 標 plan.md 內的 done TODOs、更新 ASCII tree 反映 docs/ 集中結構

### Human Decision / Review
**使用者真正介入、修正 AI 的方向，或拍板選項的地方**：

- **檔案格式**：使用者直接拍板「純文字」，推翻舊 LOG.md 的 JSON 決議（AI 給了三個選項，使用者選最保守的）。
- **Phase 1 UI 策略**：使用者選「raylib 一路到底」，否決 AI 建議的 terminal-first（風險較低的選項）。
- **人力配置**：使用者修正 AI 的「solo」假設 → 告知「前期一人，中後期有夥伴」 → AI 據此重排 Phase 順序（讓 Solver / Editor 為夥伴可接段）。
- **CLAUDE.md 改寫範圍**：使用者拍板「先改高 + 中優先項目，砍真的可砍的」，並批准 AI 的 trim 提案後執行。
- **AI 紀錄 canonical 檔反覆 + 最終裁定**：第一輪使用者選 docs/LOG.md 取代 AI_usage_note.md → 中間使用者在另一個 session 把 Phase 0/1 寫到 AI_usage_note.md → 第二輪使用者再次拍板「**仍以 docs/LOG.md 為 canonical，把 AI_usage_note.md 內容搬進來**」。最後一次的修正特別重要 — 避免文件分裂。
- **檔案清單**：使用者拍板「CLAUDE / STATUS / DEV_GUIDE / LOG / learning-notes / plan / my-note 共 7 個」，比 AI 一開始建議的 5 個多出 DEV_GUIDE 與 learning-notes。
- **learning-notes 寫/不寫規則**：AI 原本建議「對應 LOG.md 大事/小事分流」 → 使用者修正為「**判準是有沒有非平凡概念可教，跟事件大小無關**」。這個修正比 AI 的版本更精確。
- **LOG.md 排序**：AI 推薦「保持時間正序」、可選加 Latest 索引 → 使用者選「**完整索引 + 最新在上 + annotated**」 → AI 實作該結構。
- **語言選擇**：AI 建議「learning-notes 也改中文」 → 使用者修正為「中英文皆可、混用 OK」（技術名詞用英文較自然）。
- **HD/R 段內容判定**：使用者讀 AI 整理出來的 Phase 0/1 entry 後，**指出 HD/R 段塞了 design rationale 不是真正的使用者介入** → AI 重新分類，把設計選擇搬到 Details，HD/R 只留真正的使用者修正。
- **Phase 1 誠實性**：使用者進一步揭露「Phase 1 其實是 AI 自己決定的、使用者沒參與」 → AI 改寫 Phase 1 HD/R 為「使用者未介入，process gap 需改善」並承諾 Phase 2 開始實施 (1) plan 提案 (2) 實作中 (3) 實作後 三點 review。

### Details
**整體流程**：使用者並行跑兩個 Claude session。在我這邊做 doc 規劃時，另一邊跑 Phase 0/1 程式實作。Mid-session 才發現對方做了大量重組（檔案搬進 docs/、寫 CMakeLists.txt + src/、寫 DEV_GUIDE.md），AI 在這邊中斷 doc cleanup、重讀新狀態（git status + 各檔內容）、再繼續對齊路徑。

**修動 / 新增的檔（總清單）**：
- **新增**：[docs/STATUS.md](STATUS.md)（重寫）、[docs/learning-notes.md](learning-notes.md)（重寫）、[x.md](../x.md)（從舊 CLAUDE.md 改寫，待 stable 後 rename）
- **修改**：[docs/plan.md](plan.md)（路徑、ASCII tree、done TODOs）、[docs/LOG.md](LOG.md)（加大事/小事分流、加索引、加 Phase 0/1 entry、修正 HD/R 段、追加本 entry）
- **刪除**：docs/AI_usage_note.md（內容搬進 LOG.md）

**文件分工最終定案**（每個 single purpose）：
- [plan.md](plan.md) = 規劃 / 階段 / 進度時間軸
- [STATUS.md](STATUS.md) = 即時進度 + scoring checklist + 已知 bug
- [LOG.md](LOG.md) = 過程紀錄（給口頭 AI 30%）
- [learning-notes.md](learning-notes.md) = 主題式技術筆記（給程式熟悉度 15%）
- [DEV_GUIDE.md](DEV_GUIDE.md) = 夥伴 onboarding Quick Start
- [x.md](../x.md)（→ CLAUDE.md）= 長期工作規範
- [my-note.md](my-note.md) = 個人 scratchpad

### Verification / Tests Performed
- 用 grep 掃過所有 doc 的 stale references（`AI_usage_note`、`../plan.md`、bare `[plan.md](plan.md)`、bare `LOG.md`），除了刻意保留的歷史紀錄（LOG.md 第一個 entry 內提到 "AI_USAGE_LOG.md 改名為 LOG.md"）與 deprecation 註記，**全部清乾淨**。
- 確認 docs/ 下的 7 個 canonical 檔都存在且有內容。
- 確認 docs/AI_usage_note.md 已刪除。

**尚未驗證**：
- Markdown anchor 連結（LOG.md 索引指向 entry）在 GitHub / VSCode preview / Cursor preview 上是否都能直接點開（中文標題的 anchor 行為各 viewer 略有差異） — 留 fallback「Cmd+F 搜日期或標題」說明。
- x.md 是否還有少數遺漏的 stale ref。

### Result
- **7 個 canonical doc 結構就緒**，每個 single purpose、不互相重複。
- plan.md 有 Phase 0–7 規劃 + Phase 1 詳細計劃 + 已勾掉 Phase 0/1（在另一個 session 完成）。
- CLAUDE.md 草稿（x.md）stable：5 條 Hard Constraints、大/小事 workflow、AI 紀錄方式、Tech Stack 版本明確。
- STATUS.md 把 scoring.md 全項目對齊配分 checklist，加銀行存款累積區。
- LOG.md 有完整索引（最新在上）+ template + 3 個 entry（流程奠基、Phase 0、Phase 1）+ 本 entry。
- 後續 Phase 紀錄直接套既有結構，不會再有「該寫哪裡」的猶豫。

### Risks / Follow-ups
- **x.md → CLAUDE.md rename**：x.md 內容已 stable，下一個 commit 前可 rename。rename 後 docs/STATUS.md / docs/learning-notes.md 的 `../CLAUDE.md` 連結會生效。
- ~~**LOGa-trival.md merge**~~：原規劃 Phase 6 處理，後續提前完成（merge 成本檔 Phase 0 「技術棧決策」entry，原檔 2026-05-22 同日刪除）。
- **Phase 2 開始要落實 process gap 改善**：使用者要在 (1) plan 提案、(2) 實作中、(3) 實作後 各 review 一次，才能讓 Phase 2+ 的 HD/R 有實質內容。**這條是這次 session 最重要的後續 action item**。
- Markdown anchor 在不同 viewer 行為差異 → 若 VSCode preview 點不到索引連結，要改成顯式 HTML anchor 或純文字索引。
- README.md 還是空的 → demo 場景用不到，可以延後到 Phase 7。

### Other Notes
**口頭報告素材**（這個 entry 本身就是「如何用 AI 實作程式」30% 那項的核心案例）：

- **文件結構演化過程**：原本 4 個檔互相矛盾 → 收斂到 7 個 canonical 檔，每個 single purpose、不重複。可以講「我跟 AI 一起做的不是寫 code，是先把工作介面定義清楚」。
- **Hard Constraints 設計**：把「會直接扣分」的剛性限制獨立列出，避免被淹沒在一般 rules 裡。這是 AI 主動建議、使用者批准的結構。
- **大事 vs 小事 workflow**：用 7 個具體 trigger 判斷，避免每次都走完整 11 步流程。展現了「我有意識降低協作儀式成本」。
- **HD/R 誠實性原則**：寧可寫「使用者沒介入」也不要把 AI 的決定包裝成「使用者拍板」。**這是學術誠信的核心 — 老師很可能會欣賞這個自覺。**
- **跨 session 同步教訓**：對話中段才發現另一個 session 並行改動 → 提醒以後跨 session 要先讀 `git status` / `git log` 再動工。可以講「我學到要把 git 當成 session 之間的同步機制」。
- **規劃與實作分離**：這次 session 全程沒寫 code，只整理文件結構。展現了「我不是把 AI 當代碼產生器，而是當設計協作者」的差異。


## 2026-05-22 — 文件 rename 與 reference 統一（small）
Phase: 0  
Commit: 本 commit

文件命名統一化：
- `Log.md` → `LOG.md`（uppercase，跟 README 同風格）
- `LEARNING_NOTES.md` → `learning-notes.md`（kebab-case）
- `mynote.md` → `my-note.md`（kebab-case）

連帶：同步更新 STATUS / plan / DEV_GUIDE / learning-notes / my-note / LOG / x.md 共 7 檔的所有 reference；刪掉 x.md 的「Already migrated」段（檔案實體已不存在）；修 plan.md 兩處殘留 LOGa-trival broken link 為 plain text。grep 驗證無殘留 stale ref。
