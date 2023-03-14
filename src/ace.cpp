
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

}