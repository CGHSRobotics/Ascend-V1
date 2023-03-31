
#include "ace.h"

namespace ace::auton {

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

	void three_side() {
		// hi barry
	}

	void two_side() {
		// hi barry
	}

}