/*
 * preProcessorDirectives.h
 *
 * Created: 20.12.2011 17:10:33
 *  Author: tbergmueller
 */ 


#ifndef PREPROCESSORDIRECTIVES_H_
	#define PREPROCESSORDIRECTIVES_H_

	// Boolean
	#define 	TRUE					1
	#define 	FALSE					0

	// port address helpers
	#define DDR(x) ((x)-1)    // address of data direction register of port x
	#define PIN(x) ((x)-2)    // address of input register of port x

	#define CONCATx(a,b) a##b
	#define CONCAT(a,b) CONCATx(a,b)
	
#endif /* PREPROCESSORDIRECTIVES_H_ */