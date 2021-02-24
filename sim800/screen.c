#include "screen.h"

#include <SDL/SDL.h>
#include <SDL/SDL_config.h>
#include <nspireio/nspireio.h>
#include <os.h>

// #include "SDL/SDL_image.h"

SDL_Surface *screen;
nSDL_Font *font;

#define PITCH (screen->pitch / 4)
#define WIDTH 320
#define HEIGHT 240

// #define BGCOLOR 0x000000
// #define COLOR1 0x333333
#define FGCOLOR 0xffffff

#define BGCOLOR 0x333333
#define COLOR1 0x000000

void screen_init() {
    SDL_Init(SDL_INIT_VIDEO);
    screen =
        SDL_SetVideoMode(WIDTH, HEIGHT, has_colors ? 16 : 8, SDL_SWSURFACE);
    font = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 255, 255, 255);
    SDL_FillRect(screen, NULL, BGCOLOR);
}

void screen_clear() { SDL_FillRect(screen, NULL, BGCOLOR); }

void screen_render() { SDL_Flip(screen); }

void drawrect(int x, int y, int width, int height, int c) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    // Uint32 color2 = SDL_MapRGB(screen->format, r, g, b);
    SDL_FillRect(screen, &rect, c);
}

void drawrect_outline(int x, int y, int width, int height, int c, int c2) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_Rect rect2;
    rect2.x = x + 1;
    rect2.y = y + 1;
    rect2.w = width - 2;
    rect2.h = height - 2;

    // Uint32 color2 = SDL_MapRGB(screen->format, r, g, b);

    SDL_FillRect(screen, &rect, c);
    SDL_FillRect(screen, &rect2, c2);
}

void draw_panel(int x, int y, int width, int height) {
    drawrect(x + 1, y + 1, width - 2, height - 2, COLOR1);
}

void screen_draw_title(char title[]) {
    // SDL_Surface *banner;
    // banner = SDL_CreateRGBSurface(0, 320, 20, 8, 0, 0, 0, 0);
    // SDL_FillRect(banner, NULL, SDL_MapRGB(banner->format, 255, 255, 0));

    // drawrect_outline(0, 0, WIDTH, 10, COLOR1, BGCOLOR);
    // drawrect(1, 1, WIDTH - 2, 10 - 2, COLOR1);
    draw_panel(0, 0, WIDTH, 10);

    // nSDL_DrawString(screen, font, 1, 1,
    //                 "AT:ERR Status:ERR Batt:XXX Sig:XXX Conn:XXXXXXX");
    nSDL_DrawString(screen, font, 1, 1, title);
    screen_render();
}

struct page screen_make_page() {
    struct page p1;
    p1.x = 0;
    p1.y = 10;
    p1.w = WIDTH;
    p1.h = HEIGHT - 10;
    p1.next_avail_row = 0;
    p1.row_height = 10;
    // p1.selected_row = 1;
    draw_panel(p1.x, p1.y, p1.w, p1.h);
    return p1;
}

void screen_clear_page(struct page p1) { draw_panel(p1.x, p1.y, p1.w, p1.h); }

void screen_draw_menu(char **options, char *title, int n, int sel) {
    // int sel = 1;
    nSDL_DrawString(screen, font, 3, 10 + 3, title);
    drawrect_outline(2, 20 + 2 + 10 * sel, WIDTH - 4, 10, BGCOLOR, COLOR1);
    // size_t n = sizeof(options) / sizeof(*options);
    for (int i = 0; i < n; i++) {
        nSDL_DrawString(screen, font, 10, 20 + 3 + 10 * i, options[i]);
    }
    screen_render();
}

void screen_handle_menu(char **menu_options, char *menu, int n, int *selected) {
    if (isKeyPressed(KEY_NSPIRE_DOWN)) {
        (*selected)++;
        if (*selected > n - 1) {
            *selected = n - 1;
        }
        while (isKeyPressed(KEY_NSPIRE_DOWN))
            ;
    }
    if (isKeyPressed(KEY_NSPIRE_UP)) {
        (*selected)--;
        if (*selected < 0) {
            *selected = 0;
        }
        while (isKeyPressed(KEY_NSPIRE_UP))
            ;
    }
    if (isKeyPressed(KEY_NSPIRE_ENTER)) {
        // menu = "sms";
        strcpy(menu, (menu_options[*selected]));
        *selected = 0;
        while (isKeyPressed(KEY_NSPIRE_ENTER))
            ;
    }
}

void screen_draw_input(struct page *p1, char *prompt, char *value,
                       int selected) {
    // nSDL_DrawString(screen, font, 3, 10 + 3, "SEND SMS");
    char value_buffer[100];
    strcpy(value_buffer, value);
    // char *value_buffer = strdup(value);
    nSDL_DrawString(screen, font, p1->x + 2,
                    p1->y + 3 + p1->row_height * p1->next_avail_row, prompt);
    drawrect_outline(p1->x + 40,
                     p1->y + 3 + p1->row_height * p1->next_avail_row,
                     p1->w - 40 - 2, p1->row_height, FGCOLOR, COLOR1);
    nSDL_DrawString(screen, font, p1->x + 42,
                    p1->y + 3 + p1->row_height * p1->next_avail_row,
                    value_buffer);
    if (selected == p1->next_avail_row) {
        // size_t n = sizeof(value) / sizeof(*value);
        size_t n = strlen(value);
        drawrect(p1->x + 42 + 6 * n,
                 p1->y + 3 + p1->row_height * p1->next_avail_row + 5, 5, 1,
                 FGCOLOR);
    }
    p1->next_avail_row = p1->next_avail_row + 1;
}

void append(char *s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

void bkspace(char *s) { s[strlen(s) - 1] = 0; }

// https://stackoverflow.com/questions/1431500/how-can-i-modify-a-2d-array-passed-to-a-function
// https://stackoverflow.com/questions/13169215/pass-by-reference-string-array-to-function-and-modify-content-in-c
void screen_handle_input(char *form_values[], int n, int *selected) {
    if (isKeyPressed(KEY_NSPIRE_DOWN)) {
        (*selected)++;
        if (*selected > n - 1) {
            *selected = 0;
        }
        while (isKeyPressed(KEY_NSPIRE_DOWN))
            ;
    }
    if (isKeyPressed(KEY_NSPIRE_UP)) {
        (*selected)--;
        if (*selected < 0) {
            *selected = 0;
        }
        while (isKeyPressed(KEY_NSPIRE_UP))
            ;
    }
    if (isKeyPressed(KEY_NSPIRE_DEL)) {
        bkspace(form_values[*selected]);
        while (isKeyPressed(KEY_NSPIRE_DEL))
            ;
    }
    // if (isKeyPressed(KEY_NSPIRE_A)) {
    //     append(form_values[*selected], 'a');
    //     while (isKeyPressed(KEY_NSPIRE_A))
    //         ;
    // }
    if (any_key_pressed()) {
        if (!isKeyPressed(KEY_NSPIRE_ESC)) {
            int adaptive_cursor_state;
            append(form_values[*selected],
                   nio_ascii_get(&adaptive_cursor_state));
            while (any_key_pressed())
                ;
        }
    }
}