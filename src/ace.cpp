
#include "ace.h"

namespace ace {

	/* ========================================================================== */
	/*                        Global Variables Definitions                        */
	/* ========================================================================== */

	// Variables
	bool launcher_standby_enabled = false;
	bool intake_enabled = false;
	bool intake_reverse_enabled = false;

	bool partner_connected = false;

	bool launch_short_enabled = false;
	bool launch_long_enabled = false;
	bool endgame_enabled = false;

	// Controllers
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_PARTNER);

	// Motors
	A_Motor intakeMotor(PORT_INTAKE, MOTOR_GEARSET_18, false);
	A_Motor launcherMotor(PORT_LAUNCHER, MOTOR_GEARSET_06, false);

	// Buttons
	Btn_Digi btn_intake_toggle(pros::E_CONTROLLER_DIGITAL_L1);
	Btn_Digi btn_intake_reverse(pros::E_CONTROLLER_DIGITAL_L2);

	Btn_Digi btn_launch_short(pros::E_CONTROLLER_DIGITAL_R1);
	Btn_Digi btn_launch_long(pros::E_CONTROLLER_DIGITAL_R2);

	Btn_Digi btn_roller_forward(pros::E_CONTROLLER_DIGITAL_A);
	Btn_Digi btn_roller_reverse(pros::E_CONTROLLER_DIGITAL_B);

	Btn_Digi btn_endgame(pros::E_CONTROLLER_DIGITAL_DOWN);

	Btn_Digi btn_standby(pros::E_CONTROLLER_DIGITAL_UP);
	// Array that holds past drawing operations for priority
	std::vector<std::string> cntr_draw_priority_arr = {};

	// array of things to draw on controller scree
	std::vector<cntrlr_scr_txt> cntr_to_draw_arr = {};

	std::vector<cntrlr_scr_txt> cntrlr_next_to_draw = {};

	/* -------------------------------------------------------------------------- */
	/*                             User Control Stuffs                            */
	/* -------------------------------------------------------------------------- */

	void launch(float speed, bool isLong) {

		if (launcherMotor.get_actual_velocity() < (speed - LAUNCHER_SPEED_CUTOFF) * 6.0)
		{
			launcherMotor.move_velocity(600);
			return;
		}
		else {
			launcherMotor.move_velocity(speed * 600.0 / 100.0);
			intakeMotor.spin_percent(-100);
		}

	}

	void launch_standby(bool enabled, float speed) {

		intakeMotor.spin_percent(0);

		if (enabled)
			launcherMotor.move_voltage(12000 * (speed / 100.0));
		else
			launcherMotor.move_voltage(0);
	}



	void reset_motors() {
		launcherMotor.move_voltage(0);
		intakeMotor.move_voltage(0);

		launcher_standby_enabled = false;

		endgamePneumatics.set_value(false);
	}


	void endgame_toggle(bool enabled) {
		if (enabled) {
			endgame_timer.reset();
			endgamePneumatics.set_value(1);
			return;
		}
		else {
			if (endgame_timer.done())
			{
				endgamePneumatics.set_value(0);
				return;
			}

			endgame_timer.update(20);
			endgamePneumatics.set_value(1);
		}
	}

	void roller_forward(bool enabled) {
		if (enabled) {
			intakeMotor.spin_percent(roller_speed);
		}
		else {
			intakeMotor.spin_percent(0);
		}
	}

	void roller_reverse(bool enabled) {
		if (enabled) {
			intakeMotor.spin_percent(-roller_speed);
		}
		else {
			intakeMotor.spin_percent(0);


		}
	}

	void intake_toggle() {
		intakeMotor.spin_percent(intake_speed);
	}

	void intake_reverse() {
		intakeMotor.spin_percent(-intake_speed);
	}

	/* -------------------------------------------------------------------------- */
	/*                              Controller Stuffs                             */
	/* -------------------------------------------------------------------------- */

	void auton_page_up() {



	}
	void auton_page_down() {


	}


























}



