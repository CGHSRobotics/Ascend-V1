
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
			return "true";
		else
			return "false";
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
	extern bool launcher_standby_enabled;

	extern bool intake_enabled;

	extern bool intake_reverse_enabled;

	extern bool launch_short_enabled;

	extern bool launch_long_enabled;

	extern bool endgame_enabled;

	const std::vector<std::string> auton_selection = { "One Side", "Two Side", "Three Side", "Skills" };

	extern std::string selected_auton;

	extern float selected_auton_num;

	extern float auton_selection_index;
	/* ------------------------------- SPEEEEEEED ------------------------------- */
	const float roller_speed = 80.0;

	const float intake_speed = 100.0;

	const float LAUNCH_SPEED_SHORT = 80.0;

	const float LAUNCH_SPEED_LONG = 100.0;

	const float LAUNCH_SPEED_STANDBY = 50.0;

	const float LAUNCHER_SPEED_CUTOFF = 10.0;

	/* --------------------------- Custom Motor Class --------------------------- */
	class A_Motor: public pros::Motor {
		public:

		using Motor::Motor;

		bool has_init = false;

		void init() {
			if (!has_init) {
				has_init = true;

				set_encoder_units(MOTOR_ENCODER_DEGREES);
			}
		}

		// get temp in farenheit
		float get_temp() {

			init();

			return util::cel_to_faren(get_temperature());
		}
		void spin_percent(float percent) {

			init();

			if (get_gearing() == MOTOR_GEARSET_06) {
				move_velocity(percent / 100.0f * 600.0f);
			}
			else if (get_gearing() == MOTOR_GEARSET_18) {
				move_velocity(percent / 100.0f * 200.0f);
			}
			else if (get_gearing() == MOTOR_GEARSET_36) {
				move_velocity(percent / 100.0f * 100.0f);
			}
			else {
				printf("ERROR CARTRIDGE NOT FOUND");
			}
			move_velocity(percent / 100.0f * 600.0f);
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
	extern A_Motor launcherMotor;

	// Motor for intake, roller, and DTS
	extern A_Motor intakeMotor;


	// Vision sensor
	const pros::Vision visionSensor(PORT_VISION);

	//
	const pros::IMU imuSensor(PORT_IMU);

	const pros::ADIDigitalOut endgamePneumatics(false);


	/* ========================================================================== */
	/*                                   Buttons                                  */
	/* ========================================================================== */

	class Btn_Digi {
		public:
		// vars for btns
		pros::controller_digital_e_t btn_master;
		pros::controller_digital_e_t btn_partner;
		// operating mode for btn. 0 == ur mum gay, 1 == master only, 2 == partner only, 3 == both but preferably partner
		u_int8_t mode;
		// Constructor with one btn
		Btn_Digi(pros::controller_digital_e_t btn_assign, bool is_master = true) {
			if (is_master) {
				mode = 1;
				btn_master = btn_assign;
			}
			else {
				mode = 2;
				btn_partner = btn_assign;
			}
		};
		// Constructor with both keybinds
		Btn_Digi(pros::controller_digital_e_t btn_master, pros::controller_digital_e_t btn_partner) {
			btn_master = btn_master;
			btn_partner = btn_partner;
			mode = 3;
		};
		// get whether button pressed
		bool get_press() {
			if (mode == 3)
				if (partner.is_connected())
					return partner.get_digital(btn_partner);
				else
					return master.get_digital(btn_master);
			else if (mode == 2)
				if (partner.is_connected())
					return partner.get_digital(btn_partner);
				else
					return false;
			else if (mode == 1)
				return master.get_digital(btn_master);
			return false;
		};
		// get whether new button press
		bool get_press_new() {
			if (mode == 3)
				if (partner.is_connected())
					return partner.get_digital_new_press(btn_partner);
				else
					return master.get_digital_new_press(btn_master);
			else if (mode == 2)
				if (partner.is_connected())
					return partner.get_digital_new_press(btn_partner);
				else
					return false;
			else if (mode == 1)
				return master.get_digital_new_press(btn_master);
			return false;
		};
	};

	extern Btn_Digi btn_intake_toggle;
	extern Btn_Digi btn_intake_reverse;
	extern Btn_Digi btn_endgame;

	extern Btn_Digi btn_launch_short;
	extern Btn_Digi btn_launch_long;

	extern Btn_Digi btn_roller_forward;
	extern Btn_Digi btn_roller_reverse;

	extern Btn_Digi btn_auton_page_up;
	extern Btn_Digi btn_auton_page_down;

	extern Btn_Digi btn_standby;


	/* ========================================================================== */
	/*                            Function Declarations                           */
	/* ========================================================================== */

	/* --------------------------------- Standby -------------------------------- */

	/* ------------------------- Intake Toggle Function ------------------------- */
	extern void intake_toggle();

	/* ------------------------ Emergency Intake Reverse ------------------------ */
	extern void intake_reverse();

	/* --------------------------------- Roller --------------------------------- */
	extern void roller_forward(bool enabled);
	extern void roller_reverse(bool enabled);

	/* --------------------------------- Launch --------------------------------- */
	extern void launch(float speed, bool isLong);
	extern void launch_standby(bool enabled, float speed);

	/* --------------------------------- Endgame -------------------------------- */
	static util::timer endgame_timer(200);
	extern void endgame_toggle(bool enabled);

	/* ------------------------------- Flap Toggle ------------------------------ */

	/* --------------------------------- Motors --------------------------------- */
	extern void reset_motors();
	/* ------------------------------- controller ------------------------------- */
	extern void auton_page_up();
	extern void auton_page_down();

	/* ========================================================================== */
	/*                           Controller Screen Task                           */
	/* ========================================================================== */

	extern bool partner_connected;

	// Struct that holds info for drawing stuff to screen
	// Default priority is 4; Max is 8
	struct cntrlr_scr_txt {
		u_int8_t priority;

		u_int8_t mode;

		std::string name;
		std::string txt_to_display;

		u_int8_t col;
		u_int8_t row;
	};

	// Array that holds past drawing operations for priority
	extern std::vector<std::string> cntr_draw_priority_arr;

	// array of things to draw on controller scree
	extern std::vector<cntrlr_scr_txt> cntr_to_draw_arr;

	// adds controller txt to array
	static void add_cntrlr_txt(cntrlr_scr_txt input) {

		// add to priority arr if not already in it
		bool has_name_already = false;
		for (int i = 0; i < cntr_draw_priority_arr.size(); i++)
		{
			if (input.name == cntr_draw_priority_arr[i]) {
				has_name_already = true;
				//printf(cntr_draw_priority_arr[i].c_str());
			}
		}
		if (!has_name_already) {
			printf((input.name).c_str());
			cntr_draw_priority_arr.insert(cntr_draw_priority_arr.begin(), input.name);
		}

		// clear array of old txt if they havent been drawn by the time the newest one comes around
		for (int i = 0; i < cntr_to_draw_arr.size(); i++) {
			cntrlr_scr_txt element = cntr_to_draw_arr[i];
			if (element.name == input.name) {
				cntr_to_draw_arr[i] = input;
				return;
			}
		}

		cntr_to_draw_arr.push_back(input);
	};

	// Function that creates struct from parameters
	static void create_cntrlr_screen_txt(std::string name, std::string txt_to_display, u_int8_t mode, u_int8_t col, u_int8_t row, u_int8_t priority = 4) {

		cntrlr_scr_txt output;

		output.name = name;
		output.txt_to_display = txt_to_display;
		output.mode = mode;
		output.col = col;
		output.row = row;
		output.priority = priority;

		add_cntrlr_txt(output);
	}

	extern std::vector<cntrlr_scr_txt> cntrlr_next_to_draw;

	// draw controller screen
	static void pick_next_screen() {


		if (partner.is_connected())
		{
			if (!partner_connected)
			{
				partner_connected = true;

				cntrlr_scr_txt blank;
				blank.txt_to_display = "";
				blank.mode = 0;
				blank.col = 1;
				blank.row = 1;
				blank.name = "name";

				for (int i = 0; i <= 1000; i += 50)
				{
					cntrlr_next_to_draw.push_back(blank);
				}

				cntrlr_scr_txt clear;
				clear.mode = 4;
				cntrlr_next_to_draw.push_back(clear);

				for (int i = 0; i <= 150; i += 50)
				{
					cntrlr_next_to_draw.push_back(blank);
				}

				cntrlr_scr_txt partner_txt;
				partner_txt.txt_to_display = "Partner";
				partner_txt.mode = 2;
				partner_txt.col = 1;
				partner_txt.row = 0;
				partner_txt.name = "partner";

				cntrlr_next_to_draw.push_back(partner_txt);

				return;
			}
		}
		else
		{
			if (partner_connected)
			{
				partner_connected = false;
			}
		}

		// For priority starting at 8, decreasing
		for (int p = 8; p >= 0; p--)
		{
			// Get last thing to be drawn
			for (int i = 0; i < cntr_draw_priority_arr.size(); i++)
			{
				const std::string cur_priority = std::string(cntr_draw_priority_arr[i]);

				for (int j = 0; j < cntr_to_draw_arr.size(); j++)
				{
					cntrlr_scr_txt element = cntrlr_scr_txt(cntr_to_draw_arr[j]);
					if (element.priority == p)
					{
						if (element.name == cur_priority)
						{
							if (element.mode == 1)
							{
								cntrlr_next_to_draw.push_back(cntrlr_scr_txt(element));
							}
							else if (element.mode == 2)
							{
								cntrlr_next_to_draw.push_back(cntrlr_scr_txt(element));
							}
							else if (element.mode == 3)
							{
								element.mode = 1;
								cntrlr_next_to_draw.push_back(cntrlr_scr_txt(element));
								element.mode = 2;
								cntrlr_next_to_draw.push_back(cntrlr_scr_txt(element));
							}

							// delete draw request from array
							cntr_to_draw_arr.erase(std::next(cntr_to_draw_arr.begin(), j));

							// move name to back of priority list
							cntr_draw_priority_arr.erase(std::next(cntr_draw_priority_arr.begin(), i));
							cntr_draw_priority_arr.push_back(cur_priority);

							return;
						}
					}
				}
			}
		}
	};

	static void draw_controller_screen() {

		while (1)
		{
			if (cntrlr_next_to_draw.size() < 1) {
				pick_next_screen();
			}

			if (cntrlr_next_to_draw.size() > 0) {

				cntrlr_scr_txt element = cntrlr_next_to_draw[0];

				if (element.mode == 1)
				{
					master.set_text(element.row, element.col, element.txt_to_display);
					cntrlr_next_to_draw.erase(std::next(cntrlr_next_to_draw.begin(), 0));
				}
				else if (element.mode == 2)
				{
					partner.set_text(element.row, element.col, element.txt_to_display);
					cntrlr_next_to_draw.erase(std::next(cntrlr_next_to_draw.begin(), 0));
				}
				else if (element.mode == 4)
				{
					partner.clear();
					cntrlr_next_to_draw.erase(std::next(cntrlr_next_to_draw.begin(), 0));
				}
				else if (element.mode == 0)
				{
					cntrlr_next_to_draw.erase(std::next(cntrlr_next_to_draw.begin(), 0));
				}
			}

			pros::delay(50);
		}
	}

	// pros task for function
	const pros::Task cntr_screen_task(draw_controller_screen, "draw_cntr_screen");


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