import { currentMonth, exists, write } from './docs-utils.mjs';

// Scaffolds docs/log/YYYY-MM.md for the current month (or one passed as argv[2]).
const month = process.argv[2] || currentMonth();
if (!/^\d{4}-\d{2}$/.test(month)) {
  console.error(`bad month "${month}"; expected YYYY-MM`);
  process.exit(1);
}
const file = `docs/log/${month}.md`;
if (exists(file)) {
  console.log(`${file} already exists; nothing to do`);
  process.exit(0);
}
write(file, `# 開發紀錄 — ${month}

> 本月 active log。新 entry **直接 append 到本檔最下方**（時間正序），不維護倒序索引。
> 寫法（10 欄 template / 大事 vs 小事）見 [docs/CLAUDE.md](../CLAUDE.md#log-寫法)。

---
`);
console.log(`created ${file}`);
