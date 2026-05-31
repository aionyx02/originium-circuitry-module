import { exists, read, walk } from './docs-utils.mjs';

// Catches dead internal links after the restructure. Only links that explicitly
// start with docs/ or scripts/ are validated (relative links like (plan.md) are
// resolved per-file below). Directory links (ending in /) are skipped.
const markdownFiles = walk('docs', f => f.endsWith('.md')).concat(['README.md', 'CLAUDE.md'].filter(exists));
const linkPatterns = [
  /`((?:docs|scripts)\/[^`\n]+?)`/g,
  /\]\(((?:docs|scripts)\/[^)\n#]+)(?:#[^)\n]+)?\)/g
];

let failed = false;
for (const file of markdownFiles) {
  const content = read(file).replace(/```[\s\S]*?```/g, '');
  for (const pattern of linkPatterns) {
    for (const m of content.matchAll(pattern)) {
      const target = m[1].trim().replace(/,$/, '');
      if (target.endsWith('/')) continue;            // directory link
      if (target.includes('*')) continue;            // glob example, not a real path
      if (/YYYY|MM|DD/.test(target)) continue;        // template placeholder path
      if (!target.match(/\.(md|mjs|json|yml|yaml)$/)) continue;
      const normalized = target.split('#')[0];
      if (!exists(normalized)) {
        console.error(`LINK FAIL ${file}: ${normalized} does not exist`);
        failed = true;
      }
    }
  }
}

if (!failed) console.log('links ok');
process.exit(failed ? 1 : 0);
