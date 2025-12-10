#!/bin/bash

OUTPUT="ardan"
gcc -o "$OUTPUT" src/*.c main.c -lm -lpng

if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT created."
else
    echo "Build failed."
fi
