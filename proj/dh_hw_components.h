#ifndef DH_HW_COMPONENTS_H
#define DH_HW_COMPONENTS_H 1

#include "digit.h"
#include "systemc.h"


/*
 * COMPONENT TEMPLATE
 *

SC_MODULE (COMPONENT) {
	sc_in<>	input;
	
	sc_out<> output;
	
	void func(void) {
	}
	
	SC_CTOR(COMPONENT) : init{
			SC_(func);
				sensitive << input;
	}
};
*/

/*
 * Multiplier
 */
SC_MODULE (multiplier) {
	sc_in<NN_HALF_DIGIT>	in1;
	sc_in<NN_HALF_DIGIT>	in2;
	
	sc_out<NN_DIGIT> out;
	
	void mult_func() {
		out.write(in1.read() * in2.read());
	}
	
	SC_CTOR(multiplier) {
		SC_METHOD(mult_func);
			sensitive << in1 << in2;
	}
};


/*
 * Adder
 */
SC_MODULE (adder) {
	sc_in<NN_DIGIT> in1;
	sc_in<NN_DIGIT> in2;
	
	sc_out<NN_DIGIT> sum;
	
	void func(void) {
		sum.write(in1.read() + in2.read());
	}
	
	SC_CTOR(adder) {
		SC_METHOD(func);
			sensitive << in1 << in2;
	}
};


/*
 * Shift input left by half - logical shift
 */
SC_MODULE (half_shift_left) {
	sc_in<NN_DIGIT> 	input;
	
	sc_out<NN_DIGIT>	output;
	
	void func(void) {
		output.write(input.read() << NN_HALF_DIGIT_BITS);
	}
	
	SC_CTOR(half_shift_left) {
			SC_METHOD(func);
				sensitive << input;
	}
};


/*
 * Shift input right by half - logical shift
 */
SC_MODULE (half_shift_right) {
	sc_in<NN_DIGIT> 	input;
	
	sc_out<NN_DIGIT>	output;
	
	void func(void) {
		output.write(input.read() >> NN_HALF_DIGIT_BITS);
	}
	
	SC_CTOR(half_shift_right) {
			SC_METHOD(func);
				sensitive << input;
	}
};


/*
 * Constants
 * Output is based on sel value
 * 	0: 1 shifted left by NN_HALF_DIGIT_BITS
 *		1: 1
 */
SC_MODULE (const_mem) {
	sc_in<sc_uint<1> > 	sel;
	
	sc_out<NN_DIGIT>	output;
	
	void func(void) {
		switch (sel.read()) {
			case 0:
				output.write(1 << NN_HALF_DIGIT_BITS);
				break;
			case 1:
				output.write(1);
				break;
		}
	}
	
	SC_CTOR(const_mem) {
			SC_METHOD(func);
				sensitive << sel;
	}
};


/*
 * 2 Input Multiplexer
 */
SC_MODULE (mux_2) {
	sc_in<NN_DIGIT>	input0;
	sc_in<NN_DIGIT>	input1;
	sc_in<sc_uint<1> >	mux;
		
	sc_out<NN_DIGIT>	output;
	
	void func(void) {
		switch (mux.read()) {
			case 0:
				output.write(input0.read());
				break;
			case 1:
				output.write(input1.read());
				break;
		}
	}
	
	mux_2(sc_module_name name) : sc_module(name) {
		SC_METHOD(func);
			sensitive << input0 << input1 << mux;
	}
	
	SC_HAS_PROCESS(mux_2);
};


/*
 * 3 Input Multiplexer
 */
SC_MODULE (mux_3) {
	sc_in<NN_DIGIT>	input0;
	sc_in<NN_DIGIT>	input1;
	sc_in<NN_DIGIT>	input2;
	sc_in<sc_uint<2> >	mux;
		
	sc_out<NN_DIGIT>	output;
	
	void func(void) {
		switch (mux.read()) {
			case 0:
				output.write(input0.read());
				break;
			case 1:
				output.write(input1.read());
				break;
			case 2:
				output.write(input2.read());
				break;
			case 3:
				// Fall through to default - error case
			default:
				output.write(MAX_NN_DIGIT);
				break;
		}
	}
	
	SC_CTOR(mux_3) {
			SC_METHOD(func);
				sensitive << input0 << input1 << input2 << mux;
	}
};


/*
 * Input splitter - split inputs into high half and low half
 */
SC_MODULE (input_splitter) {
	sc_in<NN_DIGIT>	in0;
	sc_in<NN_DIGIT>	in1;
	
	sc_out<NN_HALF_DIGIT>	in0_low;
	sc_out<NN_HALF_DIGIT>	in0_high;
	sc_out<NN_HALF_DIGIT>	in1_low;
	sc_out<NN_HALF_DIGIT>	in1_high;
	
	void func(void) {
		in0_low.write(in0.read() & MAX_NN_HALF_DIGIT);
		in0_high.write((in0.read() >> NN_HALF_DIGIT_BITS) & MAX_NN_HALF_DIGIT);
		in1_low.write(in1.read() & MAX_NN_HALF_DIGIT);
		in1_high.write((in1.read() >> NN_HALF_DIGIT_BITS) & MAX_NN_HALF_DIGIT);
	}
	
	SC_CTOR(input_splitter) {
			SC_METHOD(func);
				sensitive << in1 << in0;
	}
};


/*
 * Memory unit - copy input to output on rising clock if enable is set
 */
template <class T>
SC_MODULE (mem_unit) {
	sc_in<T>		input;
	sc_in<bool>	enable;
	
	sc_out<T>	output;
	
	void func(void) {
		while(1) {
			if (enable.read()) {
				output.write(input.read());
			}
			wait();
		}
	}
	
	SC_CTOR(mem_unit) :
		input("input"), enable("enable"), output("output") {
			SC_THREAD(func);
				sensitive << enable;
	}
};

#endif