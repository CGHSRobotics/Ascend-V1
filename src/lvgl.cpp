

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
	static lv_style_t style_bar;
	static lv_style_t style_bar_indic;
	static lv_style_t style_btn;
	static lv_style_t style_tabview_bg;
	static lv_style_t style_tabview_indic;
	static lv_style_t style_tabview_pr;
	static lv_style_t style_tabview_rel;	
	static lv_style_t style_tabview_adm;

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

	/* ------------------------------- Main Screen ------------------------------ */
	static lv_obj_t* main_screen;

	static lv_obj_t* main_cont;
	static lv_obj_t* main_img;

	static lv_obj_t* main_bar;
	static lv_obj_t* main_bar_label;

	static lv_obj_t* main_btn;
	static lv_obj_t* main_btn_label;

	/* ------------------------------- Menu Screen ------------------------------ */
	static lv_obj_t* menu_screen;

	static lv_obj_t* menu_tabview;

	static lv_obj_t* menu_tab1;
	static lv_obj_t* menu_tab1_cont_main;
	static lv_obj_t* menu_tab1_cont1;
	static lv_obj_t* menu_tab1_cont2;
	static lv_obj_t* menu_tab1_cont2_btn_main;
	static lv_obj_t* menu_tab1_cont2_btn_main_label;

	static lv_obj_t* menu_tab2;
	static lv_obj_t* menu_tab2_auton_drop;
	static lv_obj_t* menu_tab2_cont_main;
	static lv_obj_t* menu_tab2_cont1;

	static lv_obj_t* menu_tab3;

	static lv_obj_t* menu_tab4;

	/* -------------------------- Function Declarations ------------------------- */
	static void init_lvgl();
	static void init_styles();

	static void init_loading_screen();
	static void init_main_screen();
	static void init_menu_screen();

	static void start_preloader_anim();

	static lv_res_t main_btn_click(lv_obj_t* btn);

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

		/* ------------------------------- Init Styles ------------------------------ */
		init_styles();

		/* --------------------------- init loading screen -------------------------- */
		init_loading_screen();

		/* ---------------------------- init image screen --------------------------- */
		init_main_screen();

		/* ---------------------------- init menu screen ---------------------------- */
		init_menu_screen();

		/* --------------------------- load loading screen -------------------------- */
		lv_scr_load(load_screen);
	}

	/* ========================================================================== */
	/*                                 Init Styles                                */
	/* ========================================================================== */
	static void init_styles() {

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

		// Preloader Style
		lv_style_copy(&style_preload, &lv_style_plain);
		style_preload.line.width = 10;
		style_preload.line.color = LV_COLOR_RED;
		style_preload.body.border.width = 6;
		style_preload.body.padding.hor = 0;
		style_preload.body.main_color = LV_COLOR_BLACK;
		style_preload.body.grad_color = LV_COLOR_BLACK;
		style_preload.body.border.color = LV_COLOR_BLACK;

		// Bar Style
		lv_style_copy(&style_bar, &lv_style_pretty);
		style_bar.body.main_color = LV_COLOR_BLACK;
		style_bar.body.grad_color = LV_COLOR_GRAY;
		style_bar.body.radius = 0;
		style_bar.body.border.color = LV_COLOR_WHITE;

		// Bar Style Indicator
		lv_style_copy(&style_bar_indic, &lv_style_pretty);
		style_bar_indic.body.grad_color = LV_COLOR_YELLOW;
		style_bar_indic.body.main_color = LV_COLOR_RED;
		style_bar_indic.body.radius = 0;
		style_bar_indic.body.shadow.width = 5;
		style_bar_indic.body.shadow.color = LV_COLOR_ORANGE;
		style_bar_indic.body.padding.hor = 3;
		style_bar_indic.body.padding.ver = 3;

		// Button Style Indicator
		lv_style_copy(&style_btn, &lv_style_pretty);
		style_btn.body.grad_color = LV_COLOR_BLACK;
		style_btn.body.main_color = LV_COLOR_BLACK;
		style_btn.body.border.color = LV_COLOR_RED;
		style_btn.body.border.width = 3;
		style_btn.body.radius = 0;
		style_btn.body.padding.hor = 5;
		style_btn.body.padding.ver = 5;
		style_btn.body.border.opa = 255;
		style_btn.body.shadow.width = 0;

		// Tabview BG Style Indicator
		lv_style_copy(&style_tabview_bg, &lv_style_pretty);
		style_tabview_bg.body.grad_color = LV_COLOR_BLACK;
		style_tabview_bg.body.main_color = LV_COLOR_BLACK;
		style_tabview_bg.body.border.color = LV_COLOR_RED;
		style_tabview_bg.body.border.width = 2;
		style_tabview_bg.body.radius = 0;
		style_tabview_bg.body.padding.hor = 5;
		style_tabview_bg.body.padding.ver = 5;
		style_tabview_bg.body.border.opa = 255;
		style_tabview_bg.body.shadow.width = 0;

		// Tabview Indicator Style Indicator
		lv_style_copy(&style_tabview_indic, &lv_style_pretty);
		style_tabview_indic.body.grad_color = LV_COLOR_RED;
		style_tabview_indic.body.main_color = LV_COLOR_RED;
		style_tabview_indic.body.border.color = LV_COLOR_RED;
		style_tabview_indic.body.border.width = 2;
		style_tabview_indic.body.radius = 0;
		style_tabview_indic.body.padding.hor = 5;
		style_tabview_indic.body.padding.ver = 5;
		style_tabview_indic.body.border.opa = 255;
		style_tabview_indic.body.shadow.width = 0;

		// Tabview Indicator Style Indicator
		lv_style_copy(&style_tabview_pr, &lv_style_pretty);
		style_tabview_pr.body.grad_color = LV_COLOR_RED;
		style_tabview_pr.body.main_color = LV_COLOR_RED;
		style_tabview_pr.body.border.color = LV_COLOR_RED;
		style_tabview_pr.body.border.width = 3;
		style_tabview_pr.body.radius = 0;
		style_tabview_pr.body.padding.hor = 5;
		style_tabview_pr.body.padding.ver = 5;
		style_tabview_pr.body.border.opa = 255;
		style_tabview_pr.body.shadow.width = 0;

		// Tabview Indicator Style Indicator
		lv_style_copy(&style_tabview_rel, &lv_style_pretty);
		style_tabview_rel.body.grad_color = LV_COLOR_BLACK;
		style_tabview_rel.body.main_color = LV_COLOR_BLACK;
		style_tabview_rel.body.border.color = LV_COLOR_RED;
		style_tabview_rel.body.border.width = 2;
		style_tabview_rel.body.radius = 0;
		style_tabview_rel.body.padding.hor = 5;
		style_tabview_rel.body.padding.ver = 15;
		style_tabview_rel.body.border.opa = 255;
		style_tabview_rel.body.shadow.width = 0;
		style_tabview_rel.text = style_text_title.text;
		// Tab 2 Auton Dropdown Style
		lv_style_copy(&style_tabview_adm, &lv_style_pretty);
		style_tabview_adm.body.grad_color = LV_COLOR_BLACK;
		style_tabview_adm.body.main_color = LV_COLOR_BLACK;
		style_tabview_adm.body.border.color = LV_COLOR_RED;
		style_tabview_adm.body.border.width = 2;
		style_tabview_adm.body.radius = 0;
		style_tabview_adm.body.padding.hor = 5;
		style_tabview_adm.body.padding.ver = 15;
		style_tabview_adm.body.border.opa = 255;
		style_tabview_adm.body.shadow.width = 0;

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
	}

	/* ========================================================================== */
	/*                             Brain Image Screen                             */
	/* ========================================================================== */
	static void init_main_screen() {

		/* -------------------------- Screen And Container -------------------------- */
		main_screen = lv_obj_create(NULL, NULL);
		lv_obj_set_style(main_screen, &style_screen);

		main_cont = lv_cont_create(main_screen, NULL);
		lv_obj_set_size(main_cont, 480, 240);
		lv_cont_set_fit(main_cont, false, false);
		lv_obj_align(main_cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
		lv_preload_set_style(main_cont, LV_PRELOAD_STYLE_MAIN, &style_preload);

		/* ------------------------------- Main Image ------------------------------- */
		main_img = lv_img_create(main_cont, NULL);
		lv_obj_align(main_img, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
		lv_img_set_src(main_img, "S:/usd/ace.bin");

		/* ----------------------------- Launcher Speed ----------------------------- */
		main_bar = lv_bar_create(main_cont, NULL);
		lv_obj_set_size(main_bar, 30, 220);
		lv_bar_set_range(main_bar, 0, 3600);
		lv_bar_set_value(main_bar, 0.70 * 3600);
		lv_bar_set_style(main_bar, LV_BAR_STYLE_BG, &style_bar);
		lv_bar_set_style(main_bar, LV_BAR_STYLE_INDIC, &style_bar_indic);
		lv_obj_align(main_bar, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

		main_bar_label = lv_label_create(main_cont, NULL);
		lv_obj_align(main_bar_label, NULL, LV_ALIGN_IN_TOP_LEFT, 50, (1 - ((float)lv_bar_get_value(main_bar) / 3600.0)) * 220 + 5);
		lv_label_set_text(main_bar_label, std::to_string(lv_bar_get_value(main_bar)).c_str());
		lv_label_set_style(main_bar_label, &style_text);

		/* ------------------------------- Menu Button ------------------------------ */
		main_btn = lv_btn_create(main_cont, NULL);
		lv_btn_set_fit(main_btn, false, false);
		lv_obj_set_size(main_btn, 90, 50);
		lv_obj_align(main_btn, NULL, LV_ALIGN_IN_TOP_LEFT, 370, 170);
		lv_btn_set_style(main_btn, LV_BTN_STYLE_REL, &style_btn);
		lv_btn_set_action(main_btn, LV_BTN_ACTION_PR, main_btn_click);

		main_btn_label = lv_label_create(main_btn, NULL);
		lv_label_set_text(main_btn_label, "Menu");
		lv_label_set_style(main_btn_label, &style_text_title);

	}


	/* ========================================================================== */
	/*                                 Menu Screen                                */
	/* ========================================================================== */
	static void init_menu_screen() {

		/* --------------------------------- Screen --------------------------------- */
		menu_screen = lv_obj_create(NULL, NULL);

		menu_tabview = lv_tabview_create(menu_screen, NULL);
		lv_tabview_set_style(menu_tabview, LV_TABVIEW_STYLE_BG, &style_tabview_bg);
		lv_tabview_set_style(menu_tabview, LV_TABVIEW_STYLE_INDIC, &style_tabview_indic);
		lv_tabview_set_style(menu_tabview, LV_TABVIEW_STYLE_BTN_PR, &style_tabview_pr);
		lv_tabview_set_style(menu_tabview, LV_TABVIEW_STYLE_BTN_TGL_PR, &style_tabview_pr);
		lv_tabview_set_style(menu_tabview, LV_TABVIEW_STYLE_BTN_REL, &style_tabview_rel);
		lv_tabview_set_style(menu_tabview, LV_TABVIEW_STYLE_BTN_TGL_REL, &style_tabview_rel);

		/* ------------------------------ Tab 1 - Home ------------------------------ */
		menu_tab1 = lv_tabview_add_tab(menu_tabview, "Home");

		menu_tab1_cont_main = lv_cont_create(menu_tab1, NULL);
		lv_cont_set_fit(menu_tab1_cont_main, false, false);
		lv_obj_set_size(menu_tab1_cont_main, 480, 200);
		lv_obj_set_style(menu_tab1_cont_main, &style_container_empty);
		lv_cont_set_layout(menu_tab1_cont_main, LV_LAYOUT_ROW_M);

		menu_tab1_cont1 = lv_cont_create(menu_tab1_cont_main, NULL);
		lv_cont_set_fit(menu_tab1_cont1, false, false);
		lv_obj_set_size(menu_tab1_cont1, 220, 180);
		lv_obj_set_style(menu_tab1_cont1, &style_container_red);
		lv_cont_set_layout(menu_tab1_cont1, LV_LAYOUT_COL_M);

		menu_tab1_cont2 = lv_cont_create(menu_tab1_cont_main, NULL);
		lv_cont_set_fit(menu_tab1_cont2, false, false);
		lv_obj_set_size(menu_tab1_cont2, 220, 180);
		lv_obj_set_style(menu_tab1_cont2, &style_container_red);
		lv_cont_set_layout(menu_tab1_cont2, LV_LAYOUT_COL_M);

		menu_tab1_cont2_btn_main = lv_btn_create(menu_tab1_cont2, NULL);

		menu_tab1_cont2_btn_main_label = lv_label_create(menu_tab1_cont2_btn_main, NULL);
		lv_label_set_text(menu_tab1_cont2_btn_main_label, "Main");   

		/* ------------------------------ Tab 2 - Auton ----------------------------- */
		menu_tab2 = lv_tabview_add_tab(menu_tabview, "Auton");
		menu_tab2_cont_main = lv_cont_create(menu_tab2, NULL);
		lv_cont_set_fit(menu_tab2_cont_main, false, false);
		lv_obj_set_style(menu_tab2_cont_main, &style_container_empty);
		lv_cont_set_layout(menu_tab2_cont_main, LV_LAYOUT_ROW_M);

		menu_tab2_cont1 = lv_cont_create(menu_tab2_cont_main, NULL);
		lv_cont_set_fit(menu_tab2_cont1, false, false);
		lv_obj_set_size(menu_tab2_cont1, 220, 180);
		lv_obj_set_style(menu_tab2_cont1, &style_container_red);
		menu_tab2_auton_drop = lv_ddlist_create(menu_tab2_cont1, NULL);
		lv_ddlist_set_options(menu_tab2_auton_drop, "skills\n"
		"one-side\n"
		"two-side\n"
		"three-side\n"
		);
		lv_obj_align(menu_tab2_auton_drop,NULL,LV_ALIGN_IN_TOP_LEFT, 100, 100);
		lv_obj_set_free_num(menu_tab2_auton_drop, 1);   
		lv_obj_set_style(menu_tab2_auton_drop, &style_tabview_adm);
		/* ------------------------------ Tab 3 - Temp ------------------------------ */
		menu_tab3 = lv_tabview_add_tab(menu_tabview, "Temp");

		/* ------------------------------ Tab 4 - Ross ------------------------------ */
		menu_tab4 = lv_tabview_add_tab(menu_tabview, "Ross");
	}


	/* ========================================================================== */
	/*                             Animation Functions                            */
	/* ========================================================================== */
	static void start_preloader_anim() {

		lv_obj_set_hidden(ross_img, false);

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

		lv_anim_create(&a);
		lv_obj_set_hidden(preload2, true);
		lv_obj_set_hidden(preload3, true);

		pros::delay(1000);

		lv_scr_load(main_screen);
	}

	/* ========================================================================== */
	/*                             Button Click Events                            */
	/* ========================================================================== */
	static lv_res_t main_btn_click(lv_obj_t* btn)
	{
		lv_scr_load(menu_screen);
		return LV_RES_OK; /*Return OK if the button is not deleted*/
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