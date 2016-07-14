#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"


void dh_hw_mult::do_mult() {
	
	NN_DIGIT a[2], b, c, t, u;
	NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;
	
	// Read inputs	
	b = in_data_1.read();
	c = in_data_2.read();
		
	// Original code from NN_DigitMult()...		
	bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
	bLow = (NN_HALF_DIGIT)LOW_HALF (b);
	cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
	cLow = (NN_HALF_DIGIT)LOW_HALF (c);

	a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
	t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
	u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
	a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;

	if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
	u = TO_HIGH_HALF (t);

	if ((a[0] += u) < u) a[1]++;
	a[1] += HIGH_HALF (t);
	
	// Write outputs
	out_data_low.write(a[0]);
	out_data_high.write(a[1]);
}

/* void dh_hw_mult::process_hw_mult() {

} */

void dh_hw_mult::state_reg() {
				std::cout << "Ping!" << endl;
	for (;;) {
		//If we want to add a reset
		//if (reset.read() == SC_LOGIC_1) state.write(S0_BEGIN);
		//else 
		state.write(next_state.read());
		wait();
	}
}

void dh_hw_mult::state_transition() {
	switch(state.read()) {
		case S0_WAIT:
			if (hw_mult_enable.read() == true) {
				next_state.write(S1_EXECUTE);
				std::cout << "Going to EXECUTE" << endl;
			}
			else {
				next_state.write(S0_WAIT);
			}
			break;
		case S1_EXECUTE:
			dh_hw_mult::do_mult(); // Part 3 is to cut this up into its own machine
			next_state.write(S2_OUTPUT);
			break;
		case S2_OUTPUT:
				std::cout << "Going to FINISH" << endl;
			// Extract output from do_mult?
			hw_mult_done.write(true);
			next_state.write(S3_FINISH);
			break;
		case S3_FINISH:
			if (hw_mult_enable.read() == true) {
				next_state.write(S3_FINISH);
			}
			else {
				std::cout << "Going to WAIT" << endl;
				hw_mult_done.write(false);
				next_state.write(S0_WAIT);
			}
			break;
		default:
			next_state.write(S0_WAIT);
	}
}
