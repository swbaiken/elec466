#include "systemc.h"
#include "digit.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

enum ctrl_state {S0_WAIT, S1_EXECUTE, S2_OUTPUT, S3_FINISH, S99_INIT};

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
		SC_THREAD(state_transition);
			sensitive << state;
		
		state.write(S0_WAIT);
		next_state.write(S99_INIT);
		
		out_data_low.initialize(0);
		out_data_high.initialize(0);
		hw_mult_done.initialize(true);
	}
  
};

#endif /* end _DH_HW_MULT_H_ */
