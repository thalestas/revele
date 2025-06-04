#include "savings_screen.h"
#include "core/lv_obj.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_scroll.h"
#include "core/lv_obj_style.h"
#include "core/lv_obj_style_gen.h"
#include "display/lv_display.h"
#include "draw/lv_draw_label.h"
#include "esp_lvgl_port.h"
#include "font/lv_font.h"
#include "layouts/grid/lv_grid.h"
#include "lv_api_map_v8.h"
#include "misc/lv_anim.h"
#include "misc/lv_area.h"
#include "misc/lv_color.h"
#include "misc/lv_event.h"
#include "misc/lv_palette.h"
#include "misc/lv_style.h"
#include "misc/lv_style_gen.h"
#include "misc/lv_types.h"
#include "widgets/bar/lv_bar.h"
#include "widgets/chart/lv_chart.h"
#include "widgets/image/lv_image.h"
#include "widgets/scale/lv_scale.h"

static void set_temp(void * bar, int32_t temp)
{
	    lv_bar_set_value((lv_obj_t *)bar, temp, LV_ANIM_ON);
}

static void bar_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target_obj(e);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.font = LV_FONT_DEFAULT;

    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_bar_get_value(obj));

    lv_point_t txt_size;
    lv_text_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX,
                     label_dsc.flag);

    lv_area_t txt_area;
    txt_area.x1 = 0;
    txt_area.x2 = txt_size.x - 1;
    txt_area.y1 = 0;
    txt_area.y2 = txt_size.y - 1;

    lv_area_t empty_area;
    lv_obj_get_coords(obj, &empty_area);
	lv_area_set_height(&empty_area, lv_area_get_height(&empty_area) * (100-lv_bar_get_value(obj)) / 100);

    /*If the empty bar space is long enough put the text outside on the top*/
    if(lv_area_get_height(&empty_area) > txt_size.y + 15) {
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
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_draw_label(layer, &label_dsc, &txt_area);
}

lv_obj_t *create_bar(lv_obj_t *obj, int32_t initial_value)
{
	//Bar style
    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, BAR_OPACITY);
	lv_style_set_bg_color(&style_indic, BAR_BG_COLOR);
	lv_style_set_bg_grad_color(&style_indic, BAR_GRAD_COLOR);
	lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);
	lv_style_set_radius(&style_indic, 3);

    lv_obj_t *bar = lv_bar_create(obj);
	lv_obj_add_event_cb(bar, bar_event_cb, LV_EVENT_DRAW_MAIN_END, NULL);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
	lv_obj_set_style_radius(bar, 3, LV_PART_MAIN);
    lv_bar_set_range(bar, 0, 100);
    lv_obj_set_size(bar, 30, 80);
	lv_obj_set_style_margin_all(bar, 0, 0);

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_exec_cb(&anim, set_temp);
    lv_anim_set_duration(&anim, 1500);
    lv_anim_set_var(&anim, bar);
    lv_anim_set_values(&anim, 0, initial_value);
    lv_anim_set_repeat_count(&anim, 1);
    lv_anim_start(&anim);

	return bar;
}

void box_style_init(lv_style_t *const style)
{
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

void subtitle_style_init(lv_style_t *const style)
{
	lv_style_set_text_color(style, lv_color_white());
	lv_style_set_text_font(style, &lv_font_montserrat_12);
	lv_style_set_border_width(style, 0);
	lv_style_set_outline_width(style, 0);
	lv_style_set_pad_all(style, 0);
}

void symbol_style_init(lv_style_t *const style)
{
	lv_style_set_text_color(style, lv_color_white());
	lv_style_set_radius(style, 5);
	lv_style_set_border_width(style, 0);
	lv_style_set_border_color(style, lv_color_white());
	lv_style_set_outline_width(style, 0);
	lv_style_set_pad_all(style, 2);
}

void bar_label_style_init(lv_style_t *const style) 
{
	lv_style_set_text_color(style, lv_color_white());
	lv_style_set_opa(style, LV_OPA_50);
	lv_style_set_text_font(style, &lv_font_montserrat_12);
	lv_style_set_transform_rotation(style, -900);
	lv_style_set_transform_pivot_x(style, 8);
	lv_style_set_transform_pivot_y(style, 8);
	lv_style_set_border_width(style, 0);
	lv_style_set_pad_all(style, 0);
}

void savings_screen(uint32_t start_value1, uint32_t start_value2) 
{
	lvgl_port_lock(0);

	lv_obj_t *scr = lv_obj_create(NULL);

	LV_IMAGE_DECLARE(background_dither);
	lv_obj_set_style_bg_img_src(scr, &background_dither, 0);

	/************************ SAVING BOX *******************************/
	//Box
	static lv_style_t box_style;
	lv_style_init(&box_style);
	box_style_init(&box_style);

	lv_obj_t *saving_box = lv_obj_create(scr);
	lv_obj_set_scrollbar_mode(saving_box, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(saving_box, &box_style, 0);
	lv_obj_set_style_size(saving_box, 80, 100, 0);
	lv_obj_align(saving_box, LV_ALIGN_BOTTOM_RIGHT, -5, -5);

	//subtitle
	static lv_style_t subtitle_style;
	lv_style_init(&subtitle_style);
	subtitle_style_init(&subtitle_style);

	lv_obj_t *title = lv_label_create(saving_box);
	lv_label_set_text(title, " Poupanca");
	lv_obj_add_style(title, &subtitle_style, 0);
	lv_obj_align(title,LV_ALIGN_TOP_LEFT, 0, 0);

	//Box bar+label
	lv_obj_t *bar_box1 = lv_obj_create(saving_box);
	lv_obj_set_scrollbar_mode(bar_box1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(bar_box1, &box_style, 0);
	lv_obj_set_style_bg_opa(bar_box1, LV_OPA_0, 0);
	lv_obj_set_style_size(bar_box1, 40, 80, 0);
	lv_obj_align(bar_box1, LV_ALIGN_BOTTOM_LEFT, 0, -2);

	lv_obj_t *bar_box2 = lv_obj_create(saving_box);
	lv_obj_set_scrollbar_mode(bar_box2, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(bar_box2, &box_style, 0);
	lv_obj_set_style_bg_opa(bar_box2, LV_OPA_0, 0);
	lv_obj_set_style_size(bar_box2, 40, 80, 0);
	lv_obj_align(bar_box2, LV_ALIGN_BOTTOM_RIGHT, 0, -2);

	//bars
	lv_obj_t *bar = create_bar(bar_box1, start_value1);
	lv_obj_align(bar, LV_ALIGN_CENTER, 0, 0);
	lv_obj_t *bar2 = create_bar(bar_box2, start_value2);
	lv_obj_align(bar2, LV_ALIGN_CENTER, 0, 0);

	//bars labels
	static lv_style_t bar_label_style;
	lv_style_init(&bar_label_style);
	bar_label_style_init(&bar_label_style);

	lv_obj_t *aforro = lv_label_create(bar_box1);
	lv_label_set_text(aforro, "aforro");
	lv_obj_add_style(aforro, &bar_label_style, 0);
	lv_obj_align_to(aforro, bar, LV_ALIGN_BOTTOM_LEFT, 15, -4);

	lv_obj_t *trade_rep = lv_label_create(bar_box2);
	lv_label_set_text(trade_rep, "trade rep");
	lv_obj_add_style(trade_rep, &bar_label_style, 0);
	lv_obj_align(trade_rep, LV_ALIGN_BOTTOM_LEFT, 20, -4);

	/************************ SPEND STATUS BOX ***************************/

	lv_obj_t *spend_box = lv_obj_create(scr);
	lv_obj_set_scrollbar_mode(spend_box, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(spend_box, &box_style, 0);
	lv_obj_set_style_size(spend_box, 145, 70, 0);
	lv_obj_align(spend_box, LV_ALIGN_BOTTOM_LEFT, 5, -5);

	lv_obj_t *chart = lv_chart_create(spend_box);
	lv_obj_set_size(chart, 141, 66);
	lv_obj_center(chart);
	lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
	lv_obj_set_style_bg_opa(chart, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_chart_set_div_line_count(chart, 4, 12);
	lv_obj_set_style_border_width(chart, 0, LV_PART_MAIN);
	lv_obj_set_style_line_color(chart, lv_palette_darken(LV_PALETTE_BLUE_GREY, 3), LV_PART_MAIN);
	lv_obj_set_style_line_opa(chart, LV_OPA_50, LV_PART_MAIN);
	lv_obj_set_style_line_width(chart, 1, LV_PART_ITEMS);
	lv_obj_set_style_size(chart, 3, 3, LV_PART_INDICATOR);

	lv_obj_t *scale_bottom = lv_scale_create(spend_box);
	lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
	lv_obj_set_size(scale_bottom, 141, 20);
	lv_scale_set_total_tick_count(scale_bottom, 12);
	lv_scale_set_major_tick_every(scale_bottom, 1);
	lv_obj_set_style_pad_hor(scale_bottom, lv_chart_get_first_point_center_offset(chart), 0);
	static const char* month[] = {"J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D"};
	lv_scale_set_text_src(scale_bottom, month);
	lv_obj_set_style_text_font(scale_bottom, &lv_font_montserrat_10, LV_PART_INDICATOR);
	lv_obj_set_style_text_opa(scale_bottom, LV_OPA_50, LV_PART_INDICATOR);
	lv_obj_set_style_text_color(scale_bottom, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), LV_PART_INDICATOR);
	lv_obj_set_style_line_opa(scale_bottom, LV_OPA_TRANSP, LV_PART_INDICATOR);
	lv_obj_set_style_line_opa(scale_bottom, LV_OPA_TRANSP, LV_PART_MAIN);
	lv_obj_align_to(scale_bottom, chart, LV_ALIGN_OUT_BOTTOM_MID, 0, -15);

	lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_darken(LV_PALETTE_BLUE, 2), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_darken(LV_PALETTE_ORANGE, 2), LV_CHART_AXIS_PRIMARY_Y);
	int32_t i;
	for(i=0; i<10; i++) 
	{
		lv_chart_set_next_value(chart, ser1, (int32_t)lv_rand(50, 90));
		lv_chart_set_next_value(chart, ser2, (int32_t)lv_rand(10, 50));
	}

	lv_chart_refresh(chart);

	/************************ BILL  STATUS BOX ***************************/

	//static int32_t column_dsc[] = {20, 20, LV_GRID_TEMPLATE_LAST};
	static int32_t column_dsc[] = {
		LV_GRID_FR(1), // house
		LV_GRID_FR(1), // car
		LV_GRID_FR(1), // light
		LV_GRID_FR(1), // wifi
		LV_GRID_FR(1), // water
		LV_GRID_FR(1), // dumbbell
		LV_GRID_FR(1), // insurance
		LV_GRID_TEMPLATE_LAST
	};
	static int32_t row_dsc[] = {25, LV_GRID_TEMPLATE_LAST};

	lv_obj_t *bill_box = lv_obj_create(scr);
	lv_obj_set_scrollbar_mode(bill_box, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(bill_box, &box_style, 0);
	lv_obj_set_style_size(bill_box, 145, 25, 0);
	lv_obj_set_style_pad_left(bill_box, 5, 0);
	lv_obj_set_style_pad_right(bill_box, 5, 0);
	lv_obj_set_grid_dsc_array(bill_box, column_dsc, row_dsc);
	lv_obj_align_to(bill_box, spend_box,  LV_ALIGN_OUT_TOP_MID, 0, -5);

	static lv_style_t symbol_style;
	lv_style_init(&symbol_style);
	symbol_style_init(&symbol_style);

	lv_obj_t *house = lv_label_create(bill_box);
	lv_obj_add_style(house, &symbol_style, 0);
	lv_obj_set_style_text_font(house, &fontawesome, 0);
	lv_obj_set_style_text_color(house, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_label_set_text(house, SYMBOL_HOUSE);

	lv_obj_t *car = lv_label_create(bill_box);
	lv_obj_add_style(car, &symbol_style, 0);
	lv_obj_set_style_text_font(car, &fontawesome, 0);
	lv_obj_set_style_text_color(car, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_label_set_text(car, SYMBOL_CAR);

	lv_obj_t *light = lv_label_create(bill_box);
	lv_obj_add_style(light, &symbol_style, 0);
	lv_obj_set_style_text_font(light, &fontawesome, 0);
	lv_obj_set_style_text_color(light, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_obj_set_style_opa(light, LV_OPA_20, 0);
	lv_label_set_text(light, SYMBOL_LIGHT);

	lv_obj_t *wifi = lv_label_create(bill_box);
	lv_obj_add_style(wifi, &symbol_style, 0);
	lv_obj_set_style_text_font(wifi, &fontawesome, 0);
	lv_obj_set_style_text_color(wifi, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_label_set_text(wifi, SYMBOL_WIFI);

	lv_obj_t *water = lv_label_create(bill_box);
	lv_obj_add_style(water, &symbol_style, 0);
	lv_obj_set_style_text_font(water, &fontawesome, 0);
	lv_obj_set_style_text_color(water, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_obj_set_style_opa(water, LV_OPA_20, 0);
	lv_label_set_text(water, SYMBOL_WATER);

	lv_obj_t *dumbbell = lv_label_create(bill_box);
	lv_obj_add_style(dumbbell, &symbol_style, 0);
	lv_obj_set_style_text_font(dumbbell, &fontawesome, 0);
	lv_obj_set_style_text_color(dumbbell, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_label_set_text(dumbbell, SYMBOL_DUMBBELL);

	lv_obj_t *insurance = lv_label_create(bill_box);
	lv_obj_add_style(insurance, &symbol_style, 0);
	lv_obj_set_style_text_font(insurance, &fontawesome, 0);
	lv_obj_set_style_text_color(dumbbell, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3), 0);
	lv_obj_set_style_opa(insurance, LV_OPA_20, 0);
	lv_label_set_text(insurance, SYMBOL_INSURANCE);

	lv_obj_set_grid_cell(house, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_set_grid_cell(car, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_set_grid_cell(light, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_set_grid_cell(wifi, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_set_grid_cell(water, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_set_grid_cell(dumbbell, LV_GRID_ALIGN_CENTER, 5, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_set_grid_cell(insurance, LV_GRID_ALIGN_CENTER, 6, 1, LV_GRID_ALIGN_CENTER, 0, 1);

	lv_screen_load(scr);

	lvgl_port_unlock();
}
