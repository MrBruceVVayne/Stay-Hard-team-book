#!/bin/bash
# Стресс-тест: ./stress.sh sol brute gen   (исходники sol.cpp brute.cpp gen.cpp, gen принимает seed в argv[1])
set -e
for f in "$@"; do g++ -std=c++20 -O2 -Wall -Wextra -Wshadow -D_GLIBCXX_DEBUG -fsanitize=address,undefined "$f.cpp" -o "$f"; done
for ((i = 1; ; i++)); do
        ./$3 $i > in
        ./$1 < in > out1
        ./$2 < in > out2
        diff -w out1 out2 > /dev/null || { echo "WA on test $i"; break; }
        echo "ok $i"
done
