/*******************************************************************************
 * Size: 16 px
 * Bpp: 4
 * Opts: --bpp 4 --size 16 --no-compress --font fa-solid-900.ttf --range 57350
 *--format lvgl -o fontawesome.c
 ******************************************************************************/

#include "lvgl.h"

#ifndef FONTAWESOME
#define FONTAWESOME 1
#endif

#if FONTAWESOME

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E006 "" */
    0x0,  0x0,  0x0,  0xcc, 0x0,  0x0,  0x0,  0x0,  0x0,  0xce, 0xca, 0xff,
    0xac, 0xec, 0x0,  0x0,  0x0,  0xcf, 0xdb, 0x88, 0xbd, 0xfc, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x66, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xff,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7f, 0xff, 0xf7, 0x0,  0x0,  0x0,
    0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb4, 0x0,  0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4,
    0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x0,  0x0,  0x6f, 0xff,
    0xf6, 0x3,  0xff, 0xff, 0x0,  0x0,  0x2,  0x66, 0x20, 0x0,  0xcf, 0xfc,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x6,  0x60, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xd,  0xd0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xc,  0xc0};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0,
     .adv_w = 0,
     .box_w = 0,
     .box_h = 0,
     .ofs_x = 0,
     .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0,
     .adv_w = 256,
     .box_w = 16,
     .box_h = 16,
     .ofs_x = 0,
     .ofs_y = -2}};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] = {
    {.range_start = 57350,
     .range_length = 1,
     .glyph_id_start = 1,
     .unicode_list = NULL,
     .glyph_id_ofs_list = NULL,
     .list_length = 0,
     .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY}};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_montserrat_16;

/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t fontawesome = {
#else
lv_font_t fontawesome = {
#endif
    .get_glyph_dsc =
        lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to get glyph's data*/
    .get_glyph_bitmap =
        lv_font_get_bitmap_fmt_txt, /*Function pointer to get glyph's bitmap*/
    .line_height = 16, /*The maximum line height required by the font*/
    .base_line = 2,    /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc, /*The custom font data. Will be accessed by
                         `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_16,
#endif
    .user_data = NULL,
};

#endif /*#if FONTAWESOME*/
