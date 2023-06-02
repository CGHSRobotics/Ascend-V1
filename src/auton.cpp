
#include "ace.h"

namespace ace::auton {

	/* ========================================================================== */
	/*                        Global Variables Definitions                        */
	/* ========================================================================== */

	int auton_selection_index = 0;

	/* ========================================================================== */
	/*                              Three Side Auton                              */
	/* ========================================================================== */
	void three_side() {

		launch_standby(true, 92.5);

		turn_chassis(0.0, TURN_SPEED);
		roller_auton(100);
		drive_chassis(-3.0, DRIVE_SPEED);
		drive_chassis(5.0, DRIVE_SPEED);


		/* ----------------------------- Pickup 3rd Disk ---------------------------- */
		/*
		drive_chassis(5.0, DRIVE_SPEED);

		intake_toggle(true);
		turn_chassis(-45.0, TURN_SPEED);
		drive_chassis(5.5 * rad2, DRIVE_SPEED);
		turn_chassis(45.0, TURN_SPEED);
		drive_chassis(12.0 * rad2, DRIVE_SPEED);

		turn_chassis(0.0, TURN_SPEED);
		drive_chassis(2.0, DRIVE_SPEED);
		turn_chassis(-7.5, TURN_SPEED);

		launch_auton(4000.0, 94.0, true);
		launch_standby(true, 88);
		intake_toggle(true);

		turn_chassis(0.0, TURN_SPEED);
		drive_chassis(-12.0, DRIVE_SPEED);

		turn_chassis(45.0, TURN_SPEED);
		drive_chassis(24.0 * rad2, DRIVE_SPEED_INTAKE);

		turn_chassis(-45.0, TURN_SPEED);
		drive_chassis(4.0 * rad2, DRIVE_SPEED, false);
		chassis.wait_until(10.0 * rad2);
		intake_toggle(false);

		turn_chassis(-20.0, TURN_SPEED);

		pros::delay(1000);
		*/
		turn_chassis(4, TURN_SPEED);
		launch_auton(15000.0, 92.5, true);
		launch_standby(false, 92.5);
		intake_toggle(false);
	}

	// ross is good

	/* ========================================================================== */
	/*                               Two Side Auton                               */
	/* ========================================================================== */
	void two_side() {

		launch_standby(true, 87.5);

		/* ------------------------------- Get Roller ------------------------------- */

		drive_chassis(-20.0, DRIVE_SPEED);
		turn_chassis(90.0, TURN_SPEED);

		//Need to start rollers before touching them - Nick
		roller_auton(100);
		drive_chassis(-11.0, DRIVE_SPEED);

		drive_chassis(5.0, DRIVE_SPEED);

		turn_chassis(110.0, TURN_SPEED);
		//92
		//4000
		launch_auton(3000.0, 87.5, false);
		launch_standby(true, 85.0);

		intake_toggle(true);

		/* ----------------------------- Move To Shoot 1 ---------------------------- */

		turn_chassis(45.0, TURN_SPEED);

		drive_chassis(48.0 * rad2, DRIVE_SPEED_INTAKE);

		/* ------------------------------- Get 3 Disks ------------------------------ */

		pros::delay(1500);

		turn_chassis(146.0, TURN_SPEED);

		launch_auton(4000.0, 85.0, false);

		launch_standby(false, 10.0);
		intake_toggle(false);

	}

	/* ========================================================================== */
	/*                                   Skills                                   */
	/* ========================================================================== */
	void skills() {

		turn_chassis(0, TURN_SPEED);

		launch_standby(true, LAUNCH_SPEED_STANDBY);
		flap_toggle(true);
		drive_chassis(-3, DRIVE_SPEED_INTAKE);

		/* ------------------------------ Get Roller 1 ------------------------------ */

		roller_auton(135);

		/* ------------------- Intake 1 Disk / Move To 2nd Roller ------------------- */
		drive_chassis(5, DRIVE_SPEED);
		intake_toggle(true);
		turn_chassis(-45, TURN_SPEED);
		drive_chassis(12 * rad2, DRIVE_SPEED);

		turn_chassis(90, TURN_SPEED);
		drive_chassis(-14, DRIVE_SPEED_INTAKE);

		/* ------------------------------ Get Roller 2 ------------------------------ */

		roller_auton(135);
		drive_chassis(5.5, DRIVE_SPEED);

		/* -------------------------- Move To Red High Goal ------------------------- */
		turn_chassis(0, TURN_SPEED);
		drive_chassis(60, DRIVE_SPEED);

		/* ------------------------------ Shoot 3 Disks ----------------------------- */
		intake_toggle(false);
		//turn_chassis(-5, TURN_SPEED);
		auto_targeting_enabled = true;
		launch_auton(4000, LAUNCH_SPEED, true);

		/* ------------------ Intake 3 Disks Along Edge Of Low Goal ----------------- */

		turn_chassis(0, TURN_SPEED);
		drive_chassis(0, DRIVE_SPEED);
		intake_toggle(true);

		turn_chassis(88, TURN_SPEED);
		drive_chassis(36, DRIVE_SPEED_INTAKE);
		turn_chassis(90, TURN_SPEED);
		drive_chassis(-35, DRIVE_SPEED);
		turn_chassis(0, TURN_SPEED);

		drive_chassis(0, DRIVE_SPEED);
		turn_chassis(0, TURN_SPEED);

		/* ------------------------ Shoot 2nd Set Of 3 Disks ------------------------ */
		intake_toggle(false);
		//turn_chassis(-5, TURN_SPEED);
		auto_targeting_enabled = true;
		launch_auton(4000, LAUNCH_SPEED, true);

		/* -------------------------- Intake Diagonal Disks ------------------------- */
		turn_chassis(0, TURN_SPEED);
		drive_chassis(-26, DRIVE_SPEED);

		turn_chassis(90, TURN_SPEED);
		intake_toggle(true);

		drive_chassis(24, DRIVE_SPEED);
		turn_chassis(45, TURN_SPEED);

		drive_chassis(24 * rad2, DRIVE_SPEED);


		/* --------------------- Shoot 3rd Set Of 3 Without Flap -------------------- */
		turn_chassis(-45, TURN_SPEED);
		drive_chassis(2.5 * rad2, DRIVE_SPEED);

		flap_toggle(false);

		turn_chassis(-60, TURN_SPEED);
		intake_toggle(false);
		launch_auton(4000, LAUNCH_SPEED, true);
		auto_targeting_enabled = true;
		// turn_chassis(-45, TURN_SPEED);

		/* --------------- Intake Other 3 Disks Along Edge Of Low Goal -------------- */
		intake_toggle(true);

		turn_chassis(-2, TURN_SPEED);

		drive_chassis(54, DRIVE_SPEED_INTAKE);

		drive_chassis(-6, DRIVE_SPEED);
		turn_chassis(-90, TURN_SPEED);

		/* ---------------------- Shoot 4th Set Of 3 Without Flap ---------------------- */
		flap_toggle(false);
		turn_chassis(-110, TURN_SPEED);
		pros::delay(1000);
		intake_toggle(false);
		launch_auton(4000, LAUNCH_SPEED, true);
		//turn_chassis(-90, TURN_SPEED);
		auto_targeting_enabled = true;

		/* ---------------------------- Move To Roller 3 ---------------------------- */

		drive_chassis(-50, DRIVE_SPEED);

		turn_chassis(-180, TURN_SPEED);

		drive_chassis(-6, DRIVE_SPEED);

		/* ------------------------------ Get Roller 3 ------------------------------ */

		roller_auton(135);
		drive_chassis(4, DRIVE_SPEED);

		/* ---------------------------- Move To Roller 4 ---------------------------- */
		intake_toggle(true);

		turn_chassis(-225, TURN_SPEED);

		drive_chassis(16 * rad2, DRIVE_SPEED);

		turn_chassis(-90, TURN_SPEED);

		drive_chassis(-12, DRIVE_SPEED);

		/* ------------------------------ Get Roller 4 ------------------------------ */

		roller_auton(135);

		drive_chassis(20, DRIVE_SPEED);

		turn_chassis(-135, TURN_SPEED);

		drive_chassis(-12, DRIVE_SPEED);

		/* --------------------------------- Endgame -------------------------------- */
		endgame_auton();

		reset_motors();

	}

	/* ========================================================================== */
	/*                         Auton Function Definitions                         */
	/* ========================================================================== */

	/* ------------------------------ Auton Page Up ----------------------------- */
	void auton_page_up() {
		auton_selection_index += 1;
		if (auton_selection_index > auton_selection.size() - 1) {
			auton_selection_index = 0;
		}
	}

	/* ----------------------------- Auton Page Down ---------------------------- */
	void auton_page_down() {
		auton_selection_index -= 1;
		if (auton_selection_index < 0) {}
		auton_selection_index = auton_selection.size() - 1;
	}

	/* ------------------------------ Roller Auton ------------------------------ */
	/*
	void roller_auton(float rollerDegrees) {
	
		float actual_degrees = rollerDegrees * 3.0;
		//Untested, previously only for 1 motor
		intakeMotorLeft.move_relative(actual_degrees, 100);
		intakeMotorRight.move_relative(actual_degrees, 100);
		while (intakeMotorLeft.is_stopped() == 0)
		{
			pros::delay(ez::util::DELAY_TIME);
		}
	}
	*/
	/* --------------------------- Drive Chassis Auton -------------------------- */
	void drive_chassis(float distance, float speed, bool wait) {

		bool slew_enabled = distance >= 14;
		chassis.set_drive_pid(distance, speed, slew_enabled);
		if (wait)
		{
			chassis.wait_drive();
		}
	}

	/* --------------------------- Turn Chassis Auton --------------------------- */
	void turn_chassis(float angle, float speed, bool wait) {
		chassis.set_turn_pid(angle, speed);
		if (wait)
		{
			chassis.wait_drive();
		}
	}

	/* ------------------------------ Launch Auton ------------------------------ */
	void launch_auton(float time, float speed, bool isLong, bool early_exit) {

		ace::util::timer launch_timer(time);

		int launchedCounter = 0;
		while (1)
		{
			launch(speed, isLong);

			// detect if disk launched
			if (light_sensor_detect())
			{
				launchedCounter++;
				long_launch_timer.reset();
			}

			// if 3, exit
			if (launchedCounter >= 3 && early_exit)
			{
				//break;
			}

			// Timer
			launch_timer.update(ez::util::DELAY_TIME);
			if (launch_timer.done())
			{
				break;
			}

			pros::delay(ez::util::DELAY_TIME);
		}
		launcherMotor.spin_percent(0);
	}

	/* ------------------------------ Endgame Auton ----------------------------- */
	void endgame_auton() {

		endgame_toggle(true);
		while (!endgame_timer.done())
		{
			pros::delay(ez::util::DELAY_TIME);
			endgame_toggle(false);
		}
		endgame_toggle(false);
	}
}