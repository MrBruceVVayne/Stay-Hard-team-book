#!/usr/bin/env python3
"""Сборка teambook.pdf. Читает таблицу из CONTENTS.md (status=in), готовит листинги без
`#pragma once` и локальных #include (зависимости остаются комментарием `// + файл`), собирает LaTeX.
Использование: python3 tools/build_book.py [--font 8] [--no-pdf]"""
import re, os, subprocess, sys, shutil

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
font = 8
for i, a in enumerate(sys.argv):
    if a == '--font': font = int(sys.argv[i + 1])
build = 'build/book'
shutil.rmtree(build, ignore_errors=True)
os.makedirs(build + '/src', exist_ok=True)

rows = []
for line in open('CONTENTS.md', encoding='utf8'):
    if line.startswith('|') and not line.startswith('| section') and not line.startswith('|---'):
        c = [x.strip() for x in line.strip().strip('|').split('|')]
        if len(c) >= 5 and c[4] == 'in': rows.append(c)

def esc(s): return s.replace('%', '\\%').replace('_', '\\_').replace('&', '\\&').replace('#', '\\#')

def prepare(path):
    out = []
    for l in open(path, encoding='utf8').read().rstrip('\n').split('\n'):
        if l.strip() == '#pragma once': continue
        m = re.match(r'#include\s+"([^"]+)"', l)
        if m:
            if m.group(1) != 'tmp.cpp': out.append('// + ' + os.path.basename(m.group(1)))
            continue
        l = l.replace('\t', '        ').rstrip()
        n = len(l) - len(l.lstrip(' '))
        out.append(' ' * (n // 8 * 2 + n % 8) + l.lstrip(' '))   # 8 пробелов -> 2
    while out and not out[0].strip(): out.pop(0)
    dst = os.path.join(build, 'src', path.replace('/', '__'))
    open(dst, 'w', encoding='utf8').write('\n'.join(out) + '\n')
    return dst

head = open('tools/preamble.tex', encoding='utf8').read().replace('FONTSIZE', str(font)).replace('FONTSKIP', str(font + 0.5 if font < 8 else font))
BANNERS = ['STAY HARD.', "WHO'S GONNA CARRY THE BOATS AND THE LOGS?", "THEY DON'T KNOW US, SON.", 'STAY FOCUSED. STAY FUCKING HARD.', 'CALLUS YOUR MIND.', 'NO ONE IS COMING TO SAVE YOU. SOLVE IT.', 'WE ARE, MOTHERFUCKERS. WE ARE.', 'SUCK IT UP. SUBMIT THE SOLUTION.', 'BE UNCOMMON AMONGST UNCOMMON PEOPLE.', 'MOTIVATION IS CRAP. DISCIPLINE WINS.', "YOU'RE ONLY AT 40%. KEEP GOING.", "DON'T STOP WHEN YOU'RE TIRED. STOP WHEN YOU'RE DONE.", 'ONE MORE TEST. ONE MORE ATTEMPT.', 'DISCIPLINE EQUALS FREEDOM.', 'EMBRACE THE SUCK.', 'TAKE SOULS.']
TWO = {'Памятка', 'Факты'}
mode = 3
cnt = 0
bn = 0
body = []
cur = None
first_fact = True
for sec, path, title, prio, status, *_ in rows:
    if sec != cur:
        newmode = 2 if sec in TWO else 3
        if cur is not None and newmode == mode and sec not in TWO:
            body.append('\\banner{%s}' % esc(BANNERS[bn % len(BANNERS)]).replace('\\%', '\\%')); bn += 1
        if newmode != mode or cur is None:
            sty = '\\small\\linespread{1.3}\\selectfont\\setlength{\\parskip}{4pt plus 1pt}' if newmode == 2 else '\\footnotesize\\linespread{1}\\selectfont'
            body.append('\\end{multicols*}\\newpage\\begin{multicols*}{%d}%s' % (newmode, sty))
            mode = newmode
        body.append('\\section{%s}' % esc(sec))
        cur = sec
    if path.endswith('.tex'):
        if not first_fact:
            body.append('\\banner{%s}' % esc(BANNERS[bn % len(BANNERS)])); bn += 1
        first_fact = False
    if not path.endswith('.tex'):   # у файлов фактов свои \subsection
        cnt += 1
        if cnt % 3 == 0:
            body.append('\\banner{%s}' % esc(BANNERS[bn % len(BANNERS)])); bn += 1
        body.append('\\subsection{%s}' % esc(title))
    if path.endswith('.tex'):
        body.append('\\input{%s}' % os.path.join(ROOT, path).replace('\\', '/'))
    else:
        lang = '[language=bash]' if path.endswith('.sh') else ''
        body.append('\\lstinputlisting%s{%s}' % (lang, os.path.join(ROOT, prepare(path)).replace('\\', '/')))
head = head.replace('ROOT', ROOT)
open(build + '/cover.tex', 'w', encoding='utf8').write(open('tools/cover.tex', encoding='utf8').read().replace('ROOT', ROOT))
tex = head + '\n'.join(body) + '\n\\end{multicols*}\n\\end{document}\n'
open(build + '/teambook.tex', 'w', encoding='utf8').write(tex)
print('rows in book:', len(rows))
if '--no-pdf' in sys.argv: sys.exit(0)
for _ in range(2):
    r = subprocess.run(['pdflatex', '-interaction=nonstopmode', '-halt-on-error', 'teambook.tex'], cwd=build, capture_output=True, text=True, errors='replace')
    if r.returncode != 0:
        print(r.stdout[-3000:]); sys.exit(1)
pages = re.search(r'Output written on teambook.pdf \((\d+) pages', r.stdout)
print('pages:', pages.group(1) if pages else '?')
os.makedirs('output/pdf', exist_ok=True)
shutil.copy(build + '/teambook.pdf', 'output/pdf/algo-teambook.pdf')
shutil.copy(build + '/teambook.tex', 'output/pdf/algo-teambook.tex')
