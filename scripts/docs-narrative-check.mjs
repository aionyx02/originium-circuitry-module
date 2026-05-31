import { read } from './docs-utils.mjs';

// Keeps execution narrative OUT of the current-state docs. Dated log-style
// entries and large pasted command output belong in docs/log/, not here.
const checked = ['docs/index.md', 'docs/STATUS.md'];
const banned = [
  [/^##\s*20\d{2}-\d{2}-\d{2}/m, 'dated log-style heading (put it in docs/log/)'],
  [/```(?:bash|sh|zsh|powershell|text)?\n[\s\S]{800,}?```/m, 'large pasted command output (put it in docs/log/)']
];

let failed = false;
for (const file of checked) {
  let content = '';
  try { content = read(file); } catch { continue; }
  for (const [pattern, why] of banned) {
    if (pattern.test(content)) {
      console.error(`NARRATIVE ROUTING FAIL ${file}: ${why}`);
      failed = true;
    }
  }
}

if (!failed) console.log('narrative routing ok');
process.exit(failed ? 1 : 0);
