#pragma once

#include "esp_event.h"
#include "lvgl.h"

static lv_obj_t *status_bar;
static lv_obj_t *wifi_status;
static lv_obj_t *status_bar_title;

void status_bar_init();
void wifi_event_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data);
void set_status_bar_opa(lv_opa_t value);
void set_status_bar_text(const char *text);
