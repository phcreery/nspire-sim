// make clean
// make

#include <os.h>
#include <nspireio/nspireio.h>
#include "serial.h"
#include <string.h>

int key_already_pressed = 0;

int main(void)
{
    // Define Variables
    char input[1024] = {0}; // Input from the Serial Port
    // char input2[100] = {0}; // Input from the on screen prompt

    // Setup Screen
    assert_ndless_rev(874);

    nio_console csl;
    nio_init(&csl, NIO_MAX_COLS, NIO_MAX_ROWS, 0, 0, NIO_COLOR_WHITE, NIO_COLOR_BLACK, TRUE);
    nio_set_default(&csl);
    nio_color(&csl, NIO_COLOR_WHITE, NIO_COLOR_BLACK); // black on white

    nio_printf("Compiled %s %s\n", __DATE__, __TIME__);
    nio_puts("Press any ESC to exit and CTRL to send msg...\n");
    nio_puts("Press R if stuck on receiving message\n");

    // Main Loop
    while (!isKeyPressed(KEY_NSPIRE_ESC))
    {

        // Send UART
        if (isKeyPressed(KEY_NSPIRE_CTRL) && !key_already_pressed)
        {
            nio_printf(">");
            char command[100] = {0};
            char response[1024] = {0};
            nio_getsn(command, 100); // Blocking
            // serial_send(input2);
            serial_request(command, response);
            nio_printf(response);
            nio_printf("\n");
            key_already_pressed = 1;
        }

        // Receive UART - Grabbing only the 8th character??? Try now, I modified uart_getchar_mdf
        if (0) // uart_ready()
        {
            // nio_puts("<");
            char input[1024] = {0};
            uart_getsn_mdf(input, 1024);
            // uart_getsn_mdf2(input, 1024);
            // nio_puts(input);
            nio_printf(input); // printf is magic bc puts dont work
            nio_printf("\n");
            if (strstr(input, "OK") != NULL)
            {
                nio_printf("Yes!\n");
            }
        }

        // Test Receive
        if (isKeyPressed(KEY_NSPIRE_T) && !key_already_pressed)
        {
            nio_printf("*<");
            nio_puts("1\r2\n3\r\n4");
            nio_printf("1\r2\n3\r\n4");
            nio_puts("\r\nOK\r\n");
            nio_printf("\r\nOK\r\n");
            nio_printf("*<");
            // uart_getsn_mdf_test(input, 10);
            nio_puts(input);

            // Print Raw ASCII Values
            for (int i = 0; i < 10; i++)
            {
                char snum[5];
                itoa((int)input[i], snum, 10);
                nio_puts(snum);
            }
            key_already_pressed = 1;
        }

        // Reset Keypress Status
        if (!any_key_pressed())
        {
            key_already_pressed = 0;
        }
    }

    // Exiting
    nio_puts("Tests finished.");
    nio_free(&csl);
    wait_key_pressed();

    return 0;
}