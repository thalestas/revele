#include "styles.h"

void box_style_init(lv_style_t *const style) {
  lv_style_set_radius(style, 5);
  lv_style_set_bg_opa(style, LV_OPA_COVER);
  lv_style_set_bg_color(style, (lv_color_t)BOX_BG_COLOR);
  lv_style_set_bg_grad_color(style, (lv_color_t)BOX_GRAD_COLOR);
  lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_HOR);
  lv_style_set_shadow_width(style, 1);
  lv_style_set_shadow_color(style, lv_palette_main(LV_PALETTE_GREY));
  lv_style_set_border_width(style, 0);
  lv_style_set_outline_width(style, 0);
  lv_style_set_pad_all(style, 0);
}

void subtitle_style_init(lv_style_t *const style) {
  lv_style_set_text_color(style, (lv_color_t)LABEL_COLOR);
  lv_style_set_text_font(style, &lv_font_montserrat_12);
  lv_style_set_border_width(style, 0);
  lv_style_set_outline_width(style, 0);
  lv_style_set_pad_all(style, 0);
}
