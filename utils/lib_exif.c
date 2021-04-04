#include <stdio.h>
#include <stdlib.h>
#include "lib_exif.h"
#include <err.h>
#include <string.h>

void put_string(char string[], char dest[])
{
    for(size_t i = 0; i < 1024 && string[i] != 0; i++)
    {
        dest[i] = string[i];
    }
}

void exif_init(exif* e)
{
    e->artist_name = calloc(1024, sizeof(char));
    e->camera_model = calloc(1024, sizeof(char));
    e->xp_author = calloc(1024, sizeof(char));
    e->exposure_time = calloc(1024, sizeof(char));
    e->date_time = calloc(1024, sizeof(char));
    e->copyright_tag = calloc(1024, sizeof(char));
    e->focal_length = calloc(1024, sizeof(char));
    e->aperture_value = calloc(1024, sizeof(char));
    e->user_comment = calloc(1024, sizeof(char));
    e->image_description = calloc(1024, sizeof(char));
    e->maker_note = calloc(1024, sizeof(char));
}

void exif_destroy(exif* e)
{
    free(e->artist_name);
    free(e->camera_model);
    free(e->xp_author);
    free(e->exposure_time);
    free(e->date_time);
    free(e->copyright_tag);
    free(e->focal_length);
    free(e->aperture_value);
    free(e->user_comment);
    free(e->image_description);
    free(e->maker_note);
}

/*void exif_reset(exif* e)
{
    char nul[1024] = {0};
    put_string(nul, e->artist_name);
    put_string(nul, e->camera_model);
    put_string(nul, e->xp_author);
    put_string(nul, e->exposure_time);
    put_string(nul, e->date_time);
    put_string(nul, e->copyright_tag);
    put_string(nul, e->focal_length);
    put_string(nul, e->aperture_value);
    put_string(nul, e->user_comment);
    put_string(nul, e->image_description);
    put_string(nul, e->maker_note);

}
*/
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

void get_tag(ExifData* data, exif* e, ExifIfd ifd, ExifTag tag)
{
    ExifEntry* entry = exif_content_get_entry(data->ifd[ifd], tag);
    if (entry)
    {
        char buffer[1024];

        exif_entry_get_value(entry, buffer, sizeof(buffer));

        trim_spaces(buffer);
        if(*buffer)
        {
            switch(tag)
            {
                case EXIF_TAG_ARTIST:
                    put_string(buffer,e->artist_name);
                    break;
                case EXIF_TAG_MODEL:
                    put_string(buffer, e->camera_model);
                    break;
                case EXIF_TAG_XP_AUTHOR:
                    put_string(buffer, e->xp_author);
                    break;
                case EXIF_TAG_EXPOSURE_TIME:
                    put_string(buffer, e->exposure_time);
                    break;
                case EXIF_TAG_COPYRIGHT:
                    put_string(buffer, e->copyright_tag);
                    break;
                case EXIF_TAG_DATE_TIME:
                    put_string(buffer, e->date_time);
                    break;
                case EXIF_TAG_FOCAL_LENGTH:
                    put_string(buffer, e->focal_length);
                    break;
                case EXIF_TAG_APERTURE_VALUE:
                    put_string(buffer, e->aperture_value);
                    break;
                case EXIF_TAG_USER_COMMENT:
                    put_string(buffer, e->user_comment);
                    break;
                case EXIF_TAG_IMAGE_DESCRIPTION:
                    put_string(buffer, e->image_description);
                    break;
                default:
                    warnx("Switch tag fail");
                    break;
            }
        }
    }
}

void get_mnote_tag(ExifData* data, exif* e, unsigned tag)
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
                        put_string(buffer, e->maker_note);
                }
            }
        }
    }
}

void show_info(char* path, exif* e)
{
    ExifData* data;
    ExifEntry* entry;

    data = exif_data_new_from_file(path);
    if (data == NULL)
    {
        warnx("Function fail or unreadable file");
        return;
    }
    get_tag(data, e, EXIF_IFD_0, EXIF_TAG_ARTIST);
    get_tag(data, e, EXIF_IFD_0, EXIF_TAG_XP_AUTHOR);
    get_tag(data, e, EXIF_IFD_0, EXIF_TAG_COPYRIGHT);
    get_tag(data, e, EXIF_IFD_EXIF, EXIF_TAG_EXPOSURE_TIME);
    get_tag(data, e, EXIF_IFD_0, EXIF_TAG_MODEL);
    get_tag(data, e, EXIF_IFD_0, EXIF_TAG_DATE_TIME);
    get_tag(data, e, EXIF_IFD_EXIF, EXIF_TAG_FOCAL_LENGTH);
    get_tag(data, e, EXIF_IFD_EXIF, EXIF_TAG_APERTURE_VALUE);

    get_tag(data, e, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT);
    get_tag(data, e, EXIF_IFD_0, EXIF_TAG_IMAGE_DESCRIPTION);
    get_tag(data, e, EXIF_IFD_1, EXIF_TAG_IMAGE_DESCRIPTION);

    entry = exif_content_get_entry(data->ifd[EXIF_IFD_0], EXIF_TAG_MAKE);
    if (entry != NULL)
    {
        char buffer[64];
        if(exif_entry_get_value(entry, buffer, sizeof(buffer)))
        {
            trim_spaces(buffer);
            if(!strcmp(buffer, "Canon"))
                get_mnote_tag(data, e, 9);
        }
    }

    exif_data_unref(data);
}
