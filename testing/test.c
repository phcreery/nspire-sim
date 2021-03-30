
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIST_LENGTH 20

// char *console_history_buffer[24] = {
//     "First entry", "Second entry", "Third entry", "4 entry", "5 entry",
//     "6 entry",     "7 entry",      "8 entry",     "9 entry", "10 entry",

// };
// const char *console_history_buffer[24];
char console_history_buffer[24][1024];

void print_history() {
    for (int i = 0; i < HIST_LENGTH; i++) {
        printf("%d: %s\n", i, console_history_buffer[i]);
    }
}

// void populate_history() {
//     for (int i = 0; i < HIST_LENGTH; i++) {
//         // form_values[i] = (char *)calloc(100, sizeof(char *));
//         console_history_buffer[i] = (char *)malloc(1024);
//     }
// }

void get_history(char *strings[]) {  //
    // return console_history_buffer;
    // ^^ bad practice: char **get_history(){} with char** temp = get_history();
    for (int i = 0; i < HIST_LENGTH; i++) {
        strings[i] = console_history_buffer[i];
    }
}

void history_append(char *string) {
    // shift right and add to 0
    for (int k = HIST_LENGTH; k > 0; k--) {
        // console_history_buffer[k] = console_history_buffer[k - 1];
        strcpy(console_history_buffer[k], console_history_buffer[k - 1]);
    }
    // console_history_buffer[0] = string;
    strcpy(console_history_buffer[0], string);
}

int main() {
    printf("\nA sample C program\n\n");
    // print_history();
    // populate_history();
    print_history();
    char *new = "NEW";
    history_append(new);
    print_history();

    // char *temp = get_history();
    // for (int i = 0; i < 20; i++) {
    //     printf("%d: %s\n", i, temp[i]);
    // }

    char *hist[20];
    get_history(hist);
    for (int i = 0; i < 20; i++) {
        printf("%d: %s\n", i, hist[i]);
        // screen_draw_page_text(&page1, hist[i]);
    }

    return 0;
}