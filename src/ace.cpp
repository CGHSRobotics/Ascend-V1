
#include "ace.h"

namespace ace {

	/* ========================================================================== */
	/*                        Global Variables Definitions                        */
	/* ========================================================================== */

	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_MASTER);

	Btn_Digi btn_intake_toggle(pros::E_CONTROLLER_DIGITAL_L1);
	Btn_Digi btn_intake_reverse(pros::E_CONTROLLER_DIGITAL_L2);

	Btn_Digi btn_launch_short(pros::E_CONTROLLER_DIGITAL_R1);
	Btn_Digi btn_launch_long(pros::E_CONTROLLER_DIGITAL_R2);


	// Array that holds past drawing operations for priority
	std::vector<std::string> cntr_draw_priority_arr = {};

	// array of things to draw on controller scree
	std::vector<cntrlr_scr_txt> cntr_to_draw_arr = {};

}