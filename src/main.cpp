#include "ace.h"


/* ========================================================================== */
/*                                 Initialize                                 */
/* ========================================================================== */
void initialize() {

	// load lvgl loading screen
	ace::lvgl::create_load_screen();

	// Start imu Calibration
	ace::imuSensor.reset();

	/* ------------------- DO STUFF HERE WHISLT IMU CALIBRATES ------------------ */

	// clear screen on master controller
	ace::master.clear();

	while (ace::imuSensor.is_calibrating()) {
		pros::delay(10);
	}

	// load image screen
	ace::lvgl::create_img_screen();

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


		if (ace::btn_intake_toggle.get_press_new())
			ace::intake_toggle();


		ace::create_cntrlr_screen_txt(
			"partner",
			"Partner: " + ace::util::bool_to_str(ace::partner.is_connected()),
			0, 0
		);

		pros::delay(20);
	}

}



















