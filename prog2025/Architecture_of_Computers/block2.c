#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void ul1_1();       // 1 Done
void ul1_2();       // 1
void ul1_3();       // 3
void ul1_4();       // 2
void ul2();         // 3 Done
void ul3_2();       // 1
void ul5_1();       // 1
void ul6();         // 4

int add_assembler(int input1, int input2, int output);
int multiply_by_two_assembler(int num);
int convert_assembler(int num);
void print_num_list(int* num, int len);
void decrypt_file(const char *input_filename, const char *output_filename);


int main()
{
    // ul1_1();
    // ul1_2();
    // ul1_3();
    // ul1_4();
    // ul2();
    // ul3_2(); // terminal in .run folder
    // ul5_1();
    // ul6(); // terminal in .run folder

    return 0;
}


void ul1_1()
{
    int first_number;
    int second_number;
    int output = 0;
    printf("Enter the first number: ");
    scanf("%d", &first_number);
    printf("Enter the second number: ");
    scanf("%d", &second_number);
    output = add_assembler(first_number, second_number, output);
    printf("%d+%d = %d\n", first_number, second_number, output);
}

void ul1_2()
{
    int number = 5;
    printf("%d -> %d\n", number, multiply_by_two_assembler(number));
}

void ul1_3()
{
    for (int i = 0; i <= 10; i++)
    {
        printf("%2d -> '%c'\n", i, convert_assembler(i));
    }
}

void ul1_4()
{
    char buffer[256];
    FILE *fp = popen("sysctl -n machdep.cpu.brand_string", "r"); // popen make shell command
    if (fp == NULL) {
        printf("Unexplained ERROR\n");
        return;
    }

    fgets(buffer, sizeof(buffer), fp); // Put stream fp to buffer
    printf("Processor: %s", buffer);

    fclose(fp);
}

void ul2()
{
    char pole[] = "architektura_pocitacov_je_super_predmet";
    char* pointer = pole;
    printf("%s\n", pole);
    printf("%p\n", pointer);
    pointer++;
    *pointer = 'm';
    printf("%s\n", pole);

    printf("\n");

    int numbers[] = {10, 20, 30, 40, 50, 0};
    int len_num = sizeof(numbers)/sizeof(numbers[0]);
    int* pointer_for_num = numbers;

    print_num_list(numbers, len_num);

    printf("%p\n", pointer_for_num);
    pointer_for_num++;
    *pointer_for_num = 10;

    print_num_list(numbers, len_num);
}

void ul3_2()
{
    char* inp_f = "../prog2025/Architecture_of_Computers/koniec_poviedky.html";
    char* out_f = "../prog2025/Architecture_of_Computers/decrypted.html";
    decrypt_file(inp_f, out_f);
}

void ul5_1()
{
    int RED = 0;
    int GREEN = 0;
    int BLUE = 0;

    for (int i = 1; i <= 10; i++)
    {
        RED = 255; GREEN = 0; BLUE = 0;
        if (i>2) {RED = 255; GREEN = 255; BLUE = 0;}
        if (i>4) {RED = 0; GREEN = 255; BLUE = 0;}
        if (i>6) {RED = 0; GREEN = 255; BLUE = 255;}
        if (i>8) {RED = 0; GREEN = 0; BLUE = 255;}
        sleep(1);
        printf("\x1b[38;2;%d;%d;%dm %003d \x1b[0m\n", RED, GREEN, BLUE, i);
    }
}

void ul6()
{
    initscr();              // Initializing ncurses
    keypad(stdscr, TRUE);   // Enable function key processing
    curs_set(0);            // Remove blinking cursor
    noecho();               // Disable character output to the screen
    cbreak();               // Disable input buffering
    nodelay(stdscr, TRUE);  // Not wait for input, but return ERR if the key is not pressed.

    start_color();          // Initializing colorfull text
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Green Text


    int button;
    int random_letter = 0;
    time_t start_time, end_time, current_time;
    double elapsed;
    bool run_timer = false;

    mvprintw(LINES/2, COLS/2-(strlen("Press F3 for instruction")/2), "Press F3 for instruction");
    while(1)
    {
        button = getch();

        if (button == 27 || button == KEY_F(10))
            break;

        if (button == KEY_F(3))
        {
            clear();
            run_timer = false;
            printw("###################################################################\n");
            printw("#                                                                 #\n");
            printw("# Instruction:                                                    #\n");
            printw("#  - Press F3 to view these instructions.                         #\n");
            printw("#  - Press F4 to start the timer.                                 #\n");
            printw("#  - Tap the same letter as on the screen to stop the timer.      #\n");
            printw("#  - Press F10 or ESC to end the program.                         #\n");
            printw("#                                                                 #\n");
            printw("###################################################################\n");
        }

        if (button == KEY_F(4))
        {
            clear();
            random_letter = 'A' + rand() % 26;

            srand(time(NULL));
            mvprintw(LINES/2, COLS/2, "%c", random_letter);
            refresh();

            time(&start_time);
            run_timer = true;
        }

        if (run_timer)
        {
            time(&current_time);
            elapsed = difftime(current_time, start_time);
            mvprintw(LINES/2+2, COLS/2-(strlen("Time: 999 seconds")/2), "Time: %.2f seconds", elapsed);
            refresh();
            usleep(50000);
        }

        if (button == random_letter || button == random_letter + 32)
        {
            run_timer = false;
            clear();

            time(&end_time);
            elapsed = difftime(end_time, start_time);
            mvprintw(LINES/2, COLS/2, "%c", random_letter);
            attron(COLOR_PAIR(2));
            mvprintw(LINES/2+2, COLS/2-(strlen("Time: 999 seconds")/2), "Time: %.2f seconds", elapsed);
            attroff(COLOR_PAIR(2));
            refresh();
            random_letter = 0;
        }
    }

    endwin(); // End of ncurses
}


int add_assembler(int input1, int input2, int output)
{
#if defined(__aarch64__)
    __asm__(
        "ldr w0, %[input1]\n"
        "ldr w1, %[input2]\n"
        "add w2, w0, w1\n"
        "str w2, %[output]\n"
        : [output] "=m" (output)
        : [input1] "m" (input1), [input2] "m" (input2)
        : "w0", "w1", "w2"
    );
#endif
    return output;
}

int multiply_by_two_assembler(int num) {
    int result;
#if defined(__aarch64__)
    __asm__(
        "lsl %w[result], %w[input], #1\n" // Shift bits left by 1 -> multiplication equivalent of 2
        : [result] "=r" (result)
        : [input] "r" (num)
    );
#endif
    return result;
}

int convert_assembler(int num)
{
    char output;
#if defined(__aarch64__)
    __asm__(
        "ldr w0, %[num]\n"
        "cmp w0, #10\n"
        "add w1, w0, '0'\n"
        "b.lt 1f\n"

        "mov w2, #55\n"
        "add w1, w0, w2\n"

    "1:\n"
        "strb w1, %[output]\n"
        : [output] "=m" (output)
        : [num] "m" (num)
        : "w0", "w1", "w2"
    );
#endif
    return output;
}

void print_num_list(int* num, const int len)
{
    for (int i = 0; i<len; i++)
        printf("%d ", num[i]);
    printf("\n");
}

void decrypt_file(const char *input_filename, const char *output_filename)
{
    FILE* encrypted = fopen(input_filename, "rb");
    if (encrypted == NULL)
    {
        printf("File could not be opened\n");
        return;
    }

    FILE* decrypted = fopen(output_filename, "wb");
    if (decrypted == NULL)
    {
        printf("Error opening file\n");
        fclose(encrypted);
        return;
    }

    int ch;
    while ((ch = fgetc(encrypted)) != EOF)
    {
        ch = ch ^ 27;
        printf("%c", ch);
        fputc(ch, decrypted);
    }

    fclose(encrypted);
    fclose(decrypted);
}
