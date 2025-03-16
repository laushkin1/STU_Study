#!/bin/bash

EXEC="../../../.run/z1"
TEST_DIR="./tests_z1"

for scenario in "$TEST_DIR"/s*/; do
    scenario_name=$(basename "$scenario")
    echo "$scenario_name:"

    for infile in "$scenario"/i*.txt; do
        testnum=$(basename "$infile" | grep -oE '[0-9]+')
        expected_outfile="$scenario/o$testnum.txt"

        if [ ! -f "$expected_outfile" ]; then
            echo "   -- $testnum. ❌ The expected output file is missing"
            continue
        fi

        $EXEC < "$infile" > temp_output.txt

        if diff -q --strip-trailing-cr temp_output.txt "$expected_outfile" >/dev/null; then
            echo "   -- $testnum. ✅ Done"
        else
            echo "   -- $testnum. ❌ False"
        fi
    done
done

rm -f temp_output.txt

