/*
 *	memory.h
 *	
 *	ELEC 466, Assignment 2, Question 1, Shaun Aiken, Jun 2016
 */
 
#ifndef MEMORY_H
#define MEMORY_H

#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3

#define SC_LV8_Z	"ZZZZZZZZ"

#include "systemc.h"

SC_MODULE(mem_ctrl) {
// Ports
	sc_inout 	<sc_lv<8> > data; // Bidirectional port of logic-vector type
	sc_in 		< sc_uint<8> > addr;
	sc_in 		< sc_uint<2> > comm;
	sc_in 		<bool> new_comm; // Input port signaling “start”
	sc_in			<bool> reset;
	sc_out 		<bool> complete; // Output port signaling “done”
	sc_in_clk	clk;

	void mem_process() {
		sc_uint <2> comm_s; // Internal variable: command sample
		sc_uint <8> addr_s; // Internal variable: address sample
		sc_lv <8> data_s; // Internal variable: data sample

		data.write(SC_LV8_Z);
		complete.write(0);
		wait();

		while (true) {
			if (reset.read() == true) {
				/*
				for (addr_s=0;addr_s<=256;addr_s++) {
					memory[addr_s] = 0;
				}
				*/
				complete.write(false);
			}
			else {
				if (new_comm.read() == true) {
					comm_s = comm.read(); // Sample command port

					switch (comm_s) {
						case RDBLK:
							addr_s = addr.read();				// Read address
							data_s = memory[addr_s++];		// Get first block
							wait();

							data.write(data_s);	// Output first block
							cout << "@" << sc_time_stamp() << ": RDBLK, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							data_s = memory[addr_s++];	// Get second block
							wait();

							data.write(data_s);	// Output second block
							cout << "@" << sc_time_stamp() << ": RDBLK, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							data_s = memory[addr_s++];	// Get third block
							wait();

							data.write(data_s);	// Output third block
							cout << "@" << sc_time_stamp() << ": RDBLK, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							data_s = memory[addr_s++];	// Get fourth block
							wait();

							data.write(data_s);	// Output fourth block
							cout << "@" << sc_time_stamp() << ": RDBLK, address = "
							 << addr_s << ", data = " << data_s << endl;

						break;

						case WTBYT:
							addr_s = addr.read(); 
							data_s = data.read(); // Sample address & data ports
							cout << "@" << sc_time_stamp() << ": WTBYT, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							memory[addr_s] = data_s; // After 1 cycle, write into memory space
						break;

						case RDBYT:
							addr_s = addr.read(); // Sample address port
							data_s = memory[addr_s]; // Read from memory space
							cout << "@" << sc_time_stamp() << ": RDBYT, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							data.write(data_s); // After 1 cycle, output to data port
						break;

						default:
							cout << "Illegal command : " << comm_s << endl;
						break;

						case WTBLK:
							addr_s = addr.read();
							data_s = data.read(); // Sample address & data ports
							cout << "@" << sc_time_stamp() << ": WTBLK-0, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							memory[addr_s] = data_s; // After 1 cycle, write into memory space
							addr_s++; 
							data_s = data.read(); // Increment address & sample data port
							cout << "@" << sc_time_stamp() << ": WTBLK-1, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							memory[addr_s] = data_s; // After 1 cycle, write into memory space
							addr_s++;
							data_s = data.read(); // Increment address & sample data port
							cout << "@" << sc_time_stamp() << ": WTBLK-2, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							memory[addr_s] = data_s; // After 1 cycle, write into memory space
							addr_s++;
							data_s = data.read(); // Increment address & sample data port
							cout << "@" << sc_time_stamp() << ": WTBLK-3, address = "
							 << addr_s << ", data = " << data_s << endl;
							wait();

							memory[addr_s] = data_s; // After 1 cycle, write into memory space
						break;
					}

 					complete.write(true);

					while (new_comm.read() == true) {

						if (reset.read() == true) {
							break;
						}
						wait();
					}

					if (comm_s == RDBYT) {
						data.write(SC_LV8_Z); // Stop driving (a finished read)
					} 
					else if (comm_s == RDBLK) {
						data.write(SC_LV8_Z); // Stop driving (a finished read)
					}

					complete.write(false);
				}
			}

			wait();
		}
	}

	SC_CTOR(mem_ctrl) {
		SC_CTHREAD(mem_process, clk.pos());
	}

	private:

		sc_lv 	<8> memory[256]; // Internal memory space (256 8-bit words)

};

#endif