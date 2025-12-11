#!/bin/bash

OUTPUT="ardan"
gcc -o "$OUTPUT" engine/*.c engine/io/*.c engine/termicast/*.c engine/util/*.c main.c -lm -lpng

if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT created."
else
    echo "Build failed."
fi