

#include <stdio.h>
#include <lvgl.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include <bsp.h>


#define TAG "app_main"

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target_obj(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
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

void lv_example_get_started_2(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
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

    //bsp_display_backlight_off();

    lv_example_get_started_2();
    lv_button_backlight_on();
    lv_button_backlight_off();
    lv_button_backlight_off_when_pressed();

    xTaskCreate(vTaskFunction, "vTaskFunction", 8192, NULL, 5, NULL);


    for( ;; )
    {
        vTaskDelay( 100 / portTICK_PERIOD_MS );

    }


}

