import { spawnSync } from 'node:child_process';

const steps = [
  ['docs:guard-size', 'scripts/docs-guard-size.mjs'],
  ['docs:guard-links', 'scripts/docs-guard-links.mjs'],
  ['docs:guard-placeholders', 'scripts/docs-guard-placeholders.mjs'],
  ['docs:narrative-check', 'scripts/docs-narrative-check.mjs']
];

for (const [name, script] of steps) {
  console.log(`\n> ${name}`);
  const result = spawnSync(process.execPath, [script], { stdio: 'inherit' });
  if (result.status !== 0) process.exit(result.status ?? 1);
}
console.log('\ndocs refresh ok');
