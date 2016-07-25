#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"




/* void dh_hw_mult::do_mult() {
	
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
				}
				else {
					next_state.write(S0_WAIT);
				}
				std::cout << hw_mult_enable.read() << " WAIT" << endl;
				break;
				
			case S1_EXECUTE:
				if (mult_done.read() == true) {
					next_state.write(S2_OUTPUT);
				}
				std::cout << mult_done.read() << " EXECUTE" << endl;
				break;
				
			case S2_OUTPUT:
				if (hw_mult_enable.read() == true) {
					next_state.write(S2_OUTPUT);
				}
				else {
					next_state.write(S3_FINISH);
				}
				std::cout << hw_mult_enable.read() << " OUTPUT" << endl;
				std::cout << in_data_1.read() << " " << in_data_2.read() << " " << out_data_high.read() << " " << out_data_low.read() << endl;
				break;
				
			case S3_FINISH:
				next_state.write(S0_WAIT);
				std::cout << "FINISH" << endl;
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
				mult_enable.write(true);
				// std::cout << "EXECUTE - out data: " << out_data_low.read() << " " << out_data_high.read() << endl;
				//dh_hw_mult::do_mult(); // Part 3 is to cut this up into its own machine
				break;
				
			case S2_OUTPUT:
				// std::cout << "OUTPUT - out data: " << out_data_low.read() << " " << out_data_high.read() << endl;
				// Extract output from do_mult?
				mult_enable.write(false);
				//hw_mult_done_s.write(true);
				hw_mult_done.write(true);
				break;
				
			case S3_FINISH:
				// std::cout << "FINISH" << endl;
				//hw_mult_done_s.write(false);
				hw_mult_done.write(true);
				break;
				
			case S98_INIT:
				// std::cout << "98 to 99 - done: " << hw_mult_done.read() << endl;
				//hw_mult_done_s.write(true);
				hw_mult_done.write(true);
				break;
				
			case S99_INIT:
				// std::cout << "99 to 0 - done: " << hw_mult_done.read() << endl;
				//hw_mult_done_s.write(false);
				hw_mult_done.write(false);
				break;
				
			default:
				break;
		}
		wait();
	}
}

// must run on the clock
void dh_hw_mult::multiplier_control() {
	
	while(1) {
		switch (mult_state.read()) {
			case MS0_WAIT:
				if (mult_enable.read()) {
					mult_state = MS1_RUN;
				}
				break;
				
			case MS1_RUN:
				// mux' should all be set to the multiplier input
				// constants should be set to shift 1 to left half
				a0_in_mux.write(0);
				a1_in_mux.write(0);
				t_in_mux.write(0);
				u_in_mux.write(0);
				constants_sel.write(0);
				wait();
				
				// Enable copy of values from the multipliers
				a0_en.write(1);
				a1_en.write(1);
				u_en.write(1);
				t_en.write(1);
				wait();
				
				// Stop updating a0, a1, u, and update t with adder output
				a0_en.write(0);
				a1_en.write(0);
				u_en.write(0);
				t_in_mux.write(1);
				wait();
				
				// Stop updating t, conditionally switch a1 mux and update a1, update u
				t_en.write(0);
				
				if (t_out.read() < u_out.read()) {
					a1_in_mux.write(1);
					a1_en.write(1);
					wait();
					a1_en.write(0);
				}
				u_in_mux.write(1);
				u_en.write(1);
				wait();
				
				// Stop updating u, switch a0 mux, and allow a0 to update
				u_en.write(0);
				a0_in_mux.write(1);
				a0_en.write(1);
				wait();
				
				// Stop updating a0, if a0 < u then switch the constant to add to a1 and en a1
				// regardless, add the high half of t
				a0_en.write(0);
				a1_en.write(1);
				if (a0_out.read() < u_out.read() ) {
					constants_sel.write(1);
					wait();
				}
				a1_in_mux.write(2);
				wait();
				
				// stop updating a1 and signal that multiply is done
				a1_en.write(0);
				out_data_low.write(a0_out);
				out_data_high.write(a1_out);
				mult_done.write(true);
				mult_state = MS2_DONE;
				break;
				
			case MS2_DONE:
				if (mult_enable.read() == false) {
					mult_done.write(false);
					mult_state.write(MS0_WAIT);
				}
				break;
				
			default:
				a0_in_mux.write(0);
				a1_in_mux.write(0);
				t_in_mux.write(0);
				u_in_mux.write(0);
				constants_sel.write(0);
				mult_state.write(MS0_WAIT);
				break;
		}
		wait();
	}
}
