import fs from 'node:fs';
import path from 'node:path';

export const ROOT = process.cwd();
export const DOCS = path.join(ROOT, 'docs');

export function read(file) {
  return fs.readFileSync(path.join(ROOT, file), 'utf8');
}

export function write(file, content) {
  const full = path.join(ROOT, file);
  fs.mkdirSync(path.dirname(full), { recursive: true });
  fs.writeFileSync(full, content, 'utf8');
}

export function exists(file) {
  return fs.existsSync(path.join(ROOT, file));
}

export function walk(dir, predicate = () => true) {
  const base = path.join(ROOT, dir);
  if (!fs.existsSync(base)) return [];
  const out = [];
  for (const entry of fs.readdirSync(base, { withFileTypes: true })) {
    const rel = path.join(dir, entry.name);
    if (entry.isDirectory()) out.push(...walk(rel, predicate));
    else if (predicate(rel)) out.push(rel.split(path.sep).join('/'));
  }
  return out;
}

export function today() {
  return new Date().toISOString().slice(0, 10);
}

export function currentMonth() {
  return new Date().toISOString().slice(0, 7);
}
