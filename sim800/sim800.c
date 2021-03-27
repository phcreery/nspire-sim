#include "sim800.h"

#include <nspireio/nspireio.h>
#include <os.h>
#include <string.h>

int last_stat = 0;
int com_lock = 0;

#define HIST_LENGTH 20
char *console_history_buffer[24];

void print_history() {
    for (int i = 0; i < HIST_LENGTH; i++) {
        uart_printf("%d: %s\n", i, console_history_buffer[i]);
    }
}

void populate_history() {
    for (int i = 0; i < HIST_LENGTH; i++) {
        // form_values[i] = (char *)calloc(100, sizeof(char *));
        console_history_buffer[i] = (char *)malloc(100);
    }
}

void get_history(char *strings[]) {  //
    // return console_history_buffer;
    // ^^ bad practice: char **get_history(){} with char** temp = get_history();
    for (int i = 0; i < HIST_LENGTH; i++) {
        strings[i] = console_history_buffer[i];
    }
}

void history_append(char *string) {
    // shift right and add to 0
    for (int k = HIST_LENGTH - 1; k > 0; k--) {
        console_history_buffer[k] = console_history_buffer[k - 1];
    }
    console_history_buffer[0] = string;
}

char *sim_request(char *command) {
    history_append(command);

    char response[20];
    // response = await_serial_rec(command);
    strcpy(response, "__AT__OK__\0");
    // history_append(response);

    char *str = malloc(20 * sizeof(char));
    strcpy(str, response);
    history_append(str);
    // print_history();
    return str;
}

int comp_request(char command[], char expect[]) {
    // char command[] = "AT?\0";
    char *response = sim_request(command);
    if (strcmp(response, expect) == 0) {
        // uart_printf("response is okay.\n");
        free(response);
        return 1;
    }
    free(response);
    return 0;
}

int sim_is_ok() { return comp_request("AT\0", "__AT__OK__\0"); }
int sim_get_batt() { return 0; }
int sim_get_sig_strength() { return 0; }

// https://stackoverflow.com/questions/14416759/return-char-string-from-a-function/14416798
// although this works, I find the following to work better. maybe not easier,
// but when called repeately, memory issues
// char *sim_get_conn() {
//     char *str = malloc(10 * sizeof(char));
//     strcpy(str, "NO-CONN\0");
//     return str;
// }

void sim_get_conn(char *str) {  //
    strcpy(str, "NO-CONN\0");
}

void sim_get_status(struct simstatus *ss) {
    static int at;
    static int batt;
    static int st;
    static char conn[20];
    at = sim_is_ok();
    batt = sim_get_batt();
    st = sim_get_sig_strength();
    sim_get_conn(conn);
    ss->at = at;
    ss->last_stat = last_stat;
    ss->batt = batt;
    ss->sig_strength = st;
    ss->sig_conn = conn;
}

// https://github.com/cristiansteib/Sim800l/blob/master/Sim800l.cpp
// returns 0 for processing, 1 for success, -1 for error?
// see https://github.com/carrascoacd/ArduinoSIM800L/blob/master/src/Result.h
// for better implimentation
int sim_send_text() {
    static bool is_sending = 0;
    static bool step = 0;
    // uart_printf("a\n");

    if (is_sending == 0) {
        // uart_printf("b0\n");
        // populate_history();
        // msleep(1000);
        // pass, let screen update?
        is_sending = 1;
        return 0;
    } else if (is_sending == 1) {
        if (step == 0) {
            // uart_printf("b1 Step 0\n");
            if (sim_is_ok() == 1) {
                // uart_printf("response is really okay.\n");
                is_sending = 0;
                return 1;
            } else {
                return 0;
            }
        } else if (step > 0) {
            return 2;
        }
    }

    return 0;
}

// https://github.com/carrascoacd/ArduinoSIM800L
void sim_get_http() {}
