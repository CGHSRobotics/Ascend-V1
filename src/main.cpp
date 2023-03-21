#include "ace.h"

/*
	arm-none-eabi-addr2line -faps -e ./bin/hot.package.elf
*/

/*
		0x3835714
        0x3835700
        0x7802450
        0x384f098
        0x384d584
        0x3847a14
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
	pros::delay(200);

	lv_label_set_text(ace::lvgl::label_load_shenan, "IMU Calibrate -  OK");

	lv_scr_load(ace::lvgl::img_screen);
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
			ace::master.get_analog(ANALOG_LEFT_Y),
			ace::master.get_analog(ANALOG_RIGHT_Y),
			10.0
		);

		/* ------------------------------ User Control ------------------------------ */

		// Intake Toggle
		if (ace::btn_intake_toggle.get_press_new()) {
			ace::intake_toggle();
		}

		// Intake Reverse
		if (ace::btn_intake_reverse.get_press_new()) {
			ace::intake_reverse();
		}


		if (ace::btn_roller_forward.get_press_new()) {
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

		/* --------------------------- Controller Drawing --------------------------- */

		ace::create_cntrlr_screen_txt((std::string)"auton", "Auton: " + std::to_string(666) + "   ", 1, 1, 2);
		ace::create_cntrlr_screen_txt("partner2", "Partner: " + ace::util::bool_to_str(ace::partner_connected) + "   ", 1, 1, 1);


		/* ---------------------------------- Delay --------------------------------- */

		pros::delay(20);
	}

}



















