#include <stdio.h>
#include <stdlib.h>
#include "lib_exif.h"
#include <err.h>
#include <string.h>

void trim_spaces(char* string)
{
    char* current = string - 1;
    for(; *string; ++string)
    {
        if(*string != ' ')
            current = string;
    }
    *++current = 0;
}

void print_tag(ExifData* data, ExifIfd ifd, ExifTag tag)
{
    ExifEntry* entry = exif_content_get_entry(data->ifd[ifd], tag);
    if (entry)
    {
        char buffer[1024];

        exif_entry_get_value(entry, buffer, sizeof(buffer));

        trim_spaces(buffer);
        if(*buffer)
            printf("%s: %s\n", exif_tag_get_name_in_ifd(tag, ifd), buffer);
    }
}

void show_info(char* path)
{
    ExifData* data;

    data = exif_data_new_from_file(path);
    if (data == NULL)
    {
        warnx("Function fail or unreadable file");
        return;
    }
    print_tag(data, EXIF_IFD_0, EXIF_TAG_ARTIST);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_XP_AUTHOR);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_COPYRIGHT);

    print_tag(data, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_IMAGE_DESCRIPTION);
    print_tag(data, EXIF_IFD_1, EXIF_TAG_IMAGE_DESCRIPTION);
}
