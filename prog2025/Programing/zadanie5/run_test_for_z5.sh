#!/bin/bash

EXEC="./z5_code/z5" # Path to the executable file
TEST_DIR="./z5_testing" # Path to tests dir
JSON_TXT="json_txt" # Path to json_txt dir

for scenario in "$TEST_DIR"/cmd/s*/; do
    scenario_name=$(basename "$scenario")
    echo "$scenario_name:"

    for infile in "$scenario"example*.txt; do
        testnum=$(basename "$infile" | sed -E 's/example(.*)\.txt/\1/')
        instruction_file="$TEST_DIR/cmd/$scenario_name/example$testnum.txt"
        expected_outfile="$TEST_DIR/output/$scenario_name/example$testnum.txt"



        if [ ! -f "$instruction_file" ]; then
            printf "   -- %-20s ❌ The expected instruction file is missing\n" "$instruction_file"
            continue
        fi

        if [ ! -f "$expected_outfile" ]; then
            printf "   -- %-20s ❌ The expected output file is missing\n" "$testnum"
            continue
        fi


        command=$(grep "^Spustenie:" "$instruction_file" | sed 's/Spustenie: //' | sed 's/z5\.exe//') # Remuve .exe
        command=$(echo "$command" | sed "s#txt/#$JSON_TXT/txt/#g")

        full_command="$EXEC ${command# }"


        json_file=$(grep "^Databaza predmetov:" "$instruction_file" | sed 's/Databaza predmetov: //' | sed 's/ (.*)//')
        if [[ "$json_file" =~ \.json$ ]]; then
            path_to_json_file="$JSON_TXT/json/$json_file"
            cp "$path_to_json_file" "$(dirname "$EXEC")/items.json"
        fi

        $full_command > temp_output.txt

        if diff temp_output.txt "$expected_outfile" --ignore-trailing-space >/dev/null; then
            printf "   -- %-20s ✅ Done\n" "$testnum"
        else
            printf "   -- %-20s ❌ False\n" "example_$testnum"
        fi
    done
done

rm -f "$(dirname "$EXEC")/items.json"
rm -f temp_output.txt
