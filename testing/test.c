
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int last_stat = 0;

struct simstatus {
    int at;
    int last_stat;
    int batt;
    int sig_strength;
    char *sig_conn;
};

struct simstatus simstatus;

char *sim_request(char *command) {
    char response[20];
    // response = await_serial_rec(command);
    strcpy(response, "__AT____OK\0");
    char *str = malloc(20 * sizeof(char));
    strcpy(str, response);
    return str;
}

int is_OK(char command[]) {
    if (strstr(command, "____OK") != NULL) {
        return 1;
    }
    return 0;  // else
}

int comp_request(char command[], char expect[]) {
    char *response = sim_request(command);
    // char response[] = "__AT____OK";
    if (strcmp(response, expect) == 0) {
        // free(response);
        return 1;
    }
    // free(response);
    return 0;
}

// int comp_request() { return 0; };

int sim_is_ok() {
    if ((strstr("__at+cpin?___+CPIN: READY____OK", "READY") != NULL) &&
        (comp_request("AT\r", "__AT____OK") == 1)) {
        return 1;
    }
    return 0;
}
int sim_get_batt() { return 0; }
int sim_get_sig_strength() { return 0; }
void sim_get_conn(char *str) {  //
    strcpy(str, "NO-CONN\0");
}

void sim_get_status(struct simstatus *ss) {
    static int at;
    static int batt;
    static int st;
    static char conn[20];
    at = sim_is_ok();
    // batt = sim_get_batt();
    // st = sim_get_sig_strength();
    // sim_get_conn(conn);
    ss->at = at;
    // ss->last_stat = last_stat;
    // ss->batt = batt;
    // ss->sig_strength = st;
    // ss->sig_conn = conn;
}

void sim_tester_status() {
    // "AT:ERR Status:ERR Batt:XXX Sig:XXX Conn:XXXXXXX";
    // strcat(*title, "AT:"); // works but bad
    static char title[100];
    static int ticks = 101;

    if (ticks > 100) {  // 100 ticks ~= 2.5s (3/6/21)
        ticks = 0;
        // uart_printf("updating title\n");
        sim_get_status(&simstatus);

        char at[5];
        if (simstatus.at == 1) {
            strcpy(at, "OK");
        } else {
            strcpy(at, "ERR");
        }

        char last_stat[5];
        if (simstatus.last_stat == 1) {
            strcpy(last_stat, "OK");
        } else {
            strcpy(last_stat, "ERR");
        }

        snprintf(title, sizeof title, "%s%-2s %s%-2s %s%-3d %s%-3d %s%-6s",
                 "AT:", at, "STAT:", last_stat, "BATT:", simstatus.batt,
                 "SIG:", simstatus.sig_strength, "CONN:", simstatus.sig_conn);

        // strcpy(title, "AT:ERR Status:ERR Batt:XXX Sig:XXX Conn:XXXXXXX");
    }
    ticks++;

    // screen_draw_title(title);
    printf("%s", title);
}

int main() {
    printf("\nA sample C program\n\n");
    sim_tester_status();
    // int ret = is_OK("yoyo____OK");
    // printf("%d", ret);
    printf("\n\n");
    return 0;
}