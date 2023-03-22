

#include "ace.h"

// Declare Custom Fonts
LV_FONT_DECLARE(ace_IBMPlexMono_20);
LV_FONT_DECLARE(ace_IBMPlexMono_30);
LV_FONT_DECLARE(ace_IBMPlexMono_40);

namespace ace::lvgl {

	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */

	/* ------------------------------ File Drivers ------------------------------ */
	typedef  FILE* pc_file_t;

	static lv_fs_res_t pcfs_open(void* file_p, const char* fn, lv_fs_mode_t mode);
	static lv_fs_res_t pcfs_close(void* file_p);
	static lv_fs_res_t pcfs_read(void* file_p, void* buf, uint32_t btr, uint32_t* br);
	static lv_fs_res_t pcfs_seek(void* file_p, uint32_t pos);
	static lv_fs_res_t pcfs_tell(void* file_p, uint32_t* pos_p);

	/* --------------------------------- Styles --------------------------------- */
	static lv_style_t style_screen;
	static lv_style_t style_text;
	static lv_style_t style_text_title;
	static lv_style_t style_preload;
	static lv_style_t style_container_empty;
	static lv_style_t style_container_red;

	/* ----------------------------- Loading Screen ----------------------------- */
	static lv_obj_t* load_screen;
	static lv_obj_t* container_load;

	static lv_obj_t* container_text;
	static lv_obj_t* label_load;
	static lv_obj_t* label_load_flap;
	static lv_obj_t* label_load_shenan;
	static lv_obj_t* label_load_imu;

	static lv_obj_t* container_preloader;
	static lv_obj_t* preload1;
	static lv_obj_t* preload2;
	static lv_obj_t* preload3;

	static lv_obj_t* ross_img;
	static lv_obj_t* main_img_load_cont;
	static lv_obj_t* main_img_load;

	/* ------------------------------ Image Screen ------------------------------ */
	static lv_obj_t* img_screen;

	static lv_obj_t* main_img;


	/* -------------------------- Function Declarations ------------------------- */
	static void init_lvgl();

	static void init_loading_screen();
	static void start_preloader_anim();

	static void init_image_screen();
	static void init_menu_screen();

	/* ========================================================================== */
	/*                             Init Lvgl Function                             */
	/* ========================================================================== */

	static void init_lvgl() {

		/* ------------------------------ File Drivers ------------------------------ */
		lv_fs_drv_t pcfs_drv;                         /*A driver descriptor*/
		memset(&pcfs_drv, 0, sizeof(lv_fs_drv_t));    /*Initialization*/

		pcfs_drv.file_size = sizeof(pc_file_t);       /*Set up fields...*/
		pcfs_drv.letter = 'S';
		pcfs_drv.open = pcfs_open;
		pcfs_drv.close = pcfs_close;
		pcfs_drv.read = pcfs_read;
		pcfs_drv.seek = pcfs_seek;
		pcfs_drv.tell = pcfs_tell;
		lv_fs_add_drv(&pcfs_drv);

		/* ------------------------------ Setup Styles ------------------------------ */

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
	static void init_loading_screen() {

		/* ---------------------------- Screen Definition --------------------------- */
		load_screen = lv_obj_create(NULL, NULL);
		lv_obj_set_style(load_screen, &style_screen);

		container_load = lv_cont_create(load_screen, NULL);
		lv_cont_set_layout(container_load, LV_LAYOUT_ROW_M);
		lv_obj_set_size(container_load, 480, 240);
		lv_obj_set_style(container_load, &style_container_empty);

		/* ========================================================================== */
		/*                                    Text                                    */
		/* ========================================================================== */

		/* ----------------------------- Text Container ----------------------------- */
		container_text = lv_cont_create(container_load, NULL);
		lv_cont_set_layout(container_text, LV_LAYOUT_COL_L);
		lv_obj_set_size(container_text, 240, 220);
		lv_obj_set_style(container_text, &style_container_red);

		/* ------------------------------ Loading Text ------------------------------ */
		label_load = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load, "Loading ... \n");
		lv_obj_set_style(label_load, &style_text_title);

		/* ------------------------ Setting Variable Launcher ----------------------- */
		label_load_flap = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load_flap, "Init Flap       -  ");
		lv_obj_set_style(label_load_flap, &style_text);

		/* ---------------------- Initializing The Shenanigans ---------------------- */
		label_load_shenan = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load_shenan, "Init Shenan     -  ");
		lv_obj_set_style(label_load_shenan, &style_text);

		/* ---------------------------- Loading Imu Text ---------------------------- */
		label_load_imu = lv_label_create(container_text, NULL);
		lv_label_set_text(label_load_imu, "IMU Calibrate  -");
		lv_obj_set_style(label_load_imu, &style_text);

		/* ========================================================================== */
		/*                                 Preloaders                                 */
		/* ========================================================================== */

		/* --------------------------- Preloader Container -------------------------- */
		container_preloader = lv_cont_create(container_load, NULL);
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
		preload1 = lv_preload_create(container_preloader, NULL);
		lv_obj_set_size(preload1, 175, 175);
		lv_preload_set_arc_length(preload1, 135);
		lv_preload_set_spin_time(preload1, 2000);
		lv_obj_align(preload1, NULL, LV_ALIGN_CENTER, 5, 0);
		lv_preload_set_style(preload1, LV_PRELOAD_STYLE_MAIN, &style_preload);

		/* -------------------------------- Preload 2 ------------------------------- */
		preload2 = lv_preload_create(container_preloader, NULL);
		lv_obj_set_size(preload2, 150, 150);
		lv_preload_set_arc_length(preload2, 90);
		lv_preload_set_spin_time(preload2, 1000);
		lv_obj_align(preload2, NULL, LV_ALIGN_CENTER, 5, 0);
		lv_preload_set_style(preload2, LV_PRELOAD_STYLE_MAIN, &style_preload);

		/* -------------------------------- Preload 3 ------------------------------- */
		preload3 = lv_preload_create(container_preloader, NULL);
		lv_obj_set_size(preload3, 125, 125);
		lv_preload_set_arc_length(preload3, 45);
		lv_preload_set_spin_time(preload3, 500);
		lv_obj_align(preload3, NULL, LV_ALIGN_CENTER, 5, 0);
		lv_preload_set_style(preload3, LV_PRELOAD_STYLE_MAIN, &style_preload);

		/* ------------------------------- Ross Image ------------------------------- */
		ross_img = lv_img_create(container_preloader, NULL);
		lv_obj_set_hidden(ross_img, true);
		lv_obj_align(ross_img, NULL, LV_ALIGN_CENTER, 28, -12);
		lv_img_set_src(ross_img, "S:/usd/ross_background.bin");

		main_img_load_cont = lv_cont_create(load_screen, NULL);
		lv_cont_set_fit(main_img_load_cont, true, true);
		lv_obj_align(main_img_load_cont, NULL, LV_ALIGN_CENTER, 0, -240);
		lv_preload_set_style(main_img_load_cont, LV_PRELOAD_STYLE_MAIN, &style_preload);

		main_img_load = lv_img_create(main_img_load_cont, NULL);
		lv_obj_set_hidden(main_img_load, true);
		lv_obj_align(main_img_load, NULL, LV_ALIGN_CENTER, 0, 0);
		lv_img_set_src(main_img_load, "S:/usd/ace.bin");
	}

	static void start_preloader_anim() {
		lv_anim_t a;
		a.var = preload1;
		a.start = lv_preload_get_arc_length(preload1);
		a.end = 359;
		a.fp = (lv_anim_fp_t)lv_preload_set_arc_length;
		a.path = lv_anim_path_linear;
		a.end_cb = NULL;
		a.act_time = 0;
		a.time = 200;
		a.playback = 0;
		a.playback_pause = 0;
		a.repeat = 0;
		a.repeat_pause = 0;

		lv_anim_t img;
		img.var = main_img_load_cont;
		img.start = -120;
		img.end = 120;
		img.fp = (lv_anim_fp_t)lv_obj_set_y;
		img.path = lv_anim_path_linear;
		img.end_cb = NULL;
		img.act_time = -200;
		img.time = 200;
		img.playback = 0;
		img.playback_pause = 0;
		img.repeat = 0;
		img.repeat_pause = 0;

		lv_obj_set_hidden(main_img_load, false);
		lv_obj_set_hidden(ross_img, false);

		lv_obj_set_hidden(preload2, true);
		lv_obj_set_hidden(preload3, true);

		lv_anim_create(&a);
	}

	/* ========================================================================== */
	/*                             Brain Image Screen                             */
	/* ========================================================================== */
	static void init_image_screen() {


	}


	/* ========================================================================== */
	/*                                 Menu Screen                                */
	/* ========================================================================== */
	static void init_menu_screen() {

	}


	/* ========================================================================== */
	/*                                File Drivers                                */
	/* ========================================================================== */
	static lv_fs_res_t pcfs_open(void* file_p, const char* fn, lv_fs_mode_t mode)
	{
		errno = 0;
		const char* flags = "";
		if (mode == LV_FS_MODE_WR) flags = "wb";
		else if (mode == LV_FS_MODE_RD) flags = "rb";
		else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "a+";

		char buf[256];
		sprintf(buf, "/%s", fn);
		pc_file_t f = fopen(buf, flags);

		if (f == NULL)
			return LV_FS_RES_UNKNOWN;
		else {
			fseek(f, 0, SEEK_SET);
			pc_file_t* fp = (pc_file_t*)file_p;
			*fp = f;
		}

		return LV_FS_RES_OK;
	}

	static lv_fs_res_t pcfs_close(void* file_p)
	{
		pc_file_t* fp = (pc_file_t*)file_p;
		fclose(*fp);
		return LV_FS_RES_OK;
	}

	static lv_fs_res_t pcfs_read(void* file_p, void* buf, uint32_t btr, uint32_t* br)
	{
		pc_file_t* fp = (pc_file_t*)file_p;
		*br = fread(buf, 1, btr, *fp);
		return LV_FS_RES_OK;
	}

	static lv_fs_res_t pcfs_seek(void* file_p, uint32_t pos)
	{
		pc_file_t* fp = (pc_file_t*)file_p;
		fseek(*fp, pos, SEEK_SET);
		return LV_FS_RES_OK;
	}

	static lv_fs_res_t pcfs_tell(void* file_p, uint32_t* pos_p)
	{
		pc_file_t* fp = (pc_file_t*)file_p;
		*pos_p = ftell(*fp);
		return LV_FS_RES_OK;
	}


}