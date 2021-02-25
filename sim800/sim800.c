#include "sim800.h"

#include <nspireio/nspireio.h>
#include <os.h>
#include <string.h>

int last_stat = 0;
int com_lock = 0;

char *console_history_buffer[20];

void history_append() {}

char *sim_request(char *command) {
    // history_append(command);

    char response[20];
    // response = await_serial_rec(command);
    strcpy(response, "__AT__OK__");

    char *str = malloc(20 * sizeof(char));
    strcpy(str, response);
    return str;
}

int sim_is_ok() { return 0; }
int sim_get_batt() { return 0; }
int sim_get_sig_strength() { return 0; }

// https://stackoverflow.com/questions/14416759/return-char-string-from-a-function/14416798
char *sim_get_conn() {
    char *str = malloc(10 * sizeof(char));
    strcpy(str, "NO-CONN\0");
    return str;
}

void sim_get_status(struct simstatus *ss) {
    int at = sim_is_ok();
    int batt = sim_get_batt();
    int st = sim_get_sig_strength();
    char *conn = sim_get_conn();
    ss->at = at;
    ss->last_stat = last_stat;
    ss->batt = batt;
    ss->sig_strength = st;
    ss->sig_conn = conn;
    free(conn);
}

int sim_send_text() {  // returns 0 for processing, 1 for success, 2 for error?
    static bool is_sending = 0;
    uart_printf("a\n");

    if (is_sending == 0) {
        uart_printf("b0\n");
        // send command
        // msleep(1000);
        // pass, let screen update?
        is_sending = 1;
        return 0;
    } else if (is_sending == 1) {
        uart_printf("b1\n");
        // return value
        char command[] = "AT?\0";
        char *response = sim_request(command);
        if (strcmp(response, "__AT__OK__") == 0) {
            uart_printf("response is okay.\n");
            is_sending = 0;  // sent success, done sending
            free(response);
            return 1;
        }
        free(response);
    }

    return 0;
}
