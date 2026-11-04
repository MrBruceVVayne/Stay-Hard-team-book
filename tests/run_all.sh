#!/bin/bash
# Запуск всех стресс-тестов: ./tests/run_all.sh [имя1 имя2 ...]
# Каждый tests/*.cpp компилируется с sanitizer'ами и гоняется на случайных данных.
# Семя печатается при падении через TEST_SEED (по умолчанию случайное).
cd "$(dirname "$0")/.." || exit 1
mkdir -p build
CXX=${CXX:-g++}
FLAGS="-std=c++20 -O1 -g -Wall -Wextra -Wshadow -Wno-unused-parameter -Wno-return-type -Wno-sign-compare -D_GLIBCXX_DEBUG -fsanitize=address,undefined -fno-sanitize-recover=undefined -I. -Itests"
if [ $# -gt 0 ]; then names=("$@"); else names=($(ls tests/*.cpp | xargs -n1 basename | sed 's/\.cpp$//')); fi
run_one() {
        n=$1
        if ! $CXX $FLAGS "tests/$n.cpp" -o "build/$n" 2> "build/$n.cerr"; then
                echo "COMPILE FAIL  $n"; head -20 "build/$n.cerr"; return 1
        fi
        if timeout ${TEST_TIMEOUT:-120} "build/$n" > "build/$n.out" 2> "build/$n.err"; then
                echo "ok            $n"
        else
                echo "FAIL          $n"; head -20 "build/$n.err"; head -5 "build/$n.out"; return 1
        fi
}
export -f run_one; export CXX FLAGS
printf '%s\n' "${names[@]}" | xargs -P ${JOBS:-4} -I{} bash -c 'run_one {}' | tee build/summary.txt
fail=$(grep -c -E "^(FAIL  |COMPILE FAIL)" build/summary.txt)
echo "----"; echo "tests: ${#names[@]}, failed: $fail"
# факты (python)
if [ $# -eq 0 ]; then
        for f in tests/facts/*.py; do
                [ -f "$f" ] || continue
                if python3 -I "$f" > "build/$(basename $f).out" 2>&1; then echo "ok            facts/$(basename $f)"
                else echo "FAIL          facts/$(basename $f)"; tail -5 "build/$(basename $f).out"; fail=$((fail+1)); fi
        done
fi
[ "$fail" -eq 0 ]
