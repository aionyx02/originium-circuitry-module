# 協作治理（Governance）

> Read-on-demand。`../CLAUDE.md`（bootstrap）只放最小規則；詳細流程、文件規範、log 寫法都在這裡。
> 做**大改**前讀這份；小改只需看下方「小改輕流程」+「Log 寫法」。

---

## 文件衝突優先序

當不同來源說法不一致，依此序裁決（高的贏）：

1. 使用者當前明確指令
2. `../CLAUDE.md` 的 Hard Constraints
3. `docs/STATUS.md`（目前到哪、勾了哪些分）
4. `docs/plan.md`（階段策略與細節）
5. `docs/scoring.md`（配分標的）
6. `docs/log/`、`docs/Log.md`（歷史紀錄）

若請求與 `plan.md` 衝突，先把衝突講出來，不要默默偏離。

---

## Collaboration Workflow

依改動大小分兩軌。

### 大改 / 高風險 — 完整流程

符合任一即視為「大改」：
- 動到 `CMakeLists.txt`
- 改 `Part` / `Board` public interface
- 引入新第三方相依
- 刪超過 ~50 行
- 修改 `docs/` 下任何檔
- 修改 `docs/plan.md` 或 `../CLAUDE.md`
- 以非顯然方式跨越 core/UI 邊界

步驟：
1. 檢視相關檔。
2. 簡述當前設計。
3. 提小規模實作計劃。
4. **等使用者核准**才動手。
5. 實作後：列出變更檔。
6. 說明手動測試方式。
7. 對應配分項（引用 [scoring.md](scoring.md) 行號）。
8. 點出風險 / edge case。
9. 更新 [plan.md](plan.md) 進度表，並在 [STATUS.md](STATUS.md) 勾項。
10. 在 [docs/log/](log/) 當月檔 append 一條 10 欄 entry。
11. 若引入口試要解釋的非 trivial 概念，補一則 [learning-notes.md](learning-notes.md)。

### 小改 — 輕流程

Bug fix、補 helper、調 UI 常數、加測資、改錯字：

1. 直接做。
2. 一句話說明改了什麼、怎麼驗。
3. 在 [docs/log/](log/) 當月檔 append 一行 one-liner（見下方「Log 寫法」）。

`plan.md` / `STATUS.md` 不必為小改更新，除非剛好完成一個 checklist 項。

---

## 文件規範

canonical 文件集如下。**未經核准不要新增 `.md`**（會讓文件碎片化）。每個檔單一用途；資訊若可放兩處，放最貼合用途的那個。文件地圖見 [index.md](index.md)。

- **[index.md](index.md)** — 文件路由表，session 開頭先讀。
- **[plan.md](plan.md)** — 階段、配分策略、進度表。要做什麼的 source of truth。
- **[STATUS.md](STATUS.md)** — 現況：到哪、勾了哪些分、已知 bug。落地就勾，不放計劃 / 決策。
- **[DEV_GUIDE.md](DEV_GUIDE.md)** — 上手 Quick Start：build / run / test、專案結構、載測資。
- **[docs/log/](log/)** — AI 協作 active log（依月分檔）。每次有意義變更後 append。口試「如何用 AI 實作程式」素材（口頭報告 30%）。
- **[Log.md](Log.md)** — 已封存的舊 log（2026-05-31 前），凍結不動。
- **[learning-notes.md](learning-notes.md)** — 各非 trivial 功能的教學筆記。口試「對程式的熟悉度」素材（15%）。
- **[scoring.md](scoring.md)** — 配分標的細節。
- **[game-rule.md](game-rule.md)** — 遊戲規則。
- **[my-note.md](my-note.md)** — 使用者個人 scratchpad，未經要求不要寫。

資訊若不明確屬於以上任一個，先問再新增檔。

---

## Log 寫法

> 新 entry append 到 [docs/log/](log/) 當月檔（如 `2026-05.md`）最下方，時間正序，**不維護倒序索引**。

### 大事 vs 小事

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

### 10 欄 Entry Template

```
## YYYY-MM-DD — 摘要
Type: Discussion / Implementation / Debug / Refactor
Phase:  (if have)
Feature:  (if have)
Commit:  (if have)

### Context
這次紀錄的背景：為什麼做、處於哪個階段、跟配分或後續實作的關係。

### AI Contribution
AI 協助了什麼（整理需求、分析規則、提架構、比較方案、產 code、修 bug、建議測試）。

### Human Decision / Review
使用者討論、決定、修正、要求的內容（怎麼描述需求、選哪個方案、修正方向、限制條件、如何 review）。

### Details
更完整的過程紀錄；先把重要資訊放進來，報告前再統整。

### Verification / Tests Performed
驗證 / 測試方式。還沒寫 code 就寫「尚未進行程式測試」並說明用需求 / 配分比對或人工 review。

### Result
這次工作的結果（完成某文件 / 確定技術選型 / 實作功能 / 修好 bug）。

### Risks / Follow-ups
未解風險、後續要補的事、需再確認處。

### Other Notes
額外備註（可放進報告 / 老師可能會問 / 自己要再理解）。
```
