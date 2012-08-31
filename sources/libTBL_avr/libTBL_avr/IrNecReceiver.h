/*
 * IrNecReceiver.h
 *
 *
 *	Asynchronous receiver module of NEC-coded Infrared-Signals
 *
 *
 * Created: 15/02/2012 18:33:44
 *  Author: Thomas Bergmüller, GFI
 *	Version: 1.0
 *	February 2012
 *
 *	License: For private use only, commercial use for GFI m.b.H Austria only
 *
 *	Please refer PDF-provided Documentation and/or demo project for further information
 * 
 */ 


#ifndef IRNECRECEIVER_H_
	#define IRNECRECEIVER_H_
	
	#include <inttypes.h>
	 
	#define	INR_STATE_MSG_START_bp		0
	#define INR_STATE_MSG_REPEAT_bp		1
	#define INR_STATE_MSG_END_bp		2
	
		
	// Objects of this Type are holding state and Data of the irNec-Receiver Module.
	typedef struct 
	{
		// State's bits indicate different events
		// Bit0 ... Initial Message Received, Data is held in irNecReceiver_t.Command
		// Bit1 ... Repeated Message Received, Data is held in irNecReceiver_t.Command
		// Bit2 ... Key Released, irNecReceiver_t.Command is the KeyCommand of the released Key
		// IMPORTANT: State has to be reset programmatically!! 
		uint8_t State;
		
		// Received Command, if State is Set 
		uint8_t Command;
		
		// Address of the Remote Control that should be read
		uint8_t Address;
		
	} irNecReceiver_t;

	// Initialising Method, call if Interrupt und Timer is set up
	// ~~~ Parameters:
	// rReceiver: Pointer to instance of irNecReceiver-Object (make sure rReceiver->Address is set properly)
	// vPort: Pointer to SFR, pass e.g. &PORTB
	// vPin_bp: Pin Position in Port, where Datawire of IrDa-Module is connected, pass e.g. PB0
	// vInverted: If Logic Levels are inverted (which is quite normal when dealing with IrDa-Receiver), pass 1, 0 otherwise
	// InterruptEnable: Pointer to Interrupt Enable Function of Pin Change Interrupt (has to be implemented outside irNec-Module)
	// InterruptDisable: Pointer to Interrupt Disable Function of Pin Change Interrupt (has to be implemented outside irNec-Module)
	// Timer250usOn: Pointer to Timer-On Function, Timer has to generate an Interrupt every 250us and has to be implemented outside irNec-Module
	// Timer250usOff: Pointer to Timer-Off Function, Timer stops
	void inr_init(volatile irNecReceiver_t* rReceiver, volatile uint8_t* vPort, uint8_t vPin_bp, uint8_t vInverted, void (*InterruptEnable)(), void (*InterruptDisable)(), void(*Timer250usOn)(), void (*Timer250usOff)());
	
	// This Method has to be called inside Pin-Change ISR, every logic Change @ Data Input Pin of the irNec-Module has to trigger this Interrupt
	void inr_pinChangedInterrupt();
	
	// This Method has to be called inside 250us-Tick-Interrupt, it has to be called every 250us, as long as the timer is switched on
	void inr_250usElapsed();

#endif /* IRNECRECEIVER_H_ */
