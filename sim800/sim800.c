#include "sim800.h"

#include <nspireio/nspireio.h>
#include <os.h>
#include <string.h>

int last_stat = 0;

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

int sim_send_text() { return 0; }