build:
    mkdir -p bin
    gcc -Wall -Wextra src/*.c -o bin/tokenize

run *args:
    bin/tokenize {{args}}

debug *args:
    mkdir -p bin
    gcc -g src/*.c -o bin/tokenize
    gdb -tui bin/tokenize -ex "b main" -ex "r {{args}}"
