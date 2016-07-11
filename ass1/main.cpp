#include "systemc.h"

#include "filter.h"
#include "stim.h"
#include "mon.h"

int sc_main(int argc, char* argv[]) {
	sc_signal <float> filter_in, filter_out;
	sc_clock TestClk("TickTock",10,SC_NS,0.5,1,SC_NS);

	stim Stim1("Stimulus");
	Stim1.x(filter_in);
	Stim1.clock(TestClk);

	filter DUT("filter");
	DUT.x(filter_in);
	DUT.y(filter_out);
	DUT.clock(TestClk);

	mon Mon1("Monitor");
	Mon1.x(filter_in);
	Mon1.y(filter_out);
	Mon1.clock(TestClk);

	// Waveform trace generation

	sc_trace_file* Tf=sc_create_vcd_trace_file("filter_trace");
	
	Tf->set_time_unit(1, SC_NS);
	sc_trace(Tf, TestClk, "clk");
	sc_trace(Tf, filter_in, "FilterInput");
	sc_trace(Tf, DUT.s1, "S1");
	sc_trace(Tf, DUT.s2, "S2");
	sc_trace(Tf, DUT.s3, "S3");
	sc_trace(Tf, filter_out, "FilterOutput");

	sc_start(200,SC_NS);

	sc_close_vcd_trace_file(Tf);
	
	return 0;
}
