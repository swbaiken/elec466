/*
 *	producer.h
 *
 *	ELEC 466, Assignment 2, Question 2, Shaun Aiken, Jun 2016
 *
 */

#ifndef PRODUCER_H
#define PRODUCER_H

#include <iomanip>

using namespace std;

template<class T>
class producer:public sc_module{

public:
	sc_port <fifo_out_if<T> > out;
	sc_in_clk	clk;
	
	void produce(){
		T counter = 0;
		stringstream outbuffer;
		
		while(1) {
			wait();
			wait();
			
			outbuffer << setw(8) << sc_time_stamp() << setw(8) << "WRITE[" 
				<< setw(2) << counter << "]: ";
			
			if(out->write(counter) == true) {
				outbuffer << "write successful";
				counter++;
			}
			else {
				outbuffer << "write failed";
			}
			cout << outbuffer.str() << endl;
			outbuffer.str("");	// Clear output buffer
			
		}
	}
	
	SC_CTOR(producer) {
		SC_THREAD(produce);
		sensitive << clk.pos();
	}
};

#endif
