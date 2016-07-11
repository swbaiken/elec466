#ifndef ADDER_H
#define ADDER_H

// Simple dataflow adder

template <class T> SC_MODULE (DF_Adder) {
	
	sc_in_clk		clock;
	
	sc_fifo_in <T> 	input1;
	sc_fifo_in <T> 	input2;
	
	sc_in <bool>	ready;

	sc_out <T>		data;
	sc_out <bool>	valid;
	
	//sc_fifo_out <T> 	output;
	

	void write (const T& x) { // blocking write
	
		data.write (x) ;
		valid.write (true); // drive data line, signal that data is valid
		
		// Wait until data is read
		do {
			wait (clock->posedge_event()); 
		} while (ready.read() != true);
		
		valid.write (false); // make sure data is not read twice
	}

	void process() {
		while (1) {
			write (input1.read() + input2.read());
		}
	}
	
	SC_CTOR (DF_Adder) {
		SC_THREAD (process);
	}
};

#endif