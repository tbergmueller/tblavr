/*
 * dcs_internal.h
 *
 * Created: 13.12.2011 14:40:34
 *  Author: tbergmueller
 */ 


#ifndef DCS_INTERNAL_H_
	#define DCS_INTERNAL_H_

	#include "devComSlave.h"
	#include "m3sV2_basicFunctions.h"

	#define DCS_VERSION		2
	#define DCS_SUBVERSION  0
	#define DCS_REVISION	'a'

	
	// PRE-Processor Directives that have to be Set in Compiler:
	/*
		PROCESSOR=ATMEGA32		// See preprocessor Directives, this sets the processor for dcs_hardwareFunctions compiling
		
		RWOUT=FALSE				// defines, whether the dcs_hardwareFunction implements RW-output or not
	
	*/



	DevComSlave_t* currentSlave; // Holds Pointer to current Slave (must be created somewhere outside and is passed to init...
	DevComSlaveRxCCallback_t dcs_rxc_function;
	DevComSlaveTxCCallback_t dcs_txc_function;

	#define DCS_STATUS_DEFAULT				0
	#define DCS_STATUS_RECRESET_bm			0x01 // LSB... check reset, es wurde ein ResetCtrlByte empfangen, während dem normalen Betrieb. prüfe weiteren Rahmen
	#define DCS_STATUS_DORESET_bm			0x02 // Wenn dieses Bit gesetzt wird, dann werden alle Variablen (static im processData()) zurückgesetzt
	#define DCS_STATUS_BYPASS_bm			0x04 // Wenn irgendwelche Daten nicht für mich bestimmt sind
	#define DCS_STATUS_BYPASS_ACK_bm		0x10 // Diese DAten sind im speziellen ein Acknowledgeframe (nur 5 Byte lang)
	#define DCS_STATUS_RECEIVE_CHECKSUM_bm	0x20 // Beginne nun mit dem Empfang der Prüfsumme (letzten 3 Byte)

	#define		TRUE		1
	#define		FALSE		0

	

	#define DCS_BUFFERSIZE		(256 + M3S_OVERHEAD_LENGTH)
	
	// Buffer und gültige Daten togglen immer hin und her..
	unsigned char dcs_space1[DCS_BUFFERSIZE];
	unsigned char dcs_space2[DCS_BUFFERSIZE];
	
	uint32_t dc_fcpu;
	uint32_t dc_baudrate;
	
	volatile unsigned char changeBaudrateFlag;	
	volatile unsigned char ackToSend;
	

#endif /* DCS_INTERNAL_H_ */