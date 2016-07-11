/*
 *	main.cpp
 *	
 *	ELEC 466, Assignment 2, Question 1, Shaun Aiken, Jun 2016
 */

#include "systemc.h"

#include "memory.h"
#include "stim.h"
#include "mon.h"

int sc_main(int argc, char* argv[]) {
	sc_signal_rv		<8> data;
	sc_signal			<sc_uint <8> > addr;
	sc_signal			<sc_uint <2> > command;
	sc_signal <bool>	start;
	sc_signal <bool>	complete;
	sc_signal <bool>	reset;
	sc_clock TestClk("TickTock",100,SC_NS,50,0,SC_NS);

	stim Stim1("Stimulus");
	Stim1.data(data);
	Stim1.addr(addr);
	Stim1.comm(command);
	Stim1.start(start);
	Stim1.complete(complete);
	Stim1.reset(reset);
	Stim1.clk(TestClk);

	mem_ctrl DUT("Memory");
	DUT.data(data);
	DUT.addr(addr);
	DUT.comm(command);
	DUT.new_comm(start);
	DUT.complete(complete);
	DUT.clk(TestClk);
	DUT.reset(reset);

	mon Mon1("Monitor");
	Mon1.data(data);
	Mon1.addr(addr);
	Mon1.comm(command);
	Mon1.complete(complete);
	Mon1.start(start);
	Mon1.reset(reset);
	Mon1.clk(TestClk);

	// Waveform trace generation

	sc_trace_file* Tf=sc_create_vcd_trace_file("mem_trace");
	
	Tf->set_time_unit(1, SC_NS);
	sc_trace(Tf, data, "Data");
	sc_trace(Tf, addr, "Address");
	sc_trace(Tf, command, "Command");
	sc_trace(Tf, start, "Start");
	sc_trace(Tf, complete, "Complete");
	sc_trace(Tf, reset, "Reset");
	sc_trace(Tf, TestClk, "Clock");

	sc_start(26000,SC_NS);

	sc_close_vcd_trace_file(Tf);
	
	return 0;
}
