#include "systemc.h"
#include "digit.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

enum ctrl_state {S0_WAIT, S1_EXECUTE, S2_OUTPUT, S3_FINISH, S98_INIT, S99_INIT};

SC_MODULE (dh_hw_mult)
{
	sc_in_clk			clk;
	
	sc_in<bool>			hw_mult_enable; 
	sc_in<NN_DIGIT>	in_data_1;
	sc_in<NN_DIGIT>	in_data_2;

	sc_out<NN_DIGIT>	out_data_low;
	sc_out<NN_DIGIT>	out_data_high;
	sc_out<bool>		hw_mult_done;
	
	sc_signal<sc_uint<NN_DIGIT_BITS> > mult_a0_out, mult_a1_out, mult_u_out, mult_t_out;
	sc_signal<bool> a0_in_mux, t_in_mux, u_in_mux;
	sc_signal<sc_uint<2> > a1_in_mux;
	sc_signal<sc_uint<NN_DIGIT_BITS> > a0_mux_out, a1_mux_out, t_mux_out, u_mux_out;
	
	
	
	sc_signal<ctrl_state>	state, next_state;

	//void process_hw_mult();
	void state_transition();
	void state_reg();
	void state_output();
	void do_mult();

	SC_CTOR (dh_hw_mult)
	{
		SC_CTHREAD(state_reg, clk.pos());
		SC_THREAD(state_output);
			sensitive << state;
		SC_THREAD(state_transition);
			sensitive << state << hw_mult_enable;// << in_data_1 << in_data_2;
			
		SC_METHOD(mult_a0);
			sensitive << in_data_1 << in_data_2;
		SC_METHOD(mult_a1);
			sensitive << in_data_1 << in_data_2;
		SC_METHOD(mult_t);
			sensitive << in_data_1 << in_data_2;
		SC_METHOD(mult_u);
			sensitive << in_data_1 << in_data_2;
		
		
		
		
		state.write(S98_INIT);
		next_state.write(S98_INIT);
		
		// out_data_low.initialize(0);
		// out_data_high.initialize(0);
		// hw_mult_done.initialize(true);
	}
  
};

#endif /* end _DH_HW_MULT_H_ */
