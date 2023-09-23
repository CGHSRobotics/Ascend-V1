

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
	static lv_style_t style_ddm;
	static lv_style_t style_btnm;
	static lv_style_t style_chart;

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
	static lv_obj_t* menu_tab1_cont1_battery_label;


	static lv_obj_t* menu_tab2;
	static lv_obj_t* menu_tab2_auton_drop;
	static lv_obj_t* menu_tab2_cont_main;
	static lv_obj_t* menu_tab2_cont1;
	static lv_obj_t* menu_tab2_cont2;
	static lv_obj_t* menu_tab2_btnm_auton;
	static lv_obj_t* menu_tab2_btnm_alliance;
	static lv_obj_t* menu_tab2_cont1_labelCurrAuton;


	static lv_obj_t* menu_tab3;
	static lv_obj_t* menu_tab3_cont_main;
	static lv_obj_t* menu_tab3_cont1;
	static lv_obj_t* menu_tab3_cont2;
	static lv_obj_t* menu_tab3_cont1_labelTemp1;
	static lv_obj_t* menu_tab3_cont2_labelTemp2;

	static lv_obj_t* menu_tab4;
	static lv_obj_t* menu_tab4_ross;
	static lv_obj_t* menu_tab4_ddlist;
	static lv_obj_t* menu_tab4_chart;
	static lv_chart_series_t* menu_tab4_chart_ser_rpm;
	static lv_chart_series_t* menu_tab4_chart_ser_setrpm;
	static lv_chart_series_t* menu_tab4_chart_ser_torque;

	/* -------------------------- Function Declarations ------------------------- */
	static void brain_screen_update();
	static pros::Task __task_brain_screen_update = pros::Task(brain_screen_update, "screen_update");

	static bool has_init = false;
	static void init_lvgl();
	static void init_styles();

	static void init_loading_screen();
	static void init_main_screen();
	static void init_menu_screen();

	static void start_preloader_anim();

	static lv_res_t main_btn_click(lv_obj_t* btn);
	static lv_res_t menu_btn_click(lv_obj_t* btn);
	static lv_res_t menu_btnm_auton(lv_obj_t* btnm, const char* text);
	static lv_res_t menu_btnm_alliance(lv_obj_t* btnm, const char* text);

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
		lv_scr_load(load_screen);

		/* ---------------------------- init image screen --------------------------- */
		init_main_screen();

		/* ---------------------------- init menu screen ---------------------------- */
		init_menu_screen();

		/* ------------------------------- Start Task ------------------------------- */
		__task_brain_screen_update.set_priority(TASK_PRIORITY_DEFAULT - 1);
		has_init = true;
	}

	/* ========================================================================== */
	/*                             Screen Update Task                             */
	/* ========================================================================== */
	static void brain_screen_update() {

		int selected = 0;

		while (1)
		{
			if (has_init) {

				//Launcher Slider
				int launch_rpm = ace::launcherMotor.get_actual_velocity() * 6;
				lv_bar_set_value(main_bar, launch_rpm);
				lv_label_set_text(main_bar_label, std::to_string(launch_rpm).c_str());
				lv_obj_align(main_bar_label, NULL, LV_ALIGN_IN_TOP_LEFT, 50, (1 - ((float)lv_bar_get_value(main_bar) / 3600.0)) * 220 + 5);

				// Main Screen Text
				lv_label_set_text(menu_tab1_cont1_battery_label,
					(
						"Battery: " + std::to_string(pros::battery::get_capacity()) + "\n" +
						"Master Battery: " + std::to_string(master.get_battery_capacity()) + "\n" +
						"Partner Battery: " + std::to_string(partner.get_battery_capacity()) + "\n" +
						"Theta: " + std::to_string(ace::theta) + "\n" +
						"ERRNO: " + std::to_string(errno)
						).c_str()
				);

				// Set temp Text
				lv_label_set_text(menu_tab3_cont1_labelTemp1,
					(
						(std::string)" Launcher: " + std::to_string((int)ace::launcherMotor.get_temp()) + "\n\n" +
						" Chassis L F: " + std::to_string((int)ace::util::cel_to_faren(chassis.left_motors[0].get_temperature())) + "\n" +
						" Chassis L B: " + std::to_string((int)ace::util::cel_to_faren(chassis.left_motors[1].get_temperature())) + "\n" +
						" "
						).c_str()
				);
				lv_label_set_text(menu_tab3_cont2_labelTemp2,
					(
						
						(std::string)" Intake: " + std::to_string((int)ace::intakeMotorLeft.get_temp()) + "\n\n" +
						" Chassis R F: " + std::to_string((int)ace::util::cel_to_faren(chassis.right_motors[0].get_temperature())) + "\n" +
						" Chassis R B: " + std::to_string((int)ace::util::cel_to_faren(chassis.right_motors[1].get_temperature())) + "\n" +
						" " 
						).c_str()
				);

				// Check launcher History
				if (lv_tabview_get_tab_act(menu_tabview) == 3)
				{

					int curr_selected = lv_ddlist_get_selected(menu_tab4_ddlist);

					// if ddlist selected has changed
					if (selected != curr_selected)
					{
						lv_chart_clear_serie(menu_tab4_chart, menu_tab4_chart_ser_rpm);
						lv_chart_clear_serie(menu_tab4_chart, menu_tab4_chart_ser_setrpm);
						lv_chart_clear_serie(menu_tab4_chart, menu_tab4_chart_ser_torque);
						// if changed to launcher
						if (curr_selected == 1)
						{
							lv_chart_set_range(menu_tab4_chart, -5, 105);
						}
						// if changed to intake
						if (curr_selected == 2)
						{
							lv_chart_set_range(menu_tab4_chart, -105, 105);
						}
						// if changed to light
						if (curr_selected == 3)
						{
							lv_chart_set_range(menu_tab4_chart, 0, 2048);
						}
					}

					// if ross's beautiful face
					if (curr_selected == 0)
					{
						lv_obj_set_hidden(menu_tab4_chart, true);
						lv_obj_set_hidden(menu_tab4_ross, false);
					}
					// if launcher
					else if (curr_selected == 1)
					{
						lv_obj_set_hidden(menu_tab4_ross, true);
						lv_obj_set_hidden(menu_tab4_chart, false);

						lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_rpm, ace::launcherMotor.get_percent_velocity());
						lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_setrpm, ace::launcherMotor.get_voltage() / 120.0f);
						lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_torque, ace::launcherMotor.get_percent_torque());
					}
					// if intake
					else if (curr_selected == 2)
					{
						lv_obj_set_hidden(menu_tab4_ross, true);
						lv_obj_set_hidden(menu_tab4_chart, false);
						//Change for both intake motors?
						lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_rpm, ace::intakeMotorLeft.get_percent_velocity());
						lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_setrpm, ace::intakeMotorLeft.get_voltage() / 120.0f);
						lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_torque, ace::intakeMotorLeft.get_percent_torque());

					}
					// if light
					// inop due to dd menu label removal
					else if (curr_selected == 3)
					{
						lv_obj_set_hidden(menu_tab4_ross, true);
						lv_obj_set_hidden(menu_tab4_chart, false);

						//lv_chart_set_next(menu_tab4_chart, menu_tab4_chart_ser_rpm, ace::lightSensor.get_value());
					}
					else {
						//printf("options in tab4 ddlist no worky :(");
					}

					selected = curr_selected;
				}

			}
			pros::delay(100);
		}
	}

	/* ========================================================================== */
	/*                                 Init Styles                                */
	/* ========================================================================== */
	static void init_styles() {

		/* ------------------------------ Setup Styles ------------------------------ */

		// Style for Screen
		lv_style_copy(&style_screen, &lv_style_plain);
		style_screen.body.main_color = LV_COLOR_WHITE;
		style_screen.body.grad_color = LV_COLOR_GRAY;

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
		style_container_empty.body.grad_color = LV_COLOR_GRAY;
		style_container_empty.body.main_color = LV_COLOR_BLACK;
		style_container_empty.body.border.color = LV_COLOR_BLACK;
		style_container_empty.body.border.width = 0;
		style_container_empty.body.padding.hor = 0;
		style_container_empty.body.padding.ver = 0;
		style_container_empty.body.padding.inner = 10;

		// Style for Container with Red Border
		lv_style_copy(&style_container_red, &lv_style_plain);
		style_container_red.body.grad_color = LV_COLOR_GRAY;
		style_container_red.body.main_color = LV_COLOR_BLACK;
		style_container_red.body.border.color = LV_COLOR_RED;
		style_container_red.body.border.width = 2;
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
		style_btn.body.border.width = 2;
		style_btn.body.radius = 10;
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
		style_tabview_bg.body.padding.hor = 0;
		style_tabview_bg.body.padding.ver = 0;
		style_tabview_bg.body.border.opa = 255;
		style_tabview_bg.body.shadow.width = 0;

		// Tabview Indicator Style Indicator
		lv_style_copy(&style_tabview_indic, &lv_style_pretty);
		style_tabview_indic.body.grad_color = LV_COLOR_RED;
		style_tabview_indic.body.main_color = LV_COLOR_RED;
		style_tabview_indic.body.border.color = LV_COLOR_RED;
		style_tabview_indic.body.border.width = 2;
		style_tabview_indic.body.radius = 0;
		style_tabview_indic.body.padding.hor = 0;
		style_tabview_indic.body.padding.ver = 0;
		style_tabview_indic.body.border.opa = 255;
		style_tabview_indic.body.shadow.width = 0;

		// Tabview Indicator Style Indicator
		lv_style_copy(&style_tabview_pr, &lv_style_pretty);
		style_tabview_pr.body.grad_color = LV_COLOR_RED;
		style_tabview_pr.body.main_color = LV_COLOR_RED;
		style_tabview_pr.body.border.color = LV_COLOR_RED;
		style_tabview_pr.body.border.width = 3;
		style_tabview_pr.body.radius = 0;
		style_tabview_pr.body.padding.hor = 0;
		style_tabview_pr.body.padding.ver = 0;
		style_tabview_pr.body.border.opa = 255;
		style_tabview_pr.body.shadow.width = 0;

		// Tabview Indicator Style Indicator
		lv_style_copy(&style_tabview_rel, &lv_style_pretty);
		style_tabview_rel.body.grad_color = LV_COLOR_BLACK;
		style_tabview_rel.body.main_color = LV_COLOR_BLACK;
		style_tabview_rel.body.border.color = LV_COLOR_RED;
		style_tabview_rel.body.border.width = 2;
		style_tabview_rel.body.radius = 0;
		style_tabview_rel.body.padding.hor = 0;
		style_tabview_rel.body.padding.ver = 12;
		style_tabview_rel.body.border.opa = 255;
		style_tabview_rel.body.shadow.width = 0;
		style_tabview_rel.text = style_text_title.text;

		// Tab 2 Auton Dropdown Style
		lv_style_copy(&style_ddm, &lv_style_pretty);
		style_ddm.body.grad_color = LV_COLOR_BLACK;
		style_ddm.body.main_color = LV_COLOR_BLACK;
		style_ddm.body.border.color = LV_COLOR_RED;
		style_ddm.body.border.width = 2;
		style_ddm.body.radius = 4;
		style_ddm.body.padding.hor = 5;
		style_ddm.body.padding.ver = 10;
		style_ddm.body.padding.inner = 5;
		style_ddm.body.border.opa = 255;
		style_ddm.body.shadow.width = 0;
		style_ddm.text = style_text_title.text;

		//Button Matrix Style
		lv_style_copy(&style_btnm, &lv_style_pretty);
		style_btnm.body.grad_color = LV_COLOR_BLACK;
		style_btnm.body.main_color = LV_COLOR_BLACK;
		style_btnm.body.border.color = LV_COLOR_RED;
		style_btnm.body.border.width = 2;
		style_btnm.body.radius = 4;
		style_btnm.body.padding.hor = 5;
		style_btnm.body.padding.ver = 10;
		style_btnm.body.padding.inner = 5;
		style_btnm.body.border.opa = 255;
		style_btnm.body.shadow.width = 0;
		style_btnm.text = style_text.text;

		// Chart Style
		lv_style_copy(&style_chart, &lv_style_pretty);
		style_chart.body.grad_color = LV_COLOR_MAKE(0x22, 0x22, 0x11);
		style_chart.body.main_color = LV_COLOR_MAKE(0x22, 0x22, 0x11);
		style_chart.body.border.color = LV_COLOR_RED;
		style_chart.body.border.width = 2;
		style_chart.body.radius = 5;
		style_chart.body.padding.hor = 0;
		style_chart.body.padding.ver = 0;
		style_chart.body.border.opa = 255;
		style_chart.body.shadow.width = 0;
		style_chart.line.color = LV_COLOR_RED;
		style_chart.line.width = 1;

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
		lv_obj_set_size(main_btn, 80, 40);
		// Change y back to 170 if brain screen is not broke
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
		lv_tabview_set_anim_time(menu_tabview, 0);
		lv_tabview_set_sliding(menu_tabview, false);

		/* ------------------------------ Tab 1 - Home ------------------------------ */
		menu_tab1 = lv_tabview_add_tab(menu_tabview, "Home");
		lv_page_set_sb_mode(menu_tab1, LV_SB_MODE_OFF);

		// Main Container
		menu_tab1_cont_main = lv_cont_create(menu_tab1, NULL);
		lv_cont_set_fit(menu_tab1_cont_main, false, false);
		lv_obj_set_size(menu_tab1_cont_main, 480, 180);
		lv_obj_set_style(menu_tab1_cont_main, &style_container_empty);
		lv_cont_set_layout(menu_tab1_cont_main, LV_LAYOUT_ROW_T);
		lv_obj_align(menu_tab1_cont_main, NULL, LV_ALIGN_IN_TOP_LEFT, 0, -5);

		// Container on the Right
		menu_tab1_cont1 = lv_cont_create(menu_tab1_cont_main, NULL);
		lv_cont_set_fit(menu_tab1_cont1, false, false);
		lv_obj_set_size(menu_tab1_cont1, 225, 170);
		lv_obj_set_style(menu_tab1_cont1, &style_container_red);
		lv_cont_set_layout(menu_tab1_cont1, LV_LAYOUT_COL_M);

		// Container on the Left
		menu_tab1_cont2 = lv_cont_create(menu_tab1_cont_main, NULL);
		lv_cont_set_fit(menu_tab1_cont2, false, false);
		lv_obj_set_size(menu_tab1_cont2, 225, 170);
		lv_obj_set_style(menu_tab1_cont2, &style_container_red);
		lv_cont_set_layout(menu_tab1_cont2, LV_LAYOUT_COL_M);

		// Return home button 
		menu_tab1_cont2_btn_main = lv_btn_create(menu_tab1_cont2, NULL);
		lv_btn_set_action(menu_tab1_cont2_btn_main, LV_BTN_ACTION_PR, menu_btn_click);
		lv_obj_set_size(menu_tab1_cont2_btn_main, 80, 40);
		lv_btn_set_style(menu_tab1_cont2_btn_main, LV_BTN_STYLE_PR, &style_btn);
		lv_btn_set_style(menu_tab1_cont2_btn_main, LV_BTN_STYLE_REL, &style_btn);

		// Return home button label
		menu_tab1_cont2_btn_main_label = lv_label_create(menu_tab1_cont2_btn_main, NULL);
		lv_label_set_text(menu_tab1_cont2_btn_main_label, "Main");
		lv_obj_set_style(menu_tab1_cont2_btn_main_label, &style_text_title);

		// Battery percent labels
		menu_tab1_cont1_battery_label = lv_label_create(menu_tab1_cont1, NULL);
		lv_obj_set_style(menu_tab1_cont1_battery_label, &style_text);


		/* ------------------------------ Tab 2 - Auton ----------------------------- */
		menu_tab2 = lv_tabview_add_tab(menu_tabview, "Auton");
		lv_page_set_sb_mode(menu_tab2, LV_SB_MODE_OFF);

		menu_tab2_cont_main = lv_cont_create(menu_tab2, menu_tab1_cont_main);
		lv_obj_align(menu_tab2_cont_main, NULL, LV_ALIGN_IN_TOP_LEFT, 0, -5);

		menu_tab2_cont1 = lv_cont_create(menu_tab2_cont_main, menu_tab1_cont1);

		menu_tab2_cont2 = lv_cont_create(menu_tab2_cont_main, menu_tab1_cont2);
		lv_obj_set_style(menu_tab2_cont2, &style_container_empty);

		static const char* btnm_auton_map[32];
		int num_lines = 0;
		for (int i = 0; i < ace::auton::auton_selection.size(); i++) {
			btnm_auton_map[i] = ace::auton::auton_selection[i].c_str();
		}
		btnm_auton_map[ace::auton::auton_selection.size()] = "";

		lv_obj_t* menu_tab2_btnm_auton = lv_btnm_create(menu_tab2_cont2, NULL);
		lv_btnm_set_map(menu_tab2_btnm_auton, btnm_auton_map);
		lv_btnm_set_action(menu_tab2_btnm_auton, menu_btnm_auton);
		lv_obj_set_size(menu_tab2_btnm_auton, 225, 80);
		lv_btnm_set_style(menu_tab2_btnm_auton, LV_BTNM_STYLE_BTN_REL, &style_btnm);

		static const char* btnm_alliance_map[] = { "Red", "Blue", ""};
		lv_obj_t* menu_tab2_btnm_alliance = lv_btnm_create(menu_tab2_cont2, NULL);
		lv_btnm_set_map(menu_tab2_btnm_alliance, btnm_alliance_map);
		lv_btnm_set_action(menu_tab2_btnm_alliance, menu_btnm_alliance);
		lv_obj_set_size(menu_tab2_btnm_alliance, 225, 80);
		lv_btnm_set_style(menu_tab2_btnm_alliance, LV_BTNM_STYLE_BTN_REL, &style_btnm);

		/*
		menu_tab2_cont1_labelCurrAuton = lv_label_create(menu_tab2_cont1, NULL);
		lv_obj_align(menu_tab2_cont_main, NULL, LV_LABEL_ALIGN_LEFT, 0, 0);
		lv_label_set_text("Curr Auton: " + ace::auton::auton_selection + "/n" + "Curr Ally " + ace::auton::alliance_selection).c_str();
		*/
		/* ------------------------------ Tab 3 - Temp ------------------------------ */
		menu_tab3 = lv_tabview_add_tab(menu_tabview, "Temp");
		lv_page_set_sb_mode(menu_tab3, LV_SB_MODE_OFF);

		menu_tab3_cont_main = lv_cont_create(menu_tab3, menu_tab1_cont_main);
		lv_obj_align(menu_tab3_cont_main, NULL, LV_ALIGN_IN_TOP_LEFT, 0, -5);

		menu_tab3_cont1 = lv_cont_create(menu_tab3_cont_main, menu_tab1_cont1);
		menu_tab3_cont2 = lv_cont_create(menu_tab3_cont_main, menu_tab1_cont2);

		menu_tab3_cont1_labelTemp1 = lv_label_create(menu_tab3_cont1, NULL);
		lv_obj_set_style(menu_tab3_cont1_labelTemp1, &style_text);

		menu_tab3_cont2_labelTemp2 = lv_label_create(menu_tab3_cont2, NULL);
		lv_obj_set_style(menu_tab3_cont2_labelTemp2, &style_text);

		/* ------------------------------ Tab 4 - Ross ------------------------------ */
		menu_tab4 = lv_tabview_add_tab(menu_tabview, "Ross");
		lv_page_set_sb_mode(menu_tab4, LV_SB_MODE_OFF);

		menu_tab4_ross = lv_img_create(menu_tab4, NULL);
		lv_obj_align(menu_tab4_ross, NULL, LV_ALIGN_IN_TOP_LEFT, 300, 120);
		lv_img_set_src(menu_tab4_ross, "S:/usd/ross_background.bin");

		menu_tab4_ddlist = lv_ddlist_create(menu_tab4, menu_tab2_auton_drop);
		lv_obj_set_size(menu_tab4_ddlist, 100, 60);
		lv_ddlist_set_options(menu_tab4_ddlist, "ROSS\nLauncher\nIntake");
		lv_obj_align(menu_tab4_ddlist, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);
		lv_ddlist_set_anim_time(menu_tab4_ddlist, 0);

		menu_tab4_chart = lv_chart_create(menu_tab4, NULL);
		lv_obj_set_hidden(menu_tab4_chart, true);
		lv_obj_set_size(menu_tab4_chart, 340, 170);
		lv_obj_align(menu_tab4_chart, NULL, LV_ALIGN_IN_TOP_LEFT, 120, -5);
		lv_chart_set_style(menu_tab4_chart, &style_chart);
		lv_chart_set_point_count(menu_tab4_chart, 100);

		menu_tab4_chart_ser_rpm = lv_chart_add_series(menu_tab4_chart, LV_COLOR_MAKE(0xff, 0x00, 0x00));
		menu_tab4_chart_ser_setrpm = lv_chart_add_series(menu_tab4_chart, LV_COLOR_MAKE(0xff, 0xaa, 0xaa));
		menu_tab4_chart_ser_torque = lv_chart_add_series(menu_tab4_chart, LV_COLOR_MAKE(0x00, 0xff, 0x00));
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
	/*                             Button Click Events                      s      */
	/* ========================================================================== */
	static lv_res_t main_btn_click(lv_obj_t* btn)
	{
		lv_scr_load(menu_screen);
		return LV_RES_OK; /*Return OK if the button is not deleted*/
	}

	static lv_res_t menu_btn_click(lv_obj_t* btn)
	{
		lv_scr_load(main_screen);
		return LV_RES_OK; /*Return OK if the button is not deleted*/
	}

	// set the auton index based off of selected text
	static lv_res_t menu_btnm_auton(lv_obj_t* btnm, const char* text)
	{
		for (int i = 0; i < ace::auton::auton_selection.size(); i++) {
			if (text == ace::auton::auton_selection[i].c_str()) {
				ace::auton::auton_selection_index = i;
			}
		}
		return LV_RES_OK; /*Return OK if the drop down list is not deleted*/
	}

	// set the alliance based off of selected text
	static lv_res_t menu_btnm_alliance(lv_obj_t* btnm, const char* text)
	{
		if (text == "Red") {
			is_red_alliance = true;
		}

		else if (text == "Blue") {
			is_red_alliance = false;
		}

		else {
			ace::update_cntr_haptic("---");
		}
		return LV_RES_OK; /*Return OK if the drop down list is not deleted*/
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