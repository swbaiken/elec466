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
	
	sc_signal<NN_HALF_DIGIT> in0_low, in0_high, in1_low, in1_high;
	sc_signal<sc_uint<NN_DIGIT_BITS> > mult_a0_out, mult_a1_out, mult_u_out, mult_t_out;
	sc_signal<sc_uint<1> > a0_in_mux, t_in_mux, u_in_mux;
	sc_signal<sc_uint<1> > constants_sel;	
	sc_signal<sc_uint<2> > a1_in_mux;
	sc_signal<sc_uint<NN_DIGIT_BITS> > a0_mux_out, a1_mux_out, t_mux_out, u_mux_out;
	sc_signal<sc_uint<NN_DIGIT_BITS> > a0_add_out, a1_add1_out, a1_add2_out, t_add_out;
	sc_signal<sc_uint<NN_DIGIT_BITS> > a0_val, a1_val, u_val, t_val;
	sc_signal<sc_uint<NN_DIGIT_BITS> > t_shift_left_out, t_shift_right_out;
	sc_signal<sc_uint<NN_DIGIT_BITS> > constants_out;
	
	
	multiplier 		mult_a0, mult_a1, mult_u, mult_t;
	adder				a0_plus_u, a1_plus_shift_t, al_plus_const, t_plus_u;
	const_mem		constants;
	mux_2				a0_mux, u_mux, t_mux;
	mux_3				a1_mux;
	half_shift_left	t_shift_left;
	half_shift_right	t_shift_right;
	input_splitter	splitter;
			
	
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
		
		state.write(S98_INIT);
		next_state.write(S98_INIT);
		
		// out_data_low.initialize(0);
		// out_data_high.initialize(0);
		// hw_mult_done.initialize(true);
		
		splitter.in0(in_data_1);
		splitter.in1(in_data_2);
		splitter.in0_low(in0_low);
		splitter.in0_high(in0_high);
		splitter.in1_low(in1_low);
		splitter.in1_high(in1_high);
		
		mult_a0.in1();
		mult_a0.in2();
		mult_a0.out(mult_a0_out);
		
		mult_a1.in1();
		mult_a1.in2();
		mult_a1.out(mult_a1_out);
		
		mult_u.in1();
		mult_u.in2();
		mult_u.out(mult_u_out);
		
		mult_t.in1();
		mult_t.in2();
		mult_t.out(mult_t_out);
		
		a0_plus_u.in1(a0_mux_out);
		a0_plus_u.in2(u_mux_out);
		a0_plus_u.sum(a0_add_out);
		
		a1_plus_shift_t.in1(a1_mux_out);
		a1_plus_shift_t.in2(t_shift_right_out);
		a1_plus_shift_t.sum(a1_add1_out);
		
		al_plus_const.in1(a1_mux_out);
		al_plus_const.in2(constants_out);
		al_plus_const.sum(a1_add2_out);
		
		t_plus_u.in1(t_mux_out);
		t_plus_u.in2(u_mux_out);
		t_plus_u.sum(t_add_out);
		
		a0_mux.input0(mult_a0_out);
		a0_mux.input1(a0_add_out);
		a0_mux.mux(a0_in_mux);
		a0_mux.output(a0_mux_out);
		
		a1_mux.input0(mult_a1_out);
		a1_mux.input1(a1_add1_out);
		a1_mux.input2(a1_add2_out);
		a1_mux.mux(a1_in_mux);
		a1_mux.output(a1_mux_out);
		
		u_mux.input0(mult_u_out);
		u_mux.input1(t_shift_left_out);
		u_mux.mux(u_in_mux);
		u_mux.output(u_mux_out);
		
		t_mux.input0(mult_t_out);
		t_mux.input1(t_add_out);
		t_mux.mux(t_in_mux);
		t_mux.output(t_mux_out);
		
		t_shift_left.input(t_mux_out);
		t_shift_left.output(t_shift_left_out);
		
		t_shift_right.input(t_mux_out);
		t_shift_right.output(t_shift_right_out);
		
		constants.sel(constants_sel);
		constants.output(constants_out);
		
	}
  
};

#endif /* end _DH_HW_MULT_H_ */
