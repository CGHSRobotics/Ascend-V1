
#include "lvgl.cpp"

/*
	arm-none-eabi-addr2line -faps -e ./bin/hot.package.elf
	arm-none-eabi-addr2line -faps -e ./bin/cold.package.elf
*/

/*
0x380662e
		0x3832e0c
*/

/* ========================================================================== */
/*                                 Initialize                                 */
/* ========================================================================== */
void initialize() {

	// load lvgl loading screen
	ace::lvgl::init_lvgl();

	pros::delay(500); // Stop the user from doing anything while legacy ports configure.

	//pros::ADILED led({ INTERNAL_ADI_PORT,'B' }, 60);
	//led.set_all(0xffffff);

	// Configure your chassis controls
	chassis.toggle_modify_curve_with_controller(false); // Enables modifying the controller curve with buttons on the joysticks
	chassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
	chassis.set_curve_default(10, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
	default_constants(); // Set the drive to your own constants from autons.cpp!
	exit_condition_defaults(); // Set the exit conditions to your own constants from autons.cpp!

	// init flap
	lv_label_set_text(ace::lvgl::label_load_flap, "Init Flap       -  OK");

	// clear screen on master controller
	master.set_text(0, 1, "Master");
	lv_label_set_text(ace::lvgl::label_load_shenan, "Init Shenan     -  OK");

	// init chassis
	chassis.initialize();
	ace::intakeMotor.init();
	ace::launcherMotor.init();
	pros::lcd::shutdown();

	ace::endgame_timer.currTime = ace::endgame_timer.maxTime + 100;

	// get ambient light sample
	ace::ambient_light = ace::lightSensor.get_value();

	// Go to main screen
	lv_label_set_text(ace::lvgl::label_load_imu, "IMU Calibrate -  OK");
	ace::lvgl::start_preloader_anim();

	ace::update_cntr_haptic(".");
}

/* -------------------------------- Disabled -------------------------------- */
void disabled() {}

/* ----------------------------       ---------------------------- */
void competition_initialize() {}

/* ========================================================================== */
/*                                 Autonomous                                 */
/* ========================================================================== */
void autonomous() {

	chassis.reset_pid_targets(); // Resets PID targets to 0
	chassis.reset_gyro(); // Reset gyro position to 0
	chassis.reset_drive_sensor(); // Reset drive sensors to 0
	chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

	std::string curr_auton = ace::auton::auton_selection[ace::auton::auton_selection_index];

	if (curr_auton == "two") {
		ace::auton::two_side();
	}
	else if (curr_auton == "three") {
		ace::auton::three_side();
	}

	//ace::update_cntr_haptic("...");
}

/* ========================================================================== */
/*                                User Control                                */
/* ========================================================================== */
void opcontrol() {

	int i = 0;

	chassis.set_drive_brake(MOTOR_BRAKE_COAST);

	while (true) {

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

		// auto targeting toggle
		if (ace::btn_auto_targeting.get_press_new())
		{
			ace::auto_targeting_enabled = !ace::auto_targeting_enabled;
		}

		// auto targeting toggle
		if (ace::btn_flap.get_press_new())
		{
			ace::flap_enabled = !ace::flap_enabled;
		}

		//Auton Page Up
		if (ace::btn_auton.get_press_new())
		{
			ace::auton::auton_page_up();
		}

		//Alliance Toggle
		if (ace::btn_alliance.get_press_new())
		{
			ace::is_red_alliance = !ace::is_red_alliance;
		}

		if (ace::light_sensor_detect()) {
			//ace::update_cntr_haptic("..");
		}


		/* --------------------------- Chassis Tank Drive --------------------------- */
		if (ace::auto_targeting_enabled)
		{
			ace::auto_target(true);
		}
		/*else if (std::abs(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_Y)) < 32 && std::abs(master.get_analog(ANALOG_LEFT_Y)) > 95) {

			float avg = (master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_Y)) / 2.0;

			//chassis.set(avg, avg);
		}*/
		else {
			ace::auto_target(false);
			chassis.tank();
		}


		/* ------------------------------ User Control ------------------------------ */

		for (int i = 0; i < 1; i++)
		{

			// Endgame
			ace::endgame_toggle(ace::endgame_enabled);

			// flap
			ace::flap_toggle(ace::flap_enabled);


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

			// launcher standby
			ace::launch_standby(ace::launcher_standby_enabled, ace::LAUNCH_SPEED_STANDBY);

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
				ace::intake_toggle(true);
				break;
			}
			else {
				ace::intakeMotor.spin_percent(0);
			}
		}

		/* ------------------------- Controller Screen Draw ------------------------- */

		ace::update_cntr_text(ace::cntr_master, 0,
			(std::string)"Master" +
			"  " + std::to_string((int)ace::intakeMotor.get_temp()) + "F" +
			"  " + std::to_string((int)pros::battery::get_capacity()) + "%"
		);
		ace::update_cntr_text(ace::cntr_partner, 0,
			(std::string)"Partner" +
			"  " + std::to_string((int)ace::intakeMotor.get_temp()) + "F" +
			"  " + std::to_string((int)pros::battery::get_capacity()) + "%"
		);


		std::string ally_str = "";
		if (ace::is_red_alliance) {
			ally_str = "red";
		}
		else {
			ally_str = "blue";
		}
		ace::update_cntr_text(ace::cntr_both, 1,
			"auto? " + ace::util::bool_to_str(ace::auto_targeting_enabled) +
			"  " + ace::auton::auton_selection[ace::auton::auton_selection_index] +
			" " + ally_str
		);

		ace::update_cntr_text(ace::cntr_both, 2,
			(std::string)"idle? " + ace::util::bool_to_str(ace::launcher_standby_enabled) +
			"  flap? " + ace::util::bool_to_str(ace::flap_enabled)
		);

		/* ---------------------------------- Delay --------------------------------- */

		pros::delay(ez::util::DELAY_TIME);
	}

}



















