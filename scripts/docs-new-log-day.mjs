import { today, exists, write } from './docs-utils.mjs';

// Scaffolds docs/log/YYYY-MM-DD.md for today (or a date passed as argv[2]).
const day = process.argv[2] || today();
if (!/^\d{4}-\d{2}-\d{2}$/.test(day)) {
  console.error(`bad date "${day}"; expected YYYY-MM-DD`);
  process.exit(1);
}
const file = `docs/log/${day}.md`;
if (exists(file)) {
  console.log(`${file} already exists; nothing to do`);
  process.exit(0);
}
write(file, `# 開發紀錄 — ${day}

> 當日 active log（一天一檔）。新 entry **直接 append 到本檔最下方**（時間正序），不維護倒序索引。
> 寫法（10 欄 template / 大事 vs 小事）見 [docs/CLAUDE.md](../CLAUDE.md#log-寫法)。

---
`);
console.log(`created ${file}`);
