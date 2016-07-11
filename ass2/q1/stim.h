/*
 *	stim.h
 *	
 *	ELEC 466, Assignment 2, Question 1, Shaun Aiken, Jun 2016
 */
 
#ifndef STIM_H
#define STIM_H

#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3

#define SC_LV8_Z			"ZZZZZZZZ"
#define TEST_SEQUENCE 0xC3
#define MEM_SIZE			256

#include "systemc.h"

SC_MODULE (stim) {

	sc_inout	<sc_lv<8> > data;	// Bidirectional port of logic-vector type
	sc_out		<sc_uint<8> > addr;	// Memory address
	sc_out		<sc_uint<2> > comm;	// Command to execute
	sc_in 		<bool> complete;	// Input port signaling “done”
	sc_out 		<bool> start;			// Output port signaling “start”
	sc_out		<bool> reset;
	sc_in_clk	clk;

	void signal_gen() {
		unsigned int counter;

		wait();
		wait();
		data.write(SC_LV8_Z);
		start.write(0);
		reset.write(0);

		// Wait for the memory to be available
		while(complete.read() == 1) {
			wait();
		}
		// Write byte -----------------------------------------------------------
		data.write(0xAA);
		addr.write(0x00);
		comm.write(WTBYT);
		start.write(1);

		while(complete.read() == 0) {
			wait();
		}

		data.write(SC_LV8_Z);
		start.write(0);

		while(complete.read() == 1) {
			wait();
		}

		// Read byte ------------------------------------------------------------
		comm.write(RDBYT);
		addr.write(0x00);
		start.write(1);

		while(complete.read() == 0) {
			wait();
		}

		start.write(0);

		while(complete.read() == 1) {
			wait();
		}

		// Write block ----------------------------------------------------------
		data.write(0x50);
		addr.write(0x00);
		comm.write(WTBLK);
		start.write(1);
		wait();

		data.write(0x51);
		wait();

		data.write(0x52);
		wait();

		data.write(0x53);

		while (complete.read() == 0) {
			wait();
		}

		data.write(SC_LV8_Z);
		start.write(0);

		while (complete.read() == 1) {
			wait();
		}

		// Read block -----------------------------------------------------------
		addr.write(0x00);
		comm.write(RDBLK);
		start.write(1);

		while (complete.read() == 0) {
			wait();
		}

		start.write(0);

		while (complete.read() == 1) {
			wait();
		}

		// Check Reset  ---------------------------------------------------------
		// Initial RDBYT with delayed handshake
		comm.write(RDBYT);
		addr.write(0x00);
		start.write(1);

		while(complete.read() == 0) {
			wait();
		}

		wait();
		wait();

		start.write(0);

		while(complete.read() == 1) {
			wait();
		}

		// Another RDBYT with delay and reset to show difference in memory behaviour
		comm.write(RDBYT);
		addr.write(0x00);
		start.write(1);

		while(complete.read() == 0) {
			wait();
		}

		reset.write(1);
		wait();
		wait();

		start.write(0);
		wait();

		reset.write(0);
		wait();

		while(complete.read() == 1) {
			wait();
		}

		// Fill memory ----------------------------------------------------------
		for(counter = 0;counter < MEM_SIZE;counter++) {
			data.write(TEST_SEQUENCE);	// Bit sequence defined at top
			addr.write(0x00+counter); 	// Fill memory sequentially
			comm.write(WTBYT);
			start.write(1);

			while(complete.read() == 0) {
				wait();
			}

			data.write(SC_LV8_Z);
			start.write(0);

			while(complete.read() == 1) {
				wait();
			}

		}

		// Check memory ---------------------------------------------------------
		for(counter = 0;counter < MEM_SIZE;counter++) {
			comm.write(RDBYT);
			addr.write(0x00+counter);
			start.write(1);

			while(complete.read() == 0) {
				wait();
			}

			start.write(0);

			while(complete.read() == 1) {
				wait();
			}
		}

		//sc_stop();
	}

	SC_CTOR(stim) {
		SC_THREAD(signal_gen);
		sensitive << clk.pos();
	}
};

#endif