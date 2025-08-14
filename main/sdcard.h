#pragma once

#define SDCARD_PATH "/sdcard"

void init_sdcard();
void list_dir(const char *path);
void get_next_img_path(const char *img);
