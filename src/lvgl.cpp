
#include "ace.h"

// Declare Custom Fonts
LV_FONT_DECLARE(ace_IBMPlexMono_20);
LV_FONT_DECLARE(ace_IBMPlexMono_30);
LV_FONT_DECLARE(ace_IBMPlexMono_40);

namespace ace::lvgl {

	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */

	lv_style_t style_screen;
	lv_style_t style_text;
	lv_style_t style_text_title;
	lv_style_t style_preload;
	lv_style_t style_container_empty;
	lv_style_t style_container_red;

	lv_obj_t* load_screen = lv_obj_create(NULL, NULL);

	/*lv_obj_t* label_load_flap = lv_obj_create(NULL, NULL);
	lv_obj_t* label_load_shenan = lv_obj_create(NULL, NULL);
	lv_obj_t* label_load_imu = lv_obj_create(NULL, NULL);*/

	lv_obj_t* img_screen = lv_obj_create(NULL, NULL);

	/* ========================================================================== */
	/*                             Init Lvgl Function                             */
	/* ========================================================================== */

	void init_lvgl() {

		/* ------------------------------ Global Stuffs ----------------------------- */

		// Style for Screen
		lv_style_copy(&style_screen, &lv_style_plain);
		style_screen.body.main_color = LV_COLOR_BLACK;
		style_screen.body.grad_color = LV_COLOR_BLACK;

		// Style for text
		lv_style_copy(&style_text, &lv_style_pretty);
		style_text.body.main_color = LV_COLOR_RED;
		style_text.body.grad_color = LV_COLOR_RED;
		style_text.text.color = LV_COLOR_RED;
		style_text.text.font = &ace_IBMPlexMono_20;

		// Style for text
		lv_style_copy(&style_text_title, &style_text);
		style_text_title.body.main_color = LV_COLOR_RED;
		style_text_title.body.grad_color = LV_COLOR_RED;
		style_text_title.text.color = LV_COLOR_RED;
		style_text_title.text.font = &ace_IBMPlexMono_30;

		// Style for Empty Container
		lv_style_copy(&style_container_empty, &lv_style_plain);
		style_container_empty.body.grad_color = LV_COLOR_BLACK;
		style_container_empty.body.main_color = LV_COLOR_BLACK;
		style_container_empty.body.border.color = LV_COLOR_BLACK;
		style_container_empty.body.border.width = 2;
		style_container_empty.body.padding.hor = 10;
		style_container_empty.body.padding.ver = 10;
		style_container_empty.body.padding.inner = 10;

		// Style for COntainer with Red Border
		lv_style_copy(&style_container_red, &lv_style_plain);
		style_container_red.body.grad_color = LV_COLOR_BLACK;
		style_container_red.body.main_color = LV_COLOR_BLACK;
		style_container_red.body.border.color = LV_COLOR_RED;
		style_container_red.body.border.width = 4;
		style_container_red.body.radius = 10;
		style_container_red.body.padding.hor = 10;
		style_container_red.body.padding.ver = 10;
		style_container_empty.body.padding.inner = 10;

		/* --------------------------- init loading screen -------------------------- */
		init_loading_screen();

		/* ---------------------------- init image screen --------------------------- */
		init_image_screen();

		/* ---------------------------- init menu screen ---------------------------- */
		init_menu_screen();

		/* --------------------------- load loading screen -------------------------- */
		lv_scr_load(load_screen);
	}


	/* ========================================================================== */
	/*                               Loading Screen                               */
	/* ========================================================================== */

	void init_loading_screen() {


		/* ---------------------------- Screen Definition --------------------------- */

		lv_obj_set_style(load_screen, &style_screen);



		lv_obj_t* container_load = lv_cont_create(load_screen, NULL);
		lv_cont_set_layout(container_load, LV_LAYOUT_ROW_M);
		lv_obj_set_size(container_load, 480, 240);
		lv_obj_set_style(container_load, &style_container_empty);

		/* ========================================================================== */
		/*                                    Text                                    */
		/* ========================================================================== */

		/* ----------------------------- Text Container ----------------------------- */

		lv_obj_t* container_text = lv_cont_create(container_load, NULL);
		lv_cont_set_layout(container_text, LV_LAYOUT_COL_L);
		lv_obj_set_size(container_text, 240, 220);
		lv_obj_set_style(container_text, &style_container_red);

		/* ------------------------------ Loading Text ------------------------------ */

		lv_obj_t* label_load = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load, "Loading ... \n");
		lv_obj_set_style(label_load, &style_text_title);

		/* ------------------------ Setting Variable Launcher ----------------------- */

		static lv_obj_t* label_load_flap = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load_flap, "Init Flap       -  ");
		lv_obj_set_style(label_load_flap, &style_text);

		/* ---------------------- Initializing The Shenanigans ---------------------- */

		static lv_obj_t* label_load_shenan = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load_shenan, "Init Shenan     -  ");
		lv_obj_set_style(label_load_shenan, &style_text);

		/* ---------------------------- Loading Imu Text ---------------------------- */

		static lv_obj_t* label_load_imu = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load_imu, "IMU Calibrate  -");
		lv_obj_set_style(label_load_imu, &style_text);


		/* ========================================================================== */
		/*                                 Preloaders                                 */
		/* ========================================================================== */

		/* --------------------------- Preloader Container -------------------------- */

		lv_obj_t* container_preloader = lv_cont_create(container_load, NULL);
		lv_cont_set_layout(container_preloader, LV_LAYOUT_OFF);
		lv_obj_set_size(container_preloader, 200, 220);
		lv_obj_set_style(container_preloader, &style_container_empty);


		/* ----------------------------- Preloader Style ---------------------------- */

		lv_style_copy(&style_preload, &lv_style_plain);
		style_preload.line.width = 10;
		style_preload.line.color = LV_COLOR_RED;
		style_preload.body.border.width = 6;
		style_preload.body.padding.hor = 0;
		style_preload.body.main_color = LV_COLOR_BLACK;
		style_preload.body.grad_color = LV_COLOR_BLACK;
		style_preload.body.border.color = LV_COLOR_BLACK;


		/* -------------------------------- Preload 1 ------------------------------- */

		lv_obj_t* preload1 = lv_preload_create(container_preloader, NULL);
		lv_obj_set_size(preload1, 175, 175);
		lv_preload_set_arc_length(preload1, 135);
		lv_preload_set_spin_time(preload1, 2000);
		lv_obj_align(preload1, NULL, LV_ALIGN_CENTER, 5, 0);
		lv_preload_set_style(preload1, LV_PRELOAD_STYLE_MAIN, &style_preload);


		/* -------------------------------- Preload 2 ------------------------------- */

		lv_obj_t* preload2 = lv_preload_create(container_preloader, NULL);
		lv_obj_set_size(preload2, 150, 150);
		lv_preload_set_arc_length(preload2, 90);
		lv_preload_set_spin_time(preload2, 1000);
		lv_obj_align(preload2, NULL, LV_ALIGN_CENTER, 5, 0);
		lv_preload_set_style(preload2, LV_PRELOAD_STYLE_MAIN, &style_preload);


		/* -------------------------------- Preload 3 ------------------------------- */

		lv_obj_t* preload3 = lv_preload_create(container_preloader, NULL);
		lv_obj_set_size(preload3, 125, 125);
		lv_preload_set_arc_length(preload3, 45);
		lv_preload_set_spin_time(preload3, 500);
		lv_obj_align(preload3, NULL, LV_ALIGN_CENTER, 5, 0);
		lv_preload_set_style(preload3, LV_PRELOAD_STYLE_MAIN, &style_preload);

	}


	/* ========================================================================== */
	/*                             Brain Image Screen                             */
	/* ========================================================================== */

	void init_image_screen() {


	}


	/* ========================================================================== */
	/*                                 Menu Screen                                */
	/* ========================================================================== */

	void init_menu_screen() {

	}

}