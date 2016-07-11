/*
 *	main.cpp
 *
 *	ELEC 466, Assignment 2, Question 2, Shaun Aiken, Jun 2016
 *
 *	Instantiates a fifo channel for testing with producer and consumer modules.
 *
 */
 
#include "systemc.h"

#include "fifo.h"
#include "producer.h"
#include "consumer.h"


int sc_main(int argc, char* argv[]) {
	
	typedef float fifo_T;	//set the type of the fifo data
	
	// Different clocks allow depletion or saturation of the channel
	sc_clock consumeClk("consumeClk",30,SC_NS,50,0,SC_NS);
	sc_clock produceClk("produceClk",10,SC_NS,50,0,SC_NS);
		
	fifo<fifo_T, 16> fifo1("Fifo_1");
	consumer<fifo_T> consume1("Consumer_1");
	producer<fifo_T> produce1("Producer_1");
		
	consume1.in(fifo1);
	consume1.clk(consumeClk);
	produce1.out(fifo1);
	produce1.clk(produceClk);
		
	sc_start(200, SC_NS);
	
	return 0;
}
