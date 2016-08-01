#include "systemc.h"
#include "digit.h"
#include "dh_sw.h"
//#include "test.h"
#include "dh_hw_mult.h"

int sc_main(int argc , char *argv[])
{

	sc_signal <bool> enable, done;
	sc_signal <NN_DIGIT> operand1, operand2, result1, result2;
	sc_clock clock("Test_clock",10,SC_NS);
	
	enable.write(false);
	done.write(false);
	
	dh_sw DH_SW("DH_Software");
	DH_SW.out_data_1 (operand1);		// operand1 to hardware
	DH_SW.out_data_2 (operand2);		// operand2 to hardware
	DH_SW.in_data_high (result2);		// result2 from hardware
	DH_SW.in_data_low (result1);		// result1 from hardware
	DH_SW.hw_mult_enable (enable);	// enable hardware
	DH_SW.hw_mult_done (done);			// hardware done
	
	
	
	dh_hw_mult DH_HW_MULT("DH_Hardware_Multiplier");
	DH_HW_MULT.clk(clock);
	DH_HW_MULT.hw_mult_enable (enable);	// enable hardware
	DH_HW_MULT.in_data_1 (operand1);		// operand1 from software
	DH_HW_MULT.in_data_2 (operand2);		// operand2 from software
	DH_HW_MULT.out_data_high (result2);	// result2 to software
	DH_HW_MULT.out_data_low (result1);	// result1 to software
	DH_HW_MULT.hw_mult_done (done);		// hardware done
	
	// Waveform trace generation

	sc_trace_file* Tf=sc_create_vcd_trace_file("mult_trace");
	
	Tf->set_time_unit(1, SC_NS);
	sc_trace(Tf, enable, "Enable");
	sc_trace(Tf, done, "Done");
	sc_trace(Tf, operand1, "Operand1");
	sc_trace(Tf, operand2, "Operand2");
	sc_trace(Tf, result1, "Result1");
	sc_trace(Tf, result2, "Result2");
	sc_trace(Tf, clock, "Clock");

	sc_start();
	//sc_start(600,SC_SEC);						// limit total run time to 10 minutes

	return(0);
}
