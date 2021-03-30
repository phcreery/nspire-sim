#ifndef _SCREEN_ /* This is an "include guard" */
#define _SCREEN_ /* prevents the file from being included twice. */
/* Including a header file twice causes all kinds */
/* of interesting problems.*/

/**
 * This is a function declaration.
 * It tells the compiler that the function exists somewhere.
 */

struct page {
    int x;
    int y;
    int w;
    int h;
    int row_height;      // each row has to have the same height
    int next_avail_row;  // next row index number
    int num_rows;        //
    int selected_row;    // 0,1,... needed??
};

void screen_init();
void screen_render();

void screen_draw_title();
struct page screen_make_page();
void screen_clear_page();
void screen_draw_page_title();
void screen_draw_page_text();

void screen_draw_menu();
void screen_draw_input();

void screen_handle_input();
void screen_handle_page();
void screen_handle_menu();
void screen_handle_selection();
#endif