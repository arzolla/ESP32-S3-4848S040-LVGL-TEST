

#include <stdio.h>
#include <lvgl.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include <bsp/esp-bsp.h>


#define TAG "app_main"

#define initial_brightness 35

static lv_obj_t * slider_label;



static void btn_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    static uint8_t cnt = 0;
    cnt++;

    lv_obj_t * label = lv_event_get_user_data(e);
    lv_label_set_text_fmt(label, "Button: %d", cnt);
}

static void backlight_on_event_cb(lv_event_t * e)
{
    LV_UNUSED(e);  // evita warning

    bsp_display_backlight_on();
}

static void backlight_off_event_cb(lv_event_t * e)
{
    LV_UNUSED(e);  // evita warning

    bsp_display_backlight_off();
}

static void slider_event_cb(lv_event_t * e)
{

    lv_obj_t * slider = lv_event_get_target(e);

    int slider_value = (int)lv_slider_get_value(slider);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", slider_value);
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    bsp_display_brightness_set(slider_value);

}

void lv_example_get_started_2(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, label); 
}


void lv_button_backlight_on(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 70);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, backlight_on_event_cb, LV_EVENT_CLICKED, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "On");                     /*Set the labels text*/
    lv_obj_center(label);
}

void lv_button_backlight_off(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 130);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, backlight_off_event_cb, LV_EVENT_CLICKED, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Off");                     /*Set the labels text*/
    lv_obj_center(label);
}


void lv_button_backlight_off_when_pressed(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 190);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, backlight_off_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(btn, backlight_on_event_cb, LV_EVENT_RELEASED, NULL);

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Press");                     /*Set the labels text*/
    lv_obj_center(label);
}



void lv_example_slider_1(void)
{
    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(slider, 50, 300);  

    lv_slider_set_value(slider, initial_brightness, LV_ANIM_OFF); 

    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*Create a label below the slider*/
    slider_label = lv_label_create(lv_scr_act());

    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", initial_brightness);

    lv_label_set_text(slider_label, buf);

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

void ui_task(void *pvParameter)
{

    bsp_display_lock(0);
    lv_example_get_started_2();
    lv_button_backlight_on();
    lv_button_backlight_off();
    lv_button_backlight_off_when_pressed();
    lv_example_slider_1();
    bsp_display_unlock();


    vTaskDelete(NULL);
}

void vTaskFunction( void * pvParameters )
{
    /* Block for 500ms. */
    const TickType_t xDelay = 3000 / portTICK_PERIOD_MS;

    for( ;; )
    {

        //bsp_display_backlight_on();
        vTaskDelay( xDelay );
        //bsp_display_backlight_off();
        vTaskDelay( xDelay );

    }
}



void app_main(void)
{



    bsp_display_start();

    bsp_display_brightness_set(20);

    xTaskCreate(vTaskFunction, "vTaskFunction", 8192, NULL, 5, NULL);
    xTaskCreate(ui_task, "ui", 8192, NULL, 5, NULL);

    for( ;; )
    {
        vTaskDelay( 100 / portTICK_PERIOD_MS );

    }


}

