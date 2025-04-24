#!/bin/bash

EXEC="./z5_code/z5git " # Path to the executable file
TEST_DIR="./z5_testing" # Path to tests dir
JSON_TXT="json_txt" # Path to json_txt dir
DEBAG_MODE=0; # Change to 1 to turn on debag mode

for scenario in "$TEST_DIR"/cmd/s*/; do
    scenario_name=$(basename "$scenario")
    echo "$scenario_name:"

    for infile in "$scenario"example*.txt; do
        testnum=$(basename "$infile" | sed -E 's/example(.*)\.txt/\1/')
        instruction_file="$TEST_DIR/cmd/$scenario_name/example$testnum.txt"
        expected_outfile="$TEST_DIR/output/$scenario_name/example$testnum.txt"


        if [ ! -f "$instruction_file" ]; then
            printf "   -- %-20s ❌ Missing instruction file\n" "$testnum"
            continue
        fi
        if [ ! -f "$expected_outfile" ]; then
            printf "   -- %-20s ❌ Missing expected output\n" "$testnum"
            continue
        fi


        command=$(grep "^Spustenie:" "$instruction_file" \
                | sed 's/Spustenie: //' | sed 's/z5\.exe//') # Remuve .exe
        command=$(echo "$command" | sed "s#txt/#$JSON_TXT/txt/#g")

        full_command=($EXEC $command)


        json_file=$(grep "^Databaza predmetov:" "$instruction_file" \
                    | sed 's/Databaza predmetov: //' | sed 's/ .*//')
        if [[ "$json_file" =~ \.json$ ]]; then
            cp "$JSON_TXT/json/$json_file" "./items.json"
        fi

        # $full_command > temp_output.txt

        if [[ "$scenario_name" == "s1" || "$scenario_name" == "s5" ]]; then
            "${full_command[@]}" > temp_output.txt 2>/dev/null
        else
            "${full_command[@]}" > /dev/null 2> temp_output.txt
        fi
        

        if diff temp_output.txt "$expected_outfile" \
            --ignore-trailing-space --strip-trailing-cr >/dev/null; then
                printf "   -- %-20s ✅ Done\n" "$testnum"
        else
            
            if (( DEBAG_MODE )); then
                printf "   -- %-20s ❌ False\n" "example_$testnum"
                echo "------------------------------"
                printf "\e[31m"
                printf "Your output:\n"
                cat temp_output.txt
                printf "\e[32m"
                printf "Correct output:\n"
                cat $expected_outfile
                echo "------------------------------"
                printf "\e[37m\n\n"
            else
                printf "   -- %-20s ❌ False\n" "example_$testnum"
            fi
            
        fi
    done
done

rm -f "./items.json"
rm -f temp_output.txt
