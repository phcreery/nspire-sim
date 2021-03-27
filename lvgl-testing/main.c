// #include <lvgl.h>
#include "lvgl/lvgl.h"
// #include <TFT_eSPI.h>
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.*/
#include "lv_examples/lv_demo.h"
#include <nspireio/nspireio.h>

// TFT_eSPI tft = TFT_eSPI(); /*TFT instance*/

/*Change to your screen resolution*/
static uint32_t screenWidth = 320;
static uint32_t screenHeight = 240;



// inline void setPixel(int x, int y, int color) {
//   unsigned char* p = (unsigned char*)(SCREEN_BASE_ADDRESS + ((x >> 1) + (y << 7) + (y << 5)));
//   *p = (x & 1) ? ((*p & 0xF0) | color) : ((*p & 0x0F) | (color << 4));
// }

/*Display flushing*/
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    // uint32_t w = (area->x2 - area->x1 + 1);
    // uint32_t h = (area->y2 - area->y1 + 1);


    // lcd_blit(void* buffer, scr_type_t type)
    // lcd_blit(area, SCR_320x240_8);
    lcd_blit(&color_p->full, SCR_320x240_8);

    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    // int32_t x;
    // int32_t y;
    // for(y = area->y1; y <= area->y2; y++) {
    //     for(x = area->x1; x <= area->x2; x++) {
    //         /*Put a pixel to the display. For example:*/
    //         /*put_px(x, y, *color_p)*/
    //         setPixel(x, y, *color_p);
    //         color_p++;
    //     }
    // }

    // tft.startWrite();
    // tft.setAddrWindow(area->x1, area->y1, w, h);
    // tft.pushColors(&color_p->full, w * h, true);
    // tft.endWrite();

    lv_disp_flush_ready(disp);
}

void setup()
{

    lv_init();

    // tft.begin();        /*TFT init*/
    // tft.setRotation(1); /*Landscape orientation*/
    lcd_init(SCR_320x240_8); // or SCR_320x240_4s


    static lv_disp_draw_buf_t draw_buf;
    // static lv_color_t buf_1[screenWidth * 10];
    static lv_color_t buf_1[320 * 10];                          /*A buffer for 10 rows*/
    // lv_draw_buf_init(&draw_buf, buf_1, NULL, screenWidth * 10);
    // lv_disp_buf_init(&draw_buf, buf_1, NULL, screenWidth * 10);
    lv_disp_draw_buf_init(&draw_buf, buf_1, NULL, screenWidth * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Try an example from the lv_examples Arduino library
      make sure to include it as written above.*/
    lv_example_btn_1();
}


int main(void){
    setup();
    while(1){
        lv_timer_handler(); /*let the GUI do its work*/
        msleep(5);
    }
}