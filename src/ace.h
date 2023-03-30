
#ifndef ACE_CGHS
#define ACE_CGHS

#include "main.h"

using std::string;


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

	/* ----------------------------- Bool To String ----------------------------- */
	static std::string bool_to_str(bool input) {
		if (input)
			return "y";
		else
			return "n";
	}

	/* ------------------------------- Timer Class ------------------------------ */
	class timer {
		public:

		float maxTime = 0;
		float currTime = 0;

		timer(float maxTime) {
			maxTime = maxTime;
		}

		void update(float updateTime) {
			currTime += updateTime;
		}

		bool done() {
			return currTime >= maxTime;
		}

		void reset() {
			currTime = 0;
		}
	};
}


/* ========================================================================== */
/*                             Main Ace Namespace                             */
/* ========================================================================== */
namespace ace {


	/* ========================================================================== */
	/*                                Device Ports                                */
	/* ========================================================================== */

	/* --------------------------------- Chassis -------------------------------- */
	#define PORT_CHASSIS_L_F -13
	#define PORT_CHASSIS_L_C -12
	#define PORT_CHASSIS_L_B -14

	#define PORT_CHASSIS_R_F 17
	#define PORT_CHASSIS_R_C 18
	#define PORT_CHASSIS_R_B 16

	/* ------------------------- Other Motors / Devices ------------------------- */
	#define PORT_INTAKE 19
	#define PORT_LAUNCHER 11

	#define PORT_VISION 9
	#define PORT_IMU 15


	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */

	/* ------------------------------ Miscellaneous ----------------------------- */

	// Controller type enum
	enum cntr_t {
		cntr_master = 1,
		cntr_partner = 2,
		cntr_both = 3
	};
	// bool whether
	static bool partner_connected = false;
	static std::vector<std::string> cntr_master_text_arr = { "", "", "", "" };
	static std::vector<std::string> cntr_partner_text_arr = { "", "", "", "" };

	static util::timer endgame_timer(200);

	/* ----------------------- User Control Enabled Bools ----------------------- */

	static bool launcher_standby_enabled = false;
	static bool intake_enabled = false;
	static bool intake_reverse_enabled = false;
	static bool launch_short_enabled = false;
	static bool launch_long_enabled = false;
	static bool endgame_enabled = false;
	static bool roller_forward_enabled = false;
	static bool roller_reverse_enabled = false;

	/* ------------------------------- Autonomous ------------------------------- */

	const std::vector<std::string> auton_selection = { "One Side", "Two Side", "Three Side", "Skills" };
	extern std::string selected_auton;
	extern float selected_auton_num;
	extern float auton_selection_index;

	/* ------------------------------- SPEEEEEEED ------------------------------- */

	const float ROLLER_SPEED = 100.0;
	const float INTAKE_SPEED = 100.0;

	const float LAUNCH_SPEED_SHORT = 85.0;
	const float LAUNCH_SPEED_LONG = 100.0;
	const float LAUNCH_SPEED_STANDBY = LAUNCH_SPEED_SHORT;
	const float LAUNCHER_SPEED_CUTOFF = 10.0;

	/* --------------------------- Custom Motor Class --------------------------- */
	class A_Motor: public pros::Motor {
		public:
		using Motor::Motor;
		bool has_init = false;

		void init();
		float get_temp();
		void spin_percent(float percent);
	};

	/* --------------------------- Custom Button Class -------------------------- */
	class Btn_Digi {
		public:

		pros::controller_digital_e_t btn_master;
		pros::controller_digital_e_t btn_partner;
		cntr_t mode;

		// Constructor with one btn
		Btn_Digi(pros::controller_digital_e_t btn_assign, bool is_master = true);
		// Constructor with both keybinds
		Btn_Digi(pros::controller_digital_e_t btn_master, pros::controller_digital_e_t btn_partner);
		// get whether button pressed
		bool get_press();
		// get whether new button press
		bool get_press_new();
	};

	/* ========================================================================== */
	/*                      Device Declaration / Definitions                      */
	/* ========================================================================== */

	/* --------------------------------- Chassis -------------------------------- */

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

	// master controller
	static pros::Controller master(pros::E_CONTROLLER_MASTER);

	// partner controller
	static pros::Controller partner(pros::E_CONTROLLER_PARTNER);

	// Launcher motor
	static A_Motor launcherMotor(PORT_LAUNCHER, MOTOR_GEARSET_06, false);

	// Motor for intake, roller, and DTS
	static A_Motor intakeMotor(PORT_INTAKE, MOTOR_GEARSET_18, false);

	// Vision sensor
	const pros::Vision visionSensor(PORT_VISION);

	// IMU Sensor
	const pros::IMU imuSensor(PORT_IMU);

	// Endgame Pneumatics
	const pros::ADIDigitalOut endgamePneumatics(1, false);


	/* ========================================================================== */
	/*                                   Buttons                                  */
	/* ========================================================================== */

	// Custom Button for Intake Toggle
	static Btn_Digi btn_intake_toggle(pros::E_CONTROLLER_DIGITAL_L1);

	// Custom Button for Intake Reverse
	static Btn_Digi btn_intake_reverse(pros::E_CONTROLLER_DIGITAL_L2);

	// Custom Button for Short Launch
	static Btn_Digi btn_launch_short(pros::E_CONTROLLER_DIGITAL_R1);

	// Custom Button for Long Launch
	static Btn_Digi btn_launch_long(pros::E_CONTROLLER_DIGITAL_R2);

	// Custom Button for Roller Forward
	static Btn_Digi btn_roller_forward(pros::E_CONTROLLER_DIGITAL_A);

	// Custom Button for Roller Reverse
	static Btn_Digi btn_roller_reverse(pros::E_CONTROLLER_DIGITAL_B);

	// Custom Button for Endgame
	static Btn_Digi btn_endgame(pros::E_CONTROLLER_DIGITAL_DOWN);

	// Custom Button for Standby
	static Btn_Digi btn_standby(pros::E_CONTROLLER_DIGITAL_UP, pros::E_CONTROLLER_DIGITAL_UP);


	/* ========================================================================== */
	/*                            Function Declarations                           */
	/* ========================================================================== */

	/* --------------------------------- Standby -------------------------------- */

	/**
	 * @brief 	runs intake forward
	 *
	 */
	extern void intake_toggle();

	/**
	 * @brief 	runs intake reverse
	 *
	 */
	extern void intake_reverse();

	extern void roller_forward();
	extern void roller_reverse();

	/**
	 * @brief	launch function, called once per frame
	 *
	 * @param speed		speed at which to launch disks
	 * @param isLong	bool whether is long launch or not
	 */
	extern void launch(float speed, bool isLong);

	/**
	 * @brief	launch standby, sets speed / enabled once per frame
	 *
	 * @param enabled	bool whether enabled
	 * @param speed		speed % on how fast standby is
	 */
	extern void launch_standby(bool enabled, float speed);

	/**
	 * @brief	endgame toggle, minimum 200 msec timer on press
	 *
	 * @param enabled	bool whether enabled or not
	 */
	extern void endgame_toggle(bool enabled);

	/* ------------------------------- Flap Toggle ------------------------------ */

	/**
	 * @brief 	resets all motors to 0
	 *
	 */
	extern void reset_motors();

	/**
	 * @brief 	pages up auton control int by one
	 *
	 */
	extern void auton_page_up();

	/**
	 * @brief 	pages down auton control int by one
	 *
	 */
	extern void auton_page_down();

	/* ========================================================================== */
	/*                           Controller Screen Task                           */
	/* ========================================================================== */

	/**
	 * @brief	updates controller text buffers at given row
	 *
	 * @param cntr	controller to display on, from cntr_t enum
	 * @param row	row (0-2) at which to draw text
	 * @param text	std::string text that wants to be drawn
	 */
	extern void update_cntr_text(cntr_t cntr, u_int8_t row, std::string text);

	/**
	 * @brief	compiles controller string arrays into a single string separated by newline; mainly for internal use
	 *
	 * @param arr	array to compile
	 * @return		string in stated format
	 */
	extern std::string cntr_compile_string(std::vector<std::string> arr);

	/**
	 * @brief	function that runs every 50ms and updates controller screen
	 *
	 */
	extern void update_cntr_task();

	// Actual pros::Task for controller update screen function
	static pros::Task __task_update_cntr_task(update_cntr_task, "cntr_update");
}


/* ========================================================================== */
/*                             Ace Auton Namepace                             */
/* ========================================================================== */
namespace ace::auton {







}


/* ========================================================================== */
/*                            Ace Launch Namespace                            */
/* ========================================================================== */


#endif