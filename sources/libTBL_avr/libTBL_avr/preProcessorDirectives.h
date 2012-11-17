/*
 * preProcessorDirectives.h
 *
 * Created: 20.12.2011 17:10:33
 *  Author: tbergmueller
 */ 


#ifndef PREPROCESSORDIRECTIVES_H_
	#define PREPROCESSORDIRECTIVES_H_

	#include "dcs_hardwareFunctions.h"
	
	// Boolean
	#define 	TRUE					1
	#define 	FALSE					0

	// port address helpers
	
	#if defined (XMEGA_ARCH)
		#define DDR(x) ((x)-4)    // address of data direction register of port x
		#define PIN(x) ((x)+4)    // address of input register of port x
	#else
		#define DDR(x) ((x)-1)    // address of data direction register of port x
		#define PIN(x) ((x)-2)    // address of input register of port x
	#endif
	#define CONCATx(a,b) a##b
	#define CONCAT(a,b) CONCATx(a,b)
	
#endif /* PREPROCESSORDIRECTIVES_H_ */