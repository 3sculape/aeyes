#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <stdio.h>
#include "utils/lib_sdl.h"
#include "algos/basic.h"
#include "algos/convert.h"
#include "algos/color.h"
#include "utils/lib_exif.h"
#include "algos/unsharp_masking.h"
#include "algos/color_histogram.h"
#include "algos/whites.h"
#include "algos/blurs.h"
#include "algos/secret_sauce.h"


SDL_Window *sdl_window;
SDL_Renderer *sdl_renderer;
SDL_Texture *texture;

GError* error;
GtkDrawingArea *gtk_da;
void *gdk_window;
void *window_id;


typedef struct {
    GtkWidget *w_dlg_file_choose;                 // Pointer to file chooser dialog box
    GtkWidget *w_dlg_save_choose;                 // Pointer to save file chooser dialog box
    GtkWidget *w_dlg_about;                       // Pointer to the about dialog box

    GtkWidget *w_dlg_sharpen;                     // Pointer to sharpening dialog box
    GtkWidget *w_dlg_mean_blur;                   // Pointer to mean blur dialog box
    GtkWidget *w_dlg_gaussian_blur;               // Pointer to gaussian blur dialog box
    GtkWidget *w_dlg_direct_blur;                 // Pointer to directional blur dialog box
    GtkWidget *w_dlg_resize;                      // Pointer to resize dialog box
    GtkWidget *w_dlg_rotation;                    // Pointer to rotation dialog box
    GtkWidget *w_dlg_scale;                       // Pointer to scale dialog box

    GtkWidget *w_image_window;                    // Pointer to image widget
    GtkWidget *w_histo_window;                    // Pointer to histogram widget
    GtkWidget *w_check_rotation_resize_to_fit;    // Pointer to resize to fit of rotation
    gchar     *image_path;                        // Image path to give to Pre-Processing
    gchar     *save_path;                         // Image path to give to Pre-Processing
    GtkWidget *w_on_btn_save_clicked;             // Pointer to the save button of save window
    GtkWidget *w_on_btn_cancel_save_clicked;      // Pointer to the save button of save window

    GtkWidget *w_wb_spin_btn;                     // Pointer to White Balance Spin Button widget
    GtkWidget *w_tint_spin_btn;                   // Pointer to Tint Spin Button widget
    GtkWidget *w_exposure_spin_btn;               // Pointer to Exposure Spin Button widget
    GtkWidget *w_whites_spin_btn;                 // Pointer to Whites Spin Button widget
    GtkWidget *w_highlights_spin_btn;             // Pointer to Highlights Spin Button widget
    GtkWidget *w_shadows_spin_btn;                // Pointer to Shadows Spin Button widget
    GtkWidget *w_blacks_spin_btn;                 // Pointer to Blacks Spin Button widget
    GtkWidget *w_saturation_spin_btn;             // Pointer to Saturation Spin Button widget

    GtkWidget *w_strength_sharp_spin_btn;         // Pointer to strength of Sharpening Spin Button widget
    GtkWidget *w_size_mean_blur_spin_btn;         // Pointer to size of mean blur Spin Button widget
    GtkWidget *w_size_gaussian_blur_spin_btn;     // Pointer to size of gaussian blur Spin Button widget
    GtkWidget *w_angle_direct_blur_spin_btn;      // Pointer to angle of directional blur Spin Button widget
    GtkWidget *w_size_direct_blur_spin_btn;       // Pointer to size of directional blur Spin Button widget
    GtkWidget *w_height_resize_spin_btn;          // Pointer to new height of resize Spin Button widget
    GtkWidget *w_width_resize_spin_btn;           // Pointer to new width of resize Spin Button widget
    GtkWidget *w_angle_rotation_spin_btn;         // Pointer to angle of rotation Spin Button widget
    GtkWidget *w_factor_scale_spin_btn;           // Pointer to Scale Spin Button widget

    GtkWidget *w_lbl_exif_capture_date;           // Pointer to capture date EXIF
    GtkWidget *w_lbl_exif_capture_time;           // Pointer to capture time EXIF
    GtkWidget *w_lbl_exif_size;                   // Pointer to size EXIF
    GtkWidget *w_lbl_exif_exposure;               // Pointer to exposure EXIF
    GtkWidget *w_lbl_exif_focal_length;           // Pointer to focal length EXIF
    GtkWidget *w_lbl_exif_iso;                    // Pointer to ISO EXIF
    GtkWidget *w_lbl_exif_model;                  // Pointer to camera model EXIF

    GtkWidget *w_menu_edit;                       // Pointer to tool bar menu edit
    GtkWidget *w_menu_view;                       // Pointer to tool bar menu view
    GtkWidget *w_menu_filters;                    // Pointer to tool bar menu filters

    GtkWidget *w_btn_new;                         // Pointer to tool bar new file
    GtkWidget *w_btn_save_as;                     // Pointer to tool bar save as
    GtkWidget *w_btn_resize;                      // Pointer to tool bar resize
    GtkWidget *w_btn_preferences;                 // Pointer to tool bar preferences
    GtkWidget *w_btn_clipping_reveal;             // Pointer to tool bar clipping reveal
    GtkWidget *w_btn_fit_to_scale;                // Pointer to tool bar fit to scale
    GtkWidget *w_btn_median_blur;                 // Pointer to tool bar median blur
    GtkWidget *w_btn_edge_enhance;                // Pointer to tool bar edge enhancement
    GtkWidget *w_btn_sav;                        // Pointer to the save button of tool bar

    GtkWidget *w_btn_apply_grayscale;             // Pointer to apply button of grayscale
    GtkWidget *w_btn_apply_wb;                    // Pointer to apply button of wb
    GtkWidget *w_btn_apply_tint;                  // Pointer to apply button of tint
    GtkWidget *w_btn_apply_exposure;              // Pointer to apply button of exposure
    GtkWidget *w_btn_apply_whites;                // Pointer to apply button of whites
    GtkWidget *w_btn_apply_highlights;            // Pointer to apply button of highlights
    GtkWidget *w_btn_apply_shadows;               // Pointer to apply button of shadows
    GtkWidget *w_btn_apply_blacks;                // Pointer to apply button of blacks
    GtkWidget *w_btn_apply_saturation;            // Pointer to apply button of saturation

    SDL_Texture *texture;

} app_widgets;



int main(int argc, char *argv[])

{
    GtkBuilder *builder;
    GtkWidget *window;

    app_widgets *widgets = g_slice_new(app_widgets);

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    SDL_CreateWindowAndRenderer(1,1, 0, &sdl_window, &sdl_renderer);

    if(argc != 1)
        errx(3, "This program does not require arguments to run");

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));


    widgets->w_dlg_file_choose = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_file_choose"));
    widgets->w_dlg_save_choose = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_save_choose"));
    widgets->w_dlg_about = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_about"));
    widgets->w_on_btn_save_clicked = GTK_WIDGET(gtk_builder_get_object(builder,
            "on_btn_save_clicked"));


    widgets->w_dlg_sharpen = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_sharpen"));
    widgets->w_dlg_mean_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_mean_blur"));
    widgets->w_dlg_gaussian_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_gaussian_blur"));
    widgets->w_dlg_direct_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_direct_blur"));
    widgets->w_dlg_resize = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_resize"));
    widgets->w_dlg_rotation = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_rotation"));
    widgets->w_dlg_scale = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_scale"));



    widgets->w_image_window = GTK_WIDGET(gtk_builder_get_object(builder,
            "image_window"));
    widgets->w_histo_window = GTK_WIDGET(gtk_builder_get_object(builder,
            "histo_window"));
    widgets->w_wb_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "wb_spin_btn"));
    widgets->w_tint_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "tint_spin_btn"));
    widgets->w_exposure_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "exposure_spin_btn"));
    widgets->w_whites_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "whites_spin_btn"));
    widgets->w_highlights_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "highlights_spin_btn"));
    widgets->w_shadows_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "shadows_spin_btn"));
    widgets->w_blacks_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "blacks_spin_btn"));
    widgets->w_saturation_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "saturation_spin_btn"));

    widgets->w_strength_sharp_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "strength_sharp_spin_btn"));
    widgets->w_size_mean_blur_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "size_mean_blur_spin_btn"));
    widgets->w_size_gaussian_blur_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "size_gaussian_blur_spin_btn"));
    widgets->w_angle_direct_blur_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "angle_direct_blur_spin_btn"));
    widgets->w_size_direct_blur_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "size_direct_blur_spin_btn"));
    widgets->w_height_resize_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "height_resize_spin_btn"));
    widgets->w_width_resize_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "width_resize_spin_btn"));
    widgets->w_angle_rotation_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "angle_rotation_spin_btn"));
    widgets->w_factor_scale_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "factor_scale_spin_btn"));



    widgets->w_lbl_exif_capture_date = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_capture_date"));
    widgets->w_lbl_exif_capture_time = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_capture_time"));
    widgets->w_lbl_exif_size = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_size"));
    widgets->w_lbl_exif_exposure = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_exposure"));
    widgets->w_lbl_exif_focal_length = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_focal_length"));
    widgets->w_lbl_exif_iso = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_iso"));
    widgets->w_lbl_exif_model = GTK_WIDGET(gtk_builder_get_object(builder,
            "lbl_exif_model"));


    widgets->w_menu_edit = GTK_WIDGET(gtk_builder_get_object(builder,
            "menu_edit"));
    widgets->w_menu_view = GTK_WIDGET(gtk_builder_get_object(builder,
            "menu_view"));
    widgets->w_menu_filters = GTK_WIDGET(gtk_builder_get_object(builder,
            "menu_filters"));

    widgets->w_btn_new = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_new"));
    widgets->w_btn_save_as = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_save_as"));
    widgets->w_btn_resize = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_resize"));
    widgets->w_btn_preferences = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_preferences"));
    widgets->w_btn_clipping_reveal = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_clipping_reveal"));
    widgets->w_btn_fit_to_scale = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_fit_to_scale"));
    widgets->w_btn_median_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_median_blur"));
    widgets->w_btn_edge_enhance = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_edge_enhance"));
    widgets->w_check_rotation_resize_to_fit = GTK_WIDGET(gtk_builder_get_object(builder,
            "check_rotation_resize_to_fit"));
    widgets->w_btn_sav = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_sav"));



    widgets->w_btn_apply_grayscale = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_grayscale"));
    widgets->w_btn_apply_wb = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_wb"));
    widgets->w_btn_apply_tint = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_tint"));
    widgets->w_btn_apply_exposure = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_exposure"));
    widgets->w_btn_apply_whites = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_whites"));
    widgets->w_btn_apply_highlights = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_highlights"));
    widgets->w_btn_apply_shadows = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_shadows"));
    widgets->w_btn_apply_blacks = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_blacks"));
    widgets->w_btn_apply_saturation = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_saturation"));



    widgets->texture = NULL;


    gtk_widget_set_sensitive(widgets->w_menu_edit, FALSE);
    gtk_widget_set_sensitive(widgets->w_menu_view, FALSE);
    gtk_widget_set_sensitive(widgets->w_menu_filters, FALSE);

    gtk_widget_set_sensitive(widgets->w_btn_new, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_save_as, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_resize, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_preferences, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_clipping_reveal, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_fit_to_scale, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_median_blur, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_edge_enhance, FALSE);
    gtk_widget_set_sensitive(widgets->w_check_rotation_resize_to_fit, FALSE);


    gtk_widget_set_sensitive(widgets->w_btn_apply_grayscale, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_wb, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_tint, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_exposure, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_whites, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_highlights, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_shadows, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_blacks, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_saturation, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_sav, FALSE);


    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show(window);


    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}










//-----------------------------------------------//
//-------------- QUITTING FUNCTIONS -------------//
//-----------------------------------------------//

gboolean on_quit()
{
    quit(sdl_window, sdl_renderer, texture);
    gtk_main_quit();
    return True;
}

gboolean on_btn_quit_activate()
{
    quit(sdl_window, sdl_renderer, texture);
    gtk_main_quit();
    return True;
}

void on_window_main_destroy()
{
    quit(sdl_window, sdl_renderer, texture);
    gtk_main_quit();
}




void update_image(SDL_Surface *surface, app_widgets *app_wdgts)
{
    savePNG("./tmp.png", surface);
    SDL_DestroyTexture(app_wdgts->texture);
    app_wdgts->texture = surface_to_texture(surface, sdl_renderer);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
    show_l_histo(surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./l_histo.PNG");
}


void on_btn_refresh_texture_activate(GtkMenuItem *btn_open __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

}


void put_string1(char string[], char dest[])
{
    for(size_t i = 0; i < 1024 && string[i] != 0; i++)
    {
        dest[i] = string[i];
    }
}









// ---------------------------------------------- //
// ------------------- LOAD --------------------- //
// ---------------------------------------------- //

void on_btn_open_activate(GtkMenuItem *btn_open __attribute__((unused)), app_widgets *app_wdgts)
{
    // Show the "Open Image" dialog box
    gtk_widget_show(app_wdgts->w_dlg_file_choose);
    // Check return value from Open Image dialog box to see
    //if user clicked the Open button
    if (gtk_dialog_run(GTK_DIALOG(app_wdgts->w_dlg_file_choose))
            == GTK_RESPONSE_OK){
        // Get the file name from the dialog box
        app_wdgts->image_path =
            gtk_file_chooser_get_filename(
                    GTK_FILE_CHOOSER(app_wdgts->w_dlg_file_choose));
        if (app_wdgts->image_path != NULL) {
            gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window),
                    app_wdgts->image_path);

            SDL_Surface *surface = load(app_wdgts->image_path);
            app_wdgts->texture = surface_to_texture(surface, sdl_renderer);



            show_l_histo(surface);
            gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./l_histo.PNG");

            SDL_FreeSurface(surface);

            //---- LOAD EXIF ----//

            exif list_exif;
            exif_init(&list_exif);
            show_info(app_wdgts->image_path, &list_exif);


            gchar time_str[1024] = {0};     // buffer for time string
            put_string1(list_exif.date_time, time_str);
            gchar size_str[1024] = {0};     // buffer for size string
            put_string1(list_exif.maker_note, size_str);
            gchar exposure_str[1024] = {0}; // buffer for exposure string
            put_string1(list_exif.exposure_time, exposure_str);
            gchar focal_str[1024] = {0};    // buffer for focal string
            put_string1(list_exif.focal_length, focal_str);
            gchar iso_str[1024] = {0};      // buffer for iso string
            put_string1(list_exif.aperture_value, iso_str);
            gchar model_str[1024] = {0};    // buffer for model string
            put_string1(list_exif.camera_model, model_str);

            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_capture_time), time_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_size), size_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_exposure), exposure_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_focal_length), focal_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_iso), iso_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_model), model_str);

            exif_destroy(&list_exif);


            gtk_widget_set_sensitive(app_wdgts->w_menu_edit, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_menu_view, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_menu_filters, TRUE);

            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_grayscale, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_wb, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_tint, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_exposure, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_whites, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_highlights, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_shadows, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_blacks, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_saturation, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_sav, TRUE);
        }
    }

    // Finished with the "Open Image" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}


// ---------------------------------------------- //
// ----------------- SAVE ----------------------- //
// ---------------------------------------------- //

void on_btn_save_activate(GtkMenuItem *btn_open __attribute__((unused)), app_widgets *app_wdgts)
{
    // Show the "Open Image" dialog box
    gtk_widget_show(app_wdgts->w_dlg_save_choose);
    // Check return value from Open Image dialog box to see
    //if user clicked the Open button
    if (gtk_dialog_run(GTK_DIALOG(app_wdgts->w_dlg_save_choose))
            == GTK_RESPONSE_OK)
    {
        // Get the file name from the dialog box
        app_wdgts->save_path =
            gtk_file_chooser_get_filename(
                    GTK_FILE_CHOOSER(app_wdgts->w_dlg_save_choose));
        if (app_wdgts->save_path != NULL)
        {
            SDL_Surface *result = texture_to_surface(app_wdgts->texture, sdl_renderer);
            savePNG(app_wdgts->save_path, result);

            SDL_FreeSurface(result);
        }
    }
}

/* void on_btn_save_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_save_choose);
} */

void on_btn_cancel_save_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_save_choose);
}














// -------------------------------------------------- //
// --------------- SIDE MENU FUNCTIONS -------------- //
// -------------------------------------------------- //

void on_btn_apply_wb_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_wb_spin_btn));
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    white_balance(surface, ((double)quantity)/100);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_tint_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_tint_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tint(surface, ((double)quantity)/100);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_exposure_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_exposure_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    exposure(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_whites_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_whites_spin_btn));


    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    whites(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_highlights_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_highlights_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    highlights(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_shadows_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_shadows_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    shadows(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_blacks_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_blacks_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    blacks(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_saturation_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_saturation_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    saturation(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}




//-----------------------------------------------//
//------------ FILTERS AND FUNCTIONS ------------//
//-----------------------------------------------//


//------------ Sharpen ------------//

void on_btn_sharpen_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_sharpen);
}

void on_btn_cancel_sharpening_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_sharpen);
}

void on_btn_apply_sharpening_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = sharpen(surface, ((double)quantity)/100, 50);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_sharpen);
}


//------------ Mean Blur ------------//

void on_btn_mean_blur_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_mean_blur);
}

void on_btn_cancel_mean_blur_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_mean_blur);
}

void on_btn_apply_mean_blur_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    box_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_mean_blur);
}


//------------ Gaussian Blur ------------//

void on_btn_gaussian_blur_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_gaussian_blur);
}

void on_btn_cancel_gaussian_blur_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 2;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_gaussian_blur);
}

void on_btn_apply_gaussian_blur_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    fast_gaussian_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 2;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_gaussian_blur);
}


//------------ Directional Blur ------------//

void on_btn_direct_blur_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_direct_blur);
}

void on_btn_cancel_direct_blur_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gint reset_value_angle = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn), reset_value_angle);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_direct_blur);
}

void on_btn_apply_direct_blur_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint angle_quantity = 0;
    gint strength_quantity = 0;

    angle_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn));
    strength_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    motion_blur(surface, (int)strength_quantity, (double)angle_quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gint reset_value_angle = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn), reset_value_angle);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_direct_blur);
}


//------------ Resize ------------//

/* void on_btn_resize_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_resize);
}

void on_btn_cancel_resize_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_height_resize_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_width_resize_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_resize);
}

void on_btn_apply_resize_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint height_quantity = 0;
    gint width_quantity = 0;

    height_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_height_resize_spin_btn));
    width_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_width_resize_spin_btn));

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_height_resize_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_width_resize_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_resize);
} */


//------------ Rotation ------------//

void on_btn_rotation_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_rotation);
}

void on_btn_cancel_rotation_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_rotation);
}

void on_btn_apply_rotation_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn));


    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = rotate(surface, (double)quantity);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);


    gdouble reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_rotation);
}


//------------ Scale ------------//

void on_btn_scale_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_scale);
}

void on_btn_cancel_scale_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gdouble reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_factor_scale_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_scale);
}

void on_btn_apply_scale_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gdouble quantity = 0;

    quantity = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app_wdgts->w_factor_scale_spin_btn));


    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = resize(surface, (double)quantity);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);


    gdouble reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_factor_scale_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_scale);
}


//------------ Binarization ------------//

void on_btn_binarization_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    binarization(surface, 50);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


//------------ Negative ------------//

void on_btn_negative_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    negative(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


//------------ Grayscale ------------//

void on_btn_apply_grayscale_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    grayscale(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

}


//------------ Deep Fry ------------//

void on_btn_deep_fry_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    Uint8 reference[3] = {200, 60, 20};
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    deep_fry(surface, reference);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


//------------ Fever Dream ------------//

void on_btn_fever_dream_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    double strength = 6.66;
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = fever_dream(surface, strength);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);
}




//------------ About ------------//

void on_btn_about_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_about);
}

void on_btn_close_about_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_about);
}






// ------------------------------------------------ //
//--------------------- UNDO ---------------------- //
// ------------------------------------------------ //


void on_btn_undo_all_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), app_wdgts->image_path);

    SDL_Surface *surface = load(app_wdgts->image_path);
    app_wdgts->texture = surface_to_texture(surface, sdl_renderer);

    show_l_histo(surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./l_histo.PNG");

    SDL_FreeSurface(surface);
}





