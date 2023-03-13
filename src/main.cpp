#include "ace.h"


/* ========================================================================== */
/*                                 Initialize                                 */
/* ========================================================================== */
void initialize() {

	if ()




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


		pros::delay(20);
	}
	if (master.get_digital_new_press(BTN_ENDGAME_TOGGLE)) {


	}
	/* if (master.get_digital_new_press(BUTTTON_STANDBY)) {
			ace::LAUNCHER_STANDBY_ENABLED = !ace::LAUNCHER_STANDBY_ENABLED;
			ace::launch::launchDisks(0, false);
		}
		*/
	/* ------------------------------ Intake Toggle ----------------------------- */
	if (master.get_digital_new_press(BTN_INTAKE_TOGGLE)) {


	}
	/* ------------------------------ Launch Disks ------------------------------ */
	if (master.get_digital_new_press(BTN_LAUNCH_DISKS)) {

		
	}
	/* ----------------------------- Endgame Toggle ----------------------------- */
	if (master.get_digital_new_press(BTN_ENDGAME_TOGGLE)) {

		
	}
	/* ---------------------------- Conveyor Forward ---------------------------- */
	if (master.get_digital_new_press(BTN_CONVEYOR_FWD)) {

		
	}
	/* ---------------------------- Conveyor Reverse ---------------------------- */
	if (master.get_digital_new_press(BTN_CONVEYOR_REVERSE)) {

		
	}
	/* ----------------------------- Roller Forward ----------------------------- */
	if (master.get_digital_new_press(BTN_ROLLER_FWD)) {

		
	}
	/* ----------------------------- Roller Reverse ----------------------------- */
	if (master.get_digital_new_press(BTN_ROLLER_REVERSE)) {

		
	}
	/* ----------------------------- Intake Reverse ----------------------------- */
	if (master.get_digital_new_press(BTN_INTAKE_REVERSE)) {

		
	}
	/* ------------------------------- Flap Toggle ------------------------------ */
	if (master.get_digital_new_press(BTN_FLAP_TOGGLE)) {

		
	}
	/* ------------------------- Roller Launcher Reverse ------------------------ */
	if (master.get_digital_new_press(BTN_ROLLER_LAUNCHER_REVERSE)) {


	}





















