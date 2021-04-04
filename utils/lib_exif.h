#ifndef LIB_EXIF_H
#define LIB_EXIF_H

#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-log.h>
#include <libexif/exif-mem.h>

typedef struct exif
{
    char* artist_name;
    char* camera_model;
    char* xp_author;
    char* exposure_time;
    char* date_time;
    char* copyright_tag;
    char* focal_length;
    char* aperture_value;
    char* user_comment;
    char* image_description;
    char* maker_note;
} exif;

void exif_init(exif* e);

void exif_destroy(exif* e);

//void exif_reset(exif* e);

void trim_spaces(char* string);

void get_tag(ExifData* data, exif* e, ExifIfd ifd, ExifTag tag);

void show_info(char* path, exif* e);

#endif
