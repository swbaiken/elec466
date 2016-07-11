
#include "systemc.h"

#include "adder.h"
#include "generator.h"
#include "fork.h"
#include "printer.h"
#include "hw_fifo.h"
#include "fifo_read_hs.h"

int sc_main (int argc, char* argv[]) {

	DF_Const	<int> 	constant("constant", 1);	// constant = 1
	DF_Adder	<int> 	adder("adder");
	DF_Fork <int> 		fork("fork");
	DF_Printer <int> 	printer("printer", 10);		// 10 interations
	
	sc_fifo <int> const_out ("const_out", 1);		// FIFO of size 1
	sc_fifo <int> printer_in ("printer_in", 1); 	// FIFO of size 1
	sc_fifo <int> feedback ("feedback", 1); 		// FIFO of size 1
	//sc_fifo <int> adder_out ("adder_out", 1); 	// FIFO of size 1

	hw_fifo <int> fifo ("hw_adder_out",1);			// HW_FIFO of size 1
	
	// Adapter and associated signals
	FIFO_READ_HS<int>		read_adapter ("read_adapter");
	
	sc_clock clock("Test_clock",10,SC_NS);
	
	sc_signal <bool>	sink_valid ("sink_valid");
	sc_signal <bool>	sink_ready ("sink_ready");
	sc_signal <int>	sink_data ("sink_data");
	
	sc_signal <bool>	source_valid ("source_valid");
	sc_signal <bool>	source_ready ("source_ready");
	sc_signal <int>	source_data ("source_data");
	
	// do not forget about initialization!
	feedback.write (40);

	constant.output (const_out);
	
	printer.input (printer_in);
	
	adder.clock(clock);
	adder.input1(const_out);
	adder.input2(feedback);
	//adder.output (adder_out);
	adder.ready(source_ready);
	adder.data(source_data);
	adder.valid(source_valid);

	//fork.input (adder_out);
	fork.input(read_adapter);
	fork.output1 (feedback);
	fork.output2 (printer_in);	
		
	read_adapter.clock(clock);
	read_adapter.data (sink_data);
	read_adapter.valid(sink_valid);
	read_adapter.ready(sink_ready);
	
	fifo.clk(clock);
	fifo.data_in(source_data);
	fifo.valid_in(source_valid);
	fifo.ready_in(sink_ready);
	fifo.data_out(sink_data);
	fifo.valid_out(sink_valid);
	fifo.ready_out(source_ready);


	// Start simulation without time limit
	// The printer module is responsible for stopping the simulation
	sc_start ();

	return 0;
}