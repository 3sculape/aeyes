#ifndef LIB_EXIF_H
#define LIB_EXIF_H

#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-log.h>
#include <libexif/exif-mem.h>

void trim_spaces(char* string);

void print_tag(ExifData* data, ExifIfd ifd, ExifTag tag);

void show_info(char* path);

#endif
