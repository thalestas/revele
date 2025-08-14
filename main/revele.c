#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "lvgl_port.h"
#include "screens/status_bar.h"
#include "sdcard.h"
#include "wifi_prov.h"

static char *TAG = "app_main";

static void set_angle(void *obj, int32_t v) {
  lv_arc_set_value((lv_obj_t *)obj, v);
}

lv_anim_t lv_arc(lv_obj_t *parent) {
  lv_obj_t *arc = lv_arc_create(parent);
  lv_arc_set_rotation(arc, 270);
  lv_arc_set_bg_angles(arc, 0, 360);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(arc, 15, 15);
  lv_obj_set_style_arc_width(arc, 3, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 3, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc, lv_palette_darken(LV_PALETTE_BLUE_GREY, 3),
                             LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 4),
                             LV_PART_MAIN);
  lv_obj_align(arc, LV_ALIGN_TOP_RIGHT, -5, 5);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, arc);
  lv_anim_set_exec_cb(&a, set_angle);
  lv_anim_set_duration(&a, 10000);
  lv_anim_set_repeat_count(&a, 1);
  lv_anim_set_values(&a, 0, 93);

  return a;
}

void app_main(void) {
  // Init LVGL
  lvgl_init();

  // Init sdcard
  init_sdcard();
  list_dir("S:");

  // Init
  status_bar_init();

  lvgl_port_lock(0);
  set_status_bar_opa(LV_OPA_TRANSP);
  set_status_bar_text("paris");
  lvgl_port_unlock();

  // Create wallpaper screen
  lv_obj_t *wallpaper = lv_obj_create(NULL);
  lv_obj_t *img = lv_img_create(wallpaper);
  const char img_path[100];
  get_next_img_path(img_path);
  lv_img_set_src(img, img_path);
  lv_anim_t spin_wallpaper = lv_arc(wallpaper);

  lv_obj_t *wallpaper2 = lv_obj_create(NULL);
  lv_obj_t *img2 = lv_img_create(wallpaper2);
  get_next_img_path(img_path);
  lv_img_set_src(img2, img_path);
  lv_arc(wallpaper2);
  lv_anim_t spin_wallpaper2 = lv_arc(wallpaper2);

  // Wifi connection
  connect_wifi(&wifi_event_handler);

  // Load screens
  while (1) {
    lvgl_port_lock(0);
    get_next_img_path(img_path);
    lv_img_set_src(img, img_path);
    lv_anim_start(&spin_wallpaper);
    lv_scr_load_anim(wallpaper, LV_SCR_LOAD_ANIM_FADE_OUT, 1500, 0, false);
    lvgl_port_unlock();
    vTaskDelay(10000 / portTICK_PERIOD_MS);

    lvgl_port_lock(0);
    get_next_img_path(img_path);
    lv_img_set_src(img2, img_path);
    lv_anim_start(&spin_wallpaper2);
    lv_scr_load_anim(wallpaper2, LV_SCR_LOAD_ANIM_FADE_OUT, 1500, 0, false);
    lvgl_port_unlock();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
