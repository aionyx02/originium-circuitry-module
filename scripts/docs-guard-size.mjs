import fs from 'node:fs';
import path from 'node:path';
import { ROOT, walk } from './docs-utils.mjs';

// Per-task / always-loaded docs must stay small (this is the whole point of the
// retrieval-first restructure). Archives and read-on-demand reference docs are
// either exempt or have generous caps. KB = 1024 bytes.
const limits = [
  ['CLAUDE.md', 8 * 1024],          // bootstrap, auto-loaded
  ['docs/index.md', 4 * 1024],      // router, read first every session
  ['docs/STATUS.md', 7 * 1024],     // current state, read every task
  ['docs/CLAUDE.md', 8 * 1024],     // governance, read on demand
  ['docs/plan.md', 30 * 1024]       // plan, read on demand
];

// Each monthly active log file: cap to force rollover into a new month file
// instead of growing unbounded like the old Log.md (132 KB).
const logCap = 45 * 1024;
for (const f of walk('docs/log', f => /\/\d{4}-\d{2}\.md$/.test(f))) {
  limits.push([f, logCap]);
}

let failed = false;
for (const [file, limit] of limits) {
  const full = path.join(ROOT, file);
  if (!fs.existsSync(full)) continue;
  const size = fs.statSync(full).size;
  if (size > limit) {
    console.error(`SIZE FAIL ${file}: ${size} > ${limit} (split or move detail to docs/log/ or plan.md)`);
    failed = true;
  } else {
    console.log(`SIZE OK ${file}: ${size}/${limit}`);
  }
}
process.exit(failed ? 1 : 0);
