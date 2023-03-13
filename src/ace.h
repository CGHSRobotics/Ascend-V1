
#ifndef ACE_CGHS
#define ACE_CGHS

#include "main.h"


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
	/*                                   Buttons                                  */
	/* ========================================================================== */

	#define BTN_INTAKE_TOGGLE  DIGITAL_L1
	#define BTN_LAUNCH_DISKS DIGITAL_R2
	#define BTN_ENDGAME_TOGGLE DIGITAL_UP
	#define BTN_CONVEYOR_FWD DIGITAL_LEFT
	#define BTN_CONVEYOR_REVERSE DIGITAL_RIGHT
	#define BTN_ROLLER_FWD DIGITAL_A
	#define BTN_ROLLER_REVERSE DIGITAL_B
	#define BTN_INTAKE_REVERSE DIGITAL_L2
	#define BTN_FLAP_TOGGLE DIGITAL_A
	#define BTN_ROLLER_LAUNCHER_REVERSE DIGITAL_R1



	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */

	/* ------------------------------ Miscellaneuos ----------------------------- */


	/* ------------------------------- SPEEEEEEED ------------------------------- */


	/* --------------------------- Custom Motor Class --------------------------- */
	class Motor {
		public:
		// constructor
		Motor(pros::Motor motor) {
			pros_motor = motor;
		};
		// pros motor
		pros::Motor pros_motor;
		// get temp in farenheit
		float get_temp() {
			return cel_2_faren(pros_motor.get_temperature());
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
	const pros::Controller master(pros::E_CONTROLLER_MASTER);

	const pros::Controller partner(pros::E_CONTROLLER_PARTNER);
	extern bool is_partner;

	// Launcher motor
	const pros::Motor launcherMotor(PORT_LAUNCHER, MOTOR_GEARSET_06, false);

	// Motor for intake, roller, and DTS
	const pros::Motor intakeMotor(PORT_INTAKE, MOTOR_GEARSET_06, false);


	//Vision sensor
	const pros::Vision  visionSensor(PORT_VISION);

	//
	const pros::IMU imuSensor(PORT_IMU);

	const pros::ADIDigitalOut endgamePneumatics(false);

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
	extern void endgame_toggle(bool enabled)
		/* ------------------------------- Flap Toggle ------------------------------ */




}


/* ========================================================================== */
/*                             Ace Util Namespace                             */
/* ========================================================================== */
namespace ace::util {

	/* ========================================================================== */
	/*                            Function Declarations                           */
	/* ========================================================================== */

	/* -------------------------- Celsius To Farenheit -------------------------- */

	/* -------------------------- Farenheit To Celsius -------------------------- */







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
