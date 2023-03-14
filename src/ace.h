
#ifndef ACE_CGHS
#define ACE_CGHS

#include "main.h"


/* ========================================================================== */
/*                             Ace Util Namespace                             */
/* ========================================================================== */
namespace ace::util {

	/* ========================================================================== */
	/*                            Function Declarations                           */
	/* ========================================================================== */

	/* -------------------------- Celsius To Farenheit -------------------------- */
	static float cel_to_faren(float celsius) {
		return (float)((celsius * 9.0 / 5.0) + 32.0);
	}

	/* -------------------------- Farenheit To Celsius -------------------------- */
	static float faren_to_cel(float farenheit) {
		return (float)((farenheit - 32.0) * 5.0 / 9.0);
	}

}


/* ========================================================================== */
/*                             Main Ace Namespace                             */
/* ========================================================================== */
namespace ace {


	/* ========================================================================== */
	/*                                Device Ports                                */
	/* ========================================================================== */

	/* --------------------------------- Chassis -------------------------------- */

	#define PORT_CHASSIS_L_F 1
	#define PORT_CHASSIS_L_C 2
	#define PORT_CHASSIS_L_B 3

	#define PORT_CHASSIS_R_F 4
	#define PORT_CHASSIS_R_C 5
	#define PORT_CHASSIS_R_B 6


	/* ------------------------- Other Motors / Devices ------------------------- */
	#define PORT_INTAKE 7
	#define PORT_LAUNCHER 8

	#define PORT_VISION 9
	#define PORT_IMU 10


	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */

	/* ------------------------------ Miscellaneuos ----------------------------- */


	/* ------------------------------- SPEEEEEEED ------------------------------- */


	/* --------------------------- Custom Motor Class --------------------------- */
	class A_Motor: public pros::Motor {
		public:

		using Motor::Motor;

		// get temp in farenheit
		float get_temp() {
			return util::cel_to_faren(get_temperature());
		}
	};

	/* ========================================================================== */
	/*                      Device Declaration / Definitions                      */
	/* ========================================================================== */

	/* --------------------------------- Chassis -------------------------------- */

	// Wheel diameter
	const okapi::QLength chassis_wheel_diameter = 3.25_in;

	const okapi::QLength chassis_wheel_track = 11.5_in;

	const std::shared_ptr<okapi::ChassisController> chassis =
		okapi::ChassisControllerBuilder()
		.withMotors(
			{ PORT_CHASSIS_L_F, PORT_CHASSIS_L_C, PORT_CHASSIS_L_B },
			{ PORT_CHASSIS_R_F, PORT_CHASSIS_R_C, PORT_CHASSIS_R_B }
		)
		.withDimensions(okapi::AbstractMotor::gearset::blue, { {chassis_wheel_diameter, chassis_wheel_track}, okapi::imev5BlueTPR })
		.build();


	/* ------------------------- Other Motors / Devices ------------------------- */

	// Controller
	extern pros::Controller master;

	extern pros::Controller partner;

	// Launcher motor
	const A_Motor launcherMotor(PORT_LAUNCHER, MOTOR_GEARSET_06, false);

	// Motor for intake, roller, and DTS
	const pros::Motor intakeMotor(PORT_INTAKE, MOTOR_GEARSET_06, false);


	//Vision sensor
	const pros::Vision  visionSensor(PORT_VISION);

	//
	const pros::IMU imuSensor(PORT_IMU);

	const pros::ADIDigitalOut endgamePneumatics(false);


	/* ========================================================================== */
	/*                                   Buttons                                  */
	/* ========================================================================== */

	class Btn_Digi {
		public:

		// Constructor with one btn
		Btn_Digi(pros::controller_digital_e_t btn_assign, bool is_master = true) {

			keybinds[0] = btn_assign;

			if (is_master)
				mode = 1;
			else
				mode = 2;

		};

		// Constructor with both keybinds
		Btn_Digi(pros::controller_digital_e_t btn_master, pros::controller_digital_e_t btn_partner) {

			keybinds[0] = btn_master;
			keybinds[1] = btn_partner;

			mode = 3;
		};

		// array of keybinds
		std::vector<pros::controller_digital_e_t> keybinds;

		// operating mode for btn. 0 == ur mum gay, 1 == master only, 2 == partner only, 3 == both but preferably partner
		u_int8_t mode;

		// get whether button pressed
		bool get_press() {

			if (mode == 3)
				if (partner.is_connected())
					return partner.get_digital(keybinds[1]);
				else
					return master.get_digital(keybinds[0]);

			else if (mode == 2)
				if (partner.is_connected())
					return partner.get_digital(keybinds[0]);
				else
					return false;

			else if (mode == 1)
				return master.get_digital(keybinds[0]);

			return false;
		};

		// get whether new button press
		bool get_press_new() {

			if (mode == 3)
				if (partner.is_connected())
					return partner.get_digital_new_press(keybinds[1]);
				else
					return master.get_digital_new_press(keybinds[0]);

			else if (mode == 2)
				if (partner.is_connected())
					return partner.get_digital_new_press(keybinds[0]);
				else
					return false;

			else if (mode == 1)
				return master.get_digital_new_press(keybinds[0]);

			return false;
		};
	};

	extern Btn_Digi btn_intake_toggle;
	extern Btn_Digi btn_intake_reverse;

	extern Btn_Digi btn_launch_short;
	extern Btn_Digi btn_launch_long;


	/* ========================================================================== */
	/*                            Function Declarations                           */
	/* ========================================================================== */

	/* ------------------------- Intake Toggle Function ------------------------- */
	extern void intake_toggle(bool enabled);

	/* ------------------------ Emergency Intake Reverse ------------------------ */
	extern void intake_reverse_toggle(bool enabled);

	/* --------------------------------- Launch --------------------------------- */

	/* ------------------------------- Long Launch ------------------------------ */

	/* --------------------------------- Endgame -------------------------------- */
	extern void endgame_toggle(bool enabled);

	/* ------------------------------- Flap Toggle ------------------------------ */




}


/* ========================================================================== */
/*                             Ace Auton Namepace                             */
/* ========================================================================== */
namespace ace::auton {







}


/* ========================================================================== */
/*                            Ace Launch Namespace                            */
/* ========================================================================== */
namespace ace::launch {







}


/* ========================================================================== */
/*                             Ace LVGL Namespace                             */
/* ========================================================================== */
namespace ace::lvgl {







}

/*
I can't do this!


Vanessa, pull yourself together.
You have to snap out of it!


You snap out of it.


You snap out of it.


- You snap out of it!
- You snap out of it!


- You snap out of it!
- You snap out of it!


- You snap out of it!
- You snap out of it!

*/
#endif 
