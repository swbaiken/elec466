/*
 *	mon.h
 *	
 *	ELEC 466, Assignment 2, Question 1, Shaun Aiken, Jun 2016
 */
 
#ifndef MON_H
#define MON_H

#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3

#include "systemc.h"
#include <iomanip>

using namespace std;

SC_MODULE (mon) {

	sc_in			<sc_lv<8> > data; // Bidirectional port of logic-vector type
	sc_in 		<sc_uint<8> > addr;
	sc_in 		<sc_uint<2> > comm;
	sc_in 		<bool> start; 		// Input port signaling “start”
	sc_in 		<bool> complete;	// Output port signaling “done”
	sc_in			<bool> reset;
	sc_in_clk	clk;

	void signal_mon() {

		cout << "Memory test begins." << endl;
		cout << setw(8) << "Time" << setw(10) << "Operation" << setw(8)
			<< "Address" << setw(10) << "Data" << setw(6) << "Start" 
			<< setw(9) << "Complete" << setw(7) << "Reset" << endl;
	
		while (1) {

			cout << setw(8) << sc_time_stamp();

			switch(comm.read()) {
				case RDBLK:
					cout << setw(10) << "RDBLK";
					break;
				case RDBYT:
					cout << setw(10) << "RDBYT";
					break;
				case WTBYT:
					cout << setw(10) << "WTBYT";
					break;
				case WTBLK:
					cout << setw(10) << "WTBLK";
					break;
				default:
					cout << setw(10) << "-----";
					break;
			}

			cout << setw(8) << addr.read() << setw(10) << data.read() 
				<< setw(6) << start.read() << setw(9) << complete.read()
				<< setw(7) << reset.read() << endl;

			wait();

		}
	}

	SC_CTOR(mon) {
		SC_THREAD(signal_mon);
		sensitive << clk.pos();
	}
};

#endif