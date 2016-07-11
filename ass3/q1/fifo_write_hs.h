#ifndef FIFO_WRITE_HS_H
#define FIFO_WRITE_HS_H

// This adapter converts the FIFO write interface into a bi-directional handshake protocol
// It is a hierarchical channel that implements the sc_fifo_out_if interface
// It has 4 ports: 1 for the data, 1 for the clock, and 2 for the control lines

template <class T> class FIFO_WRITE_HS : public sc_module, public sc_fifo_out_if <T> {
	
	public:
	sc_in_clk		clock;
	
	sc_in <bool>	ready;

	sc_out <T>		data;
	sc_out <bool>	valid;
	

	void write (const T& x) { // blocking write
	
		data.write (x) ;
		valid.write (true); // drive data line, signal that data is valid
		
		// Wait until data is read
		do {
			wait (clock->posedge_event()); 
		} while (ready.read() != true);
		
		valid.write (false); // make sure data is not read twice
	}
	
	SC_CTOR (FIFO_WRITE_HS) {
		valid.initialize (false); 
	}
	
	// provide dummy implementations for unneeded
	// sc_fifo_out_if<T> methods:
	bool nb_write(const T& x)
	{ assert(0); return false; }
	
	int num_free() const
	{ assert(0); return 0; }
	
	const sc_event& data_read_event() const
	{ static sc_event dummy; assert(0); return dummy; }
};

#endif