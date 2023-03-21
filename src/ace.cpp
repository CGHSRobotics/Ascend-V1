
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

	// Controllers
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_PARTNER);

	// Motors
	A_Motor intakeMotor(PORT_INTAKE,MOTOR_GEARSET_18, false);
	A_Motor launcherMotor(PORT_LAUNCHER, MOTOR_GEARSET_06, false);

	// Other Devices


	// Buttons
	Btn_Digi btn_intake_toggle(pros::E_CONTROLLER_DIGITAL_L1);
	Btn_Digi btn_intake_reverse(pros::E_CONTROLLER_DIGITAL_L2);

	Btn_Digi btn_launch_short(pros::E_CONTROLLER_DIGITAL_R1);
	Btn_Digi btn_launch_long(pros::E_CONTROLLER_DIGITAL_R2);

	Btn_Digi btn_roller_forward(pros::E_CONTROLLER_DIGITAL_A);
	Btn_Digi btn_roller_reverse(pros::E_CONTROLLER_DIGITAL_B);


	// Array that holds past drawing operations for priority
	std::vector<std::string> cntr_draw_priority_arr = {};

	// array of things to draw on controller scree
	std::vector<cntrlr_scr_txt> cntr_to_draw_arr = {};

	std::vector<cntrlr_scr_txt> cntrlr_next_to_draw = {};

	/* -------------------------------------------------------------------------- */
	/*                             User Control Stuffs                            */
	/* -------------------------------------------------------------------------- */


	void reset_motors() {
		launcherMotor.move_voltage(0);
		intakeMotor.move_voltage(0);

		launcher_standby_enabled = false;

		endgamePneumatics.set_value(false);
	}


	void endgame_toggle(bool enabled) {
		if (enabled) {
			endgamePneumatics.set_value(1);

		}
		else {
			endgamePneumatics.set_value(0);

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

		intake_enabled = !intake_enabled;

		if (!intake_enabled) {
			intakeMotor.spin_percent(intake_speed);
		}
		else {
			intakeMotor.spin_percent(0);



		}
	}

	void intake_reverse() {

		intake_enabled = false;

		intakeMotor.spin_percent(-intake_speed);
	}

	/* -------------------------------------------------------------------------- */
	/*                              Controller Stuffs                             */
	/* -------------------------------------------------------------------------- */

	void auton_page_up() {
		selected_auton_num = auton_selection_index += 1;



	}
	void auton_page_down() {
		selected_auton_num -= 1;
		if (selected_auton_num <= 0) {
			selected_auton_num = 4;
		} 


	}


























}



