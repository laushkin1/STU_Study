#!/bin/bash

EXEC="../../../.run/z3" # Path to the executable file
TEST_DIR="z3_testing_extra" # Path to tests dir

for scenario in "$TEST_DIR"/stdin/scenar*/; do
    scenario_name=$(basename "$scenario")
    echo "$scenario_name:"

    for infile in "$scenario"example_*.txt; do
        testnum=$(basename "$infile" | sed -E 's/example_(.*)\.txt/\1/')
        expected_outfile="$TEST_DIR/stdout/$scenario_name/example_$testnum.txt"

        if [ ! -f "$expected_outfile" ]; then
            printf "   -- %-20s ❌ The expected output file is missing\n" "$testnum"
            continue
        fi

        $EXEC < "$infile" > temp_output.txt

        if diff temp_output.txt "$expected_outfile" --ignore-trailing-space >/dev/null; then
            printf "   -- %-20s ✅ Done\n" "$testnum"
        else
            printf "   -- %-20s ❌ False\n" "example_$testnum"
        fi
    done
done

rm -f temp_output.txt
