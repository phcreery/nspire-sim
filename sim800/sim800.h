#ifndef _SIM800_ /* This is an "include guard" */
#define _SIM800_ /* prevents the file from being included twice. */
/* Including a header file twice causes all kinds */
/* of interesting problems.*/

/**
 * This is a function declaration.
 * It tells the compiler that the function exists somewhere.
 */

// https://stackoverflow.com/questions/742699/returning-an-enum-from-a-function-in-c
/* Result codes */
enum Result {
    ERROR = 0,
    SUCCESS = 1,
    WORKING = 2,
    UNKNOWN = 3,
};

struct simstatus {
    int at;
    int last_stat;
    int sim;
    int batt;
    int sig_strength;
    char* sig_conn;
};

void sim_get_status();
enum Result sim_send_text();
void get_history();
void populate_history();

/* DATA FLOW
The "AT" or "at" or “aT” or “At”prefix must be set at the beginning of each
Command line. To terminate a Command line enter <CR>. Commands are usually
followed by a response that includes.
"<CR><LF><response><CR><LF>"
LF: \n
CR: \r
*/

/* SIM COMMANDS and RETURN statements
Stat			    AT		    __at____OK
Sim Ready		    AT+CPIN?	__at+cpin?___+CPIN: READY____OK
Network Reg Stat	AT+CGREG?	__at_cgreg?___+CGREG: 0,1____OK
Sig Strength		AT+CSQ		__at+csq___+CSQ: 7,0____OK
Battery	  AT+CBC	__at+cbc___+CBC: <bcs>,<bcl>,<voltage>____OK
Operator  AT+COPS?	__at+cops?___+COPS: 0,0,"OP"____OK

SEND MESSAGE
AT+CMGF=1
AT+CMGS="MOBILE NO."
(message)   <ctrl+z> / uart_printf("%c", 26);
*/

#endif