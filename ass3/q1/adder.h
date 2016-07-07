#ifndef ADDER_H
#define ADDER_H

// Simple dataflow adder

template <class T> SC_MODULE (DF_Adder) {
	
	sc_fifo_in <T> 	input1
	sc_fifo_in <T> 	input2;
	
	sc_fifo_out <T> 	output;

	void process() {
		while (1) output.write (input1.read() + input2.read());
	}
	
	SC_CTOR (DF_Adder) {
		SC_THREAD (process);
	}
};

#endif