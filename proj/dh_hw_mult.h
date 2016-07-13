#include "systemc.h"
#include "digit.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

enum ctrl_state {S0_WAIT, S1_EXECUTE, S2_OUTPUT, S3_FINISH};

SC_MODULE (dh_hw_mult)
{
	sc_in_clk			clk;
	
	sc_in<bool>			hw_mult_enable; 
	sc_in<NN_DIGIT>	in_data_1;
	sc_in<NN_DIGIT>	in_data_2;

	sc_out<NN_DIGIT>	out_data_low;
	sc_out<NN_DIGIT>	out_data_high;
	sc_out<bool>		hw_mult_done;
	
	sc_signal<ctrl_state>	state, next_state;

	//void process_hw_mult();
	void state_transition();
	void state_reg();
	void do_mult();

	SC_CTOR (dh_hw_mult)
	{
		SC_CTHREAD(state_reg, clk.pos());
		SC_METHOD(state_transition);
			sensitive << state;
	}
  
};

#endif /* end _DH_HW_MULT_H_ */
