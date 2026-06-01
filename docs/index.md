# 文件路由（Index）

> Session 開頭讀此檔，再按意圖檢索。**不要遞迴讀完整個 `docs/`。**
> 啟動鏈：`../CLAUDE.md`（規則）→ `index.md`（本檔）→ `STATUS.md`（現況）→ 按意圖讀下表。

## 文件地圖

| 檔 | 用途 | 何時讀 |
|---|---|---|
| [STATUS.md](STATUS.md) | 現況：當前 Phase、下一步、分數、進行中、已知 bug、配分 checklist | 每個任務開頭 |
| [CLAUDE.md](CLAUDE.md) | 治理：大/小改流程、文件衝突優先序、文件規範、Log 寫法 | 做大改前 |
| [plan.md](plan.md) | 階段策略、各 Phase 細節、進度表 | 規劃 / 查某 Phase 細節 |
| [scoring.md](scoring.md) | 配分標的細節 | 對應分數時 |
| [DEV_GUIDE.md](DEV_GUIDE.md) | build / run / test、專案結構、載測資 | 要動 code / 跑程式 |
| [game-rule.md](game-rule.md) | 遊戲規則 | 釐清玩法 / 勝利條件 |
| [io/input-file-format.md](io/input-file-format.md) | 關卡檔純文字格式 | 動 Parser / 測資 |
| [learning-notes.md](learning-notes.md) | 各功能教學筆記（口試 15% 素材） | 準備口試 / 補非 trivial 概念 |
| [log/](log/) | active 開發紀錄（依日分檔 `YYYY-MM-DD.md`，口試 30% 素材） | 每次有意義變更後 append |
| [Log.md](Log.md) | 已封存舊 log（2026-05-31 前），凍結 | 查舊史 / 寫報告 |
| [my-note.md](my-note.md) | 使用者 scratchpad | 使用者要求時 |

## 按意圖檢索

- **要做某功能** → `STATUS.md`（現況/分數）→ `plan.md` 對應 Phase → 動 code 看 `DEV_GUIDE.md`。
- **流程 / 該不該走大改** → `CLAUDE.md`。
- **配分相關決策** → `scoring.md` + `STATUS.md` checklist。
- **規則 / 檔格式疑問** → `game-rule.md` / `io/input-file-format.md`。
- **記錄變更** → append 到 `log/` 當日檔（寫法見 `CLAUDE.md` §Log 寫法）。
