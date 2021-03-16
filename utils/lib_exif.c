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

void print_mnote_tag(ExifData* data, unsigned tag)
{
    ExifMnoteData *mn = exif_data_get_mnote_data(data);
    if (mn)
    {
        int num = exif_mnote_data_count(mn);
        int i;
        for (i = 0; i < num; i++)
        {
            char buffer[1024];
            if (exif_mnote_data_get_id(mn, i) == tag)
            {
                if (exif_mnote_data_get_value(mn, i, buffer, sizeof(buffer)))
                {
                    trim_spaces(buffer);
                    if (*buffer)
                    {
                        printf("%s: %s\n", exif_mnote_data_get_title(mn,i),
                                buffer);
                    }
                }
            }
        }
    }
}

void show_info(char* path)
{
    ExifData* data;
    ExifEntry* entry;

    data = exif_data_new_from_file(path);
    if (data == NULL)
    {
        warnx("Function fail or unreadable file");
        return;
    }
    print_tag(data, EXIF_IFD_0, EXIF_TAG_ARTIST);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_XP_AUTHOR);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_COPYRIGHT);
    print_tag(data, EXIF_IFD_EXIF, EXIF_TAG_EXPOSURE_TIME);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_MODEL);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_DATE_TIME);
    print_tag(data, EXIF_IFD_EXIF, EXIF_TAG_FOCAL_LENGTH);
    print_tag(data, EXIF_IFD_EXIF, EXIF_TAG_APERTURE_VALUE);

    print_tag(data, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT);
    print_tag(data, EXIF_IFD_0, EXIF_TAG_IMAGE_DESCRIPTION);
    print_tag(data, EXIF_IFD_1, EXIF_TAG_IMAGE_DESCRIPTION);

    entry = exif_content_get_entry(data->ifd[EXIF_IFD_0], EXIF_TAG_MAKE);
    if (entry != NULL)
    {
        char buffer[64];
        if(exif_entry_get_value(entry, buffer, sizeof(buffer)))
        {
            trim_spaces(buffer);
            if(!strcmp(buffer, "Canon"))
                print_mnote_tag(data, 9);
        }
    }

    exif_data_unref(data);
}
