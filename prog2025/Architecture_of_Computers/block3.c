#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/ioctl.h>


#define MY_ID "135682"

#define COLOR_GREEN   "\033[0;32m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_RESET   "\033[0m"

#define SLEEP_TIME 5


void error(const char *msg);
bool is_prime(int num);
int get_terminal_width();

void print_aligned(const char *msg, int left_padding, int max_width, char *color);
void print_user(const char *msg);
void print_server(const char *msg);

char *decrypt_xor55_to_string_bin(const unsigned char *encrypted, int len);
int compute_result(const char *id);
char* extract_primes_only(const unsigned char* input);

int parser(const char* input);
void log_in_file(FILE *f, char* message, bool is_server);
char* get_name(unsigned char *bin_buffer);



int main() {
    const char *hostname = "147.175.115.34";
    int port = 777;

    int my_socket;
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket < 0) error("ERROR opening socket");

    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(my_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    print_server("Connected to server...");

    char *client_name = "Client";
    // First message
    unsigned char bin_buffer[1024];
    strcpy((char *)bin_buffer, "Hello Server!");

    char response[100];
    snprintf(response, sizeof(response), "%s: Hello Server!", client_name);
    print_user(response);

    write(my_socket, bin_buffer, strlen((char *)bin_buffer));

    FILE *log_file = fopen("log.txt", "a"); // w
    if (!log_file) {
        perror("ERROR opening log file");
        exit(1);
    }

    int num_bytes;
    char input_buffer[1024];

    char *decrypted = NULL;

    while (1) {
        // Read message from server
        bzero(bin_buffer, sizeof(bin_buffer));
        num_bytes = read(my_socket, bin_buffer, sizeof(bin_buffer)-1);
        if (num_bytes < 0) error("ERROR reading from socket");
        if (num_bytes == 0) {
            printf("Server closed the connection.\n");
            break;
        }
        bin_buffer[num_bytes] = '\0';

        log_in_file(log_file, (char*)bin_buffer, true);

        print_server((char *)bin_buffer);
        printf("\n");

        // Hanldling commands
        int serverResponse = parser((char*)bin_buffer);
        
        
        if (serverResponse == 1){
            // Send ID
            char response[100];
            snprintf(response, sizeof(response), "%s: %s", client_name, MY_ID);
            print_user(response);
            
            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "%s", MY_ID);
        }
        else if (serverResponse == 2)
        {
            client_name = get_name(bin_buffer);

            char response[100];
            snprintf(response, sizeof(response), "%s: What?", client_name);
            print_user(response);
            
            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "What?");
        }
        else if (serverResponse == 3) {
            // checking for code
            unsigned long code = 0;
            char *codeComand = strstr((char*)bin_buffer, " code ");
            if (sscanf(codeComand, " code %lu.", &code) == 1) {
                snprintf(input_buffer, sizeof(input_buffer), "%lu", code);
                char response[128];
                snprintf(response, sizeof(response), "%s: %lu", client_name, code);
                print_user(response);

                log_in_file(log_file, response, false);

                snprintf(input_buffer, sizeof(input_buffer), "%lu", code);
            } else {goto ELSE_BLOCK;}
        }
        else if (serverResponse == 4)
        {
            // Sum of the first 5 digits of my ID
            int result = compute_result(MY_ID);

            char response[100];
            snprintf(response, sizeof(response), "%s: %d", client_name, result);
            print_user(response);
            
            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "%d", result);
        }
        else if (num_bytes == 131)
        {
            // Message that should be decrypted
            decrypted = decrypt_xor55_to_string_bin(bin_buffer, num_bytes);

            char response[num_bytes+20];
            snprintf(response, sizeof(response), "%s: %s\n", client_name, decrypted);
            print_user(response);

            log_in_file(log_file, decrypted, false);

            snprintf(input_buffer, sizeof(input_buffer), "%s", decrypted);
        }
        else if (serverResponse == 5)
        {
            // First coordinate
            char *pointer = strstr(decrypted, "coordinates ");
            if (!pointer) {goto ELSE_BLOCK;}
            pointer += strlen("coordinates ");

            double firstCoordinate;
            if (sscanf(pointer, "%lf,", &firstCoordinate) != 1) {
                goto ELSE_BLOCK;
            }

            char response[100];
            snprintf(response, sizeof(response), "%s: %lf", client_name, firstCoordinate);
            print_user(response);
            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "%lf", firstCoordinate);
        }
        else if (serverResponse == 6)
        {
            // Second coordinate
            char *pointer = strstr(decrypted, "coordinates ");
            if (!pointer) {goto ELSE_BLOCK;}
            pointer += strlen("coordinates ");

            double firstCoordinate, secondCoordinate;
            if (sscanf(pointer, "%lf, %lf", &firstCoordinate, &secondCoordinate) != 2) {
                goto ELSE_BLOCK;
            }

            char response[100];
            snprintf(response, sizeof(response), "%s: %lf", client_name, secondCoordinate);
            print_user(response);
            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "%lf", secondCoordinate);
        }
        else if (serverResponse == 7)
        {
            // Send abbreviation
            char response[100];
            snprintf(response, sizeof(response), "%s: E.T.", client_name);
            print_user(response);

            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "E.T.");
        }
        else if (serverResponse == 8)
        {
            // send PRIMENUMBER (key word)
            char response[100];
            snprintf(response, sizeof(response), "%s: PRIMENUMBER", client_name);
            print_user(response);

            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "PRIMENUMBER");
        }
        else if (num_bytes == 23)
        {
            char *extracted = extract_primes_only(bin_buffer);

            char response[1024];
            snprintf(response, sizeof(response), "%s: %s", client_name, extracted);
            print_user(response);

            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "%s", extracted);
        }
        else if (serverResponse == 9)
        {
            // Send Trinity
            char response[100];
            snprintf(response, sizeof(response), "%s: Trinity", client_name);
            print_user(response);
    
            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "Trinity");
        }
        else if (serverResponse == 10)
        {
            // Send first answer
            char response[100];
            snprintf(response, sizeof(response), "%s: polyadicke", client_name);
            print_user(response);

            log_in_file(log_file, response, false);

            snprintf(input_buffer, sizeof(input_buffer), "polyadicke");
        }
        else if (serverResponse == 11)
        {
            // Send second answer
            char response[100];
            snprintf(response, sizeof(response), "%s: half-duplex", client_name);
            print_user(response);

            log_in_file(log_file, response, false);
            
            snprintf(input_buffer, sizeof(input_buffer), "half-duplex");
        }
        else if (serverResponse == 0)
        {
            // quit
            char response[100];
            snprintf(response, sizeof(response), "%s: quit", client_name);
            print_user(response);

            log_in_file(log_file, response, false);

            printf("Exiting program by user command.\n");
            break;
        }
        else
        {
        ELSE_BLOCK:
            // User
            printf(COLOR_BLUE);
            printf("%s: ", client_name);
            bzero(input_buffer, 1024);
            fgets(input_buffer, 1023, stdin); // last byte for \0
            printf(COLOR_RESET);

            fprintf(log_file, "[CLIENT] %s", input_buffer);
            fflush(log_file);
        }

        num_bytes = write(my_socket, input_buffer, strlen(input_buffer));
        if (num_bytes < 0) error("ERROR writing extracted string to socket");
    }

    fclose(log_file);
    close(my_socket);

    return 0;
}


void error(const char *msg) {
    perror(msg);
    exit(1);
}

bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++)
        if (num % i == 0) return false;
    return true;
}

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col > 0 ? w.ws_col : 80;
}


void print_aligned(const char *msg, int left_padding, int max_width, char *color) {
    printf("%s", color);
    char line[1024];
    int line_len = 0;
    const char *ptr = msg;

    while (*ptr) {
        const char *word_start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\n') ptr++;
        int word_len = ptr - word_start;

        if (line_len + word_len + 1 > max_width) {
            line[line_len] = '\0';
            printf("%*s", left_padding, "");
            for (int i = 0; line[i] != '\0'; i++) {
                printf("%c", line[i]);
                fflush(stdout);
                usleep(SLEEP_TIME);
            }
            printf("\n");

            line_len = 0;
        }

        if (line_len != 0) line[line_len++] = ' ';
        memcpy(line + line_len, word_start, word_len);
        line_len += word_len;

        if (*ptr == ' ') ptr++;
        if (*ptr == '\n') {
            line[line_len] = '\0';
            printf("%*s", left_padding, "");
            for (int i = 0; line[i] != '\0'; i++) {
                printf("%c", line[i]);
                fflush(stdout);
                usleep(SLEEP_TIME);
            }
            printf("\n");
            line_len = 0;
            ptr++;
        }
    }

    if (line_len > 0) {
        line[line_len] = '\0';
        printf("%*s", left_padding, "");
        for (int i = 0; line[i] != '\0'; i++) {
            printf("%c", line[i]);
            fflush(stdout);
            usleep(SLEEP_TIME);
        }
        printf("\n");
    }
    printf(COLOR_RESET);
}

void print_user(const char *msg) {
    int width = get_terminal_width();
    int half = width / 2 - 1;
    print_aligned(msg, 0, half, COLOR_BLUE);
}

void print_server(const char *msg) {
    int width = get_terminal_width();
    int half = width / 2 + 1;
    int max_width = width - half;
    print_aligned(msg, half, max_width, COLOR_GREEN);
}


char *decrypt_xor55_to_string_bin(const unsigned char *encrypted, int len) {
    static char decrypted[1024];
    for (int i = 0; i < len; i++)
        decrypted[i] = encrypted[i] ^ 55;

    decrypted[len] = '\0';
    return decrypted;
}

int compute_result(const char *id) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        if (!isdigit(id[i])) { // Just in case
            fprintf(stderr, "Invalid ID format\n");
            return -1;
        }
        sum += id[i] - '0';
    }

    int fifth_digit = id[4] - '0';
    if (fifth_digit == 0) fifth_digit = 9; // Never in my id

    return sum % fifth_digit;
}

char* extract_primes_only(const unsigned char* input) {
    static char result[1024];
    int j = 0;

    for (int i = 0; i < 1024 && input[i] != '\0'; i++)
        if (is_prime(i+1)) result[j++] = (char)input[i];

    result[j] = '\0';
    return result;
}


int parser(const char* input){
    if (strstr((char*)input, "Send me your ID")) return 1;
    if (strstr((char*)input, "I need you to do something for me")) return 2;
    if (strstr((char*)input, " code ")) return 3;
    if (strstr((char*)input, "Compute the sum of first 5 digits of your student ID")) return 4;
    if (strstr((char*)input, "Send me the integral part of the first coordinate")) return 5;
    if (strstr((char*)input, "Send me the integral part of the second coordinate")) return 6;
    if (strstr((char*)input, "X.Y.")) return 7;
    if (strstr((char*)input, "PRIMENUMBER")) return 8;
    if (strstr((char*)input, "Then send me the string Trinity")) return 9;
    if (strstr((char*)input, "Ako nazyvame cislene sustavy, ktore mozeme rozvinut do mocninoveho radu")) return 10;
    if (strstr((char*)input, "Ako sa vola obojsmerna komunikacia, v pripade ze v jednej jednotke casu ide komunikacia iba jednym smerom")) return 11;
    if (strstr((char*)input, "BONUS")) return 0;
    return -1;
}

void log_in_file(FILE *f, char* message, bool is_server){
    if(is_server){
        fprintf(f, "[SERVER] %s\n", message);
        fflush(f);
    }
    else{
        fprintf(f, "[CLIENT] %s\n", message);
        fflush(f);
    }
}
    
char* get_name(unsigned char *bin_buffer) {
    static char default_name[] = "Client"; // becouse free
    
    char *start = strstr((char *)bin_buffer, "Hello ");
    if (start != NULL) {
        // skip hello
        start += 6;

        char *end = strstr(start, " I ");
        if (end != NULL) {
            int length = end - start;
            static char extracted[50];
            strncpy(extracted, start, length);
            extracted[length] = '\0';
            return extracted;
        }
    }
    
    return default_name;
}
