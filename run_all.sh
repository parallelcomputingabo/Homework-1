#!/bin/bash

all_passed=true

for i in {0..9}
do
    echo "Running case $i..."
    ./matmul $i
    if [ $? -ne 0 ]; then
        echo "Test case $i failed"
        all_passed=false
    fi
    echo "--------------------------------------"
done

if $all_passed; then
    echo "All test cases passed."
else
    echo "Some test cases failed."
fi
