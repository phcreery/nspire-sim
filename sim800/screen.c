#include "screen.h"

#include <SDL/SDL.h>
#include <SDL/SDL_config.h>
#include <nspireio/nspireio.h>
#include <os.h>

// #include "SDL/SDL_image.h"

SDL_Surface *screen;
nSDL_Font *font;

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
    p1.num_rows = 0;
    p1.selected_row = 0;
    draw_panel(p1.x, p1.y, p1.w, p1.h);
    return p1;
}

void screen_clear_page(struct page p1) { draw_panel(p1.x, p1.y, p1.w, p1.h); }

void screen_draw_page_title(struct page *p1, char *title) {
    size_t n = strlen(title) * 6;  // ~6 px per char
    nSDL_DrawString(screen, font, p1->x + 3, p1->y + 3, "<");
    nSDL_DrawString(screen, font, p1->w - 9, p1->y + 3, ">");
    nSDL_DrawString(screen, font, p1->x + p1->w / 2 - n / 2, p1->y + 3, title);
    p1->next_avail_row = p1->next_avail_row + 1;
}

void screen_draw_page_text(struct page *p1, char *title) {
    nSDL_DrawString(screen, font, p1->x + 3,
                    p1->y + 3 + p1->next_avail_row * p1->row_height, title);
    p1->next_avail_row = p1->next_avail_row + 1;
}

void screen_draw_menu(struct page *p1, char **options, int n, int sel) {
    drawrect_outline(
        p1->x + 2,
        p1->y + 2 + p1->next_avail_row * p1->row_height + p1->row_height * sel,
        WIDTH - 4, 10, BGCOLOR, COLOR1);
    // size_t n = sizeof(options) / sizeof(*options);
    for (int i = 0; i < n; i++) {
        nSDL_DrawString(screen, font, 10, 20 + 3 + 10 * i, options[i]);
        p1->next_avail_row = p1->next_avail_row + 1;
    }
    screen_render();
}

void screen_draw_input(struct page *p1, char *prompt, char *value,
                       int selected) {
    // nSDL_DrawString(screen, font, 3, 10 + 3, "SEND SMS");
    char value_buffer[100];
    strcpy(value_buffer, value);
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
        // size_t n = strlen(value);
        drawrect(p1->x + 40 + p1->w - 40 - 2 - 5,
                 p1->y + 3 + p1->row_height * p1->next_avail_row + 2, 5,
                 p1->row_height - 4, FGCOLOR);
    }
    p1->next_avail_row = p1->next_avail_row + 1;
    p1->num_rows = p1->num_rows + 1;
}

void append(char *s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

void bkspace(char *s) { s[strlen(s) - 1] = 0; }

// https://stackoverflow.com/questions/1431500/how-can-i-modify-a-2d-array-passed-to-a-function
// https://stackoverflow.com/questions/13169215/pass-by-reference-string-array-to-function-and-modify-content-in-c
void screen_handle_page(struct page *p1, int *selected, char *dest, char *prev,
                        char *next) {
    // Handle actions for pages
    // the next/prev strings will be copied into the dest pointer when the
    // enter/back buttoms are pressed
    p1->selected_row = *selected;
    if (isKeyPressed(KEY_NSPIRE_ENTER)) {
        *selected = 0;
        strcpy(dest, next);
        while (isKeyPressed(KEY_NSPIRE_ENTER))
            ;
    } else if (isKeyPressed(KEY_NSPIRE_MENU)) {
        *selected = 0;
        strcpy(dest, prev);
        while (isKeyPressed(KEY_NSPIRE_MENU))
            ;
    }
}

void screen_handle_selection(struct page *p1, int n, int *selected) {
    // Handle actions for selection options
    p1->selected_row = *selected;
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
}

// Not even needed??
void screen_handle_menu(char **menu_options, char *menu, int *selected) {
    if (isKeyPressed(KEY_NSPIRE_ENTER)) {
        strcpy(menu, (menu_options[*selected]));
        *selected = 0;
        while (isKeyPressed(KEY_NSPIRE_ENTER))
            ;
    }
}

void screen_handle_input(char *form_value) {
    // Handle actions for input boxes
    if (isKeyPressed(KEY_NSPIRE_DEL)) {
        bkspace(form_value);
        while (isKeyPressed(KEY_NSPIRE_DEL))
            ;
    } else if (any_key_pressed()) {
        if (!isKeyPressed(KEY_NSPIRE_ESC)) {
            int adaptive_cursor_state;
            append(form_value, nio_ascii_get(&adaptive_cursor_state));
            while (any_key_pressed())
                ;
        }
    }
}