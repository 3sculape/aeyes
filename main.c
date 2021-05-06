#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <stdio.h>
#include <time.h>
#include "utils/lib_sdl.h"
#include "utils/stack.h"
#include "algos/basic.h"
#include "algos/convert.h"
#include "algos/color.h"
#include "utils/lib_exif.h"
#include "algos/unsharp_masking.h"
#include "algos/color_histogram.h"
#include "algos/whites.h"
#include "algos/blurs.h"
#include "algos/effects.h"
#include "algos/secret_sauce.h"
#include "algos/tsl.h"
#include "algos/trailing.h"
#include "algos/vignette.h"
#include "algos/noise.h"
#include "algos/canny_edge_detector.h"
#include "algos/seam_carving.h"


SDL_Window *sdl_window;
SDL_Renderer *sdl_renderer;
SDL_Texture *texture;

GError* error;
GtkDrawingArea *gtk_da;
void *gdk_window;
void *window_id;
stack *undo_stack;
stack *redo_stack;

// ----- USER DATA STRUCT ----- //

typedef struct {

    //--- Dialog Boxes --- //
    GtkWidget *w_dlg_file_choose;                 // Pointer to file chooser dialog box
    GtkWidget *w_dlg_save_choose;                 // Pointer to save file chooser dialog box
    GtkWidget *w_dlg_about;                       // Pointer to the about dialog box
    GtkWidget *w_dlg_lut_choose;                  // Pointer to lut chooser dialog box

    GtkWidget *w_dlg_sharpen;                     // Pointer to sharpening dialog box
    GtkWidget *w_dlg_mean_blur;                   // Pointer to mean blur dialog box
    GtkWidget *w_dlg_median_blur;                 // Pointer to median blur dialog box
    GtkWidget *w_dlg_gaussian_blur;               // Pointer to gaussian blur dialog box
    GtkWidget *w_dlg_radial_blur;                 // Pointer to radial blur dialog box
    GtkWidget *w_dlg_direct_blur;                 // Pointer to directional blur dialog box
    GtkWidget *w_dlg_fast_blur;                   // Pointer to fast blur dialog box
    GtkWidget *w_dlg_resize;                      // Pointer to resize dialog box
    GtkWidget *w_dlg_crop;                        // Pointer to crop dialog box
    GtkWidget *w_dlg_rotation;                    // Pointer to rotation dialog box
    GtkWidget *w_dlg_scale;                       // Pointer to scale dialog box
    GtkWidget *w_dlg_binarization;                // Pointer to binarization dialog box
    GtkWidget *w_dlg_colorize;                    // Pointer to colorize dialog box
    GtkWidget *w_dlg_global_trailing;             // Pointer to Global Trailing dialog box
    GtkWidget *w_dlg_edge_trailing;               // Pointer to Edge Trailing dialog box
    GtkWidget *w_dlg_gradient_colorize;           // Pointer to gradient colorize dialog box
    GtkWidget *w_dlg_vignette;                    // Pointer to vignette dialog box
    GtkWidget *w_dlg_twist;                       // Pointer to twist dialog box
    GtkWidget *w_dlg_swirl;                       // Pointer to swirl dialog box
    GtkWidget *w_dlg_surface_blur;                // Pointer to surface blur dialog box

    //--- Windows --- //
    GtkWidget *w_image_window;                    // Pointer to image widget
    GtkWidget *w_histo_window;                    // Pointer to histogram widget
    GtkWidget *w_img_gradient_colorize;           // Pointer to gradient image widget
    GtkWidget *w_left_clip_warning;               // Pointer to warning icon of left clipping
    GtkWidget *w_right_clip_warning;              // Pointer to warning icon of right clipping

    //--- Adjustments --- //
    GtkWidget *w_height_adjustment_crop;          // Pointer to crop height adjustment widget
    GtkWidget *w_width_adjustment_crop;           // Pointer to crop width adjustment widget
    GtkWidget *w_x_adjustment_crop;               // Pointer to crop x adjustment widget
    GtkWidget *w_y_adjustment_crop;               // Pointer to crop y adjustment widget
    GtkWidget *w_start_pixel_global_trailing_adjustment; // Pointer to global trailing adjustment widget

    //--- Check Buttons --- //
    GtkWidget *w_check_rotation_resize_to_fit;    // Pointer to resize to fit of rotation
    GtkWidget *w_check_colorize_preserve_luminosity; // Pointer to keep luminance in colorization
    GtkWidget *w_global_trailing_inverse_check_btn; // Pointer to global trailing inverse direction check button
    GtkWidget *w_edge_trailing_inverse_check_btn; // Pointer to inverse edge trailing check button

    //--- Paths --- //
    gchar     *image_path;                        // Image path to give to Pre-Processing
    gchar     *save_path;                         // Image path to give to Pre-Processing
    gchar     *lut_path;                          // Image path to give to LUT Apply function


    GtkWidget *w_on_btn_save_clicked;             // Pointer to the save button of save window
    GtkWidget *w_on_btn_cancel_save_clicked;      // Pointer to the save button of save window

    //--- Spin Buttons --- //
    GtkWidget *w_wb_spin_btn;                     // Pointer to White Balance Spin Button widget
    GtkWidget *w_tint_spin_btn;                   // Pointer to Tint Spin Button widget
    GtkWidget *w_exposure_spin_btn;               // Pointer to Exposure Spin Button widget
    GtkWidget *w_whites_spin_btn;                 // Pointer to Whites Spin Button widget
    GtkWidget *w_highlights_spin_btn;             // Pointer to Highlights Spin Button widget
    GtkWidget *w_shadows_spin_btn;                // Pointer to Shadows Spin Button widget
    GtkWidget *w_blacks_spin_btn;                 // Pointer to Blacks Spin Button widget
    GtkWidget *w_saturation_spin_btn;             // Pointer to Saturation Spin Button widget
    GtkWidget *w_contrast_spin_btn;               // Pointer to Contrast Spin Button widget

    GtkWidget *w_strength_sharp_spin_btn;         // Pointer to strength of Sharpening Spin Button widget
    GtkWidget *w_size_mean_blur_spin_btn;         // Pointer to size of mean blur Spin Button widget
    GtkWidget *w_size_median_blur_spin_btn;       // Pointer to size of mean blur Spin Button widget
    GtkWidget *w_size_gaussian_blur_spin_btn;     // Pointer to size of gaussian blur Spin Button widget
    GtkWidget *w_size_fast_blur_spin_btn;         // Pointer to size of fast blur Spin Button widget
    GtkWidget *w_size_radial_blur_spin_btn;       // Pointer to radial blur spin button widget
    GtkWidget *w_angle_direct_blur_spin_btn;      // Pointer to angle of directional blur Spin Button widget
    GtkWidget *w_size_direct_blur_spin_btn;       // Pointer to size of directional blur Spin Button widget
    GtkWidget *w_height_resize_spin_btn;          // Pointer to new height of resize Spin Button widget
    GtkWidget *w_width_resize_spin_btn;           // Pointer to new width of resize Spin Button widget
    GtkWidget *w_angle_rotation_spin_btn;         // Pointer to angle of rotation Spin Button widget
    GtkWidget *w_factor_scale_spin_btn;           // Pointer to Scale Spin Button widget
    GtkWidget *w_threshold_binarization_spin_btn; // Pointer to Binarization Threshold Spin Button widget
    GtkWidget *w_start_pixel_global_trailing_spin_btn; // Pointer to start pixel of global trailing
    GtkWidget *w_strength_edge_trailing_spin_btn; // Pointer to strength of edge trailing
    GtkWidget *w_strength_vignette_spin_btn;      // Pointer to stregnth of vignette
    GtkWidget *w_strength_twist_spin_btn;         // Pointer to stregnth of twist
    GtkWidget *w_strength_swirl_spin_btn;         // Pointer to stregnth of swirl
    GtkWidget *w_size_surface_blur_spin_btn;      // Pointer to strength of surface blur
    

    GtkWidget *w_height_crop_spin_btn;            // Pointer to new height crop Spin Button widget
    GtkWidget *w_width_crop_spin_btn;             // Pointer to new width crop Spin Button widget
    GtkWidget *w_x_crop_spin_btn;                 // Pointer to x pixel crop Spin Button widget
    GtkWidget *w_y_crop_spin_btn;                 // Pointer to y pixel crop Spin Button widget

    //--- HSL Spin Buttons --- //
    GtkWidget *w_h_red_spin_btn;                  // Pointer to hue red Spin Button widget
    GtkWidget *w_h_orange_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_h_yellow_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_h_green_spin_btn;                // Pointer to hue red Spin Button widget
    GtkWidget *w_h_bluegreen_spin_btn;            // Pointer to hue red Spin Button widget
    GtkWidget *w_h_blue_spin_btn;                 // Pointer to hue red Spin Button widget
    GtkWidget *w_h_purple_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_h_magenta_spin_btn;              // Pointer to hue red Spin Button widget

    GtkWidget *w_s_red_spin_btn;                  // Pointer to hue red Spin Button widget
    GtkWidget *w_s_orange_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_s_yellow_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_s_green_spin_btn;                // Pointer to hue red Spin Button widget
    GtkWidget *w_s_bluegreen_spin_btn;            // Pointer to hue red Spin Button widget
    GtkWidget *w_s_blue_spin_btn;                 // Pointer to hue red Spin Button widget
    GtkWidget *w_s_purple_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_s_magenta_spin_btn;              // Pointer to hue red Spin Button widget

    GtkWidget *w_l_red_spin_btn;                  // Pointer to hue red Spin Button widget
    GtkWidget *w_l_orange_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_l_yellow_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_l_green_spin_btn;                // Pointer to hue red Spin Button widget
    GtkWidget *w_l_bluegreen_spin_btn;            // Pointer to hue red Spin Button widget
    GtkWidget *w_l_blue_spin_btn;                 // Pointer to hue red Spin Button widget
    GtkWidget *w_l_purple_spin_btn;               // Pointer to hue red Spin Button widget
    GtkWidget *w_l_magenta_spin_btn;              // Pointer to hue red Spin Button widget

    //--- Color Buttons --- //
    GtkWidget *w_color_btn_a_binarization;        // Pointer to button color a of binarization
    GtkWidget *w_color_btn_b_binarization;        // Pointer to button color a of binarization
    GtkWidget *w_color_btn_colorize;              // Pointer to button color of colorize

    GtkWidget *w_color_btn_a_gradient;            // Pointer to button color a of gradient colorize
    GtkWidget *w_color_btn_b_gradient;            // Pointer to button color b of gradient colorize

    GtkWidget *w_color_btn_vignette;              // Pointer to button color of vignette

    //--- EXIF Labels--- //
    GtkWidget *w_lbl_exif_capture_date;           // Pointer to capture date EXIF
    GtkWidget *w_lbl_exif_capture_time;           // Pointer to capture time EXIF
    GtkWidget *w_lbl_exif_size;                   // Pointer to size EXIF
    GtkWidget *w_lbl_exif_exposure;               // Pointer to exposure EXIF
    GtkWidget *w_lbl_exif_focal_length;           // Pointer to focal length EXIF
    GtkWidget *w_lbl_exif_iso;                    // Pointer to ISO EXIF
    GtkWidget *w_lbl_exif_model;                  // Pointer to camera model EXIF

    //--- Menu Buttons --- //
    GtkWidget *w_menu_edit;                       // Pointer to tool bar menu edit
    GtkWidget *w_menu_view;                       // Pointer to tool bar menu view
    GtkWidget *w_menu_filters;                    // Pointer to tool bar menu filters

    //--- Menu Item Buttons --- //
    GtkWidget *w_btn_new;                         // Pointer to tool bar new file
    GtkWidget *w_btn_save_as;                     // Pointer to tool bar save as
    GtkWidget *w_btn_resize;                      // Pointer to tool bar resize
    GtkWidget *w_btn_preferences;                 // Pointer to tool bar preferences
    GtkWidget *w_btn_clipping_reveal;             // Pointer to tool bar clipping reveal
    GtkWidget *w_btn_fit_to_scale;                // Pointer to tool bar fit to scale
    GtkWidget *w_btn_median_blur;                 // Pointer to tool bar median blur
    GtkWidget *w_btn_edge_enhance;                // Pointer to tool bar edge enhancement
    GtkWidget *w_btn_sav;                         // Pointer to the save button of tool bar
    GtkWidget *w_btn_smart_resize;                // Pointer to the smart resize button
    GtkWidget *w_btn_surface_blur;                // Pointer to the surface blur button

    //--- Apply Buttons --- //
    GtkWidget *w_btn_apply_grayscale;             // Pointer to apply button of grayscale
    GtkWidget *w_btn_apply_wb;                    // Pointer to apply button of wb
    GtkWidget *w_btn_apply_tint;                  // Pointer to apply button of tint
    GtkWidget *w_btn_apply_exposure;              // Pointer to apply button of exposure
    GtkWidget *w_btn_apply_whites;                // Pointer to apply button of whites
    GtkWidget *w_btn_apply_highlights;            // Pointer to apply button of highlights
    GtkWidget *w_btn_apply_shadows;               // Pointer to apply button of shadows
    GtkWidget *w_btn_apply_blacks;                // Pointer to apply button of blacks
    GtkWidget *w_btn_apply_saturation;            // Pointer to apply button of saturation
    GtkWidget *w_btn_apply_contrast;              // Pointer to apply button of contrast

    //--- HSL Apply Buttons --- //
    GtkWidget *w_btn_apply_h_red;                 // Pointer to apply button of hue on red
    GtkWidget *w_btn_apply_h_orange;              // Pointer to apply button of hue on orange
    GtkWidget *w_btn_apply_h_yellow;              // Pointer to apply button of hue on yellow
    GtkWidget *w_btn_apply_h_green;               // Pointer to apply button of hue on green
    GtkWidget *w_btn_apply_h_bluegreen;           // Pointer to apply button of hue on blue green
    GtkWidget *w_btn_apply_h_blue;                // Pointer to apply button of hue on blue
    GtkWidget *w_btn_apply_h_purple;              // Pointer to apply button of hue on purple
    GtkWidget *w_btn_apply_h_magenta;             // Pointer to apply button of hue on magenta

    GtkWidget *w_btn_apply_s_red;                 // Pointer to apply button of saturation on red
    GtkWidget *w_btn_apply_s_orange;              // Pointer to apply button of saturation on orange
    GtkWidget *w_btn_apply_s_yellow;              // Pointer to apply button of saturation on yellow
    GtkWidget *w_btn_apply_s_green;               // Pointer to apply button of saturation on green
    GtkWidget *w_btn_apply_s_bluegreen;           // Pointer to apply button of saturation on blue green
    GtkWidget *w_btn_apply_s_blue;                // Pointer to apply button of saturation on blue
    GtkWidget *w_btn_apply_s_purple;              // Pointer to apply button of saturation on purple
    GtkWidget *w_btn_apply_s_magenta;             // Pointer to apply button of saturation on magenta

    GtkWidget *w_btn_apply_l_red;                 // Pointer to apply button of luminance on red
    GtkWidget *w_btn_apply_l_orange;              // Pointer to apply button of luminance on orange
    GtkWidget *w_btn_apply_l_yellow;              // Pointer to apply button of luminance on yellow
    GtkWidget *w_btn_apply_l_green;               // Pointer to apply button of luminance on green
    GtkWidget *w_btn_apply_l_bluegreen;           // Pointer to apply button of luminance on blue green
    GtkWidget *w_btn_apply_l_blue;                // Pointer to apply button of luminance on blue
    GtkWidget *w_btn_apply_l_purple;              // Pointer to apply button of luminance on purple
    GtkWidget *w_btn_apply_l_magenta;             // Pointer to apply button of luminance on magenta

    //--- Revealers --- //
    GtkWidget *w_rvl_hue_hsl;                     // Pointer to hue revealer
    GtkWidget *w_rvl_saturation_hsl;              // Pointer to saturation revealer
    GtkWidget *w_rvl_luminance_hsl;               // Pointer to luminance revealer


    //--- Radio Buttons ---//
    GtkWidget *w_global_trailing_vertical_rd_btn; // Pointer to radio button vertical global trailing
    GtkWidget *w_global_trailing_horizontal_rd_btn; // Pointer to radio button horizontal global trailing
    GtkWidget *w_edge_trailing_soufflerie_rd_btn; // Pointer to radio button edge trailing soufflerie
    GtkWidget *w_edge_trailing_zigzag_rd_btn;     // Pointer to radio button edge trailing soufflerie


    //--- Toggle Buttons ---//

    GtkWidget *w_btn_hue_hsl;
    GtkWidget *w_btn_saturation_hsl;
    GtkWidget *w_btn_luminance_hsl;

    SDL_Texture *texture;

} app_widgets;


//gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, app_widgets *app_wdgts);


int main(int argc, char *argv[])
{
    srand(time(NULL));

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

    //g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &window_id);
    //g_signal_connect(main_window, "on_main_key_press_event", G_CALLBACK(on_key_press), &window_id);
    //g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_release), &app_widgets);


    // ----- USER DATA STRUCT GTK LINKING ----- //

    widgets->w_dlg_file_choose = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_file_choose"));
    widgets->w_dlg_save_choose = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_save_choose"));
    widgets->w_dlg_about = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_about"));
    widgets->w_on_btn_save_clicked = GTK_WIDGET(gtk_builder_get_object(builder,
            "on_btn_save_clicked"));
    widgets->w_dlg_lut_choose = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_lut_choose"));



    widgets->w_dlg_sharpen = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_sharpen"));
    widgets->w_dlg_mean_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_mean_blur"));
    widgets->w_dlg_median_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_median_blur"));
    widgets->w_dlg_gaussian_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_gaussian_blur"));
    widgets->w_dlg_radial_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_radial_blur"));
    widgets->w_dlg_fast_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_fast_blur"));
    widgets->w_dlg_direct_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_direct_blur"));
    widgets->w_dlg_surface_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_surface_blur"));
    
    widgets->w_dlg_resize = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_resize"));
    widgets->w_dlg_crop = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_crop"));
    widgets->w_dlg_rotation = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_rotation"));
    widgets->w_dlg_scale = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_scale"));
    widgets->w_dlg_binarization = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_binarization"));
    widgets->w_dlg_colorize = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_colorize"));
    widgets->w_dlg_global_trailing = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_global_trailing"));
    widgets->w_dlg_edge_trailing = GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_edge_trailing"));
    widgets->w_dlg_gradient_colorize=GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_gradient_colorize"));
    widgets->w_dlg_vignette=GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_vignette"));
    widgets->w_dlg_twist=GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_twist"));
    widgets->w_dlg_swirl=GTK_WIDGET(gtk_builder_get_object(builder,
            "dlg_swirl"));


            




    widgets->w_image_window = GTK_WIDGET(gtk_builder_get_object(builder,
            "image_window"));
    widgets->w_histo_window = GTK_WIDGET(gtk_builder_get_object(builder,
            "histo_window"));
    widgets->w_img_gradient_colorize= GTK_WIDGET(gtk_builder_get_object(builder,
            "img_gradient_colorize"));
    widgets->w_left_clip_warning= GTK_WIDGET(gtk_builder_get_object(builder,
            "left_clip_warning"));
    widgets->w_right_clip_warning= GTK_WIDGET(gtk_builder_get_object(builder,
            "right_clip_warning"));



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
    widgets->w_contrast_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "contrast_spin_btn"));
    widgets->w_start_pixel_global_trailing_spin_btn = 
            GTK_WIDGET(gtk_builder_get_object(builder,
                "start_pixel_global_trailing_spin_btn"));
    widgets->w_strength_edge_trailing_spin_btn = 
            GTK_WIDGET(gtk_builder_get_object(builder,
                "strength_edge_trailing_spin_btn"));
    widgets->w_strength_vignette_spin_btn = GTK_WIDGET(
        gtk_builder_get_object(builder,"strength_vignette_spin_btn"));
    


    widgets->w_height_crop_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "height_crop_spin_btn"));
    widgets->w_width_crop_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "width_crop_spin_btn"));
    widgets->w_x_crop_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "x_crop_spin_btn"));
    widgets->w_y_crop_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "y_crop_spin_btn"));



    widgets->w_height_adjustment_crop =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "height_adjustment_crop"));
    widgets->w_width_adjustment_crop =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "width_adjustment_crop"));
    widgets->w_x_adjustment_crop = GTK_WIDGET(gtk_builder_get_object(builder,
            "x_adjustment_crop"));
    widgets->w_y_adjustment_crop = GTK_WIDGET(gtk_builder_get_object(builder,
            "y_adjustment_crop"));
    widgets->w_start_pixel_global_trailing_adjustment =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "start_pixel_global_trailing_adjustment"));



    widgets->w_h_red_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_red_spin_btn"));
    widgets->w_h_orange_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_orange_spin_btn"));
    widgets->w_h_yellow_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_yellow_spin_btn"));
    widgets->w_h_green_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_green_spin_btn"));
    widgets->w_h_bluegreen_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_bluegreen_spin_btn"));
    widgets->w_h_blue_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_blue_spin_btn"));
    widgets->w_h_purple_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_purple_spin_btn"));
    widgets->w_h_magenta_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "h_magenta_spin_btn"));

    widgets->w_s_red_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_red_spin_btn"));
    widgets->w_s_orange_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_orange_spin_btn"));
    widgets->w_s_yellow_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_yellow_spin_btn"));
    widgets->w_s_green_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_green_spin_btn"));
    widgets->w_s_bluegreen_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_bluegreen_spin_btn"));
    widgets->w_s_blue_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_blue_spin_btn"));
    widgets->w_s_purple_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_purple_spin_btn"));
    widgets->w_s_magenta_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "s_magenta_spin_btn"));

    widgets->w_l_red_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_red_spin_btn"));
    widgets->w_l_orange_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_orange_spin_btn"));
    widgets->w_l_yellow_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_yellow_spin_btn"));
    widgets->w_l_green_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_green_spin_btn"));
    widgets->w_l_bluegreen_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_bluegreen_spin_btn"));
    widgets->w_l_blue_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_blue_spin_btn"));
    widgets->w_l_purple_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_purple_spin_btn"));
    widgets->w_l_magenta_spin_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "l_magenta_spin_btn"));



    widgets->w_strength_sharp_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder, "strength_sharp_spin_btn"));
    widgets->w_size_mean_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder, "size_mean_blur_spin_btn"));
    widgets->w_size_median_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                    "size_median_blur_spin_btn"));
    widgets->w_size_gaussian_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                    "size_gaussian_blur_spin_btn"));
    widgets->w_size_radial_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                    "size_radial_blur_spin_btn"));
    widgets->w_size_fast_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                    "size_fast_blur_spin_btn"));
    widgets->w_angle_direct_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                "angle_direct_blur_spin_btn"));
    widgets->w_size_direct_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                "size_direct_blur_spin_btn"));
    widgets->w_height_resize_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                "height_resize_spin_btn"));
    widgets->w_width_resize_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
                "width_resize_spin_btn"));
    widgets->w_angle_rotation_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "angle_rotation_spin_btn"));
    widgets->w_factor_scale_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "factor_scale_spin_btn"));
    widgets->w_threshold_binarization_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "threshold_binarization_spin_btn"));
    widgets->w_strength_twist_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "strength_twist_spin_btn"));
    widgets->w_strength_swirl_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "strength_swirl_spin_btn"));
    widgets->w_size_surface_blur_spin_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "size_surface_blur_spin_btn"));




    widgets->w_color_btn_a_binarization =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "color_btn_a_binarization"));
    widgets->w_color_btn_b_binarization =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "color_btn_b_binarization"));
    widgets->w_color_btn_colorize = GTK_WIDGET(gtk_builder_get_object(builder,
            "color_btn_colorize"));
    widgets->w_color_btn_a_gradient = GTK_WIDGET(gtk_builder_get_object(builder,
            "color_btn_a_gradient"));
    widgets->w_color_btn_b_gradient = GTK_WIDGET(gtk_builder_get_object(builder,
            "color_btn_b_gradient"));
    widgets->w_color_btn_vignette = GTK_WIDGET(gtk_builder_get_object(builder,
            "color_btn_vignette"));


    widgets->w_btn_smart_resize = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_smart_resize"));
    widgets->w_btn_surface_blur = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_surface_blur"));

            

/*     widgets->w_color_chooser_a_binarization = GTK_WIDGET(gtk_builder_get_object(builder,
            "color_chooser_a_binarization"));
    widgets->w_color_chooser_b_binarization = GTK_WIDGET(gtk_builder_get_object(builder,
            "color_chooser_b_binarization")); */



    widgets->w_lbl_exif_capture_date =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_capture_date"));
    widgets->w_lbl_exif_capture_time =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_capture_time"));
    widgets->w_lbl_exif_size =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_size"));
    widgets->w_lbl_exif_exposure =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_exposure"));
    widgets->w_lbl_exif_focal_length =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_focal_length"));
    widgets->w_lbl_exif_iso =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_iso"));
    widgets->w_lbl_exif_model =
        GTK_WIDGET(gtk_builder_get_object(builder, "lbl_exif_model"));


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



    widgets->w_check_rotation_resize_to_fit =
        GTK_WIDGET(gtk_builder_get_object(builder,
                    "check_rotation_resize_to_fit"));
    widgets->w_check_colorize_preserve_luminosity =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "check_colorize_preserve_luminosity"));
    widgets->w_global_trailing_inverse_check_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "global_trailing_inverse_check_btn"));




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
    widgets->w_btn_apply_contrast = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_contrast"));


    widgets->w_btn_apply_h_red = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_red"));
    widgets->w_btn_apply_h_orange = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_orange"));
    widgets->w_btn_apply_h_yellow = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_yellow"));
    widgets->w_btn_apply_h_green = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_green"));
    widgets->w_btn_apply_h_bluegreen =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_bluegreen"));
    widgets->w_btn_apply_h_blue = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_blue"));
    widgets->w_btn_apply_h_purple = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_purple"));
    widgets->w_btn_apply_h_magenta = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_h_magenta"));

    widgets->w_btn_apply_s_red = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_red"));
    widgets->w_btn_apply_s_orange = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_orange"));
    widgets->w_btn_apply_s_yellow = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_yellow"));
    widgets->w_btn_apply_s_green = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_green"));
    widgets->w_btn_apply_s_bluegreen =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_bluegreen"));
    widgets->w_btn_apply_s_blue = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_blue"));
    widgets->w_btn_apply_s_purple = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_purple"));
    widgets->w_btn_apply_s_magenta = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_s_magenta"));

    widgets->w_btn_apply_l_red = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_red"));
    widgets->w_btn_apply_l_orange = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_orange"));
    widgets->w_btn_apply_l_yellow = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_yellow"));
    widgets->w_btn_apply_l_green = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_green"));
    widgets->w_btn_apply_l_bluegreen =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_bluegreen"));
    widgets->w_btn_apply_l_blue = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_blue"));
    widgets->w_btn_apply_l_purple = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_purple"));
    widgets->w_btn_apply_l_magenta = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_apply_l_magenta"));





    widgets->w_rvl_hue_hsl = GTK_WIDGET(gtk_builder_get_object(builder,
            "rvl_hue_hsl"));
    widgets->w_rvl_saturation_hsl = GTK_WIDGET(gtk_builder_get_object(builder,
            "rvl_saturation_hsl"));
    widgets->w_rvl_luminance_hsl = GTK_WIDGET(gtk_builder_get_object(builder,
            "rvl_luminance_hsl"));



    widgets->w_btn_hue_hsl = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_hue_hsl"));
    widgets->w_btn_saturation_hsl = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_saturation_hsl"));
    widgets->w_btn_luminance_hsl = GTK_WIDGET(gtk_builder_get_object(builder,
            "btn_luminance_hsl"));

    
    


    widgets->w_global_trailing_vertical_rd_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "global_trailing_vertical_rd_btn"));
    widgets->w_global_trailing_horizontal_rd_btn =
        GTK_WIDGET(gtk_builder_get_object(builder,
            "global_trailing_horizontal_rd_btn"));
    widgets->w_edge_trailing_soufflerie_rd_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "edge_trailing_soufflerie_rd_btn"));
    widgets->w_edge_trailing_zigzag_rd_btn = GTK_WIDGET(gtk_builder_get_object(builder,
            "edge_trailing_zigzag_rd_btn"));  


    gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->w_rvl_hue_hsl), TRUE);
    gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->w_rvl_saturation_hsl),
            FALSE);
    gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->w_rvl_luminance_hsl),
            FALSE);


    widgets->texture = NULL;
    undo_stack = malloc(sizeof(stack));
    redo_stack = malloc(sizeof(stack));
    init_stack(undo_stack);
    init_stack(redo_stack);

    gtk_widget_set_sensitive(widgets->w_menu_edit, FALSE);
    gtk_widget_set_sensitive(widgets->w_menu_view, FALSE);
    gtk_widget_set_sensitive(widgets->w_menu_filters, FALSE);

    gtk_widget_set_sensitive(widgets->w_btn_new, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_save_as, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_resize, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_preferences, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_clipping_reveal, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_fit_to_scale, FALSE);
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
    gtk_widget_set_sensitive(widgets->w_btn_apply_contrast, FALSE);


    gtk_widget_set_sensitive(widgets->w_btn_apply_h_red, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_orange, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_yellow, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_green, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_bluegreen, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_blue, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_purple, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_h_magenta, FALSE);

    gtk_widget_set_sensitive(widgets->w_btn_apply_s_red, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_orange, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_yellow, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_green, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_bluegreen, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_blue, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_purple, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_s_magenta, FALSE);

    gtk_widget_set_sensitive(widgets->w_btn_apply_l_red, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_orange, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_yellow, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_green, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_bluegreen, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_blue, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_purple, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_apply_l_magenta, FALSE);

    gtk_widget_set_sensitive(widgets->w_btn_sav, FALSE);


    gtk_widget_set_sensitive(widgets->w_btn_smart_resize, FALSE);
    gtk_widget_set_sensitive(widgets->w_btn_surface_blur, FALSE);

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
    clear_stack_text(undo_stack);
    clear_stack_text(redo_stack);
    free(undo_stack);
    free(redo_stack);
    quit(sdl_window, sdl_renderer, texture);
    gtk_main_quit();
    return True;
}

gboolean on_btn_quit_activate()
{
    clear_stack_text(undo_stack);
    clear_stack_text(redo_stack);
    free(undo_stack);
    free(redo_stack);
    quit(sdl_window, sdl_renderer, texture);
    gtk_main_quit();
    return True;
}

void on_window_main_destroy()
{
    clear_stack_text(undo_stack);
    clear_stack_text(redo_stack);
    free(undo_stack);
    free(redo_stack);
    quit(sdl_window, sdl_renderer, texture);
    gtk_main_quit();
}




void update_image(SDL_Surface *surface, app_widgets *app_wdgts)
{
    savePNG("./tmp.png", surface);
    push_stack(undo_stack, app_wdgts->texture);
    clear_stack_text(redo_stack);
    app_wdgts->texture = surface_to_texture(surface, sdl_renderer);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
    //show_l_histo(surface);
    //gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./l_histo.PNG");
    histo_color(surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),
            "./new_histo.PNG");
    
    SDL_Surface *histogram = load("./new_histo.PNG");

    int clipping = histo_clipping(histogram);
    if (clipping == 0)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 1)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 2)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    if (clipping == 3)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    
    SDL_FreeSurface(histogram);
}


void on_btn_refresh_texture_activate(GtkMenuItem *btn_open
        __attribute__((unused)), app_widgets *app_wdgts)
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

void on_btn_open_activate(GtkMenuItem *btn_open __attribute__((unused)),
        app_widgets *app_wdgts)
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



            //show_l_histo(surface);
            //gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./l_histo.PNG");
            histo_color(surface);
            gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),
                    "./new_histo.PNG");
            
            SDL_Surface *histogram = load("./new_histo.PNG");

            int clipping = histo_clipping(histogram);
            if (clipping == 0)
            {
                gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
                gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
            }
            if (clipping == 1)
            {
                gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
                gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
            }
            if (clipping == 2)
            {
                gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
                gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
            }
            if (clipping == 3)
            {
                gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
                gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
            }
            
            SDL_FreeSurface(histogram);

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

            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_capture_time),
                    time_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_size), size_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_exposure),
                    exposure_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_focal_length),
                    focal_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_iso), iso_str);
            gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_exif_model),
                    model_str);

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
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_contrast, TRUE);

            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_red, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_orange, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_yellow, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_green, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_bluegreen, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_blue, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_purple, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_h_magenta, TRUE);

            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_red, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_orange, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_yellow, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_green, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_bluegreen, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_blue, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_purple, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_s_magenta, TRUE);

            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_red, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_orange, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_yellow, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_green, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_bluegreen, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_blue, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_purple, TRUE);
            gtk_widget_set_sensitive(app_wdgts->w_btn_apply_l_magenta, TRUE);

            gtk_widget_set_sensitive(app_wdgts->w_btn_sav, TRUE);
        }
    }

    // Finished with the "Open Image" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}


// ---------------------------------------------- //
// ----------------- SAVE ----------------------- //
// ---------------------------------------------- //

void on_btn_save_activate(GtkMenuItem *btn_open __attribute__((unused)),
        app_widgets *app_wdgts)
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
            SDL_Surface *result = texture_to_surface(app_wdgts->texture,
                    sdl_renderer);
            savePNG(app_wdgts->save_path, result);

            SDL_FreeSurface(result);
        }
    }
}

/* void on_btn_save_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_save_choose);
} */

void on_btn_cancel_save_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_save_choose);
}














// -------------------------------------------------- //
// --------------- SIDE MENU FUNCTIONS -------------- //
// -------------------------------------------------- //

void on_btn_apply_wb_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_wb_spin_btn));
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    white_balance(surface, ((double)quantity)/100);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_tint_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_tint_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tint(surface, ((double)quantity)/100);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_exposure_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_exposure_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    exposure(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_whites_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_whites_spin_btn));


    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    whites(surface, (double)quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_highlights_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_highlights_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    highlights(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_shadows_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_shadows_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    shadows(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_blacks_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_blacks_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    blacks(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_saturation_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_saturation_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    saturation(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_contrast_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_contrast_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    contrast(surface, ((double)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}




//-----------------------------------------------//
//------------ FILTERS AND FUNCTIONS ------------//
//-----------------------------------------------//


//------------ Sharpen ------------//

void on_btn_sharpen_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_sharpen);
}

void on_btn_cancel_sharpening_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_sharpen);
}

void on_btn_apply_sharpening_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = sharpen(surface, ((double)quantity)/100, 50);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);

    gint reset_value = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_strength_sharp_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_sharpen);
}


//------------ Mean Blur ------------//

void on_btn_mean_blur_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_mean_blur);
}

void on_btn_cancel_mean_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_mean_blur);
}

void on_btn_apply_mean_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    box_blur(surface, (int)quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_mean_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_mean_blur);
}


//------------ Median Blur ------------//

void on_btn_median_blur_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_median_blur);
}

void on_btn_cancel_median_blur_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_median_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_median_blur);
}

void on_btn_apply_median_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_size_median_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    median_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_median_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_median_blur);
}


//------------ Gaussian Blur ------------//

void on_btn_gaussian_blur_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_gaussian_blur);
}

void on_btn_cancel_gaussian_blur_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 2;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_gaussian_blur);
}

void on_btn_apply_gaussian_blur_clicked(
        GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    gaussian_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 2;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_gaussian_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_gaussian_blur);
}


//------------ Directional Blur ------------//

void on_btn_direct_blur_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_direct_blur);
}

void on_btn_cancel_direct_blur_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gint reset_value_angle = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn),
            reset_value_angle);
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_direct_blur);
}

void on_btn_apply_direct_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint angle_quantity = 0;
    gint strength_quantity = 0;

    angle_quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn));
    strength_quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    motion_blur(surface, (int)strength_quantity, (double)angle_quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gint reset_value_angle = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_angle_direct_blur_spin_btn),
            reset_value_angle);
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_direct_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_direct_blur);
}


// ----------- Fast Blur ------------//

void on_btn_fast_blur_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_fast_blur);
}

void on_btn_cancel_fast_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_fast_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_fast_blur);
}

void on_btn_apply_fast_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 3;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_size_fast_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    fast_gaussian_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_fast_blur_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_fast_blur);
}


// ----------- Radial Blur ------------//

void on_btn_radial_blur_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_radial_blur);
}

void on_btn_cancel_radial_blur_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_radial_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_radial_blur);
}

void on_btn_apply_radial_blur_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 3;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_size_radial_blur_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    radial_blur(surface, ((int)quantity));
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 3;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_size_radial_blur_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_radial_blur);
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

void on_btn_rotation_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_rotation);
}

void on_btn_cancel_rotation_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint reset_value = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_rotation);
}

void on_btn_apply_rotation_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gint quantity = 0;

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn));


    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = rotate(surface, (double)quantity);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);


    gdouble reset_value = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_angle_rotation_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_rotation);
}


//------------ Scale ------------//

void on_btn_scale_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_scale);
}

void on_btn_cancel_scale_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gdouble reset_value = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_factor_scale_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_scale);
}

void on_btn_apply_scale_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gdouble quantity = 0;

    quantity = gtk_spin_button_get_value(
            GTK_SPIN_BUTTON(app_wdgts->w_factor_scale_spin_btn));


    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = resize(surface, (double)quantity);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);


    gdouble reset_value = 0;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_factor_scale_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_scale);
}

// ----------- Crop ------------ //

void on_btn_crop_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);


    gdouble ow = (gdouble)(surface->w);
    gdouble oh = (gdouble)(surface->h);

    gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_width_adjustment_crop), ow);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_height_adjustment_crop), oh);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_x_adjustment_crop), ow);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_y_adjustment_crop), oh);

    gtk_widget_show(app_wdgts->w_dlg_crop);
    SDL_FreeSurface(surface);
}

void on_btn_cancel_crop_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gdouble reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_height_crop_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_width_crop_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_x_crop_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_y_crop_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_crop);
}

void on_btn_apply_crop_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    size_t new_x = (size_t)(gtk_spin_button_get_value
        (GTK_SPIN_BUTTON(app_wdgts->w_x_crop_spin_btn)));
    size_t new_y = (size_t)(gtk_spin_button_get_value
        (GTK_SPIN_BUTTON(app_wdgts->w_y_crop_spin_btn)));
    size_t new_w = (size_t)(gtk_spin_button_get_value
        (GTK_SPIN_BUTTON(app_wdgts->w_width_crop_spin_btn)));
    size_t new_h = (size_t)(gtk_spin_button_get_value
        (GTK_SPIN_BUTTON(app_wdgts->w_height_crop_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = image_crop(surface, new_x, new_y, new_w, new_h);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);

    gdouble reset_value = 0;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_height_crop_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_width_crop_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_x_crop_spin_btn), reset_value);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON
        (app_wdgts->w_y_crop_spin_btn), reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_crop);
}


//------------ Binarization ------------//

void on_btn_binarization_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    GdkRGBA black;
    black.red = 0;
    black.green = 0;
    black.blue = 0;
    black.alpha = 1;
    GdkRGBA white;
    white.red = 1;
    white.green = 1;
    white.blue = 1;
    white.alpha = 1;

    gtk_color_chooser_set_rgba (
        GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_a_binarization), &black);
    gtk_color_chooser_set_rgba (
        GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_b_binarization), &white);

    gtk_widget_show(app_wdgts->w_dlg_binarization);
}

void on_btn_cancel_binarization_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint reset_value = 127;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_threshold_binarization_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_binarization);
}

void on_btn_apply_binarization_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint quantity = 0;

    GdkRGBA colora;
    GdkRGBA colorb;

    colora.red = 0.5;
    colora.green = 0.3;
    colora.blue = 0.1;
    colorb.red = 0.4;
    colora.green = 0.9;
    colora.blue = 0.8;

    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_a_binarization), &colora);
    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_b_binarization), &colorb);

    int ra= (int)((colora.red)*255);
    int ga= (int)((colora.green)*255);
    int ba= (int)((colora.blue)*255);

    int rb= (int)((colorb.red)*255);
    int gb= (int)((colorb.green)*255);
    int bb= (int)((colorb.blue)*255);

    quantity = gtk_spin_button_get_value_as_int(
            GTK_SPIN_BUTTON(app_wdgts->w_threshold_binarization_spin_btn));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    binarization(surface, (char)quantity, ra, ga, ba, rb, gb, bb);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gint reset_value = 127;
    gtk_spin_button_set_value(
            GTK_SPIN_BUTTON(app_wdgts->w_threshold_binarization_spin_btn),
            reset_value);

    gtk_widget_hide(app_wdgts->w_dlg_binarization);
}



//------------ Colorize ------------//

void on_btn_colorize_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{

    gtk_widget_show(app_wdgts->w_dlg_colorize);
}

void on_btn_cancel_colorize_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_colorize);
}

void on_btn_apply_colorize_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    /* grayscale(surface);
    update_image(surface, app_wdgts); */

    GdkRGBA color;

    color.red = 0.5;
    color.green = 0.3;
    color.blue = 0.1;

    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_colorize), &color);

    int r= (int)((color.red)*255);
    int g= (int)((color.green)*255);
    int b= (int)((color.blue)*255);

    int keep_luminance = 0;
    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_check_colorize_preserve_luminosity)
    )))
    {
        keep_luminance = 1;
    }

    g_print("KEEP LUMINANCE: %d\n", keep_luminance);

    colorize(surface, r, g, b, keep_luminance);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
    gtk_widget_hide(app_wdgts->w_dlg_colorize);
}


//------------ Negative ------------//

void on_btn_negative_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    negative(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


//------------ Grayscale ------------//

void on_btn_apply_grayscale_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    grayscale(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

}


//------------ Deep Fry ------------//

void on_btn_deep_fry_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    Uint8 reference[3] = {200, 60, 20};
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    deep_fry(surface, reference);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


//------------ Fever Dream ------------//

void on_btn_fever_dream_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    double strength = 6.66;
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = fever_dream(surface, strength);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);
}


//------------ Mercury ------------//

void on_btn_mercury_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    mercury(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


//------------ Neon Demon ------------//

void on_btn_neon_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *res = neon(surface);
    update_image(res, app_wdgts);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(res);
}



//------------ Global Trailing ------------//

void on_btn_global_trailing_activate(
        GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    gdouble ow = (gdouble)(surface->w);

    gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_start_pixel_global_trailing_adjustment), ow);


    SDL_FreeSurface(surface);

    gtk_widget_show(app_wdgts->w_dlg_global_trailing);
}


void on_btn_cancel_global_trailing_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_global_trailing);
}

void on_btn_apply_global_trailing_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gint start_pixel = 0;
    start_pixel = gtk_spin_button_get_value_as_int
        (GTK_SPIN_BUTTON(app_wdgts->w_start_pixel_global_trailing_spin_btn));

    int inverse = 0;
    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_global_trailing_inverse_check_btn)
    )))
    {
        inverse = 1;
    }

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_global_trailing_vertical_rd_btn)
    )))
    {
        global_trailing_v(surface, (int)start_pixel, inverse);
    }

    else
    {
        global_trailing_h(surface, (int)start_pixel, inverse);
    }

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gtk_widget_hide(app_wdgts->w_dlg_global_trailing);
}

void on_global_trailing_vertical_rd_btn_toggled(
        GtkRadioButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    gdouble ow = (gdouble)(surface->w);
    gdouble oh = (gdouble)(surface->h);

    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_global_trailing_vertical_rd_btn)
    )))
    {
        gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_start_pixel_global_trailing_adjustment), ow);
        if (gtk_spin_button_get_value_as_int
        (GTK_SPIN_BUTTON(app_wdgts->w_start_pixel_global_trailing_spin_btn))>ow)
        {
            gtk_spin_button_set_value(GTK_SPIN_BUTTON
            (app_wdgts->w_start_pixel_global_trailing_spin_btn), ow);
        }
    }
    else
    {
        gtk_adjustment_set_upper(GTK_ADJUSTMENT
        (app_wdgts->w_start_pixel_global_trailing_adjustment), oh);
        if (gtk_spin_button_get_value_as_int
        (GTK_SPIN_BUTTON(app_wdgts->w_start_pixel_global_trailing_spin_btn))>oh)
        {
            gtk_spin_button_set_value(GTK_SPIN_BUTTON
            (app_wdgts->w_start_pixel_global_trailing_spin_btn), oh);
        }
    }

    SDL_FreeSurface(surface);

}

//------------ Edge Trailing -------------//

void on_btn_edge_trailing_activate(GtkMenuItem *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_edge_trailing);
}

void on_btn_cancel_edge_trailing_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_edge_trailing);
}

void on_btn_apply_edge_trailing_clicked(GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *edge_map = canny_fnc(surface);
    
    /* gdouble ow = (gdouble)(surface->w);
    gdouble oh = (gdouble)(surface->h); */

    int kernel_size = 20;

    kernel_size = gtk_spin_button_get_value_as_int
        (GTK_SPIN_BUTTON(app_wdgts->w_strength_edge_trailing_spin_btn));

    int inverse = 0;
    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_global_trailing_inverse_check_btn)
    )))
    {
        inverse = 1;
    }

    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_edge_trailing_soufflerie_rd_btn)
    )))
    {
        edge_trailing_soufflerie(surface, edge_map, kernel_size, inverse);
    }

    else
    {
        edge_trailing_zigzag(surface, edge_map, kernel_size);
    }


    update_image(surface, app_wdgts);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(edge_map);

    gtk_widget_hide(app_wdgts->w_dlg_edge_trailing);
}

/* void on_edge_trailing_soufflerie_rd_btn_toggled(GtkRadioButton *button, app_widgets *app_wdgts)
{
    if ((gtk_toggle_button_get_active  (
        GTK_TOGGLE_BUTTON(app_wdgts->w_edge_trailing_soufflerie_rd_btn)
    )))
    {
        gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(app_wdgts->w_edge_trailing_inverse_check_btn), FALSE);
    }

    else
    {
        gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(app_wdgts->w_edge_trailing_inverse_check_btn), TRUE);
    }
    
} */


//------------ About ------------//

void on_btn_about_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_about);
}

void on_btn_close_about_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_about);
}




// -------------- HSL Revealers ---------------- //

void on_btn_hue_hsl_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_hue_hsl),
            TRUE);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_luminance_hsl),
            FALSE);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_saturation_hsl),
            FALSE);
}

void on_btn_saturation_hsl_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_saturation_hsl),
            TRUE);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_luminance_hsl),
            FALSE);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_hue_hsl),
            FALSE);
}

void on_btn_luminance_hsl_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_luminance_hsl),
            TRUE);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_saturation_hsl),
            FALSE);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app_wdgts->w_rvl_hue_hsl),
            FALSE);
}



// ------------------------------------------------ //
//--------------------- UNDO ---------------------- //
// ------------------------------------------------ //


void on_btn_undo_all_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window),
            app_wdgts->image_path);

    SDL_Surface *surface = load(app_wdgts->image_path);
    app_wdgts->texture = surface_to_texture(surface, sdl_renderer);

    histo_color(surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),
            "./new_histo.PNG");
    
    SDL_Surface *histogram = load("./new_histo.PNG");

    int clipping = histo_clipping(histogram);
    if (clipping == 0)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 1)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 2)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    if (clipping == 3)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    
    SDL_FreeSurface(histogram);

    SDL_FreeSurface(surface);
}

void on_btn_undo_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    if(stack_isempty(undo_stack))
        return;
    SDL_Texture *texture = pop_stack(undo_stack);
    push_stack(redo_stack, app_wdgts->texture);
    app_wdgts->texture = texture;
    SDL_Surface *surface = texture_to_surface(texture, sdl_renderer);
    savePNG("./tmp.png", surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
    histo_color(surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./new_histo.PNG");

    SDL_Surface *histogram = load("./new_histo.PNG");

    int clipping = histo_clipping(histogram);
    if (clipping == 0)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 1)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 2)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    if (clipping == 3)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    
    SDL_FreeSurface(histogram);

    SDL_FreeSurface(surface);
}

void on_btn_redo_activate(GtkMenuItem *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    if(stack_isempty(redo_stack))
        return;
    SDL_Texture *texture = pop_stack(redo_stack);
    push_stack(undo_stack, app_wdgts->texture);
    app_wdgts->texture = texture;
    SDL_Surface *surface = texture_to_surface(texture, sdl_renderer);
    savePNG("./tmp.png", surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
    histo_color(surface);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./new_histo.PNG");

    SDL_Surface *histogram = load("./new_histo.PNG");

    int clipping = histo_clipping(histogram);
    if (clipping == 0)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 1)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
    }
    if (clipping == 2)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    if (clipping == 3)
    {
        gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
        gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
    }
    
    SDL_FreeSurface(histogram);

    SDL_FreeSurface(surface);
}





//-----------------------------------------------//
//-------------------- H S L --------------------//
//-----------------------------------------------//


// ----- HUE -----//

void on_btn_apply_h_red_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_red_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, RED, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_orange_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_orange_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, ORANGE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_yellow_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_yellow_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, YELLOW, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_green_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_green_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, GREEN, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_bluegreen_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_bluegreen_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, TEAL, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_blue_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_blue_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, BLUE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_purple_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_purple_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, PURPLE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_h_magenta_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_h_magenta_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_hue(surface, MAGENTA, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


// ----- Saturation -----//

void on_btn_apply_s_red_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_red_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, RED, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_orange_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_orange_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, ORANGE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_yellow_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_yellow_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, YELLOW, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_green_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_green_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, GREEN, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_bluegreen_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_bluegreen_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, TEAL, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_blue_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_blue_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, BLUE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_purple_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_purple_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, PURPLE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_s_magenta_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_s_magenta_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_sat(surface, MAGENTA, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


// ----- Luminance -----//

void on_btn_apply_l_red_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_red_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, RED, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_orange_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_orange_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, ORANGE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_yellow_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_yellow_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, YELLOW, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_green_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_green_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, GREEN, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_bluegreen_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_bluegreen_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, TEAL, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_blue_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_blue_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, BLUE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_purple_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_purple_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, PURPLE, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}

void on_btn_apply_l_magenta_clicked(GtkButton *button __attribute__((unused)),
        app_widgets *app_wdgts)
{
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_l_magenta_spin_btn)));

    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    tsl_val(surface, MAGENTA, quantity);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}




// -------- LUTS --------- //


void on_btn_lut_activate(GtkMenuItem *btn_open __attribute__((unused)),
        app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_lut_choose);
    if (gtk_dialog_run(GTK_DIALOG(app_wdgts->w_dlg_lut_choose))
            == GTK_RESPONSE_OK)
    {
        // Get the file name from the dialog box
        app_wdgts->lut_path =
            gtk_file_chooser_get_filename(
                    GTK_FILE_CHOOSER(app_wdgts->w_dlg_lut_choose));

        SDL_Surface *lut = load(app_wdgts->lut_path);
        SDL_Surface *surface = texture_to_surface(app_wdgts->texture,
                sdl_renderer);

        gdouble lw = (gdouble)(lut->w);
        gdouble lh = (gdouble)(lut->h);


        if (lw == 512)
        {
            if (lh == 512)
            {
                apply_lut(surface, lut);
            }
        }
        

        update_image(surface, app_wdgts);

        SDL_FreeSurface(surface);
        SDL_FreeSurface(lut);
    }
    gtk_widget_hide(app_wdgts->w_dlg_lut_choose);
}



// -------- Gradient Colorize --------- //


void on_btn_gradient_colorize_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_gradient_colorize),
        "./original_gradient.png");

    GdkRGBA black;
    black.red = 0;
    black.green = 0;
    black.blue = 0;
    black.alpha = 1;
    GdkRGBA white;
    white.red = 1;
    white.green = 1;
    white.blue = 1;
    white.alpha = 1;

    gtk_color_chooser_set_rgba (
        GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_a_gradient), &black);
    gtk_color_chooser_set_rgba (
        GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_b_gradient), &white);
    gtk_widget_show(app_wdgts->w_dlg_gradient_colorize);
}

void on_btn_cancel_gradient_colorize_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_gradient_colorize);
}

void on_btn_apply_gradient_colorize_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    
    GdkRGBA colora;
    GdkRGBA colorb;

    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_a_gradient), &colora);
    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_b_gradient), &colorb);

    int ra= (int)((colora.red)*255);
    int ga= (int)((colora.green)*255);
    int ba= (int)((colora.blue)*255);

    int rb= (int)((colorb.red)*255);
    int gb= (int)((colorb.green)*255);
    int bb= (int)((colorb.blue)*255);

    grayscale(surface);

    gradient_colorize(surface, ra, ga, ba, rb, gb, bb);

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gtk_widget_hide(app_wdgts->w_dlg_gradient_colorize);
}


void on_color_btn_a_gradient_color_set(
        GtkColorButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    GdkRGBA colora;
    GdkRGBA colorb;

    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_a_gradient), &colora);
    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_b_gradient), &colorb);

    int ra= (int)((colora.red)*255);
    int ga= (int)((colora.green)*255);
    int ba= (int)((colora.blue)*255);

    int rb= (int)((colorb.red)*255);
    int gb= (int)((colorb.green)*255);
    int bb= (int)((colorb.blue)*255);

    update_gradient_preview(ra, ga, ba, rb, gb, bb);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_gradient_colorize),
        "./gradient.png");
}

void on_color_btn_b_gradient_color_set(
        GtkColorButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    GdkRGBA colora;
    GdkRGBA colorb;

    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_a_gradient), &colora);
    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_b_gradient), &colorb);

    int ra= (int)((colora.red)*255);
    int ga= (int)((colora.green)*255);
    int ba= (int)((colora.blue)*255);

    int rb= (int)((colorb.red)*255);
    int gb= (int)((colorb.green)*255);
    int bb= (int)((colorb.blue)*255);

    update_gradient_preview(ra, ga, ba, rb, gb, bb);
    gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_gradient_colorize),
        "./gradient.png");
}






// ---------- Seam Carving -------------//

void on_btn_smart_resize_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    SDL_Surface *edge_map = canny_fnc(surface);
    SDL_Surface *sobel = load("./hypot.PNG");
    power_map(surface, sobel);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(edge_map);
    SDL_FreeSurface(sobel);
}



// -------- Vignette --------- //


void on_btn_vignette_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    GdkRGBA black;
    black.red = 0;
    black.green = 0;
    black.blue = 0;
    black.alpha = 1;

    gtk_color_chooser_set_rgba (
        GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_vignette), &black);

    gtk_widget_show(app_wdgts->w_dlg_vignette);
}

void on_btn_cancel_vignette_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_vignette);
}

void on_btn_apply_vignette_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    GdkRGBA colora;

    gtk_color_chooser_get_rgba(
            GTK_COLOR_CHOOSER(app_wdgts->w_color_btn_vignette), &colora);

    Uint8 ra = (Uint8)(colora.red * 255);
    Uint8 ga = (Uint8)(colora.green * 255);
    Uint8 ba = (Uint8)(colora.blue * 255);

    Uint8 color[3] = {ra, ga, ba};
    int quantity = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_strength_vignette_spin_btn)));
    vignette(surface, color, quantity);

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gtk_widget_hide(app_wdgts->w_dlg_vignette);
}



// -------- Twist --------- //


void on_btn_twist_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_twist);
}

void on_btn_cancel_twist_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_twist);
}

void on_btn_apply_twist_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    int strength = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_strength_twist_spin_btn)));

    double factor = (double)strength/5;

    twist(surface, factor);

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gtk_widget_hide(app_wdgts->w_dlg_twist);
}



// -------- Swirl --------- //


void on_btn_swirl_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_swirl);
}

void on_btn_cancel_swirl_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_swirl);
}

void on_btn_apply_swirl_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    int strength = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_strength_swirl_spin_btn)));

    double factor = (double)strength/20;

    swirl(surface, factor);

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gtk_widget_hide(app_wdgts->w_dlg_swirl);
}




// -------- Surface Blur --------- //


/* void on_btn_surface_blur_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_surface_blur);
}

void on_btn_cancel_surface_blur_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_surface_blur);
}

void on_btn_apply_surface_blur_clicked(
        GtkButton *button __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    int strength = (int)(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
    (app_wdgts->w_size_surface_blur_spin_btn)));

    SDL_Surface *canny = canny_fnc(surface);

    surface_blur(surface, canny);

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);

    gtk_widget_hide(app_wdgts->w_dlg_surface_blur);
}
 */


void on_btn_surface_blur_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);

    SDL_Surface *canny = canny_fnc(surface);

    surface_blur(surface, canny);

    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


// -------- Perlin Noise --------- //


void on_btn_noise_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    noise(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}


// -------- Mean --------- //


void on_btn_mean_activate(GtkMenuItem *btn_open 
        __attribute__((unused)), app_widgets *app_wdgts)
{
    SDL_Surface *surface = texture_to_surface(app_wdgts->texture, sdl_renderer);
    mean(surface);
    update_image(surface, app_wdgts);
    SDL_FreeSurface(surface);
}




gboolean on_main_window_key_press_event(GtkWidget *widget __attribute__((unused)), GdkEventKey *event, app_widgets *app_wdgts)
{
    switch (event->keyval)
    {
        case GDK_KEY_z:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + z
                on_btn_undo_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_Z:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + shift + z
                on_btn_redo_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_u:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + u
                on_btn_undo_all_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_s:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + s
                on_btn_save_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_o:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + o
                on_btn_open_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_q:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + q
                on_btn_quit_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_r:
            if (event->state & GDK_CONTROL_MASK)
            {
                // ctrl + r
                on_btn_rotation_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            }
            break;
        
        case GDK_KEY_S:
            // shift + s
            on_btn_scale_activate(GTK_MENU_ITEM(app_wdgts->w_btn_new), app_wdgts);
            break;
        
        default:
            return FALSE;
    }

    return FALSE;

    /* // If the 'f' key is pressed, moves the player 1 upwards.
    if (event->keyval == GDK_KEY_f)
    {
        printf("F TO PAY RESPECT NEW\n");
        return TRUE;
    }

    if (event->keyval == GDK_KEY_u)
    {
        if(stack_isempty(undo_stack))
            return TRUE;
        SDL_Texture *texture = pop_stack(undo_stack);
        push_stack(redo_stack, app_wdgts->texture);
        app_wdgts->texture = texture;
        SDL_Surface *surface = texture_to_surface(texture, sdl_renderer);
        savePNG("./tmp.png", surface);
        gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
        histo_color(surface);
        gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./new_histo.PNG");

        SDL_Surface *histogram = load("./new_histo.PNG");

        int clipping = histo_clipping(histogram);
        if (clipping == 0)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
        }
        if (clipping == 1)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
        }
        if (clipping == 2)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
        }
        if (clipping == 3)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
        }
        
        SDL_FreeSurface(histogram);

        SDL_FreeSurface(surface);

        return TRUE;
    }

    if (event->keyval == GDK_KEY_U)
    {
        printf("REDO\n");
        if(stack_isempty(redo_stack))
            return TRUE;
        SDL_Texture *texture = pop_stack(redo_stack);
        push_stack(undo_stack, app_wdgts->texture);
        app_wdgts->texture = texture;
        SDL_Surface *surface = texture_to_surface(texture, sdl_renderer);
        savePNG("./tmp.png", surface);
        gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
        histo_color(surface);
        gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./new_histo.PNG");

        SDL_Surface *histogram = load("./new_histo.PNG");

        int clipping = histo_clipping(histogram);
        if (clipping == 0)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
        }
        if (clipping == 1)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, FALSE);
        }
        if (clipping == 2)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, FALSE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
        }
        if (clipping == 3)
        {
            gtk_widget_set_visible (app_wdgts->w_left_clip_warning, TRUE);
            gtk_widget_set_visible (app_wdgts->w_right_clip_warning, TRUE);
        }
        
        SDL_FreeSurface(histogram);

        SDL_FreeSurface(surface);
    }

    // Otherwise, propagates the signal.
    else
        return FALSE; */
}




// Event handler for the "key-press-event" signal.
/* gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, app_widgets *app_wdgts)
{
    // If the 'f' key is pressed, moves the player 1 upwards.
    if (event->keyval == GDK_KEY_f)
    {
        printf("F TO PAY RESPECT\n");
        return TRUE;
    }

    if (event->keyval == GDK_KEY_u)
    {
        printf("UNDO CALL\n");
        if(stack_isempty(undo_stack))
            return;
        SDL_Texture *texture = pop_stack(undo_stack);
        push_stack(redo_stack, app_wdgts->texture);
        app_wdgts->texture = texture;
        SDL_Surface *surface = texture_to_surface(texture, sdl_renderer);
        savePNG("./tmp.png", surface);
        gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_image_window), "./tmp.png");
        histo_color(surface);
        gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_histo_window),"./new_histo.PNG");

        SDL_FreeSurface(surface);

        printf("UNDO FINISHED\n");
        return TRUE;
    }

    // Otherwise, propagates the signal.
    else
        return FALSE;
} */

