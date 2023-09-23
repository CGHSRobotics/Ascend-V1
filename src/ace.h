
#ifndef ACE_CGHS
#define ACE_CGHS

#include "main.h"

using std::string;

extern pros::Controller partner;

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

		timer(float max_time) {
			maxTime = max_time;
			currTime = 0;
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
	#define PORT_CHASSIS_L_F -14
	#define PORT_CHASSIS_L_B -13

	#define PORT_CHASSIS_R_F 18
	#define PORT_CHASSIS_R_B 17

	/* ------------------------- Other Motors / Devices ------------------------- */
	#define PORT_INTAKE_LEFT 20
	#define PORT_INTAKE_RIGHT 2
	#define PORT_ENDGAME_LEFT 12
	#define PORT_ENDGAME_RIGHT 19
	#define PORT_LAUNCHER 11

	#define PORT_VISION 10
	#define PORT_IMU 1
	 

	/* ------------------------------- ADI Devices ------------------------------ */

	#define PORT_PNEU_ENDGAME { INTERNAL_ADI_PORT, 'H' }

	#define PORT_PNEU_FLAP { INTERNAL_ADI_PORT, 'D' }

	#define PORT_LIMIT {INTERNAL_ADI_PORT, 'G'}

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
	extern bool partner_connected;
	static std::vector<std::string> cntr_master_text_arr = { "", "", "", "" };
	static std::vector<std::string> cntr_partner_text_arr = { "", "", "", "" };

	extern std::string cntr_haptic_text;
	extern bool new_haptic_request;
	extern bool new_haptic_request_is_master;

	//extern util::timer endgame_timer;
	extern util::timer intake_timer;
	extern util::timer long_launch_timer;

	extern double theta;
	const float auto_target_angle_adjustment = 5;

	const double rad2 = 1.4142;

	extern int ambient_light;

	/* ------------------------------ LED Variables ----------------------------- */
	//Depreciated

	// enum of possible states
	enum led_state_t {
		led_idle = 1,
		led_intake = 2,
		led_launch = 3
	};

	// current led state
	//extern led_state_t curr_led_state;

	// color for red alliance
	const int led_color_red = 0xaa0000;
	const int led_color_red_bright = 0xff0000;
	// color for blue alliance
	const int led_color_blue = 0x0000aa;
	const int led_color_blue_bright = 0x0000ff;



	/* ----------------------- User Control Enabled Bools ----------------------- */

	static bool launcher_standby_enabled = false;
	static bool intake_enabled = false;
	static bool intake_reverse_enabled = false;
	static bool launch_enabled = false;
	static bool endgame_enabled = false;
	static bool endgame_reverse_enabled = false;
	static bool auto_targeting_enabled = false;
	static bool flap_enabled = false;
	static bool lock_enabled = false;
	extern bool is_red_alliance; 

	extern float launch_speed;

	/* ------------------------------- SPEEEEEEED ------------------------------- */

	// Misc Speeds
	const float ROLLER_SPEED = 100.0;
	const float INTAKE_SPEED = 100.0;

	// Launcher Speeds
	
	const float LAUNCH_SPEED = 75.0;

	// Chassis Speeds ( * 1.27 to fit in range of [-127, 127])
	const float DRIVE_SPEED = 87.0 * 1.27; // 87
	const float DRIVE_SPEED_INTAKE = 25.0 * 1.27;
	//20
	const float TURN_SPEED = 71.0 * 1.27; // 71
	const float TURN_SPEED_SLOW = 45.0 * 1.27;
	extern bool curr_launching;

	/* --------------------------- Custom Motor Class --------------------------- */
	class A_Motor: public pros::Motor {
		public:
		using Motor::Motor;
		bool has_init = false;

		void init();
		float get_temp();
		void spin_percent(float percent);
		float get_percent_velocity();
		float get_percent_torque();
	};

	/* --------------------------- Custom Button Class -------------------------- */
	class Btn_Digi {
		public:

		pros::controller_digital_e_t btn_master;
		pros::controller_digital_e_t btn_partner;
		cntr_t mode;

		// Constructor with one btn
		Btn_Digi(pros::controller_digital_e_t btn_assign, cntr_t is_master);
		// get whether button pressed
		bool get_press();
		// get whether new button press
		bool get_press_new();
	};

	/* ========================================================================== */
	/*                      Device Declaration / Definitions                      */
	/* ========================================================================== */


	/* ------------------------- Other Motors / Devices ------------------------- */

	// Launcher motor
	extern A_Motor launcherMotor;

	// Motor for intake left
	extern A_Motor intakeMotorLeft;

	// Motor for intake right
	extern A_Motor intakeMotorRight;

	// Motor for endgame left
	extern A_Motor endgameMotorLeft;

	// Motor for endgame right
	extern A_Motor endgameMotorRight;

	// Vision Sensor
	const pros::Vision visionSensor(PORT_VISION, pros::E_VISION_ZERO_CENTER);

	// IMU Sensor
	const pros::IMU imuSensor(PORT_IMU);

	// Flap Pneumatics
	const pros::ADIDigitalOut flapPneumatics(PORT_PNEU_FLAP, false);

	// Endgame Pneumatics
	const pros::ADIDigitalOut endgamePneumatics(PORT_PNEU_ENDGAME, false);

	// Limit switch
	const pros::ADIDigitalIn limit(PORT_LIMIT);


	/* ========================================================================== */
	/*                                   Buttons                                  */
	/* ========================================================================== */

	/* --------------------------------- Master --------------------------------- */

	// Custom Button for Intake Toggle
	static Btn_Digi btn_intake_toggle(pros::E_CONTROLLER_DIGITAL_L1, cntr_master);

	// Custom Button for Intake Reverse
	static Btn_Digi btn_intake_reverse(pros::E_CONTROLLER_DIGITAL_L2, cntr_master);

	// Custom Button for Launch
	static Btn_Digi btn_launch(pros::E_CONTROLLER_DIGITAL_R1, cntr_master);

	// Custom Button for Endgame Reverse 
	static Btn_Digi btn_endgame_reverse(pros::E_CONTROLLER_DIGITAL_UP, cntr_master);

	// Custom Button for Endgmame Lock
	static Btn_Digi btn_endgame_lock(pros::E_CONTROLLER_DIGITAL_LEFT, cntr_master);

	// Custom Button for Flapjack Toggle
	static Btn_Digi btn_flap(pros::E_CONTROLLER_DIGITAL_RIGHT, cntr_master);

	// Custom Button for Engame Toggle
	static Btn_Digi btn_endgame(pros::E_CONTROLLER_DIGITAL_X, cntr_master);

	/* ---------------------------------- Both ---------------------------------- */
	//Depreciated

	/* --------------------------------- Partner -------------------------------- */

	// Custom Button to Cycle Auton	
	static Btn_Digi btn_auton(pros::E_CONTROLLER_DIGITAL_X, cntr_both);

	// Custom Button to switch alliance 
	static Btn_Digi btn_alliance(pros::E_CONTROLLER_DIGITAL_A, cntr_both);


	// Custom Button to lower short launch speed
	static Btn_Digi btn_launch_speed_increase(pros::E_CONTROLLER_DIGITAL_R1, cntr_partner);

	// Custom Button to lower short launch speed
	static Btn_Digi btn_launch_speed_decrease(pros::E_CONTROLLER_DIGITAL_R2, cntr_partner);

	static Btn_Digi btn_auto_targeting(pros::E_CONTROLLER_DIGITAL_Y, cntr_partner); 



	/* ========================================================================== */
	/*                            Function Declarations                           */
	/* ========================================================================== */

	/* --------------------------------- Standby -------------------------------- */

	/**
	 * @brief 	runs intake forward
	 *
	 */
	extern void intake_toggle(bool enabled);

	/**
	 * @brief 	runs intake reverse
	 *
	 */
	extern void intake_reverse();


	/**
	 * @brief	launch function, called once per frame
	 *
	 * @param speed		speed at which to launch disks
	 * @param isLong	bool whether is long launch or not
	 */
	extern void launch(float speed);

	/**
	 * @brief	endgame toggle, minimum 200 msec timer on press
	 *
	 * @param enabled	bool whether enabled or not
	 */

	extern void flap_toggle(bool enabled);

	/**
	 * @brief 	calls flapjack toggle
	 *
	 */

	extern void endgame(bool enabled);

	extern void endgame_reverse_toggle(bool enabled);

	/**
	 * @brief 	calls endgame reverse toggle
	 *
	 */

	extern void endgame_lock_toggle(bool enabled);

	/**
	 * @brief 	calls engame lock toggle
	 *
	 */
	extern void endgame_auton();

	/**
	 * @brief 	calls endgame toggle in skills for auton
	 *
	 */
	
	/**
	 * @brief 	resets motors when called
	 *
	 */
	extern void reset_motors();

	extern void reset_launcher(float speed);


	/* ------------------------------ Vision Sensor ----------------------------- */

	/**
	 * @brief
	 *
	 * @param enabled 	bool whether to actually move robot or not; in either case will still do theta calculations
	 * @return true
	 * @return false
	 */
	extern void auto_target(bool enabled);

	//

	/* ------------------------------ Light Sensor ------------------------------ */

	extern bool light_sensor_detect();

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
	 * @brief takes in new haptic text to be rumbled on next frame
	 *
	 * @param new_haptic the haptic text ("-", ".") to rumble. see controller.rumble()
	 */
	extern void update_cntr_haptic(std::string new_haptic, bool is_master = true);

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

	// init bool
	extern bool cntr_task_init;


	// Actual pros::Task for controller update screen function
	static pros::Task __task_update_cntr_task(update_cntr_task, "cntr_update");


	/* ========================================================================== */
	/*                                Update LED's                                */
	/* ========================================================================== */
	//Depreciated
	/**
	 * @brief	function that runs every 10ms and updates leds screen
	 *
	 */
	 //extern void update_leds_task();

	 // init bool
	 //extern bool led_task_init;

	 // Actual pros::Task for controller update screen function
	 //static pros::Task __task_update_leds_task(update_leds_task, "leds_update");
}


/* ========================================================================== */
/*                             Ace Auton Namepace                             */
/* ========================================================================== */
namespace ace::auton {

	/* ------------------------------- Autonomous ------------------------------- */

	static std::vector<std::string> auton_selection = {
		"score", "contact", "skills"
	};
	extern int auton_selection_index;

	extern void three_side();
	extern void two_side();
	extern void skills();
	extern void score();
	extern void contact();

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

	/**
	 * @brief 	spins roller by x relative degrees; autonomously
	 *
	 * @param rollerDegrees degrees to spin by
	 */

	extern void launch_auton(float speed);

	extern void drive_chassis(float distance, float speed, bool wait = true);

	extern void turn_chassis(float distance, float speed, bool wait = true);

	extern void endgame_auton();

}


/* ========================================================================== */
/*                            Ace Launch Namespace                            */
/* ========================================================================== */


#endif