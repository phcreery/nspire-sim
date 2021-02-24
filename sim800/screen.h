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
    int next_avail_row;  // pixel location
    int num_rows;        //
    int selected_row;    // 0,1,...
};

void screen_init();
void screen_draw_title();
void screen_draw_menu();
void screen_handle_menu();
void screen_draw_input();
void screen_handle_input();
struct page screen_make_page();
void screen_clear_page();
void screen_handle_page();
void screen_render();
#endif