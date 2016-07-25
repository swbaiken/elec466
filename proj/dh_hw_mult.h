#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

#include "systemc.h"
#include "digit.h"
#include "dh_hw_components.h"

enum ctrl_states {S0_WAIT, S1_EXECUTE, S2_OUTPUT, S3_FINISH, S98_INIT, S99_INIT};
enum mult_states {MS0_WAIT, MS1_RUN, MS2_DONE};

SC_MODULE (dh_hw_mult) {
	sc_in_clk			clk;
	
	sc_in<bool>			hw_mult_enable; 
	sc_in<NN_DIGIT>	in_data_1;
	sc_in<NN_DIGIT>	in_data_2;

	sc_out<NN_DIGIT>	out_data_high;
	sc_out<NN_DIGIT>	out_data_low;
	sc_out<bool>		hw_mult_done;
	
	
	
	sc_signal<ctrl_states>	state, next_state;
	sc_signal<mult_states>	mult_state;
	sc_signal<bool> mult_enable;
	sc_signal<bool> mult_done;
	
	sc_signal<NN_HALF_DIGIT> in0_low, in0_high, in1_low, in1_high;
	sc_signal<NN_DIGIT> mult_a0_out, mult_a1_out, mult_u_out, mult_t_out;
	sc_signal<sc_uint<1> > a0_in_mux, t_in_mux, u_in_mux;
	sc_signal<sc_uint<1> > constants_sel;	
	sc_signal<sc_uint<2> > a1_in_mux;
	sc_signal<NN_DIGIT> a0_mux_out, a1_mux_out, t_mux_out, u_mux_out;
	sc_signal<NN_DIGIT> a0_add_out, a1_add1_out, a1_add2_out, t_add_out;
	sc_signal<NN_DIGIT> a0_val, a1_val, u_val, t_val;
	sc_signal<NN_DIGIT> t_shift_left_out, t_shift_right_out;
	sc_signal<NN_DIGIT> constants_out;
	sc_signal<NN_DIGIT>	a0_out, a1_out, u_out, t_out;
	sc_signal<bool>		a0_en, a1_en, u_en, t_en;
	
	//sc_signal<NN_DIGIT> out_data_low_s;
	
	
	multiplier 		mult_a0, mult_a1, mult_u, mult_t;
	adder				a0_plus_u, a1_plus_shift_t, a1_plus_const, t_plus_u;
	const_mem		constants;
	mem_unit			a0, a1, u, t;
	mux_2				a0_mux, u_mux, t_mux;
	mux_3				a1_mux;
	half_shift_left	t_shift_left;
	half_shift_right	t_shift_right;
	input_splitter	splitter;
	
	
	void state_transition();
	void state_reg();
	void state_output();
	//void do_mult();
	void multiplier_control();

	SC_CTOR (dh_hw_mult) : 
		clk("clk"), hw_mult_enable("hw_mult_enable"), in_data_1("in_data_1"), in_data_2("in_data_2"),out_data_high("out_data_high"), out_data_low("out_data_low"), hw_mult_done("hw_mult_done"), mult_a0("mult_a0"), mult_a1("mult_a1"), mult_u("mult_u"), mult_t("mult_t"), a0_plus_u("a0_plus_u"), a1_plus_shift_t("a1_plus_shift"), a1_plus_const("a1_plus_const"), t_plus_u("t_plus_u"), constants("constants"), a0("a0"), a1("a1"), u("u"), t("t"), a0_mux("a0_mux"), u_mux("u_mux"), t_mux("t_mux"), a1_mux("a1_mux"), t_shift_left("t_shift_left"), t_shift_right("t_shift_right"), splitter("splitter") {
			
		SC_CTHREAD(state_reg, clk.pos());
		SC_THREAD(state_output);
			sensitive << state;
		SC_THREAD(state_transition);
			sensitive << state << hw_mult_enable;// << in_data_1 << in_data_2;
		SC_CTHREAD(multiplier_control, clk.neg());
		
		state.write(S98_INIT);
		next_state.write(S98_INIT);
		
		//out_data_low(out_data_low_s);
		
		a0_in_mux.write(0);
		a1_in_mux.write(0);
		t_in_mux.write(0);
		u_in_mux.write(0);
		
		a0_en.write(0);
		a1_en.write(0);
		u_en.write(0);
		t_en.write(0);
		
		constants_sel.write(0);
		
		out_data_low.write(0);
		out_data_high.write(0);
		hw_mult_done.write(false);
		
		a1.input(a1_mux_out);
		a1.output(a1_out);
		a1.output(out_data_high);
		a1.enable(a1_en);
		
		a0.input(a0_mux_out);
		a0.output(a0_out);
		a0.output(out_data_low);
		a0.enable(a0_en);
		
		u.input(u_mux_out);
		u.output(u_out);
		u.enable(u_en);
		
		t.input(t_mux_out);
		t.output(t_out);
		t.enable(t_en);
		
		splitter.in0(in_data_1);
		splitter.in1(in_data_2);
		splitter.in0_low(in0_low);
		splitter.in0_high(in0_high);
		splitter.in1_low(in1_low);
		splitter.in1_high(in1_high);
		
		mult_a0.in1(in0_low);
		mult_a0.in2(in1_low);
		mult_a0.out(mult_a0_out);
		
		mult_a1.in1(in0_high);
		mult_a1.in2(in1_high);
		mult_a1.out(mult_a1_out);
		
		mult_u.in1(in0_high);
		mult_u.in2(in1_low);
		mult_u.out(mult_u_out);
		
		mult_t.in1(in0_low);
		mult_t.in2(in1_high);
		mult_t.out(mult_t_out);
		
		a0_plus_u.in1(a0_out);
		a0_plus_u.in2(u_out);
		a0_plus_u.sum(a0_add_out);
		
		a1_plus_const.in1(a1_out);
		a1_plus_const.in2(constants_out);
		a1_plus_const.sum(a1_add1_out);
		
		a1_plus_shift_t.in1(a1_out);
		a1_plus_shift_t.in2(t_shift_right_out);
		a1_plus_shift_t.sum(a1_add2_out);
		
		t_plus_u.in1(t_out);
		t_plus_u.in2(u_out);
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
		
		t_shift_left.input(t_out);
		t_shift_left.output(t_shift_left_out);
		
		t_shift_right.input(t_out);
		t_shift_right.output(t_shift_right_out);
		
		constants.sel(constants_sel);
		constants.output(constants_out);
		
	}
  
};

#endif /* end _DH_HW_MULT_H_ */
