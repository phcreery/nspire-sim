// make clean
// make

#include <nspireio/nspireio.h>
#include <os.h>
#include <string.h>

#include "screen.h"
#include "serial.h"
#include "sim800.h"

int key_already_pressed = 0;
int selected = 0;
char page[] = "landing";

char* form_values[2];

void populate_string_array() {
    for (int i = 0; i < 2; i++) {
        // form_values[i] = (char *)calloc(100, sizeof(char *));
        form_values[i] = (char*)malloc(100);
    }
    strcpy(form_values[0], "806437513322\0");
    strcpy(form_values[1], "what i s uppp\0");
}

void page_landing() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);
    char* menu_options[3] = {"SMS", "CALL", "GMS"};
    int n = sizeof(menu_options) / sizeof(menu_options[0]);
    screen_draw_menu(menu_options, "OPTIONS", n, selected);
    screen_handle_menu(menu_options, &page, n, &selected);
}

void page_sms() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);
    char* menu_options[2] = {"SMS SEND", "SMS RECEIVE"};
    int n = sizeof(menu_options) / sizeof(menu_options[0]);
    screen_draw_menu(menu_options, "SMS OPTIONS", n, selected);
    screen_handle_menu(menu_options, &page, n, &selected);
}

void page_sms_send() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);

    screen_draw_input(&page1, "TO:", form_values[0], selected);
    screen_draw_input(&page1, "MSG:", form_values[1], selected);
    // int n = sizeof(form_values) / sizeof(form_values[0]);
    screen_handle_input(form_values, 2, &selected);
    // wait_key_pressed();
}

int main(void) {
    // Define Variables
    // char input[1024] = {0};  // Input from the Serial Port
    // char input2[100] = {0}; // Input from the on screen prompt

    // Setup Screen
    assert_ndless_rev(874);
    screen_init();
    // uart_printf("%.*s\n", (int)sizeof(form_values[0]),
    // form_values[0]);  // Prints a string
    populate_string_array();

    // Main Loop
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        screen_draw_title();

        // page_landing();
        if (strcmp(page, "landing") == 0) {
            page_landing();
        } else if (strcmp(page, "SMS") == 0) {
            page_sms();
        } else if (strcmp(page, "SMS SEND") == 0) {
            // selected = 1;
            page_sms_send();
        } else {
            return 0;
        }

        // wait_key_pressed();
        // msleep(10);

        // Reset Keypress Status
        // if (!any_key_pressed()) {
        //     key_already_pressed = 0;
        // }
    }

    // Exiting
    // wait_key_pressed();

    return 0;
}
