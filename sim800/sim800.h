#ifndef _SIM800_ /* This is an "include guard" */
#define _SIM800_ /* prevents the file from being included twice. */
/* Including a header file twice causes all kinds */
/* of interesting problems.*/

/**
 * This is a function declaration.
 * It tells the compiler that the function exists somewhere.
 */

struct simstatus {
    int at;
    int last_stat;
    int batt;
    int sig_strength;
    char* sig_conn;
};

void sim_get_status();
int sim_send_text();
void get_history();
void populate_history();

#endif