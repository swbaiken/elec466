#include <stdio.h>
#include "systemc.h"
#include "digit.h"

SC_MODULE (dh_sw) {
  sc_in<bool> hw_mult_done; 
  sc_in<NN_DIGIT> in_data_low;
  sc_in<NN_DIGIT> in_data_high;
  sc_out<NN_DIGIT> out_data_1;
  sc_out<NN_DIGIT> out_data_2;
  sc_out<bool> hw_mult_enable;
  
  SC_CTOR (dh_sw) {
    SC_THREAD (tester);
    sensitive << hw_mult_done;
//    dont_initialize();
  }
  
	void tester () {
		NN_DIGIT a[2]; 
		NN_DIGIT b;
		NN_DIGIT c;
		
		b = 0xf0f00000;
		c = 0xf0f0f0f0;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;		
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0x0000f0f0;
		c = 0xf0f0f0f0;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0xf0f0f0f0;
		c = 0xf0f00000;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0xf0f0f0f0;
		c = 0x0000f0f0;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0x000000ff;
		c = 0xf0f0f0f0;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0x0000ffff;
		c = 0xf0f0f0f0;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0xf0f0f0f0;
		c = 0x0000ffff;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0xf0f0f0f0;
		c = 0x000000ff;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		
		b = 0x000000ff;
		c = 0x000000ff;
		out_data_1.write(b);				// Set up multiplicands
		out_data_2.write(c);

		std::cout << "---------- " << b << " * " << c << " ----------" << endl;	
		
		hw_mult_enable.write(true);	// Send Enable to multiplier and wait for Done
		wait();
		
		a[0] = in_data_low.read();		// Get product
		a[1] = in_data_high.read();
	
		std::cout << "---------- " << " = " << a[1] << " " << a[0] << endl << endl << endl;
		
		hw_mult_enable.write(false);	// Clear Enable and wait for !Done
		wait();
		std::cout << "Done" << endl << endl;
		sc_stop();
		// Multiply complete
	   
	}

};