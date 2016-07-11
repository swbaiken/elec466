/*
 *	fifo.h
 *
 *	Specification for a fixed depth fifo channel template. Depth of fifo
 * specified at instantiation.
 *
 *	Diagnostic outputs can be enabled for testing purposes.
 *
 *	ELEC 466, Assignment 2, Question 2, Shaun Aiken, Jun 2016
 *
 */

#ifndef FIFO_H
#define FIFO_H

#define DEBUG 0		// DEBUG 1 for diagnostic output, 0 to disable output

#include "systemc.h"
#include "fifo_if.h"
#include <iterator>
#include <list>

using namespace std;

template<class T, unsigned int size>
class fifo:public sc_module, public fifo_out_if<T>, public fifo_in_if<T> {

	private:
	list<T>				fifo_queue;
	unsigned int	max_elements;			// Set at time of instantiation
	unsigned int	current_elements;	// Tracked by read and write methods
	T 						incoming;					// Data to be written to fifo
	T* 						outgoing;					// Data extracted from fifo
	sc_event 			done;							// Signals read/write methods
	sc_event			io_req;						// Activates arbiter
	bool					read_flag;				// Tells arbiter read operation is pending
	bool					write_flag;				// Tells arbiter write operation is pending
	bool					read_success;			// Tells read method that op successful
	bool					write_success;		// Tells write method that op successful
	
	typename list<T>::iterator it;	// Iterator only used to produce diagnostic output

	public:

	SC_HAS_PROCESS(fifo);

	fifo(sc_module_name nm):sc_module(nm), max_elements(size) {
		read_flag = false;
		write_flag = false;
		current_elements = 0;
		SC_THREAD(arbitrator);
		sensitive << io_req;
	}

	void arbitrator() {
		#if DEBUG == 1
		stringstream outbuffer;
		
		cout << "---------------------------------------------------------" << endl;
		cout << "FIFO SIZE: " << max_elements << endl;
		cout << "---------------------------------------------------------" << endl;
		#endif /* DEBUG */

		wait();

		while(1) {

			#if DEBUG == 1
			outbuffer << "  -------------------------------" << endl << "  BEGIN:" 
				<< endl << "  WRITE(" << write_flag << ")  READ(" << read_flag << ")"
				<< " #ELEMENTS: " << current_elements << endl << endl;

			if (fifo_queue.size() > 0) {
				outbuffer << "  Current fifo contents: ";
				for(it = fifo_queue.begin(); it != fifo_queue.end(); advance(it,1)) {
					outbuffer << *it << " ";
				}
			}
			else {
				outbuffer << "  Current fifo contents: <empty>";
			}
			
			outbuffer << endl << endl;
			#endif /* DEBUG */
			
			if (read_flag == true) {							// Read requested
				read_flag = false;
				
				if (current_elements > 0) {					// Is there data in the fifo
					*outgoing = fifo_queue.front();		// Pull from head of list
					fifo_queue.pop_front();
					current_elements--;								// Track number of elements in fifo
				
					read_success = true;							// Flag read as successful

					#if DEBUG == 1
					outbuffer << "  Removing [" << *outgoing << "] from fifo" << endl;
					#endif /* DEBUG */
				} 
				else {
					read_success = false;							// No data in fifo, read fails
					
					#if DEBUG == 1
					outbuffer << "  No elements in fifo - read failed" << endl;
					#endif /* DEBUG */
				}
				
				done.notify();											// Activate read function
			}


			if (write_flag == true) {							// Write requested
				write_flag = false;
				
				if (current_elements < max_elements) { 	// Is there room in the fifo
					fifo_queue.push_back(incoming);				// Add to end of list
					current_elements++;								// Track number of elements in fifo
					
					write_success = true;							// Flag write as successful

					#if DEBUG == 1
					outbuffer << "  Adding [" << incoming << "] to fifo" << endl;			
					#endif /* DEBUG */
				}
				else {
					write_success = false;						// No room in fifo, write fails
					
					#if DEBUG == 1
					outbuffer << "  Fifo is full - write failed" << endl;
					#endif /* DEBUG */
				}
				
				done.notify();											// Activate write function
			}

			#if DEBUG == 1
			outbuffer << endl << "  END:" << endl << "  WRITE(" << write_flag
				<< ")  READ(" << read_flag << ")" << " #ELEMENTS: " 
				<< current_elements << endl << endl;
			
			if (fifo_queue.size() > 0) {
				outbuffer << "  Resulting fifo contents: ";
				
				for(it = fifo_queue.begin(); it != fifo_queue.end(); advance(it,1)) {
					outbuffer << *it << " ";
				}
			}
			else {
				outbuffer << "  Resulting fifo contents: <empty>";
			}
			
			outbuffer << endl << "  -------------------------------" << endl;
			cout << outbuffer.str();
			outbuffer.str("");
			#endif /* DEBUG */
				
			wait();									// Wait for next io_req event
		}
	}

	bool write(T datum) {
		if (write_flag == true) {	// Cannot have multiple simultaneous writes
			return false;
		}
		else {
			write_flag = true;			// Mark incoming write request
			incoming = datum;				// Store incoming data
			io_req.notify();				// Activate arbiter
			wait(done);							// Wait for arbiter result
			
			return write_success;		// Return arbiter success/fail
		}
	}

	bool read(T& datum) {
		if (read_flag == true) {	// Cannot have multiple simultaneous writes
			return false;
		}
		else {
			read_flag = true;				// Mark incoming read request
			outgoing = &datum;			// Store address to store read value
			io_req.notify();				// Activate arbiter
			wait(done);							// Wait for arbiter
			
			return read_success;		// Return arbiter success/fail
		}
	}

	~fifo() {
		// Destructor
		fifo_queue.clear();
	}
};

#endif
