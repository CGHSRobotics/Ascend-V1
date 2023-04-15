
#include "ace.h"

namespace ace::auton {

	/* ========================================================================== */
	/*                              Three Side Auton                              */
	/* ========================================================================== */
	void three_side() {

		launch_standby(true, 94.0);

		turn_chassis(0.0, TURN_SPEED);
		drive_chassis(-6.0, DRIVE_SPEED);
		//roller_auton(180);

		/* ----------------------------- Pickup 3rd Disk ---------------------------- */
		drive_chassis(6.0, DRIVE_SPEED);

		intake_toggle(true);
		turn_chassis(-45.0, TURN_SPEED);
		drive_chassis(5.5 * rad2, DRIVE_SPEED);
		turn_chassis(45.0, TURN_SPEED);
		drive_chassis(12.0 * rad2, DRIVE_SPEED);

		turn_chassis(0.0, TURN_SPEED);
		drive_chassis(2.0, DRIVE_SPEED);
		turn_chassis(-7.5, TURN_SPEED);

		launch_auton(4000.0, 94.0);
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

		launch_auton(4000.0, 88.0);
		launch_standby(false, 90);
		intake_toggle(false);
	}

	void two_side() {


		roller_auton(45);

		return;


		launch_standby(true, 94.0);

		/* ------------------------------- Get Roller ------------------------------- */

		drive_chassis(-20.0, DRIVE_SPEED);
		turn_chassis(90.0, TURN_SPEED);

		drive_chassis(-10.0, DRIVE_SPEED);
		roller_auton(45);

		/* ----------------------------- Move To Shoot 1 ---------------------------- */
		drive_chassis(6.0, DRIVE_SPEED);
		intake_toggle(true);

		turn_chassis(45.0, TURN_SPEED);

		drive_chassis(24.0 * rad2, DRIVE_SPEED);

		pros::delay(1000);
		turn_chassis(118.0, TURN_SPEED);

		launch_auton(4000.0, 94.0);
		launch_standby(true, 92);
		intake_toggle(true);

		/* ------------------------------- Get 3 Disks ------------------------------ */

		turn_chassis(135.0, TURN_SPEED);
		drive_chassis(6.0 * rad2, DRIVE_SPEED);

		drive_chassis(-6.0 * rad2, DRIVE_SPEED);

		turn_chassis(45.0, TURN_SPEED);

		drive_chassis(24.0 * rad2, DRIVE_SPEED_INTAKE);

		pros::delay(1000);
		turn_chassis(140.0, TURN_SPEED);
		//drive_chassis(6.0 * rad2, DRIVE_SPEED);

		launch_auton(4000.0, 92.0);
		launch_standby(false, 90);
		intake_toggle(false);

	}

	int auton_selection_index = 0;

	void auton_page_up() {
		auton_selection_index += 1;
		if (auton_selection_index > auton_selection.size() - 1) {
			auton_selection_index = 0;
		}
	}

	void auton_page_down() {
		auton_selection_index -= 1;
		if (auton_selection_index < 0) {}
		auton_selection_index = auton_selection.size() - 1;
	}

	void roller_auton(float rollerDegrees) {

		float actual_degrees = rollerDegrees * 3.0;
		intakeMotor.set_zero_position(0);

		intakeMotor.move_relative(actual_degrees, 100);
		while (intakeMotor.is_stopped() == 0)
		{
			pros::delay(ez::util::DELAY_TIME);
		}
	}

	void drive_chassis(float distance, float speed, bool wait) {

		bool slew_enabled = distance >= 14;
		chassis.set_drive_pid(distance, speed, slew_enabled);
		if (wait)
		{
			chassis.wait_drive();
		}
	}

	void turn_chassis(float angle, float speed, bool wait) {
		chassis.set_turn_pid(angle, speed);
		if (wait)
		{
			chassis.wait_drive();
		}
	}

	void launch_auton(float time, float speed, bool early_exit) {

		ace::util::timer launch_timer(time);
		while (1)
		{
			launch(speed, false);

			launch_timer.update(ez::util::DELAY_TIME);
			if (launch_timer.done())
			{
				break;
			}

			pros::delay(ez::util::DELAY_TIME);
		}
		launcherMotor.spin_percent(0);
	}
}