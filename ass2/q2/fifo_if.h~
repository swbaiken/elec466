/*
 *	fifo_if.h
 *
 *	Creates interface declarations for the fifo.h channel
 *
 *	ELEC 466, Assignment 2, Question 2, Shaun Aiken, Jun 2016
 *
 */

#ifndef FIFO_IF_H
#define FIFO_IF_H

template<class T>
class fifo_out_if: virtual public sc_interface {

	public:
		virtual bool write(T) = 0;	// Non-blocking write to channel
};

template<class T>
class fifo_in_if: virtual public sc_interface {

	public:
		virtual bool read(T&) = 0;	// Non-blocking read from channel
};

#endif
