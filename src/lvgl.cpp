
#include "ace.h"

namespace ace::lvgl {

	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */




	/* ========================================================================== */
	/*                               Loading Screen                               */
	/* ========================================================================== */





	void create_load_screen() {

		lv_obj_t* load_screen = lv_obj_create(NULL, NULL);

		lv_obj_t* lbl_loading = lv_label_create(load_screen, NULL);
		lv_label_set_text(lbl_loading, " loading random shit...");


		// Delete current screen from memory and load this one
		lv_obj_clean(lv_scr_act());
		lv_scr_load(load_screen);

	}

	/* ========================================================================== */
	/*                             Brain Image Screen                             */
	/* ========================================================================== */

	void create_img_screen() {

		lv_obj_t* img_screen = lv_obj_create(NULL, NULL);

		lv_obj_t* lbl_img = lv_label_create(img_screen, NULL);
		lv_label_set_text(lbl_img, " I identify as an image and use png/jpeg pronouns");


		// Delete current screen from memory and load this one
		lv_obj_clean(lv_scr_act());
		lv_scr_load(img_screen);

	}

}