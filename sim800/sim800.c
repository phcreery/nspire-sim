#include "sim800.h"

#include <nspireio/nspireio.h>
#include <os.h>
#include <string.h>

#include "serial.h"

int last_stat = 0;
int com_lock = 0;

#define HIST_LENGTH 20

// https://stackoverflow.com/questions/1088622/how-do-i-create-an-array-of-strings-in-c
// char *console_history_buffer[24]; // const??
char console_history_buffer[24][1024];

void print_history() {
    for (int i = 0; i < HIST_LENGTH; i++) {
        uart_printf("%d: %s\n", i, console_history_buffer[i]);
    }
}

// discarded after issues with pointer being overwritten
// https://github.com/phcreery/nspire-sim/commit/e6670da922716528b6ca090e492e8948a3f2d361
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
    for (int k = HIST_LENGTH - 1; k > 0; k--) {
        // console_history_buffer[k] = console_history_buffer[k - 1];
        strcpy(console_history_buffer[k], console_history_buffer[k - 1]);
    }
    // console_history_buffer[0] = string;
    strcpy(console_history_buffer[0], string);
}

int is_OK(char response[]) {
    if (strstr(response, "____OK") != NULL) {
        return 1;
    }
    return 0;  // else
}

// char *serial_request(char *command, char *str)
char *sim_request(char *command, char *response) {
    history_append(command);
    serial_request(command, response);
    // strcpy(response, "__AT__OK__\0");
    history_append(response);
    last_stat = is_OK(response);
    // print_history();
    return response;
}

void sim_send(char *command) {
    history_append(command);
    serial_send(command);
}

int comp_request(char command[], char expect[]) {
    char response[1024] = {0};
    sim_request(command, response);
    if (strcmp(response, expect) == 0) {
        return 1;
    }
    return 0;
}

int at_is_ok() {
    char response[1024] = {0};
    sim_request("AT\r", response);
    return is_OK(response);
}

int sim_is_ok() {
    char response[1024] = {0};
    sim_request("AT+CPIN?\r", response);
    if (strstr(response, "READY") != NULL) {
        return 1;
    }
    return 0;
}
int sim_get_batt() {
    char response[1024] = {0};
    sim_request("AT+CBC\r", response);

    char find = ':';
    const char *ptr = strchr(response, find);
    if (ptr) {
        int index = ptr - response;
        const int stringLen = strlen(response);
        char currentChar;
        char stripped[128] = {0};
        int j = 0;
        for (int i = 0; i < stringLen; ++i) {
            currentChar = response[index + 4 + i];
            if ((currentChar >= '0') && (currentChar <= '9')) {
                stripped[j++] = currentChar;
            } else {
                break;
            }
        }
        // int x = response[index + 2] - '0';
        int x = atoi(stripped);
        return x;
    }
    return 0;
}
int sim_get_sig_strength() {
    char response[1024] = {0};
    sim_request("AT+CSQ\r", response);  // __at+csq___+CSQ: 7,0____OK

    char find = ':';
    const char *ptr = strchr(response, find);
    if (ptr) {
        int index = ptr - response;
        const int stringLen = strlen(response);
        char currentChar;
        char stripped[128] = {0};
        int j = 0;
        for (int i = 0; i < stringLen; ++i) {
            currentChar = response[index + 2 + i];
            if ((currentChar >= '0') && (currentChar <= '9')) {
                stripped[j++] = currentChar;
            } else {
                break;
            }
        }
        // int x = response[index + 2] - '0';
        int x = atoi(stripped);
        return x;
    }
    return 0;
}

// https://stackoverflow.com/questions/14416759/return-char-string-from-a-function/14416798
void sim_get_conn(char *str) {  //
    char response[1024] = {0};
    char foundstr[20] = {0};
    sim_request("AT+COPS?\r", response);
    // find word between quotes
    if (sscanf(response, "%*[^\"]\"%[^\"]\"", foundstr) == 1) {  // found it
        strcpy(str, foundstr);
    } else {
        strcpy(str, "NO-CONN\0");
    }
}

void sim_get_status(struct simstatus *ss) {
    static int at;
    static int sim;
    static int batt;
    static int st;
    static char conn[20];
    at = at_is_ok();
    sim = sim_is_ok();
    batt = sim_get_batt();
    st = sim_get_sig_strength();
    sim_get_conn(conn);
    ss->at = at;
    ss->sim = sim;
    ss->last_stat = last_stat;
    ss->batt = batt;
    ss->sig_strength = st;
    ss->sig_conn = conn;
}

// https://github.com/cristiansteib/Sim800l/blob/master/Sim800l.cpp
// returns 0 for error, 1 for success, 2 for processing?
// see https://github.com/carrascoacd/ArduinoSIM800L/blob/master/src/Result.h
// for better implimentation
enum Result sim_send_text(char number[], char message[]) {
    static bool is_sending = 0;
    static int step = 0;

    if (is_sending == 0) {
        is_sending = 1;
        step = 0;
        return 2;
    } else if (is_sending == 1) {
        if (step == 0) {
            char response[1024] = {0};
            sim_request("AT+CMGF=1\r", response);
            if (last_stat == 1) {
                step++;
                return WORKING;
            } else {
                return ERROR;
            }
        } else if (step == 1) {
            char request[1024] = {0};
            strcpy(request, "AT+CMGS=\"");
            strcat(request, number);
            strcat(request, "\"\r");
            sim_send(request);
            step++;
            return WORKING;
        } else if (step == 2) {
            sim_send(message);      // send message
            uart_printf("%c", 26);  // ctrl+z
            step++;
            return WORKING;
        } else if (step >= 3) {
            if (at_is_ok() == 1) {  // wait for AT OK
                step = 0;
                is_sending = 0;
                return SUCCESS;
            }
        }
    }

    return UNKNOWN;
}

// https://github.com/carrascoacd/ArduinoSIM800L
void sim_get_http() {}
