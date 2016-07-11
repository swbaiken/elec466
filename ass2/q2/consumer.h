/*
 *	consumer.h
 *	
 *	ELEC 466, Assignment 2, Question 2, Shaun Aiken, Jun 2016
 */

#ifndef CONSUMER_H
#define CONSUMER_H

#include <iomanip>

template<class T>
class consumer:public sc_module {

public:
	sc_port <fifo_in_if<T> >	in;
	sc_in_clk	clk;
	
	void consume(){
		T result;
		stringstream outbuffer;
		
		while(1) {
			wait();
			wait();
			outbuffer << setw(8) << sc_time_stamp() << setw(13) << "READ: ";
			
			if(in->read(result) == true) {	// Attempt to read from channel
				outbuffer << "read [" << result << "]";
			}
			else {
				outbuffer << "read failed";
			}
			cout << outbuffer.str() << endl;
			outbuffer.str("");	// Clear output buffer
		}
	}
	
	SC_CTOR(consumer) {
		SC_THREAD(consume);
		sensitive << clk.pos();
	}
};

#endif
