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

void dh_hw_mult::mult_a0 {
	mult_a0_out->write( (NN_DIGIT)((NN_HALF_DIGIT)LOW_HALF (b) * (NN_HALF_DIGIT)LOW_HALF (c)) );
}

void dh_hw_mult::mult_a1 {
	mult_a1_out->write( (NN_DIGIT)((NN_HALF_DIGIT)HIGH_HALF (b) * (NN_HALF_DIGIT)HIGH_HALF (c)) );
}

void dh_hw_mult::mult_t {
	mult_t_out->write( (NN_DIGIT)((NN_HALF_DIGIT)LOW_HALF (b) * (NN_HALF_DIGIT)HIGH_HALF (c)) );
}

void dh_hw_mult::mult_u {
	mult_u_out->write( (NN_DIGIT)((NN_HALF_DIGIT)HIGH_HALF (b) * (NN_HALF_DIGIT)LOW_HALF (c)) );	
}

a0_mem


/* void dh_hw_mult::process_hw_mult() {

} */

void dh_hw_mult::state_reg() {
	for (;;) {
		//If we want to add a reset
		//if (reset.read() == SC_LOGIC_1) state.write(S0_BEGIN);
		//else 
		// if (next_state.read() != state.read()) {
			// std::cout << "Curr: " << state.read() << "Next: " << next_state.read() << endl;
			state.write(next_state.read());
		// }
		wait();
	}
}

void dh_hw_mult::state_transition() {
	for(;;) {
		// std::cout << "Transitioning!! ";
		switch(state.read()) {
			case S0_WAIT:
				if (hw_mult_enable.read() == true) {
					next_state.write(S1_EXECUTE);
					// std::cout << "Going to EXECUTE" << endl;
				}
				else {
					next_state.write(S0_WAIT);
				}
				break;
				
			case S1_EXECUTE:
				next_state.write(S2_OUTPUT);
				break;
				
			case S2_OUTPUT:
				if (hw_mult_enable.read() == true) {
					next_state.write(S2_OUTPUT);
				}
				else {
					next_state.write(S3_FINISH);
				}
				break;
				
			case S3_FINISH:
				next_state.write(S0_WAIT);
				break;
				
			case S98_INIT:
				next_state.write(S99_INIT);
				break;
				
			case S99_INIT:
				next_state.write(S0_WAIT);
				break;
				
			default:
				next_state.write(S0_WAIT);
		}
		wait();
	}
}

void dh_hw_mult::state_output() {
	for(;;) {
		// std::cout << "Doing!! ";
		switch(state.read()) {
			case S0_WAIT:
				// std::cout << "WAIT - done: " << hw_mult_done.read() << endl;
				break;
				
			case S1_EXECUTE:
				// std::cout << "EXECUTE - out data: " << out_data_low.read() << " " << out_data_high.read() << endl;
				dh_hw_mult::do_mult(); // Part 3 is to cut this up into its own machine
				break;
				
			case S2_OUTPUT:
				// std::cout << "OUTPUT - out data: " << out_data_low.read() << " " << out_data_high.read() << endl;
				// Extract output from do_mult?
				hw_mult_done.write(true);
				break;
				
			case S3_FINISH:
				// std::cout << "FINISH" << endl;
				hw_mult_done.write(false);
				break;
				
			case S98_INIT:
				// std::cout << "98 to 99 - done: " << hw_mult_done.read() << endl;
				hw_mult_done.write(true);
				break;
				
			case S99_INIT:
				// std::cout << "99 to 0 - done: " << hw_mult_done.read() << endl;
				hw_mult_done.write(false);
				break;
				
			default:
				break;
		}
		wait();
	}
}
