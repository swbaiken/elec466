#include "systemc.h"

SC_MODULE (filter) {

	sc_in_clk 				clock;
	sc_in			<float>	x;
	sc_out		<float> y;

	sc_signal	<float> s1, s2, s3;

	void the_filter() {
		while (1) {
			wait();
			s1.write(s1.read()*0.4 + x.read());
			s2.write(s2.read()*(-0.8) + s3.read()*(-0.5) + x.read());
			s3.write(s2.read());
			y.write(s1.read()*0.24 + s2.read()*0.2 + s3.read()*0.25);
		}
	}

	SC_CTOR (filter) {
		SC_CTHREAD(the_filter, clock.pos());
		s1.write(0.0);
		s2.write(0.0);
		s3.write(0.0);
		y.initialize(0.0);
	}
};
