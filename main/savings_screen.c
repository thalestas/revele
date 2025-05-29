#include "savings_screen.h"
#include "core/lv_obj.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_scroll.h"
#include "core/lv_obj_style.h"
#include "core/lv_obj_style_gen.h"
#include "display/lv_display.h"
#include "draw/lv_draw_label.h"
#include "esp_lvgl_port.h"
#include "lv_api_map_v8.h"
#include "misc/lv_anim.h"
#include "misc/lv_area.h"
#include "misc/lv_color.h"
#include "misc/lv_event.h"
#include "misc/lv_palette.h"
#include "misc/lv_style.h"
#include "misc/lv_style_gen.h"
#include "widgets/bar/lv_bar.h"
#include "widgets/image/lv_image.h"

static void set_temp(void * bar, int32_t temp)
{
	    lv_bar_set_value((lv_obj_t *)bar, temp, LV_ANIM_ON);
}

static void event_cb(lv_event_t * e)
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
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
	lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_TEAL));
	lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
	lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);
	lv_style_set_radius(&style_indic, 3);

    lv_obj_t *bar = lv_bar_create(obj);
	lv_obj_add_event_cb(bar, event_cb, LV_EVENT_DRAW_MAIN_END, NULL);
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
	lv_style_set_bg_opa(style, LV_OPA_50);
	lv_style_set_bg_color(style, lv_palette_darken(LV_PALETTE_GREY,4));
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

void bar_label_style_init(lv_style_t *const style) 
{
	lv_style_set_text_color(style, lv_color_white());
	lv_style_set_opa(style, LV_OPA_40);
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

	//background(scr);
	LV_IMAGE_DECLARE(background_dither);
	lv_obj_set_style_bg_img_src(scr, &background_dither, 0);

	//Box
	static lv_style_t box_style;
	lv_style_init(&box_style);
	box_style_init(&box_style);

	lv_obj_t *box = lv_obj_create(scr);
	lv_obj_set_scrollbar_mode(box, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(box, &box_style, 0);
	lv_obj_set_style_size(box, 80, 100, 0);
	lv_obj_align(box, LV_ALIGN_BOTTOM_RIGHT, -2, -2);

	//subtitle
	static lv_style_t subtitle_style;
	lv_style_init(&subtitle_style);
	subtitle_style_init(&subtitle_style);

	lv_obj_t *title = lv_label_create(box);
	lv_label_set_text(title, " Poupanca");
	lv_obj_add_style(title, &subtitle_style, 0);
	lv_obj_align(title,LV_ALIGN_TOP_LEFT, 0, 0);

	//Box bar+label
	lv_obj_t *bar_box1 = lv_obj_create(box);
	lv_obj_set_scrollbar_mode(bar_box1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(bar_box1, &box_style, 0);
	lv_obj_set_style_bg_opa(bar_box1, LV_OPA_0, 0);
	lv_obj_set_style_size(bar_box1, 40, 80, 0);
	lv_obj_align(bar_box1, LV_ALIGN_BOTTOM_LEFT, 0, 0);

	lv_obj_t *bar_box2 = lv_obj_create(box);
	lv_obj_set_scrollbar_mode(bar_box2, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(bar_box2, &box_style, 0);
	lv_obj_set_style_bg_opa(bar_box2, LV_OPA_0, 0);
	lv_obj_set_style_size(bar_box2, 40, 80, 0);
	lv_obj_align(bar_box2, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

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

	lv_screen_load(scr);

	lvgl_port_unlock();
}
