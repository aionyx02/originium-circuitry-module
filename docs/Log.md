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

- 05-23? docs: add README.md / update learning log
- **2026-05-24** — [Phase 2 Day 2c：音效（pickup / place / spin / win）](#2026-05-24--phase-2-day-2c音效pickup--place--spin--win) · `Phase 2` · `Implementation` · 音效 1% = +1%，main loop 加 InitAudioDevice + 4 個 LoadSound + 每 frame 狀態 diff 觸發、CMake 加 post-build copy_directory assets/
- **2026-05-24** — [Day 2b pixel-perfect drag 第三輪：tray 撿起也立刻跟手](#2026-05-24--day-2b-pixel-perfect-drag-第三輪tray-撿起也立刻跟手small) · `Phase 2` · `small` · computeTarget / animateParts 把 mouseDrag 條件從「held+on board」放寬到「held」
- **2026-05-22** — [Phase 2 Day 2b：滑鼠 hover / drag / 左鍵放置 / 右鍵 cancel](#2026-05-22--phase-2-day-2b滑鼠-hover--drag--左鍵放置--右鍵-cancel) · `Phase 2` · `Implementation + Refactor` · +0%（rubric 完整性），Layout 提到 public header、Game 加 `setCursor` cursor 原語、Input 加 `pollMouse` 翻譯滑鼠到既有 Action 流程
- **2026-05-22** — [Phase 2 Day 2a：視覺 polish + 程序材質 + drop shadow](#2026-05-22--phase-2-day-2a視覺-polish--程序材質--drop-shadow) · `Phase 2` · `Implementation` · GUI 2.5% + 材質 1.5% = +4%，圓角 + 漸層 + 高光帶 + bevel + drop shadow 兩 pass
- **2026-05-22** — [learning-notes.md 加 commit-grouped 索引 + 結構重組](#2026-05-22--learning-notesmd-加-commit-grouped-索引--結構重組small) · `Phase 2` · `small` · feature 名稱升 h3、加 `*Phase X · commit <short>*` meta、可對應到原始 commit 學習
- **2026-05-22** — [Phase 2 Day 1：動畫骨架 + 旋轉/移動 pivot 中心](#2026-05-22--phase-2-day-1動畫骨架--旋轉移動-pivot-中心) · `Phase 2` · `Implementation + Refactor` · 動畫 5%+5% + 進階中心 1% = +11%，cursor 語意改成 pivot target 的設計演進
- **2026-05-22** — [Phase 1 手動驗證通過](#2026-05-22--phase-1-手動驗證通過small) · `Phase 1` · `small` · 8 項全綠、+15% 入帳
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


## 2026-05-22 — Phase 1 手動驗證通過（small）
Phase: 1  
Commit: 80f9d6d

使用者親自跑 `./build/game docs/io/Example1.txt`，8 項全綠：wasd 移動 / R 4 方向循環 / Enter 放置 / Esc 雙語意（持有→還原、盤面→拔起）/ 預覽紅框 + 錯誤訊息 / 全擺對跳 You Win banner。配分 +15%（遊戲流程整段）已 bank。STATUS.md / plan.md 同 commit 一併勾掉。


## 2026-05-22 — Phase 2 Day 1：動畫骨架 + 旋轉/移動 pivot 中心
Type: Implementation + Refactor  
Phase: 2  
Feature: 旋轉動畫、零件跟游標連續移動、旋轉/移動中心不為空  
Commit: 0fcfcd4

### Context
Phase 2 開頭三大配分項要拿：旋轉動畫 5%（[scoring.md:93](scoring.md#L93)）、零件跟游標連續移動 5%（[scoring.md:94](scoring.md#L94)）、進階旋轉/移動中心不為空 1%（[scoring.md:61](scoring.md#L61)）。Phase 1 完全沒有動畫；按 R 旋轉是瞬間切換、移動是瞬間跳格。對應 [plan.md §6](plan.md) Phase 2 Day 1。

這次 session 也是「Phase 2 開始實施 process gap 改善」（(1) plan 提案、(2) 實作中、(3) 實作後 各 review 一次）的第一個實驗 — 結果見 HD/R 段。

### AI Contribution
1. **Plan 提案**（implementation 前先 ask）：對到 scoring 哪幾項、改哪幾個檔、Day 1/2/3 切分、材質策略選項、音效策略選項。
2. **設計選擇**（implementation 中）：
   - 動畫狀態加在 `Part`（5 個 float + 1 個 bool + `rotateCount`），lerp 邏輯放 `Renderer`（它有 layout）
   - frame-rate independent lerp factor `1 - exp(-dt*k)` 取代固定 0.4，60FPS / 144FPS 動畫速度一致
   - 旋轉動畫第一版用 `std::fmod` 做 shortest-arc 角度 lerp
   - Pivot 中心格選法：離形心最近的占據格（`Part::computeCenterCell`）
   - 中心格 visual 第一版用白色小圓點（`DrawCircle` 在 pivot 位置）
3. **實作**（9 個檔改動，見 plan.md timeline）：
   - `Part.h` / `Part.cpp` 加 `centerCellRow/Col` + `rotateCount` + 5 visual fields + `computeCenterCell` / `rotatedCenterCell` 方法
   - `Parser.cpp` 載入後 call `p.computeCenterCell()`
   - `Game.cpp` `Action::Rotate` 順便 `++rotateCount`
   - `Renderer.h/cpp` signature 加 `dt`、`animateParts`、`computeTarget`、`drawPartAnimated` 用 `DrawRectanglePro` + 自訂 origin 繞 pivot 旋轉
   - `main.cpp` 傳 `GetFrameTime()`

### Human Decision / Review
**第一次 review** — 使用者跑 Day 1 重測，回報 6 項：
- A 開啟畫面 ✓
- B 移動會滑 ✓
- C 旋轉會轉 ✓
- **D shortest-arc：失敗** ← AI 的 `fmod` 公式雖然數學上正確但使用者觀察到 4 次後反向
- E 拔回 tray ✓
- F Phase 1 流程沒退化 ✓
- G 紅綠 highlight ✓

**使用者修正**：要求改視覺、加進階分。AI 回應：
- D 換成單調累加 `rotateCount`（永遠 +90°，不用 mod，不可能反向）
- 加 `centerCellRow/Col` + `rotatedCenterCell` 方法 + 白色圓點
- 同時補進階「旋轉/移動中心不為空」1%

**第二次 review** — 使用者再驗：
- D ✓、中心點可見 ✓、4 旋轉中心在占據格 ✓
- 但使用者指出：「**旋轉時白點應該不會移動**」（AI 的設計裡 pivot 螢幕位置會隨 rotation 漂移，因為 cursor 還是指 bbox 左上角）
- 使用者要求：「白色圓點換成亮色覆蓋」

**使用者拍板**：改 cursor 語意 = pivot target。
- AI 之前刻意避開這個改動（怕動到 canPlace API），但使用者明確要求視覺體驗。
- AI 做法：保留 `Board::canPlace` API 不變、在 `Game::syncHeldLocation` 內部換算 bbox top-left = cursor - rotatedCenterCell offset。
- `Game::handlePlace` 從盤面拔零件時 snap cursor 到舊 pivot 位置，避免視覺跳動。
- 中心格改用 +120/ch 亮色覆蓋取代白點。

**第三次 review**：全綠（pivot 不動 / 亮色清楚 / 旋轉 CW 一致 / 拔起不跳 / Phase 1 流程零退化）。

### Details
**`1 - exp(-dt*k)` 數學**：每 frame 把 current 往 target 拉一個比例。`k` 是「目標衰減速率」，每秒大約是 `1 - e^(-k)` 的比例被吸過去。位置 k=14（快）、角度/scale k=12。frame-rate 變了 dt 也變，公式自動補償，所以 60FPS / 144FPS 動畫速度一致。

**`rotateCount` vs shortest-arc**：原本 `fmod(target - current + 540, 360) - 180` 算最短弧。但 `current` 累積會超過 360（每按一次 R `current` 往 target 跑、target 又繼續加）；若中間有浮點誤差，下次 mod 後的 diff 可能不是預期方向。改用 `rotateCount` 單調累加 → `currentAngle` 線性 lerp 到 `rotateCount * 90.0f`，永遠 +90° per R，視覺一定 CW。代價：`currentAngle` 會無限長大、但 float 在數千度內精度都夠，render 出來看起來一樣。

**`rotatedCenterCell` 公式**：和 `rotatedShape` 用同一條旋轉公式 case 0~3，只是套到一個 cell（centerCellRow, centerCellCol）而不是整個 shape。

**Cursor=pivot 語意重設計**：
- 原本：cursor 移到 (cursorRow, cursorCol)，部件的「rotated bbox 左上角」會放這裡
- 改後：cursor 移到 (cursorRow, cursorCol)，**部件的 pivot cell** 會落這裡。bbox 左上角自動由 cursor 減去 `rotatedCenterCell` offset 算出。
- 影響：`Game::syncHeldLocation` 加 if cursorCol==TRAY_COL 分支（tray 不套 offset），其他情況 `location = cursor - rotatedCenterCell`；`Game::handlePlace`（持有時放置）改用 `p.location.row/col` 而不是 cursor；`Game::handlePlace`（從盤面拔起）snap cursor 到 `p.location + rotatedCenterCell` 避免視覺跳動。`Board::canPlace` API 完全沒動。
- 為什麼：旋轉時 cursor 不動 → pivot 螢幕位置不動 → 其他格子繞 pivot 轉，符合 Tetris-style 旋轉預期。

**`DrawRectanglePro` 數學**：raylib API 用 `rec.x/y` 當 pivot 螢幕位置 + `origin` 當「rect 左上角到 pivot 的 offset」。要把每 cell 畫成繞共同 pivot 旋轉，每 cell 的 origin = `((centerCol - col + 0.5) * scs, (centerRow - row + 0.5) * scs)` — 這是該 cell 的 top-left 到 pivot 的 vector，當 rotation=0 時還原到正確位置，當 rotation 變動時 raylib 自動把每個 cell 繞 pivot 轉。

### Verification / Tests Performed
- Build：0 warning 0 error（每次改動後 `cmake --build build`）
- Smoke：`./build/game docs/io/Example1.txt` 開窗、raylib log 正常
- **手動互動測試**（使用者親自三輪）：8 項全綠（見上 Human Decision 段）
- 未驗證：高速 spam R / 多零件並行動畫的性能（lerp 計算 O(parts * 4 fields)，數十零件內無感）

### Result
- 動畫骨架 + 旋轉 pivot 全部 land，commit `0fcfcd4`。
- 拿到分數：旋轉動畫 5% + 跟游標移動 5% + 進階中心 1% = **+11%**。
- 銀行存款：21.5 → **32.5%** / 65%。

### Risks / Follow-ups
- Day 2a 視覺 polish 代碼已寫（圓角 + 漸層 + 高光帶 + bevel + drop shadow），**uncommitted、待使用者目視驗收**。LOG entry 將在 commit 後補。
- Day 2b 滑鼠：使用者要求加；不直接加分但補 rubric 完整性。
- Day 2c 音效：使用者已生 `sound1.wav` 一個（root），待生齊 4 個 + 接 raylib `LoadSound` / `PlaySound`。

### Other Notes
**口頭報告素材**（這 entry 三個段都有口試素材）：

- **「為什麼 shortest-arc 換 rotateCount」**：經典「先 over-engineer 用 fmod、實測發現浮點累積問題、改用單調累加」的故事。可以講「fmod 數學上正確但脆弱、單調累加 robust、視覺結果一樣 → robust 比 clever 重要」。
- **「為什麼 cursor 語意要改」**：旋轉視覺預期 = pivot 不動（Tetris 直覺）。第一版實作裡 cursor 還指 bbox 左上角、pivot 螢幕位置會漂；經 review 後拍板改 cursor 語意，但**用 Game 層內部換算保住 `Board::canPlace` API**，不動 core class 公開介面。展現了「會被使用者驅動修正 + 修正方式選擇兼顧『動視覺』與『不動 API』」。
- **「為什麼 lerp 用 `1 - exp(-dt*k)` 而不是固定 0.4」**：frame-rate 獨立。可以講「如果哪天 demo 機是 144Hz 而開發機是 60Hz，固定 factor 的動畫就會跑兩倍快、感覺很假；指數衰減公式有 dt 在裡面、自動補償」。
- **「pivot 中心格選法」**：離形心最近的占據格、用 squared distance 比較。可以講「為什麼不直接用 bbox 中心 — bbox 中心對 L 型零件會落在空格上，違反 rubric『中心不能是空的』；用形心 + 最近占據格保證落在實際存在的 cell」。
- **「process gap 改善實驗」**：這 entry 是 Phase 2 開始實施「plan 提案 / 實作中 / 實作後」三段 review 的第一個案例。使用者三輪驗證都有實質介入修正（D 失敗、pivot 漂移、視覺要亮色），HD/R 段是真實的、不是 AI 包裝。可以講「我從 Phase 1 學到 AI 容易 self-direct，Phase 2 開始強制 checkpoint review」。


## 2026-05-22 — learning-notes.md 加 commit-grouped 索引 + 結構重組（small）
Phase: 2  
Commit: 本 commit

- 加「## 索引（按 commit 切分，最新在上）」段在檔頭附近，讓使用者可以對應到哪段 commit 的 code 改動產生哪些學習 entry
- 每個 entry 升結構：feature 名稱從 `### Feature\n{name}` 升到 `### {name}` h3 標題（可被 markdown 連結 + IDE outline 抓到），sub-sections 從 ### 降到 ####
- 每個 entry 開頭加一行 `*Phase X · commit <short>*` meta，方便對照原始 commit 學習
- Entry 模板同步更新成新格式
- 內容不變


## 2026-05-22 — Phase 2 Day 2a：視覺 polish + 程序材質 + drop shadow
Type: Implementation  
Phase: 2  
Feature: 圓角、背景漸層、cell 程序材質（高光帶 + bevel）、drop shadow 兩 pass  
Commit: (本 commit)

### Context
Phase 2 Day 1 動畫骨架 land 後（commit `0fcfcd4`），盤面的基本動畫 + pivot 結構就位，但視覺水準仍跟 Phase 1 同級 — 純色格、直角、無陰影、無質感。Day 2a 把 [demo/main.cpp](../demo/main.cpp) POC 的視覺水準搬進實作，對到圖形介面 15% 中的「實作 GUI 2.5%」（[scoring.md:90](scoring.md#L90)）與「使用圖片或特別材質 1.5%」（[scoring.md:92](scoring.md#L92)），共 +4%。

策略選擇（Phase 2 提案階段已拍板）：純程序材質、不引入 png 資產。理由：避免 Windows 中文路徑 + 打包 + assets/ 跨平台路徑風險，且 POC 已證明程序材質視覺可達。

### AI Contribution
1. **Plan 提案**（implementation 前 ask）：列出 6 個 polish 子項（背景漸層、板面圓角、tray 圓角、cell 高光帶 + bevel、drop shadow、Win banner 圓角），對應到哪幾個 scoring 子項。
2. **設計選擇**：
   - 圓角實作用 raylib `DrawRectangleRounded` + `DrawRectangleRoundedLinesEx`（內建 API、無需自己畫圓角）
   - 漸層用 `DrawRectangleGradientV` 一行（垂直漸層）
   - 程序材質用每 cell 上方 30% 高光帶 + 下方 10% bevel，色彩用「+70/channel」（高光）與「÷2」（bevel）— 自動適配 part 顏色
   - Drop shadow 用兩 pass：先掃所有 placed/held-on-board 部件畫 shadow、再掃畫所有部件本體 — 確保 shadow 永遠在底下，不會疊到其他 part 上方
   - 高光帶 / bevel / shadow **都用 `DrawRectanglePro`** 跟 cell 共用 pivot 與 rotation，所以旋轉時跟著 cell 一起轉
3. **實作**（單檔 [src/ui/Renderer.cpp](../src/ui/Renderer.cpp) +98 行 / 改部分函式）：
   - 新 helper `drawRoundedCell(int x, int y, int size, Color fill, Color border)` 與 `drawRoundedRect(Rectangle, roundness, fill, border, borderThick)`
   - `drawBoardBg` 三個分支（CANNOT_PLACE / CANNOT_MOVE / EMPTY-or-OCCUPIED）全部換成 `drawRoundedCell`
   - `drawTrayBg` 用 `drawRoundedRect` 取代之前的 `DrawRectangle` + `DrawRectangleLines`
   - `drawWinBanner` 同上換成 `drawRoundedRect`
   - `Renderer::draw` 加 `DrawRectangleGradientV(0, 0, screenW, screenH, ...)` 在 `ClearBackground` 後立刻畫
   - 新 `drawPartShadow(p, cellSize, alpha)`：offset (4, 5) 純黑 alpha 110，每個 cell 一個 `DrawRectanglePro` 旋轉繞 pivot
   - `drawPartAnimated` 加兩個額外的 `DrawRectanglePro` per cell：
     - 高光帶：rec width = scs - 2*inset (inset = 6% scs)、height = 32% scs、origin.y = (cr - r + 0.5) * scs（放 cell 頂部）、color +70/channel alpha 170
     - bevel：rec width = scs、height = 10% scs、origin.y = (cr - r + 0.5) * scs - (scs - edgeH)（放 cell 底部）、color ÷2 alpha 220
   - `drawParts` 改成兩 pass：先 for-loop 只畫 shadow（onBoard 的 part）、再 for-loop 畫所有 part 本體

### Human Decision / Review
**Plan 提案階段**：使用者選「視覺 polish → 材質 → 滑鼠 → 音效」順序（不是「滑鼠先」），原因是先把加分項鎖住、再做不加分但補 rubric 完整性的滑鼠。

**Implementation 中**：AI 在 plan.md §6 詳細列了 6 子項；使用者沒中途介入。

**Implementation 後 review**：使用者跑 `./build/game docs/io/Example1.txt`，手動驗收 8 項 ABCDEFGH 全綠：
- A 背景漸層、B 板面圓角、C tray 圓角、D cell 程序材質、E drop shadow、F 中心格仍亮、G Win banner 圓角、H Phase 1 流程零退化

**沒被指出的問題**：無。一次寫一次過。

### Details
**為什麼程序材質算「特別材質」**：scoring 第 92 行寫「圖片或特別材質」— 「或」字明示 procedural 也算。視覺上每 cell 的「上方高光 + 下方暗邊」構成一個鏡面/金屬感的 cell tile，跟純色塊有明顯區別、且**跟著旋轉一起轉**（不是 overlay 在螢幕上），所以是真的 cell 的材質而非裝飾。

**為什麼 drop shadow 要兩 pass**：如果在每個 part 自己的繪製順序裡先畫 shadow 再畫 body、那後畫的 part 的 shadow 會疊到前面 part 的 body 上面。兩 pass：先掃所有 part 畫 shadow、再掃所有 part 畫 body — 保證 shadow 永遠在底層。

**為什麼背景漸層 + `ClearBackground` 都要**：`ClearBackground` 用底色把 colour buffer 清掉、然後 `DrawRectangleGradientV` 蓋上漸層。其實 `DrawRectangleGradientV` 一個 call 就夠（會 cover 整個 framebuffer），保留 `ClearBackground` 是雙保險，順便保證 depth/stencil clear。

**色彩公式選擇**：
- 高光 `+70/channel`：對深色 part 是顯著提亮、對亮色（黃綠等）已接近飽和、靠 alpha 170 來軟化。**自動適配**所有 part color、不需要 hardcode highlight 表
- bevel `÷2`：對所有色都會明顯變暗，視覺上一致

**`DrawRectanglePro` 用法重點**：rec.x/y = pivot 螢幕位置、origin = rect 左上角到 pivot 的 offset。高光帶 width 比 cell 窄、origin.x 要扣掉 inset；高光帶在 cell 頂部、所以 origin.y 跟 cell 一樣（因為 rect 從 cell top 開始畫）；bevel 在 cell 底部、origin.y 要再扣掉 `(scs - edgeH)`。詳細數學見 [learning-notes.md](learning-notes.md) 「程序材質的旋轉座標數學」entry。

### Verification / Tests Performed
- Build：0 warning 0 error
- Smoke：`./build/game docs/io/Example1.txt` 開窗、raylib log 正常
- **手動互動測試**（使用者親自）：8 項 ABCDEFGH 全綠（背景漸層、板面圓角、tray 圓角、cell 程序材質、drop shadow、中心格仍亮、Win banner 圓角、Phase 1 流程零退化）

### Result
- 視覺 polish + 程序材質完成。
- 拿到分數：實作 GUI 2.5% + 圖片/材質 1.5% = **+4%**。
- 銀行存款：32.5 → **36.5%** / 65%。

### Risks / Follow-ups
- 高光帶 / bevel 顏色與透明度都是 hardcoded `+70 / ÷2 / alpha 170 / alpha 220`，未來想調可能要實驗
- Day 2b 滑鼠：使用者已要求；Day 2c 音效：使用者已生 sound1.wav 一個、待生齊 4 個

### Other Notes
**口頭報告素材**：
- **「為什麼程序材質算特別材質」**：scoring「圖片或特別材質」，AI 跟使用者一起拍板用程序材質。報告講「procedural texture 是 cell 結構的一部分、跟著旋轉一起轉、不是 overlay 在螢幕上 — 視覺上有金屬鏡面感，符合 rubric『特別材質』的精神」。順便講 trade-off：減 png 資產 → 減 Windows 中文路徑/打包風險；代價是「圖片」字面意思可能被嚴格詮釋。
- **「為什麼 drop shadow 要兩 pass」**：兩 pass = 全部 shadow 先畫完、再畫全部 body → 確保 shadow 不疊到別的 part body。展示「考慮過繪製順序對視覺正確性的影響」。
- **「為什麼用 `DrawRectanglePro` 而非 `DrawRectangleRounded` 畫旋轉 cell」**：`DrawRectangleRounded` 不支援旋轉（沒有 angle param）。raylib 圓角 API 只支援 axis-aligned 矩形。所以 part cells 還是用 `DrawRectanglePro`（無圓角但可旋轉），只有 axis-aligned 的 UI 元素（板面格、tray slot、Win banner）才用圓角 API。
- **「一次寫一次過的意義」**：跟 Day 1 三輪 review 對照，Day 2a 沒有需要 iterate。原因：Day 1 的設計有 cursor 語意這種牽動 game logic 的決策、容易出錯；Day 2a 是純視覺、改的全在 Renderer 一個檔、沒有跨核心/UI 邊界、AI 自由度比較大也容易 review。可以講「我學到改動範圍越窄、AI 一次成功率越高」。



## 2026-05-22 — Phase 2 Day 2b：滑鼠 hover / drag / 左鍵放置 / 右鍵 cancel
Type: Implementation + Refactor
Phase: 2
Feature: 滑鼠輸入（板面 + tray hit-test、drag follow、左鍵 Place、右鍵 Remove）
Commit: (本 commit)

### Context
Phase 2 Day 2a（commit `6f7abcb`）把視覺與材質補完後，剩下兩個 Day 2 子項：Day 2b 滑鼠、Day 2c 音效。Day 2b 本身**不直接加分**（鍵盤的 wasd 與 Enter 已涵蓋「移動」5% 與「放置」1%），但 [scoring.md:47](scoring.md#L47) / [:50](scoring.md#L50) 列出「或滑鼠拖移 / 左鍵放置」、補上能讓 rubric 完整性提高、demo 觀感也更接近原作。

策略決定（plan 階段使用者拍板，三個問題）：
1. **今天只做 Day 2b**（不一起做 Day 2c，因為使用者只有一個 wav 還沒生齊 4 個）
2. **設計方案 C**：Game 加 cursor 原語 + Input::pollMouse 翻譯滑鼠 — 不擴 Action enum、不破 Input→Action 模型、未來 editor 也能重用
3. **板面已放零件被左鍵點到**：未持有時 = 撿起該零件（對齊鍵盤 Enter 在 OCCUPIED 格的語意）

### AI Contribution
1. **Plan 提案**（implementation 前 ask）：比較三個跨核心/UI 邊界的設計方案（A/B/C），列出取捨；列出新檔/改檔；明確問三個 decision point。
2. **設計選擇**（C 方案的具體拆解）：
   - `Layout` struct 與 `kTraySlotHeight/Width` 提到 [Renderer.h](../src/ui/Renderer.h) public 區（之前在 Renderer.cpp anon namespace），讓 Input.cpp 能 include 後共用
   - `computeLayout(Game, screenW, screenH)` 改成 public 自由函式（不放進 class）— 兩個 caller（main 算一次給 Input、Renderer 內部自己算一次）都是 read-only 函式呼叫、不需要 instance state
   - `Game::setCursor(int row, int col, bool isTray)` 是**唯一**新增的 public Game API — 內部做 `cursorRow/Col = ...` + `clampCursor()` + `syncHeldLocation()`，跟既有 keyboard `Action::MoveX` 用同一條尾巴
   - `Input::pollMouse(const Layout& L, Game& g)`：板面 hit-test 用 `(mouseX - boardX) / cellSize`、tray hit-test 用 `(mouseY - trayY) / kTraySlotHeight`；左鍵打 `g.update(Action::Place)`、右鍵打 `g.update(Action::Remove)` — **完全重用既有 handlePlace/handleRemove 三分支邏輯**
3. **實作**（4 檔修改 + 0 新檔）：
   - [src/ui/Renderer.h](../src/ui/Renderer.h)：加 `Layout` struct、`kTraySlotHeight/Width` constexpr、`Layout computeLayout(...)` 宣告
   - [src/ui/Renderer.cpp](../src/ui/Renderer.cpp)：移除 anon ns 內的重複定義、把 `computeLayout` 移出 anon ns（變 public），再 reopen 一個 anon ns 包後面的繪圖 helper
   - [src/core/Game.h](../src/core/Game.h) + [src/core/Game.cpp](../src/core/Game.cpp)：加 `setCursor`
   - [src/ui/Input.h](../src/ui/Input.h) + [src/ui/Input.cpp](../src/ui/Input.cpp)：加 `pollMouse`
   - [src/main.cpp](../src/main.cpp)：每 frame 算一次 Layout、`Input::poll()` 後立刻 `Input::pollMouse(L, game)`

### Human Decision / Review
**Plan 提案階段**（這次很重要、是「跨 boundary 設計題」）：
- AI 提了 A/B/C 三方案，使用者**選 C**（cursor 原語 + pollMouse 翻譯滑鼠）
- AI 問「今天兩件都做、還是只先做 Day 2b」，使用者**選只做 Day 2b**（Day 2c 音效要 4 個 wav、目前只有 sound1.wav）
- AI 問「板面已放零件被左鍵點到時」，使用者**選未持有時撿起該零件**（對齊鍵盤 Enter 語意）
- AI 在 plan 中**沒問**的（自行決定）：持有時左鍵點 tray slot = no-op（不要意外丟回 tray，要丟用右鍵）；drag follow 第一版用 cell-quantized（自認為 trade-off 合理 → 使用者隨後回報「希望緊跟滑鼠」、改成 pixel-perfect，見下 Details 與 [learning-notes.md](learning-notes.md) 同篇 entry）

**Implementation 中**：使用者沒中途介入。

**Implementation 後 review**：build 0 warning 0 error、smoke OK；使用者目視驗收 8 項待跑（list 已寫在 [plan.md](plan.md) 進度時間軸 Day 2b done 段）。

### Details
**為什麼不擴 Action enum 變 `std::variant`**：方案 A 看起來最乾淨（每個 mouse action 帶 row/col），但會強迫 core/Game.h 改成 `std::variant<MoveUp, ..., PlaceAt{row,col}, ...>`，所有 update() 的 switch 都要重寫成 `std::visit`，而且 keyboard 路徑也要適配。對解決「滑鼠多帶 row/col」這個問題來說太重武器。方案 C 用「先 setCursor、再用既有 Action」分兩步完成同樣事情、core API 加 1 個小函式即可。

**為什麼 Layout 提到 public header 而不是另開新檔**：避免增加 .md/.h 檔數；Layout 是 UI 內部物、放在 Renderer.h 是 UI 內公開就足夠；Input.h include Renderer.h 不會形成循環依賴（Renderer.h → Game.h、Input.h → Renderer.h → Game.h、Game.h 不 include 任何 UI）。

**drag follow 設計演進 — cell-quantized → pixel-perfect（兩輪迭代）**：

第一版寫 cell-quantized（cursor 跟到 cell、Renderer 既有 lerp 到 cell center 自動產生 drag-follow）。使用者目視驗收前先試、回報「想要緊跟滑鼠、不要卡頓感、瞄準交給紅綠框、放開時落到框框那」。

**第二版（pixel-perfect on board）**：(1) Game 加 `mouseControlling` flag（任何 mouse activity = true、任何鍵盤 `Action::Move*` = false）；(2) Renderer::computeTarget 在「held + on board + mouseControlling」分支讀 `GetMousePosition()`、target = mouse pixel；(3) animateParts 加 `mouseDrag` 條件分支、`currentCenterX/Y` **直接 snap 不 lerp**（只 lerp angle/scale）；(4) Input::pollMouse 末尾改成「mouseControlling && held && over board → 每 frame setCursor」、不再依賴 `mouseMoved` — 這樣 click-place 自動選下一個 part 後、新 part 也立刻跟上滑鼠。

**第三版（pixel-perfect 從 tray pickup 起算）**：使用者試完第二版回報「點 tray 撿起後要等手往板面走、part 才開始跟手」。原因：第二版的 mouseDrag 條件是 `onBoardHeld && mouseControlling`，當 cursor 還在 TRAY_COL（剛點 tray 撿起、還沒移到板面）時不走 mouse-pixel 分支、用的還是 tray slot inner 位置（scale 0.5）。修法：把 `mouseDrag` 從「on board + held + mouseControlling」放寬成「held + mouseControlling」、computeTarget 對應分支也改成 mouseControlling 優先（無論 cursor 在 tray 還是 board 都走 mouse pixel + scale 1.0）。效果：點 tray slot 那一 frame、part 立刻 snap 到 mouse 像素位置、scale 同步補到 1.0；之後一路跟手。

設計分離：**cursor 仍 cell-quantized**（紅綠預覽框 + canPlace 用 cell）/ **視覺跟 mouse pixel**（drag 手感、tray 撿起即刻生效）；放開時 part 變 placed、既有 lerp 從 mouse pixel 滑到 cell center，產生「落框感」。報告講「我學到視覺與 cursor 是兩件事 — cursor 是 game logic 的『指哪一格』、視覺是 UI 的『手上拿著的零件在哪』，分離後鍵盤模式跟滑鼠模式各取所需；連 tray↔board 邊界也不需要視覺特例」。

**為什麼右鍵不需要 hit-test 也能用**：handleRemove 三分支（持有 → 退回 tray / cursorCol==TRAY_COL → no-op / OCCUPIED → 拔起）對任何 cursor 位置都成立。所以 Input::pollMouse 只要在板面 hit 到時把 cursor 設過去（讓「在哪格右鍵就拔哪格」生效）、其它情形（含 tray、空白區）右鍵就退回手上的 part — 全交給 handleRemove 處理。

**Action::Place 同樣享用三分支**：handlePlace 已寫好「持有+板面 → 放置 / 持有+TRAY → 提示移到板面 / 未持有+板面 OCCUPIED → 撿起 / 未持有+TRAY → 撿起 tray 該 part」。pollMouse 設好 cursor 再呼叫 `update(Action::Place)`、四種情況都對。

**`won` 後不再讀滑鼠**：pollMouse 第一行 `if (g.won) return;` — 跟 update() 對齊（won 後鍵盤也吃不到）。

### Verification / Tests Performed
- Build：`cmake --build build` → 0 warning 0 error、4 個 .o 重編、1 個 link
- Smoke：`./build/game docs/io/Example1.txt` 開窗 ~2 秒、raylib log 顯示完整 init（rcore/rlgl/rshapes/rtextures/rtext/rmodels/raudio 全 load、OpenGL 4.1 Metal、target 16.667ms/frame）、沒 crash、沒 stderr 警告
- **手動互動測試**（待使用者跑，8 項在 plan.md Day 2b done 段）：tray 左鍵撿起 / drag follow / 板面左鍵放置（合法+不合法）/ 右鍵退回 / 板面左鍵撿起已放零件 / 板面右鍵拔零件 / 持有時左鍵點 tray no-op / 鍵盤零退化

### Result
- 滑鼠輸入完成（代碼層面）。
- 拿到分數：**+0%**（不直接加分；補 rubric 完整性與 demo 體驗）。
- 銀行存款：36.5% / 65%（不變）。
- 新增公開 API：`Game::setCursor`、`Input::pollMouse`、`Layout` struct（+ `computeLayout`、`kTraySlotHeight/Width`）— 之後 Phase 5 editor 可重用 setCursor。

### Risks / Follow-ups
- drag follow 是 cell-quantized；若 demo 時老師覺得「不夠絲滑」、可改 Renderer::computeTarget 在 held+on-board 時讀 `GetMousePosition()` 直接拿像素位置（單檔小改）
- 沒處理 mouse capture / window focus 邊界情況（mouse 移出視窗時的行為照 raylib 預設）— 兩週專案不值得花時間
- 持有時 cursor 同時被鍵盤跟滑鼠驅動：keyboard `Action::MoveX` 改 cursor、滑鼠 hover 也改 cursor。若使用者按 W 然後立刻 mouse hover 別處、cursor 會被 mouse 蓋掉。目前 OK（drag follow 就是要這樣）；不持有時 keyboard 路徑也不會被影響（pollMouse 不持有時不動 cursor）

### Other Notes
**口頭報告素材**：
- **「為什麼不擴 Action enum 變 variant」**：方案 A 看起來最 OO 最乾淨，實際上是「為了一個小問題改動 core API 一大塊」。方案 C 用「先設 cursor、再走既有 Action」分兩步完成同樣事情、新增 1 個小函式 — 展示「設計 API 時優先考慮改動量、不要為了純度做不成比例的重構」。
- **「為什麼 Layout 提到 public header 是合理選擇」**：Layout 是 UI 內部物（不該 leak 進 Game/Parser），但它需要被 Input 跟 Renderer 兩個 UI 模組共用 — 「public to UI、private to core」就是 Renderer.h 的位置。展示「邊界不是非黑即白、要看 caller 是誰」。
- **「drag follow 從 cell-quantized 改成 pixel-perfect 的設計演進」**：第一版我自己選 cell-quantized（理由：Renderer 維持純函式、視覺對「放到哪格」更明確），使用者試之前回報想要「緊跟滑鼠、不要卡頓」。第二版分離視覺與 cursor：**cursor 仍 cell-quantized**（紅綠預覽框與 canPlace 不變）、**視覺跟 mouse pixel 且 snap 不 lerp**（手感緊）、**放開時走 placed lerp**（落框感）。報告講：「我學到 cursor 是 game logic 的『指哪一格』、視覺是 UI 的『手上拿著的零件在哪』、這兩件事可以分離。第一版我把它們綁在一起、結果手感不對；第二版用一個 UI hint flag `mouseControlling` 把它們解耦、鍵盤模式 cursor=視覺=cell、滑鼠模式 cursor=cell（瞄準）/ 視覺=pixel（手感）」。
- **「Layout 從 anon-ns 提到 public」是必要的小重構**：對應 CLAUDE.md「Default: don't refactor」原則的例外 — refactor 是為了實作新功能、不是為了純度。報告講「我學到 refactor 的合理時機：當新功能需要跨檔共用、把該共用的東西公開出來，比 copy-paste 更乾淨」。
- **「重用既有 handlePlace / handleRemove 三分支」**：keyboard 跟 mouse 走同一條尾巴 — 不需要寫「滑鼠版的 place」「滑鼠版的 remove」。展示「設計分離後的好處：input source 與 game logic 解耦」。


## 2026-05-24 — Day 2b pixel-perfect drag 第三輪：tray 撿起也立刻跟手（small）
Phase: 2
Commit: 本 commit

使用者目視驗收第二版後回報「點 tray 撿起後要等手往板面走、part 才開始跟手」。把 [Renderer.cpp](../src/ui/Renderer.cpp) computeTarget 的 mouseControlling 分支從「held + on board」放寬到「held」（涵蓋 cursor 仍在 TRAY_COL 的情況）、且 animateParts 的 `mouseDrag` 條件同步改為 `held && mouseControlling`。結果：點 tray slot 那一 frame、part 立刻 snap 到 mouse 像素位置 + scale 1.0、之後一路跟手。沒動 Game/Input；只 2 處 Renderer 小編輯。Build 0/0、smoke OK。詳見上方 Phase 2 Day 2b entry 的「第三版」段。


## 2026-05-24 — Phase 2 Day 2c：音效（pickup / place / spin / win）

Type: Implementation
Phase: 2
Feature: Sound effects（音效；[scoring.md:91](scoring.md#L91) 1%）
Commit: 待 commit

### Context

Phase 2 最後一格：音效。前面 Day 1（動畫骨架）+ Day 2a（GUI/材質/陰影）+ Day 2b（滑鼠）已把 Phase 2 圖形分項拿下 14% / 15%，剩這 1% 收尾就能進 Phase 3（雙色 + 進階功能 +12%）。

使用者策略沿用 plan §6：用 [sfxr.me](https://sfxr.me/) 線上生短音效、放 `assets/sfx/`、不引入外部音檔處理庫（raylib 的 `LoadSound` 內建 miniaudio 支援 wav/mp3/ogg/flac）。

### AI Contribution

1. **方案盤點 + 風險條列**（4 點：資產佈局 / CMake 改動 / main.cpp 改動 / 不動的東西 + 6 個 edge case）— 在動 code 前確認使用者要怎麼接（[CLAUDE.md](../CLAUDE.md) large change workflow 第 1–4 步）
2. **架構選擇**：避開「Game 產生 sound event hint / 抽 SoundManager 類別」的 over-engineer，改用「main loop snapshot 前後 frame、做 4 個 int/bool diff → 觸發音」。理由：1% 不值得擴 API，且這套做法不破 core 不能 include raylib 的界線
3. **觸發優先序設計**：發現 `autoSelectNextUnplaced()` 會在 place 成功後立刻把 heldIdx 從 -1 set 回新 part — 一個 frame 同時滿足「place 觸發」+「pickup 觸發」。設成 `win > place > pickup > spin` 單音優先序、避免重音
4. **raylib API 差異除錯**：第一次寫 `IsSoundReady`（5.0 名稱）→ compile error → grep 系統 raylib 5.5 header 確認 5.5 改名 `IsSoundValid` → 一次 replace_all 修好

### Human Decision / Review

- **拍板「下個 phase = Day 2c」**（不是 Phase 3）：保 Phase 2 完整再開新階段
- **檔案命名 + 格式**：使用者把 4 個音檔放 `assets/sfx/` 為 `pickup.mp3 / place.mp3 / spin.mp3 / victory.mp3`（plan §6 寫的是 rotate.wav、實際使用者取名 spin.mp3 — 主檔調整 LoadSound 路徑配合）
- **win 音檔候選 2 選 1**：assets/sfx/ 裡同時有 `victory.mp3` 跟 `win.wav` — AI 詢問後使用者選 `victory.mp3`（36KB MPEG layer III stereo，較有「交響樂」勝利感）
- **「補好了，開始做」**：使用者一句話結束 plan 階段、進實作

### Details

**A. 資產佈局**

`assets/sfx/` 4 個檔（raylib 5.5 LoadSound 內建 miniaudio decoder、wav/mp3 通吃）：

| 觸發 | 檔名 | 大小 | 規格 |
|---|---|---:|---|
| pickup | pickup.mp3 | 663 KB | 44.1 kHz / stereo |
| place  | place.mp3  | 6.1 KB | 44.1 kHz / stereo |
| spin   | spin.mp3   | 6.6 KB | 44.1 kHz / stereo |
| win    | victory.mp3 | 36 KB | 44.1 kHz / stereo / MPEG L3 256 kbps |

> 殘留 `assets/sfx/win.wav`（原 `sound1.wav`、4KB 8-bit mono）目前未引用、可刪可留（不影響行為）。

**B. CMakeLists.txt 改動**（[CMakeLists.txt:43–47](../CMakeLists.txt#L43)）

```cmake
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game>/assets)
```

每次 build 完把 `assets/` 整個 mirror 到 `$<TARGET_FILE_DIR:game>/assets`（debug build 在 `build/assets/`）— runtime 用相對路徑 `assets/sfx/xxx.mp3` 就找得到。Phase 6 才煩惱 Windows 最終 install 規則（也只是換成 `install(DIRECTORY ...)`）。

**C. main.cpp 改動**（[src/main.cpp:45–67, 86–119, 129–134](../src/main.cpp)）

1. 新 anon-namespace 兩個 helper：`countPlaced(Game&)` / `sumRotateCount(Game&)`
2. `InitWindow` 後加 `InitAudioDevice()`；exit 前加 `CloseAudioDevice()`
3. `LoadSound × 4`（pickup / place / spin / victory）→ exit 前 `UnloadSound × 4`
4. 進 main loop 前 4 行 snapshot：

   ```cpp
   int  prevHeld       = game.heldPartIdx;
   bool prevWon        = game.won;
   int  prevPlacedCnt  = countPlaced(game);
   int  prevRotateSum  = sumRotateCount(game);
   ```

5. 每 frame 跑完 `game.update + Input::pollMouse` 後算 4 個 now value、做 diff、依優先序最多放一個音：

   ```cpp
   if (wonNow && !prevWon)                     PlaySound(sndWin);
   else if (placedCntNow > prevPlacedCnt)      PlaySound(sndPlace);
   else if (prevHeld < 0 && heldNow >= 0)      PlaySound(sndPickup);
   else if (rotateSumNow > prevRotateSum)      PlaySound(sndSpin);
   ```

   每呼叫前 `IsSoundValid(snd)` 守一下，缺檔/載入失敗會回 false、就靜音不 crash。

6. 4 個 prev 更新：`prevHeld = heldNow; ...`

**D. 不動的東西（界線守住）**

- `core/`：Game / Part / Board / Parser / WinChecker / Action enum 完全沒動 — core 不能 include raylib 這條界線在 Phase 0 就立的，Day 2c 沒理由破
- `ui/Renderer.*` / `ui/Input.*`：滑鼠 / 鍵盤 / 動畫 / 繪圖全部不碰
- 沒新檔（沒抽 SoundManager / AudioBus 類別）

**E. raylib 5.5 API 名稱踩雷**

第一次寫 `IsSoundReady` → 4 個 compile error。grep `/opt/homebrew/Cellar/raylib/5.5/include/raylib.h` 發現 5.5 改名為 `IsSoundValid`（5.0 → 5.5 的 breaking rename，伴隨 `IsAudioStreamValid` / `IsTextureValid` 等批次 rename）。`replace_all` 一次修好。

### Verification / Tests Performed

- **Build**：`cmake --build build` → 0 warning 0 error
- **post-build copy 驗證**：`ls build/assets/sfx/` → 5 檔全部 mirror（含 win.wav 殘留）
- **Audio init smoke**：跑 `./build/game ../docs/io/Example1.txt` 1.5 秒、`grep -iE "audio|sound|wave"` raylib log：
  - `AUDIO: Device initialized successfully | Backend: miniaudio | Core Audio`
  - `FILEIO: [assets/sfx/pickup.mp3] File loaded successfully` + `WAVE: Data loaded successfully (44100 Hz, 32 bit, 2 channels)`
  - 同 4 行 for place.mp3 / spin.mp3 / victory.mp3
  - 沒 error、沒 warn
- **手動互動驗證**（2026-05-24 通過）：
  - [x] 開檔不該有聲音 — 驗收時暴露 Phase 1 keyboard-era `autoSelectNextUnplaced` 殘留 bug（零件自動跟手）；獨立 commit 修掉，與音效本身無關
  - [x] 滑鼠左鍵點 tray → pickup
  - [x] 持有時按 R → spin
  - [x] 持有時左鍵點合法格 → place
  - [x] 全部擺對 → victory（且不會跟 place 雙重播）
  - [x] 不合法格 → 紅框 + 錯誤訊息、不放 place 音（placedCount 沒上升）
  - [x] 鍵盤 wasd/R/Enter 觸發同樣的音

### Result

- **拿到分數：+1%**（音效 1%、[scoring.md:91](scoring.md#L91)）
- **銀行存款：36.5 → 37.5%** / 65%
- **Phase 2 收尾完成**：圖形介面分項 15% / 15% 全拿（GUI 2.5 + 音效 1 + 材質 1.5 + 旋轉動畫 5 + 跟游標 5）
- **新增公開介面：無**（純 main.cpp + CMake、API 表面零增長）

### Risks / Follow-ups

- **`PlaySound` 重複播放會 cut off 前次**：raylib `PlaySound` 同個 Sound 重複呼叫會中斷之前那次。連按 R 4 次旋轉、spin 音會被連續中斷而非疊著放。若想疊放要改 `LoadSoundAlias` + 多 channel；目前不需要（1% 拿到了，oral 不會被問疊放）
- **pickup.mp3 663 KB 偏大**：開檔 LoadSound 一次解碼到 RAM、不影響 runtime。若 Phase 6 打包想瘦身、用 jsfxr 重生一個短的（jsfxr 出來通常 < 10KB）
- **Phase 6 Windows 打包**：post-build copy 是 dev-time 的；Phase 6 要改成 `install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)` 加上 NSIS/zip 出貨。中文路徑 + LoadSound：raylib 5.5 在 Windows 用 UTF-8 path 應該 OK、Phase 6 dry run 時驗
- **assets/sfx/win.wav 殘留**：使用者改檔過程中留下、code 沒引用、可刪可留；不影響功能

### Other Notes

**口頭報告素材**：

- **「為什麼不抽 SoundManager 類別？」**：1% 的功能、整段純粹是「state 變了 → 播個音」、不需要狀態機 / 不需要音量管理 / 不需要 channel mixing。抽類別純粹是為了好看而不是為了解決問題。展示「YAGNI（You Aren't Gonna Need It）的判斷時機 — 不是任何 OOP 機會都該抽出類別」。
- **「為什麼 sound trigger 放 main.cpp 而不是 Game.cpp？」**：Game 是 core、不能 include raylib（界線從 Phase 0 立的）。如果要在 Game 觸發音、就得在 Game 加「sound event 事件 enum」、Renderer 或 main 訂閱 — API 變寬了。但 Day 2c 的觸發訊號 100% 從外觀察 game state 就拿得到（heldIdx / won / placedCount / rotateSum 都是 public field）— 不必擴 API。展示「界線設計：public field 已經是訊號、不必加 event 系統」。
- **「frame diff 觸發 vs event-driven 觸發」**：兩種寫法各有取捨。Diff 法：簡單、不破 API、可能誤觸（如 rotate 用 sum 變化）；Event 法：精確、需 Game 主動暴露事件。1% 功能 + Game state 已經夠表達就選 diff 法。展示「設計選擇要看 caller / cost / 是否一次性」。
- **「優先序設計：為什麼 win 蓋 place」**：放置最後一個零件那 frame、placedCount 上升（會觸發 place）+ won 變 true（會觸發 win）。如果不設優先序、兩個音同 frame 播會吵且 win 被 place 切（PlaySound 後呼叫 PlaySound 會 cut off）。優先序 `win > place > pickup > spin` 是依「事件重要性 + 用戶期待」排的。展示「不是把所有訊息都丟給用戶、而是設計『一次顯示最重要的那一個』」。
- **「raylib 5.5 API rename 的處理」**：寫 `IsSoundReady`（從 5.0 教學記憶）→ compile fail → grep 系統 raylib 5.5 header 確認 `IsSoundValid` → 修。展示「不要相信記憶中的 API、要查當下版本的 header；compile error 不是壞事、是『版本對齊』的提示」。
- **「為什麼不接 raylib 的 `PauseSound` / `StopSound`？」**：遊戲沒「暫停選單」、Win banner 出來後關掉視窗就結束、不需要中斷音樂。YAGNI。

