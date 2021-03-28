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
char page[20] = "attach";

char* form_values[2];

struct simstatus simstatus;

void populate_string_array() {
    for (int i = 0; i < 2; i++) {
        // form_values[i] = (char *)calloc(100, sizeof(char *));
        form_values[i] = (char*)malloc(100);
    }
    strcpy(form_values[0], "806437513322\0");
    strcpy(form_values[1], "what is up\0");
}

void page_attach() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);
    screen_draw_page_text(&page1, "ATTACH SIM & PRESS (enter)");
    // screen_handle_selection(&page1, 0, &selected);
    screen_handle_page(&page1, &selected, &page, "attach", "landing");
}

void page_landing() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);
    char* menu_options[4] = {"SMS", "CALL", "GPRS/HTTP", "CONFIG"};
    int n = sizeof(menu_options) / sizeof(menu_options[0]);
    screen_draw_page_title(&page1, "OPTIONS");
    screen_draw_menu(&page1, menu_options, n, selected);
    // screen_handle_menu(menu_options, &page, n, &selected);
    screen_handle_selection(&page1, n, &selected);
    screen_handle_page(&page1, &selected, &page, "attach",
                       menu_options[selected]);
}

void page_sms() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);

    char* menu_options[2] = {"SMS SEND", "SMS RECEIVE"};
    int n = sizeof(menu_options) / sizeof(menu_options[0]);

    screen_draw_page_title(&page1, "SMS OPTIONS");
    screen_draw_menu(&page1, menu_options, n, selected);

    // screen_handle_menu(menu_options, &page, &selected);
    screen_handle_selection(&page1, 2, &selected);
    screen_handle_page(&page1, &selected, &page, "landing",
                       menu_options[selected]);
}

void page_sms_send() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);

    screen_draw_page_title(&page1, "COMPOSE");

    // +1 since there is a title row above
    screen_draw_input(&page1, "TO:", form_values[0], selected + 1);
    screen_draw_input(&page1, "MSG:", form_values[1], selected + 1);

    screen_handle_page(&page1, &selected, &page, "SMS SEND", "SMS SENDING");
    screen_handle_selection(&page1, 2, &selected);
    screen_handle_input(form_values[selected]);
    // wait_key_pressed();
}

void page_sms_sending() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);

    screen_draw_page_title(&page1, "SENDING...");
    // uart_printf("Sending...\n");
    int status;
    status = sim_send_text();

    char* hist[20];
    get_history(hist);
    for (int i = 0; i < 20; i++) {
        uart_printf("%d: %s\n", i, hist[i]);
        screen_draw_page_text(&page1, hist[i]);
    }

    // uart_printf("%.*s\n", 20, resp);
    // uart_printf("status: ");
    // uart_printf("%d\n", status);
    if (status == 1) {
        uart_printf("Send success\n");
        strcpy(page, "SMS SEND SUCCESS");
    }
}

void page_sms_send_success() {
    struct page page1 = screen_make_page();
    screen_clear_page(page1);

    screen_draw_page_title(&page1, "SENDING SUCCESS");

    char* hist[20];
    get_history(hist);
    for (int i = 0; i < 20; i++) {
        // uart_printf("%d: %s\n", i, hist[i]);
        screen_draw_page_text(&page1, hist[i]);
    }

    screen_handle_page(&page1, &selected, &page, "landing", "landing");
}

void update_title() {
    // "AT:ERR Status:ERR Batt:XXX Sig:XXX Conn:XXXXXXX";
    // strcat(*title, "AT:"); // works but bad
    static char title[100];
    static int ticks = 100;

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

        static char last_stat[5];
        if (simstatus.last_stat == 1) {
            strcpy(last_stat, "OK");
        } else {
            strcpy(last_stat, "ERR");
        }

        static char sim[5];
        if (simstatus.sim == 1) {
            strcpy(sim, "OK");
        } else {
            strcpy(sim, "ERR");
        }

        snprintf(title, sizeof title,
                 "%s%-2s %s%-2s %s%-2s %s%-2d %s%-2d %s%-6s", "AT:", at,
                 "STAT:", last_stat, "SIM:", sim, "BAT:", simstatus.batt,
                 "SIG:", simstatus.sig_strength, "CON:", simstatus.sig_conn);

        // strcpy(title, "AT:ERR Status:ERR Batt:XXX Sig:XXX Conn:XXXXXXX");
    }
    ticks++;

    screen_draw_title(title);
}

void sim_handler() {
    update_title();
    // screen_draw_title();

    if (strcmp(page, "landing") == 0) {
        page_landing();
    } else if (strcmp(page, "attach") == 0) {
        page_attach();
    } else if (strcmp(page, "SMS") == 0) {
        page_sms();
    } else if (strcmp(page, "SMS SEND") == 0) {
        page_sms_send();
    } else if (strcmp(page, "SMS SENDING") == 0) {
        page_sms_sending();
        // return 0;
    } else if (strcmp(page, "SMS SEND SUCCESS") == 0) {
        page_sms_send_success();
    } else {
        strcpy(page, "attach");
        // return 0;
    }
}

int main(void) {
    // Setup Screen
    assert_ndless_rev(874);
    screen_init();
    // uart_printf("%.*s\n", (int)sizeof(form_values[0]),
    // form_values[0]);  // Prints a string
    populate_string_array();  // empties and inits form array
    populate_history();       // emptied and inits history array

    // Main Loop
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        sim_handler();
        // sim_tester_status();

        msleep(20);
    }

    // Exiting
    // wait_key_pressed();
    // free(form_values[0]);
    // free(form_values[1]);

    return 0;
}
