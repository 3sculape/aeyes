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

    GtkWidget *w_image_window;                    // Pointer to image widget
    GtkWidget *w_check_rotation_resize_to_fit;    // Pointer to resize to fit of rotation
    gchar     *image_path;                        // Image path to give to Pre-Processing

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

    GtkWidget *w_lbl_exif_capture_date;           // Pointer to capture date EXIF
    GtkWidget *w_lbl_exif_capture_time;           // Pointer to capture time EXIF
    GtkWidget *w_lbl_exif_size;                   // Pointer to size EXIF
    GtkWidget *w_lbl_exif_exposure;               // Pointer to exposure EXIF
    GtkWidget *w_lbl_exif_focal_length;           // Pointer to focal length EXIF
    GtkWidget *w_lbl_exif_iso;                    // Pointer to ISO EXIF
    GtkWidget *w_lbl_exif_model;                  // Pointer to camera model EXIF

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

    if(argc != 3)
        errx(3, "Usage : ./gtk <filename>");

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



    widgets->w_image_window = GTK_WIDGET(gtk_builder_get_object(builder,
            "image_window"));    
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

    widgets->texture = NULL;


    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show(window);


    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}





// -------------------------------------------------- //
// --------------- SIDE MENU FUNCTIONS -------------- //
// -------------------------------------------------- //

void on_btn_apply_wb_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_wb_spin_btn));

    g_print("Applying White Balance with %d value\n", quantity);
    
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    white_balance(surface, ((double)quantity)/100);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_tint_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_tint_spin_btn));

    g_print("Applying Tint with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tint(surface, ((double)quantity)/100);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_exposure_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_exposure_spin_btn));

    g_print("Applying Exposure with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    exposure(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_whites_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_whites_spin_btn));

    g_print("Applying Whites with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    whites(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_highlights_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_highlights_spin_btn));

    g_print("Applying Highlights with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    highlights(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_shadows_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_shadows_spin_btn));

    g_print("Applying Shadows with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    shadows(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_blacks_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_blacks_spin_btn));

    g_print("Applying Blacks with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    blacks(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_saturation_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_saturation_spin_btn));

    g_print("Applying Saturation with %d value\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    saturation(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
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
}


// LOAD BUTTON FUNCTION FOR THE IMAGE
void on_btn_open_activate(GtkMenuItem *btn_open, app_widgets *app_wdgts)
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

            if (app_wdgts->texture == NULL)
            {
                g_print("AH AH\n");
            }
            

            SDL_FreeSurface(surface);

            //---- LOAD EXIF ----//

            gchar date_str[100] = "10/05/2012";     // buffer for date string
            gchar time_str[100] = "23:42";     // buffer for time string
            gchar size_str[100] = "1920 x 1080";     // buffer for size string
            gchar exposure_str[100] = "1/320 at f/1.8"; // buffer for exposure string
            gchar focal_str[100] = "70mm";    // buffer for focal string
            gchar iso_str[100] = "800";      // buffer for iso string
            gchar model_str[100] = "Canon 6D mkII";    // buffer for model string

            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_capture_date), date_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_capture_time), time_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_size), size_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_exposure), exposure_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_focal_length), focal_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_iso), iso_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_model), model_str);
        }
    }

    // Finished with the "Open Image" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}


//-----------------------------------------------//
//------------ FILTERS AND FUNCTIONS ------------//
//-----------------------------------------------//


//------------ Sharpen ------------//

void on_btn_sharpen_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_sharpen);
}

void on_btn_cancel_sharpening_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_sharpen);
}

void on_btn_apply_sharpening_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn));

    g_print("Applying Sharpening with a strength of %d\n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    sharpen(surface, ((double)quantity)/100, 50);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_sharpen);
}


//------------ Mean Blur ------------//

void on_btn_mean_blur_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_mean_blur);
}

void on_btn_cancel_mean_blur_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_mean_blur);
}

void on_btn_apply_mean_blur_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn));

    g_print("Applying Mean Blur with a size of %d \n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    box_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_mean_blur);
}


//------------ Gaussian Blur ------------//

void on_btn_gaussian_blur_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_gaussian_blur);
}

void on_btn_cancel_gaussian_blur_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_gaussian_blur);
}

void on_btn_apply_gaussian_blur_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn));

    g_print("Applying Gaussian Blur with a size of %d \n", quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    gaussian_blur(surface, ((int)quantity), 5);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_gaussian_blur);
}


//------------ Directional Blur ------------//

void on_btn_direct_blur_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_direct_blur);
}

void on_btn_cancel_direct_blur_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_direct_blur);
}

void on_btn_apply_direct_blur_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint angle_quantity = 0;
    gint strength_quantity = 0;

    angle_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn));
    strength_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn));

    g_print("Applying Directional Blur with a size of %d and an angle of %d °\n", strength_quantity, angle_quantity);

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    motion_blur(surface, (int)strength_quantity, (double)angle_quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    g_print("Applying Directional Blur Finished\n");

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_direct_blur);
}


//------------ Resize ------------//

void on_btn_resize_activate(GtkMenuItem *button, app_widgets *app_wdgts)
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

    g_print("Applying Resizing with a new height of %d px and a new width of %d px\n", height_quantity, width_quantity);

    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_height_resize_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_width_resize_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_resize);
}


//------------ Rotation ------------//

void on_btn_rotation_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_rotation);
}

void on_btn_cancel_rotation_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_rotation);
}

void on_btn_apply_rotation_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn));

    g_print("Applying Rotation with an angle of %d °\n", quantity);

    gdouble reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_rotation);
}




void on_btn_binarization_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    binarization(surface, 50);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_negative_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    negative(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_deep_fry_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    Uint8 reference[3] = {200, 60, 20};
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    deep_fry(surface, reference);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_fever_dream_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    double strength = 6.66;
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = fever_dream(surface, strength);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);
}




//------------ About ------------//

void on_btn_about_activate(GtkMenuItem *button, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_about);
}

void on_btn_close_about_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_about);
}