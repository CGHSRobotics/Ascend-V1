
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

		/* -------------------------------- Get Input ------------------------------- */

		// Intake Toggle 
		if (ace::btn_intake_toggle.get_press_new()) {
			ace::intake_enabled = !ace::intake_enabled;
		}

		// Intake Reverse
		if (ace::btn_intake_reverse.get_press()) {
			ace::intake_enabled = false;
			ace::intake_reverse_enabled = true;
		}
		else {
			ace::intake_reverse_enabled = false;
		}

		// Launcher Short
		ace::launch_short_enabled = ace::btn_launch_short.get_press();

		// Launcher Short
		ace::launch_long_enabled = ace::btn_launch_long.get_press();

		// Endgame Enabled
		ace::endgame_enabled = ace::btn_endgame.get_press();

		/* ------------------------------ User Control ------------------------------ */

		for (int i = 0; i < 1; i++)
		{

			// Endgame
			if (ace::endgame_enabled) {
				ace::endgame_toggle(true);
			}
			else {
				ace::endgame_toggle(false);
			}

			// Endgame
			if (ace::launcher_standby_enabled) {
				ace::launch_standby(true);
			}
			else {
				ace::launch_standby(false, ace::);
			}

			// Launch Short
			if (ace::launch_short_enabled)
			{
				ace::launch(ace::LAUNCH_SPEED_SHORT, false);
				break;
			}

			// Launch Long
			if (ace::launch_long_enabled)
			{
				ace::launch(ace::LAUNCH_SPEED_LONG, true);
				break;
			}

			// Intake Toggle
			if (ace::intake_enabled)
			{
				ace::intake_toggle();
				break;
			}

			// Intake Reverse
			if (ace::intake_reverse_enabled)
			{
				ace::intake_reverse();
				break;
			}



		}

		/* ---------------------------------- Delay --------------------------------- */

		pros::delay(20);
	}

}



















