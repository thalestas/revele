#include "savings_screen.h"

#include "styles/styles.h"

/*
 *  Spend Bar Helper and Callbacks Functions
 */
static void spend_bars_exec_cb(void *bar, int32_t temp) {
  lv_bar_set_value((lv_obj_t *)bar, temp, LV_ANIM_ON);
}

static void spend_bars_event_cb(lv_event_t *e) {
  lv_obj_t *obj = lv_event_get_target_obj(e);

  lv_draw_label_dsc_t label_dsc;
  lv_draw_label_dsc_init(&label_dsc);
  label_dsc.font = LV_FONT_DEFAULT;

  char buf[8];
  lv_snprintf(buf, sizeof(buf), "%d", (int)lv_bar_get_value(obj));

  lv_point_t txt_size;
  lv_text_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space,
                   label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

  lv_area_t txt_area;
  txt_area.x1 = 0;
  txt_area.x2 = txt_size.x - 1;
  txt_area.y1 = 0;
  txt_area.y2 = txt_size.y - 1;

  lv_area_t empty_area;
  lv_obj_get_coords(obj, &empty_area);
  lv_area_set_height(&empty_area, lv_area_get_height(&empty_area) *
                                      (100 - lv_bar_get_value(obj)) / 100);

  /*If the empty bar space is long enough put the text outside on the top*/
  if (lv_area_get_height(&empty_area) > txt_size.y + 15) {
    lv_area_align(&empty_area, &txt_area, LV_ALIGN_BOTTOM_MID, 0, 0);
    label_dsc.color = lv_color_white();
  }
  /*If the empty bar space is short put the text inside on the top*/
  else {
    lv_area_align(&empty_area, &txt_area, LV_ALIGN_BOTTOM_MID, 0, 20);
    label_dsc.color = lv_color_white();
  }
  label_dsc.text = buf;
  label_dsc.text_local = true;
  lv_layer_t *layer = lv_event_get_layer(e);
  lv_draw_label(layer, &label_dsc, &txt_area);
}

lv_obj_t *spend_bars_create_bar(lv_obj_t *obj, int32_t initial_value) {
  static lv_style_t style_indic;
  lv_style_init(&style_indic);
  lv_style_set_bg_opa(&style_indic, BAR_OPACITY);
  lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);
  lv_style_set_radius(&style_indic, 3);

  lv_obj_t *bar = lv_bar_create(obj);
  lv_obj_add_event_cb(bar, spend_bars_event_cb, LV_EVENT_DRAW_MAIN_END, NULL);
  lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
  lv_obj_set_style_radius(bar, 3, LV_PART_MAIN);
  lv_bar_set_range(bar, 0, 100);
  lv_obj_set_size(bar, 30, 80);
  lv_obj_set_style_margin_all(bar, 0, 0);

  lv_anim_t anim;
  lv_anim_init(&anim);
  lv_anim_set_exec_cb(&anim, spend_bars_exec_cb);
  lv_anim_set_duration(&anim, 1500);
  lv_anim_set_var(&anim, bar);
  lv_anim_set_values(&anim, 0, initial_value);
  lv_anim_set_repeat_count(&anim, 1);
  lv_anim_start(&anim);

  return bar;
}

void spend_bars_label_style(lv_style_t *const style) {
  lv_style_set_text_color(style, LABEL_COLOR);
  lv_style_set_opa(style, LV_OPA_50);
  lv_style_set_text_font(style, &lv_font_montserrat_12);
  lv_style_set_transform_rotation(style, -900);
  lv_style_set_transform_pivot_x(style, 8);
  lv_style_set_transform_pivot_y(style, 8);
  lv_style_set_border_width(style, 0);
  lv_style_set_pad_all(style, 0);
}

void bills_symbol_style_init(lv_style_t *const style) {
  lv_style_set_text_color(style, lv_color_white());
  lv_style_set_radius(style, 5);
  lv_style_set_border_width(style, 0);
  lv_style_set_border_color(style, lv_color_white());
  lv_style_set_outline_width(style, 0);
  lv_style_set_pad_all(style, 3);
}

void spend_box_init(lv_obj_t *const spend_box,
                    const lv_style_t *const box_style,
                    const lv_style_t *const label_style,
                    const saving_data *const data) {
  // spend bar style
  lv_obj_add_style(spend_box, box_style, 0);
  lv_obj_set_scrollbar_mode(spend_box, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_size(spend_box, 80, 100, 0);
  lv_obj_align(spend_box, LV_ALIGN_BOTTOM_RIGHT, -5, -5);

  // label
  lv_obj_t *title = lv_label_create(spend_box);
  lv_label_set_long_mode(title, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_label_set_text(title, "Gastos Mensais");
  lv_obj_add_style(title, label_style, 0);
  lv_obj_set_width(title, 80);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 2, 0);

  // container - total
  lv_obj_t *total_box = lv_obj_create(spend_box);
  lv_obj_set_scrollbar_mode(total_box, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_style(total_box, box_style, 0);
  lv_obj_set_style_bg_opa(total_box, LV_OPA_0, 0);
  lv_obj_set_style_size(total_box, 40, 80, 0);
  lv_obj_align(total_box, LV_ALIGN_BOTTOM_LEFT, 0, -2);

  // container - general
  lv_obj_t *general_box = lv_obj_create(spend_box);
  lv_obj_set_scrollbar_mode(general_box, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_style(general_box, box_style, 0);
  lv_obj_set_style_bg_opa(general_box, LV_OPA_0, 0);
  lv_obj_set_style_size(general_box, 40, 80, 0);
  lv_obj_align(general_box, LV_ALIGN_BOTTOM_RIGHT, 0, -2);

  // total bar chart
  lv_obj_t *total_bar = spend_bars_create_bar(total_box, data->total_spend);
  lv_obj_set_style_bg_color(total_bar, BAR1_BG_COLOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(total_bar, BAR1_GRAD_COLOR, LV_PART_INDICATOR);
  lv_obj_align(total_bar, LV_ALIGN_CENTER, 0, 0);

  // general bar chart
  lv_obj_t *general_bar =
      spend_bars_create_bar(general_box, data->general_spend);
  lv_obj_set_style_bg_color(general_bar, BAR2_BG_COLOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(general_bar, BAR2_GRAD_COLOR,
                                 LV_PART_INDICATOR);
  lv_obj_align(general_bar, LV_ALIGN_CENTER, 0, 0);

  // bars labels style
  static lv_style_t bar_label_style;
  lv_style_init(&bar_label_style);
  spend_bars_label_style(&bar_label_style);

  // total label
  lv_obj_t *total_label = lv_label_create(total_box);
  lv_label_set_text(total_label, "total");
  lv_obj_add_style(total_label, &bar_label_style, 0);
  lv_obj_align_to(total_label, total_bar, LV_ALIGN_BOTTOM_LEFT, 15, -4);

  // general label
  lv_obj_t *general_label = lv_label_create(general_box);
  lv_label_set_text(general_label, "geral");
  lv_obj_add_style(general_label, &bar_label_style, 0);
  lv_obj_align(general_label, LV_ALIGN_BOTTOM_LEFT, 20, -4);
}

void add_value_chart(lv_obj_t *const chart, lv_chart_series_t *const serie,
                     const uint32_t *const serie_data) {
  int32_t i = 0;
  while (serie_data[i] > 0) {
    lv_chart_set_next_value(chart, serie, serie_data[i]);
    i++;
  }
}

void saving_chart_init(lv_obj_t *const saving_chart_box,
                       const lv_style_t *const box_style,
                       const lv_style_t *const label_style,
                       const saving_data *const data) {
  lv_obj_set_scrollbar_mode(saving_chart_box, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_style(saving_chart_box, box_style, 0);
  lv_obj_set_style_size(saving_chart_box, 145, 70, 0);
  lv_obj_align(saving_chart_box, LV_ALIGN_BOTTOM_LEFT, 5, -5);

  // chart
  lv_obj_t *chart = lv_chart_create(saving_chart_box);
  lv_obj_set_size(chart, 141, 66);
  lv_obj_align(chart, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);
  lv_chart_set_div_line_count(chart, 5, 12);
  lv_chart_set_point_count(chart, 12);
  lv_obj_set_style_bg_opa(chart, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart, 0, LV_PART_MAIN);
  lv_obj_set_style_line_color(chart, lv_palette_darken(LV_PALETTE_BLUE_GREY, 3),
                              LV_PART_MAIN);
  lv_obj_set_style_line_opa(chart, LV_OPA_50, LV_PART_MAIN);
  lv_obj_set_style_line_width(chart, 1, LV_PART_ITEMS);
  lv_obj_set_style_size(chart, 3, 3, LV_PART_INDICATOR);

  // scale
  lv_obj_t *scale = lv_scale_create(saving_chart_box);
  lv_scale_set_mode(scale, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
  lv_obj_set_size(scale, 141, 20);
  lv_scale_set_total_tick_count(scale, 12);
  lv_scale_set_major_tick_every(scale, 1);
  lv_obj_set_style_pad_hor(scale, lv_chart_get_first_point_center_offset(chart),
                           0);

  // scale label
  static const char *month[] = {"J", "F", "M", "A", "M", "J",
                                "J", "A", "S", "O", "N", "D"};
  lv_scale_set_text_src(scale, month);
  lv_obj_set_style_text_font(scale, &lv_font_montserrat_10, LV_PART_INDICATOR);
  lv_obj_set_style_text_opa(scale, LV_OPA_50, LV_PART_INDICATOR);
  lv_obj_set_style_text_color(scale, LABEL_COLOR, LV_PART_INDICATOR);
  lv_obj_set_style_line_opa(scale, LV_OPA_TRANSP, LV_PART_INDICATOR);
  lv_obj_set_style_line_opa(scale, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_align_to(scale, chart, LV_ALIGN_OUT_BOTTOM_MID, 0, -15);

  // label
  lv_obj_t *chart_title = lv_label_create(saving_chart_box);
  lv_label_set_long_mode(chart_title, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_label_set_text(chart_title, "Poupanca                            ");
  lv_obj_add_style(chart_title, label_style, 0);
  lv_obj_set_width(chart_title, 145);
  lv_obj_align(chart_title, LV_ALIGN_TOP_MID, 2, 0);

  // legend aforro
  lv_obj_t *legend_aforro = lv_label_create(saving_chart_box);
  lv_label_set_text(legend_aforro, "aforro");
  lv_obj_add_style(legend_aforro, label_style, 0);
  lv_obj_set_style_text_color(legend_aforro, CHART_LINE1_COLOR, 0);
  lv_obj_set_style_text_font(legend_aforro, &lv_font_montserrat_10, 0);
  lv_obj_set_style_opa(legend_aforro, LV_OPA_70, 0);
  lv_obj_align(legend_aforro, LV_ALIGN_BOTTOM_RIGHT, -2, -10);

  // legend trade
  lv_obj_t *legend_trade = lv_label_create(saving_chart_box);
  lv_label_set_text(legend_trade, "trade");
  lv_obj_add_style(legend_trade, label_style, 0);
  lv_obj_set_style_text_color(legend_trade, CHART_LINE2_COLOR, 0);
  lv_obj_set_style_text_font(legend_trade, &lv_font_montserrat_10, 0);
  lv_obj_set_style_opa(legend_trade, LV_OPA_70, 0);
  lv_obj_align_to(legend_trade, legend_aforro, LV_ALIGN_OUT_TOP_MID, 0, 0);

  lv_chart_series_t *aforro =
      lv_chart_add_series(chart, CHART_LINE1_COLOR, LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_series_t *trade =
      lv_chart_add_series(chart, CHART_LINE2_COLOR, LV_CHART_AXIS_PRIMARY_Y);

  add_value_chart(chart, aforro, data->aforro_value);
  add_value_chart(chart, trade, data->trade_value);

  lv_chart_refresh(chart);
}

void bill_symbol_init(lv_obj_t *const symbol_obj,
                      const lv_style_t *const symbol_style,
                      const lv_font_t *const font, const char *const symbol,
                      const bool status) {
  lv_obj_add_style(symbol_obj, symbol_style, 0);
  lv_obj_set_style_text_font(symbol_obj, font, 0);
  lv_obj_set_style_text_color(symbol_obj, LABEL_COLOR, 0);
  lv_obj_set_style_opa(symbol_obj, status ? LV_OPA_COVER : LV_OPA_20, 0);
  lv_label_set_text(symbol_obj, symbol);
}

void bill_box_init(lv_obj_t *const bill_box,
                   const lv_obj_t *const saving_chart_box,
                   const lv_style_t *const box_style,
                   const lv_style_t *const label_style,
                   const saving_data *const data) {
  static int32_t column_dsc[] = {LV_GRID_FR(1),  // house
                                 LV_GRID_FR(1),  // car
                                 LV_GRID_FR(1),  // light
                                 LV_GRID_FR(1),  // wifi
                                 LV_GRID_FR(1),  // water
                                 LV_GRID_FR(1),  // dumbbell
                                 LV_GRID_FR(1),  // insurance
                                 LV_GRID_TEMPLATE_LAST};
  static int32_t row_dsc[] = {25, LV_GRID_TEMPLATE_LAST};

  lv_obj_set_scrollbar_mode(bill_box, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_style(bill_box, box_style, 0);
  lv_obj_set_style_size(bill_box, 145, 25, 0);
  lv_obj_set_style_pad_left(bill_box, 5, 0);
  lv_obj_set_style_pad_right(bill_box, 5, 0);
  lv_obj_set_grid_dsc_array(bill_box, column_dsc, row_dsc);
  lv_obj_align_to(bill_box, saving_chart_box, LV_ALIGN_OUT_TOP_MID, 0, -5);

  static lv_style_t symbol_style;
  lv_style_init(&symbol_style);
  bills_symbol_style_init(&symbol_style);

  lv_obj_t *house = lv_label_create(bill_box);
  bill_symbol_init(house, &symbol_style, &nerdfont, SYMBOL_HOUSE,
                   data->rent_status);
  lv_obj_t *car = lv_label_create(bill_box);
  bill_symbol_init(car, &symbol_style, &nerdfont, SYMBOL_CAR, data->car_status);
  lv_obj_t *light = lv_label_create(bill_box);
  bill_symbol_init(light, &symbol_style, &nerdfont, SYMBOL_LIGHT,
                   data->light_status);
  lv_obj_t *wifi = lv_label_create(bill_box);
  bill_symbol_init(wifi, &symbol_style, &nerdfont, SYMBOL_WIFI,
                   data->wifi_status);
  lv_obj_t *water = lv_label_create(bill_box);
  bill_symbol_init(water, &symbol_style, &fontawesome, SYMBOL_WATER,
                   data->water_status);
  lv_obj_t *dumbbell = lv_label_create(bill_box);
  bill_symbol_init(dumbbell, &symbol_style, &nerdfont, SYMBOL_KETTLEBELL,
                   data->gym_status);
  lv_obj_t *insurance = lv_label_create(bill_box);
  bill_symbol_init(insurance, &symbol_style, &nerdfont, SYMBOL_INSURANCE,
                   data->insurance_status);

  lv_obj_set_grid_cell(house, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER,
                       0, 1);
  lv_obj_set_grid_cell(car, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0,
                       1);
  lv_obj_set_grid_cell(light, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER,
                       0, 1);
  lv_obj_set_grid_cell(wifi, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_CENTER,
                       0, 1);
  lv_obj_set_grid_cell(water, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER,
                       0, 1);
  lv_obj_set_grid_cell(dumbbell, LV_GRID_ALIGN_CENTER, 5, 1,
                       LV_GRID_ALIGN_CENTER, 0, 1);
  lv_obj_set_grid_cell(insurance, LV_GRID_ALIGN_CENTER, 6, 1,
                       LV_GRID_ALIGN_CENTER, 0, 1);
}

lv_obj_t *savings_screen(const saving_data *data) {
  lv_obj_t *scr = lv_obj_create(NULL);

  // Background
  LV_IMAGE_DECLARE(background_dither);
  lv_obj_set_style_bg_img_src(scr, &background_dither, 0);

  // box style
  static lv_style_t box_style;
  lv_style_init(&box_style);
  box_style_init(&box_style);

  // subtitle style
  static lv_style_t subtitle_style;
  lv_style_init(&subtitle_style);
  subtitle_style_init(&subtitle_style);

  /************************ SPEND BOX *******************************/
  lv_obj_t *spend_box = lv_obj_create(scr);
  spend_box_init(spend_box, &box_style, &subtitle_style, data);

  /************************ SAVING CHART BOX ***************************/
  lv_obj_t *saving_box = lv_obj_create(scr);
  saving_chart_init(saving_box, &box_style, &subtitle_style, data);

  /************************ BILL  STATUS BOX ***************************/
  lv_obj_t *bill_box = lv_obj_create(scr);
  bill_box_init(bill_box, saving_box, &box_style, &subtitle_style, data);

  return scr;
}
