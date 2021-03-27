// make clean
// make

#include <nspireio/nspireio.h>
#include <os.h>
#include <string.h>

#include "serial.h"

int key_already_pressed = 0;
char last_command[100] = {0};

int main(void) {
    // Define Variables
    // char input[1024] = {0};  // Input from the Serial Port
    // char input2[100] = {0}; // Input from the on screen prompt

    // Setup Screen
    assert_ndless_rev(874);

    nio_console csl;
    nio_init(&csl, NIO_MAX_COLS, NIO_MAX_ROWS, 0, 0, NIO_COLOR_WHITE,
             NIO_COLOR_BLACK, TRUE);
    nio_set_default(&csl);
    nio_color(&csl, NIO_COLOR_WHITE, NIO_COLOR_BLACK);  // black on white

    nio_printf("Compiled %s %s\n", __DATE__, __TIME__);
    nio_puts("Press any ESC to exit and CTRL to send msg\n");
    nio_puts("Press R if stuck on receiving message\n");

    // Main Loop
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        // Send UART
        if (isKeyPressed(KEY_NSPIRE_CTRL) && !key_already_pressed) {
            nio_printf(">");
            char command[100] = {0};
            char response[1024] = {0};
            nio_getsn(command, 100);  // Blocking
            strcpy(last_command, command);
            serial_request(command, response);
            nio_printf(response);
            nio_printf("\n");
            key_already_pressed = 1;
        }

        // Previous Command
        if (isKeyPressed(KEY_NSPIRE_UP) && !key_already_pressed) {
            nio_printf(">");
            nio_printf(last_command);
            nio_printf("\n");

            char response[1024] = {0};
            serial_request(last_command, response);
            nio_printf(response);
            nio_printf("\n");
            key_already_pressed = 1;
        }

        // Reset Keypress Status
        if (!any_key_pressed()) {
            key_already_pressed = 0;
        }
    }

    // Exiting
    nio_puts("Tests finished.");
    nio_free(&csl);
    wait_key_pressed();

    return 0;
}