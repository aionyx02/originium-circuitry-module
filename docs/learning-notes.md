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

<!-- 第一筆 entry 預計在 Phase 1 Parser 完成後新增 -->
