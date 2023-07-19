
#include "ace.h"

namespace ace::auton {

	/* ========================================================================== */
	/*                        Global Variables Definitions                        */
	/* ========================================================================== */

	int auton_selection_index = 0;
	/* ========================================================================== */
	/*                               Score Auton                                   */
	/* ========================================================================== */
	void score() {
	drive_chassis(12, DRIVE_SPEED);
	launch_auton(3, launch_speed);
	turn_chassis(180, TURN_SPEED);
	drive_chassis(12, DRIVE_SPEED);
	intake_toggle(true);
	pros::delay(1000);
	intake_toggle(false);
	turn_chassis(180, TURN_SPEED);
	drive_chassis(12, DRIVE_SPEED);
	launch_auton(3, launch_speed);
	//Go for more goals?
	}




	/* ========================================================================== */
	/*                               Contact Auton                                  */
	/* ========================================================================== */
	void contact() {
	drive_chassis(2, DRIVE_SPEED);
	intake_toggle(true);
	pros::delay(1000);
	intake_toggle(false);
	drive_chassis(-2, DRIVE_SPEED);
	//Point to Elevation Bar
	turn_chassis(270, TURN_SPEED);
	drive_chassis(36, DRIVE_SPEED);
	turn_chassis(-90, TURN_SPEED);
	//Endgame TBD

	}	


	





	/* ========================================================================== */
	/*                              Three Side Auton                              */
	/* ========================================================================== */
	void three_side() {
	// Dead
	}

	// ross is good

	/* ========================================================================== */
	/*                               Two Side Auton                               */
	/* ========================================================================== */
	void two_side() {
	// Dead
	}

	/* ========================================================================== */
	/*                                   Skills                                   */
	/* ========================================================================== */
	void skills() {


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
	/*void launch_auton(float time, float speed, bool isLong, bool early_exit) {

		ace::util::timer launch_timer(time);

		int launchedCounter = 0;
		while (1)
		{
			//launch(speed, isLong);
			  launch(speed);
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
	*/

	void launch_auton(float time, float speed)
	{
		ace::util::timer launch_timer(time);
		launch(ace::launch_speed);

		while(!launch_timer.done())
		{
			launch_timer.update(ez::util::DELAY_TIME);

		} {




		}

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