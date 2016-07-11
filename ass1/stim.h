#include "systemc.h"

SC_MODULE (stim) {

	sc_out	<float>	x;
	sc_in_clk				clock;

	void signal_gen() {

		while (1) {
			x.write(1);
			wait();
		}
	}

	SC_CTOR (stim) {
		SC_CTHREAD (signal_gen, clock.pos());
	}

};
