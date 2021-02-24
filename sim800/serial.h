#ifndef _SERIAL_ /* This is an "include guard" */
#define _SERIAL_ /* prevents the file from being included twice. */
                 /* Including a header file twice causes all kinds */
                 /* of interesting problems.*/

/**
 * This is a function declaration.
 * It tells the compiler that the function exists somewhere.
 */
void serial_send(char *str);
char *serial_rec();
char *serial_request();
char *uart_getsn_mdf(char *str, int num);
char *uart_getsn_mdf_test(char *str, int num);

#endif