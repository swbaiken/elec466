#include "systemc.h"
#include <iomanip>

using namespace std;

SC_MODULE (mon) {

	sc_in	<float>	x;
	sc_in	<float> y;
	sc_in_clk			clock;

	void signal_mon() {
		cout << "Filter test started." << endl;
		cout << setw(10) << "Time" << setw(10) << "In" << setw(10) << "Out" << endl;

		while(1) {
			cout << setw(10) << sc_time_stamp() << setw(10) << x.read();
			cout << setw(10) << y.read() << endl;
			wait();
		}
	}

	SC_CTOR (mon) {
		SC_CTHREAD (signal_mon, clock.pos());
	}

};
