#include <stdio.h>

#define HIST_LENGTH 20

char *console_history_buffer[20] = {
    "First entry", "Second entry", "Third entry", "4 entry", "5 entry",
    "6 entry",     "7 entry",      "8 entry",     "9 entry", "10 entry",

};
;

// void fill_array() {}

void print_array() {
    int i;

    for (i = 0; i < 20; i++) {
        printf("%d: %s\n", i, console_history_buffer[i]);
    }
}

char **get_history() {  //
    return console_history_buffer;
}

void history_append(char *string) {
    // shift right and add to 0
    for (int k = HIST_LENGTH; k > 0; k--) {
        console_history_buffer[k] = console_history_buffer[k - 1];
    }
    console_history_buffer[0] = string;
}

int main() {
    printf("\nA sample C program\n\n");
    print_array();
    // fill_array();
    print_array();
    history_append("NEW");
    print_array();
    char **temp = get_history();
    for (int i = 0; i < 20; i++) {
        printf("%d: %s\n", i, temp[i]);
    }

    return 0;
}