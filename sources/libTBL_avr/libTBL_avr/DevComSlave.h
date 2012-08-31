/*
 * DevComSlave.h
 *
 * Created: 12.12.2011 22:42:47
 *  Author: Thomas Bergmueller
 *  Mail:	info@thomasbergmueller.com
 *  
 *
 * Description: DevCom is an bipolar half duplex inter-device communication module with Master-Slave-Architecture using M3S-Protocol via interrupt driven USARTs.
 *				This DevCom-Module covers error-checking and acknowledge/response of frames if necessary, probably rudimentary comparable to a mix of TCP/IP and Ethernet over all layers.
 *              Basic Protocols are Command and Data (Broadcast, Multicast and Unicast each), in larger Implementations Filetransfer.
 *				For commands a CommandHandler-Callback-Method is available, New Data (or other updated Parameters) are announced by setting a 
 *				Flag in devComSlave_t.NewReceived.
 *				
 *				devComSlave_t.Data is a Pointer to valid Data that can be either set by Application (via dcs_setData) or is set via Bus communication by Master. DataUpperBound thereby 
 *              determines the length of Data. Set DataUpperBound 0 if Data of variable Length should be accepted. Additional algorithm has to be implemented to determine the length of Data then.
 *
 * Details:
 *				- A Slave is identified through its bus-unique address or its multicast-address. 
 *				- If Unicast-Transfer is issued, Slave always acknowledges it with a 3 Byte Frame (as long as AckRequ-Flag in Controlbyte of received Frame was set)
 */ 


#ifndef DEVCOMSLAVE_H_
	#define DEVCOMSLAVE_H_
 
	#include <inttypes.h> 
	
	
	
	typedef uint8_t (*DevComSlaveCommandHandler_t) (const void* rSlave, const unsigned char* pCmd, unsigned char pControlbyte, unsigned char pUpperBound, unsigned char pAllowsResponse);
	typedef void (*DevComSlaveFrameTransmitter_t)(unsigned char pCtrlByte, unsigned char* pData, unsigned char pDataLength); // Sendet ein Frame vom Slave an den Master (dh. keine Ack-Behandlung)
	
	typedef void (*DevComSlaveRxCCallback_t)(unsigned char pReceivedByte);
	typedef void (*DevComSlaveTxCCallback_t)();
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ DevComSlave-Object ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
	typedef struct devComSlaveStruct
	{
		unsigned char				Address;				// Slave Address
		volatile unsigned char		MCAddress;				// Multicast Address
		unsigned short				ID;						// Device-ID, configurable by User, could be used to identify device types etc.
				
		volatile unsigned char*		Data;					// Pointer to valid Data		
		volatile unsigned char		DataUpperBound;			// Length-1 (highest index) of acceptable Data Packages. Set 0 if Data of any length up to 256 Bytes should be accepted
		volatile unsigned char		NewReceived;			// 0 ... no Data available, != 0: New Received (see "New Received Bitmapping" below)
								
		// RW-Output (if enabled)
		volatile uint8_t*			RWPort;					// Port, where RW-Wire is connected (e.g. for Buscoupler,...)
		uint8_t						RW_bp;					// Bit Position, where RW-Wire is connected (Pn0...Pn7), e.g. PC3
		unsigned char				RWPolarity;				// 0... when writing, RW-Wire is logic 0, 1 when reading    
															// 1... when writing, RW-Wire is logic 1, 0 when reading
		
		void						(*Stop)(DevComSlaveRxCCallback_t rRxcFunction, DevComSlaveTxCCallback_t rTxcFunction);
		
		DevComSlaveCommandHandler_t CommandHandler;			
		// A callback-function, that handles all Commands that are NOT handled internally. Feedback Execution State (either DCS_CMDHANDLER_SUCCESS or DCS_CMDHANDLER_ERROR).
		// parameters:
		//		pCmd			Pointer to Command (pCmd[0]) and its parameters (pCmd[1]...pCmd[pUpperBound])
		//		pLastCtrlByte	Controlbyte of the Frame the command was received in
		//		pUpperBound		UpperBound of pCmd
		//		pAllowsResponse	Indicates whether the command allows responding (Cmd received via Unicast) or not (Cmd received via Broad-/Multicast)
		//
		// return:
		//		DCS_CMDHANDLER_SUCCESS		if Execution of Command was successful
		//		DCS_CMDHANDLER_ERROR		if Execution of Command was unsuccessful or command is unknown
		
		DevComSlaveFrameTransmitter_t SendFrame;
	} 
	DevComSlave_t;
	
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ New Received Bitmapping~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//Bit Positions in devCom_t.NewReceived
	
	#define DCS_REC_DATA_gm						0x07		// group mask for Data
	
	#define DCS_REC_DATA_bp						0			// Data Received by Unicast
	#define DCS_REC_DATA_MC_bp					1			// Data Received by Multicast
	#define DCS_REC_DATA_BC_bp					2			// Data Received by Broadcast
	
	
	#define DCS_CMDHANDLER_SUCCESS				0
	#define DCS_CMDHANDLER_ERROR				1
	// Reserved Bit 4-7, used for files maybe?
	
	
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~ Init-Function-Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// Initializes DevCom according to passed parameters (see code Example below). After executing initialization, DevComSlave-Module is active and waits for Reset from Master
	// Parameters
	//	- rSlave:				Reference to DevComSlave_t-Object
	//  - rInterfaceDefinition: Pass one of the Implementation codes listet in "Implementations" without () at the end!! (Make sure Processor and USART match your expectations)
	//  - vFcpu:				Clock rate of CPU in Hz. If defined in Project Settings, pass F_CPU, otherwise e.g. 16000000 for 16MHz Crystal
	//  - vBaudrate:			Baudrate of the communication in bps, e.g. 9600	
	
	/*
		########################## Code-Example 1 on atmega88: With RW-Output ############################
		
		int main()
		{
			DevComSlave_t dc;

			dcs_setDefaults(&dc,10);				// Default, with SA=10. use of dcs_setDefaults recommended..	
			dc.MCAddress = 0x04;
	
			// RW-Output
			dc.RWPort = &PORTD;						
			dc.RW_bp = PD2;
			dc.RWPolarity = 1;						
	
			// Commandhandling
			dc.CommandHandler = fooCmdHandler;		// Callbackfunction for user-commands
	
			dc.DataUpperBound = 9-1;			// Datasets are expected to be 9 Bytes long
			dc.ID = 0x1234;						// Device ID is 0x1234 (e.g. for identification by master application)
			
			dcs_atmega88_rw_init(&dc, F_CPU, 500000);	// Initialize with 500kbps
			// DevComSlave-Module now Ready and listening..
			
			while(1)
			{
				if(dc.NewReceived & DCS_REC_DATA_gm)
				{
					// Do something, received Data ranging from dc.Data[0] .. dc.Data[dc.DataUpperBound]
			
					dc.NewReceived &= ~(DCS_REC_DATA_gm); // Reset Flags
				}
			}
	
		}
		
		
		########################## Code-Example 2 on Atmega644A, USART1: Without RW-Output ############################
		
		int main()
		{
			DevComSlave_t dc;

			dcs_setDefaults(&dc,10);				// Default, with SA=10. use of dcs_setDefaults recommended..	
			dc.MCAddress = 0x04;
		
			// Commandhandling
			dc.CommandHandler = fooCmdHandler;		// Callbackfunction for user-commands
	
			dc.DataUpperBound = 9-1;			// Datasets are expected to be 9 Bytes long
			dc.ID = 0x1234;						// Device ID is 0x1234 (e.g. for identification by master application)
			
			dcs_atmega644a_usart0_init(&dc, F_CPU, 500000);	// Initialize with 500kbps
			// DevComSlave-Module now Ready and listening..
			
			while(1)
			{
				if(dc.NewReceived & DCS_REC_DATA_gm)
				{
					// Do something, received Data ranging from dc.Data[0] .. dc.Data[dc.DataUpperBound]
			
					dc.NewReceived &= ~(DCS_REC_DATA_gm); // Reset Flags
				}
			}
	
		}
	*/
		
		
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ INITIALIZING (=Start Listening on Bus) ~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*
		This library provides a lot of precompiled objectfiles in a library. You can choose to use one of those by calling the appropriate 
		init-function that fits your device, USART and RW-Output.
	
	
	*/
	
	// ######### ENCODING OF INIT-FUNCTIONS #############
	
	// dcs_PROCESSOR_*USARTn*_*RWOUTPUT*_init(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);			whereby USARTn and RWOUTPUT optional
	
	// ~~~~~~~~ Anatomy of Initfunction ~~~~~~~~~~
	//	dcs_		... Prefix
	//	PROCESSOR	... fill in processor like it is passed in -mmcu to compiler
	//	USARTn		... optional parameter, in devices with more than one USART define USART here, e.g. usart2
	//	RWOUTPUT	... optional parameter, if you'd like to have a Processorpin signaling DataDirection (Send/Receive), add this and define in devComSlave_t.RW* - Fields
	// _init(..)	... Postfix with Parameters
	
	// ~~~~~~~ Parameters ~~~~~~~~~~~~~~~~~~~~~~~~
	// rSlave		...	Pointer to devComSlave_t-Object
	// vFcpu		... CPU-Frequency, recommended to pass F_CPU (as it should be definied for compilation anyway in most projects...)
	// vBaudrate	... Baudrate USART should be operating on in bits per second.
	
		
	
	// Examples:
	//			dcs_atmega644p_usart0_init(...);		// Starts listening @ usart0 on atmega644p without RW-Output
	//			dcs_atmega88_rw_init(...);				// Starts listening @ usart on atmega88 with RW-Output	
	
	
	DevComSlave_t* dcs_create(unsigned char SlaveAddress); // Creates new instance of DevComSlave-Module with passed address
	uint8_t dcs_setData(uint8_t* rData, uint8_t vDataLength);
	// List of Prototypes:
	
	
	
	#define UART0	0
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define UART4	4
	
		
	#if defined (__AVR_ATmega88__)
		#define prototype(uN) dcs_atmega88_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)		
	#elif defined(__AVR_ATmega32__)
		#define prototype(uN) dcs_atmega32_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined(__AVR_ATmega8__)
		#define prototype(uN) dcs_atmega8_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)	
	#elif defined(__AVR_ATmega644__)
		#define prototype(uN) dcs_atmega644_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega644A__) 
		#define prototype(uN) dcs_atmega644a_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega644P__)
		#define prototype(uN) dcs_atmega644p_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega644PA__)
		#define prototype(uN) dcs_atmega644pa_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega640__)
		#define prototype(uN) dcs_atmega640_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega1280__)
		#define prototype(uN) dcs_atmega1280_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#elif defined (__AVR_ATmega2560__)
		#define prototype(uN) dcs_atmega2560_usart##uN##_rw_init
		#define dcs_start(uartNumber,DevComSlave_t_Pointer,fcpu,baudrate) prototype(uartNumber)(DevComSlave_t_Pointer,fcpu,baudrate)
	#else	
		#ifndef COMPILELIB	
			#error "Sorry, TBL.DevComSlave-Module is not available for this processor..."
		#endif			
	#endif
	
	
	
	//{dcs_prototype_list}
	void DCS_BASIC_INIT(DevComSlave_t* rSlave, uint32_t vFcpu, uint32_t vBaudrate);
		
	
#endif /* DEVCOMSLAVE_H_ */