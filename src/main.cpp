
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

	pros::ADILED led({ INTERNAL_ADI_PORT,'B' }, 60);
	led.set_all(0xffffff);

	// load lvgl loading screen
	ace::lvgl::init_lvgl();

	// init flap
	lv_label_set_text(ace::lvgl::label_load_flap, "Init Flap       -  OK");

	// clear screen on master controller
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
void autonomous() {

	std::string curr_auton = ace::auton::auton_selection[ace::auton::auton_selection_index];

	if (curr_auton == "two_side") {
		ace::auton::two_side();
	}
	else if (curr_auton == "three_side") {
		ace::auton::three_side();
	}


}

/* ========================================================================== */
/*                                User Control                                */
/* ========================================================================== */
void opcontrol() {

	int i = 0;

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

		// roller forward
		ace::roller_forward_enabled = ace::btn_roller_forward.get_press();

		// roller forward
		ace::roller_reverse_enabled = ace::btn_roller_reverse.get_press();

		// Standby Enabled
		if (ace::btn_standby.get_press_new())
		{
			ace::launcher_standby_enabled = !ace::launcher_standby_enabled;
		}

		//Auton Page Up
		if (ace::btn_auton.get_press_new())
		{
			ace::auton::auton_page_up();
		}

		/* ------------------------------ User Control ------------------------------ */

		for (int i = 0; i < 1; i++)
		{

			// Endgame
			if (ace::endgame_enabled)
			{
				ace::endgame_toggle(true);
			}
			else {
				ace::endgame_toggle(false);
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

			// roller forward
			if (ace::roller_forward_enabled)
			{
				ace::roller_forward();
				break;
			}

			// roller reverse 
			if (ace::roller_reverse_enabled)
			{
				ace::roller_reverse();
				break;
			}

			// Intake Reverse
			if (ace::intake_reverse_enabled)
			{
				ace::intake_reverse();
				break;
			}

			// Intake Toggle
			if (ace::intake_enabled)
			{
				ace::intake_toggle();
				break;
			}
			else {
				ace::intakeMotor.spin_percent(0);
			}

			// launcher standby
			if (ace::launcher_standby_enabled) {
				ace::launch_standby(true, ace::LAUNCH_SPEED_STANDBY);
			}
			else {
				ace::launch_standby(false, ace::LAUNCH_SPEED_STANDBY);
			}

		}

		/* ------------------------- Controller Screen Draw ------------------------- */

		ace::update_cntr_text(ace::cntr_master, 0, "Master");
		ace::update_cntr_text(ace::cntr_partner, 0, "Partner");

		ace::update_cntr_text(ace::cntr_both, 1, " " + ace::auton::auton_selection[ace::auton::auton_selection_index]);
		ace::update_cntr_text(ace::cntr_both, 2,
			(std::string)"  " + "idle? " + ace::util::bool_to_str(ace::launcher_standby_enabled)
			+ "    " + "flap? " + ace::util::bool_to_str(false)
		);
		printf((ace::auton::auton_selection[ace::auton::auton_selection_index] + "\n").c_str());


		/* ---------------------------------- Delay --------------------------------- */

		pros::delay(20);
	}

}



















