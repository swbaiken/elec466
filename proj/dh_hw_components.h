#ifndef DH_HW_COMPONENTS_H
#define DH_HW_COMPONENTS_H

/*
 * COMPONENT
 */

SC_MODULE (COMPONENT) {
	sc_in<>	input;
	
	sc_out<> output;
	
	void func(void) {
	}
	
	SC_CTOR(COMPONENT) : {
			SC_(func);
				sensitive << input;
	}
};

#endif