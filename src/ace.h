
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

	/* ----------------------------- Bool To String ----------------------------- */
	static std::string bool_to_str(bool input) {
		if (input)
			return "true";
		else
			return "false";
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
	extern bool intake_enabled;
	extern void intake_toggle();

	/* ------------------------ Emergency Intake Reverse ------------------------ */
	extern bool intake_reverse_enabled;
	extern void intake_reverse_toggle();

	/* --------------------------------- Launch --------------------------------- */

	/* ------------------------------- Long Launch ------------------------------ */

	/* --------------------------------- Endgame -------------------------------- */
	extern void endgame_toggle(bool enabled);

	/* ------------------------------- Flap Toggle ------------------------------ */



	/* ========================================================================== */
	/*                           Controller Screen Task                           */
	/* ========================================================================== */

	// Struct that holds info for drawing stuff to screen
	// Default priority is 4; Max is 8
	struct cntrlr_scr_txt {
		u_int8_t priority;

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
			if (input.name == cntr_draw_priority_arr[i])
				has_name_already = true;
		}
		if (!has_name_already)
			cntr_draw_priority_arr.insert(cntr_draw_priority_arr.begin(), input.name);


		// clear array of old txt if they havent been drawn by the time the newest one comes around
		for (int i = 0; i < cntr_to_draw_arr.size(); i++) {
			cntrlr_scr_txt element = cntr_to_draw_arr[i];
			if (element.name == input.name)
				cntr_to_draw_arr.erase(cntr_to_draw_arr.begin() + i);
		}

		cntr_to_draw_arr.push_back(input);
	};

	// Function that creates struct from parameters
	static void create_cntrlr_screen_txt(std::string name, std::string txt_to_display, u_int8_t col, u_int8_t row, u_int8_t priority = 4) {

		cntrlr_scr_txt output;

		output.name = name;
		output.txt_to_display = txt_to_display;
		output.col = col;
		output.row = row;
		output.priority = priority;

		add_cntrlr_txt(output);
	}

	// draw controller screen
	static void draw_controller_screen() {

		while (1) {

			for (int p = 8; p >= 0; p--)
			{
				for (int i = 0; i < cntr_draw_priority_arr.size(); i++)
				{
					const std::string cur_priority = cntr_draw_priority_arr[i];

					for (int j = 0; j < cntr_to_draw_arr.size(); j++)
					{
						const cntrlr_scr_txt element = cntr_to_draw_arr[j];
						if (element.priority == p)
						{
							if (element.name == cur_priority)
							{
								// set text to controller
								master.set_text(element.row, element.col, element.txt_to_display);

								// delete draw request from array
								cntr_to_draw_arr.erase(cntr_to_draw_arr.begin() + j);

								// move name to back of priority list
								cntr_draw_priority_arr.erase(cntr_draw_priority_arr.begin() + i);
								cntr_draw_priority_arr.push_back(cur_priority);

								goto delay_label;
							}
						}
					}
				}
			}

		delay_label:

			pros::delay(50);
		}
	};

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
namespace ace::launch {







}


/* ========================================================================== */
/*                             Ace LVGL Namespace                             */
/* ========================================================================== */
namespace ace::lvgl {

	/* ========================================================================== */
	/*                              Global Variables                              */
	/* ========================================================================== */




	/* ========================================================================== */
	/*                               Loading Screen                               */
	/* ========================================================================== */

	extern lv_obj_t* load_screen;	// lv_obj for loading screen

	extern lv_obj_t* lbl_loading;	// label for loading 

	extern void create_load_screen();




	/* ========================================================================== */
	/*                             Brain Image Screen                             */
	/* ========================================================================== */

	extern lv_obj_t* img_screen;	// lv_obj for loading screen

	extern lv_obj_t* lbl_img;	// label for loading 


	extern void create_img_screen();



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
