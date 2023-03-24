
#include "lvgl.cpp"

/*
	arm-none-eabi-addr2line -faps -e ./bin/hot.package.elf
*/

/*
		0x381f590
		0x78022d0
*/

/* ========================================================================== */
/*                                 Initialize                                 */
/* ========================================================================== */
void initialize() {

	// load lvgl loading screen
	ace::lvgl::init_lvgl();

	// init flap
	lv_label_set_text(ace::lvgl::label_load_flap, "Init Flap       -  OK");

	// clear screen on master controller
	ace::master.clear();
	pros::delay(110);
	ace::master.set_text(0, 1, "Master");
	lv_label_set_text(ace::lvgl::label_load_shenan, "Init Shenan     -  OK");

	//imu
	pros::delay(2000);

	lv_label_set_text(ace::lvgl::label_load_imu, "IMU Calibrate -  OK");
	ace::lvgl::start_preloader_anim();

	//lv_scr_load(ace::lvgl::img_screen);
}

/* -------------------------------- Disabled -------------------------------- */
void disabled() {}

/* ----------------------------       ---------------------------- */
void competition_initialize() {}

/* ========================================================================== */
/*                                 Autonomous                                 */
/* ========================================================================== */
void autonomous() {}

/* ========================================================================== */
/*                                User Control                                */
/* ========================================================================== */
void opcontrol() {


	while (true) {

		/* ------------------------------ Chassis Drive ----------------------------- */

		ace::chassis->getModel()->tank(
			((double)ace::master.get_analog(ANALOG_LEFT_Y)) / 127.0,
			((double)ace::master.get_analog(ANALOG_RIGHT_Y)) / 127.0
		);

		/* ------------------------------ User Control ------------------------------ */


		if (ace::master.get_digital(DIGITAL_R1))
			ace::launch(480, false);
		else
			ace::launch_standby(true, 300);


		// Intake Toggle
		if (ace::master.get_digital(DIGITAL_L1)) {
			ace::intake_toggle();
		}

		// Intake Reverse
		if (ace::master.get_digital(DIGITAL_L2)) {
			ace::intake_reverse();
		}


		/*if (ace::btn_roller_forward.get_press_new()) {
			ace::roller_forward(true);
		}
		else {
			ace::roller_forward(false);
		}

		if (ace::btn_roller_reverse.get_press_new()) {
			ace::roller_reverse(true);
		}
		else {
			ace::roller_reverse(false);
		}
		*/

		/* --------------------------- Controller Drawing --------------------------- */

		//ace::create_cntrlr_screen_txt((std::string)"auton", "Auton: " + std::to_string(666) + "   ", 1, 1, 2);
		//ace::create_cntrlr_screen_txt("partner2", "Partner: " + ace::util::bool_to_str(ace::partner_connected) + "   ", 1, 1, 1);


		/* ---------------------------------- Delay --------------------------------- */

		pros::delay(20);
	}

}



















